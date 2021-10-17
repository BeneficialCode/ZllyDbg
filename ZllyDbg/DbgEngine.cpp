#include "stdafx.h"
#include "resource.h"

#include "MemoryHelper.h"
#include "ModuleHelper.h"
#include "DbgEngine.h"
#include "ThreadTable.h"
#include "ThreadFrm.h"
#include "ModuleTable.h"
#include "ModuleFrm.h"
#include "ThreadHelper.h"
#include "BreakpointHelper.h"
#include "ModuleHelper.h"
#include "CpuHelper.h"



HANDLE g_hMutex;
ulong g_exceptionAddress = 0;



// c++?
ulong g_ecx;
int g_Status = STAT_NONE;

bool g_bHandled = false;

DWORD WINAPI DbgEngine::DebugThread(void* param) {
	// 创建控制台
	AllocConsole();
	// 将stdout设备输出的数据重定向到控制台输出
	freopen("CONOUT$", "w+t", stdout);
	// 将stdin设备输入的数据重定向到控制台输入
	freopen("CONIN$", "r+t", stdin);

	char* cmdLine;
	CString exe;
	switch (g_DbgInfo.DbgType) {
	case DBG_ATTACH:
		DebugActiveProcess(g_DbgInfo.Pid);
		g_Status = STAT_RUNNING;
		break;
	case DBG_CREATE:
	{
		exe = CString(g_DbgInfo.ExePath) + L" " + g_DbgInfo.Arguments;
		int size = WideCharToMultiByte(CP_ACP, 0, exe, -1, nullptr, 0, nullptr, nullptr);
		cmdLine = (char*)malloc(size * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, exe, -1, cmdLine, size, nullptr, nullptr);

		free(cmdLine);
		cmdLine = nullptr;
		break;
	}
	default:
		break;
	}
	g_DbgInfo.hEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	g_isNeedListmodule = true;
	g_DbgInfo.ContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
	do
	{
		/*if (g_DbgInfo.Event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT&&::WaitForSingleObject(g_DbgInfo.hEvent, INFINITE)) {
			Sleep(0);
			continue;
		}*/

		::WaitForDebugEvent(&g_DbgInfo.Event, INFINITE);
		if (g_DbgInfo.Event.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT)
			g_mainThreadId = g_DbgInfo.Event.dwThreadId;
		DebugEventHandler();

		ModulesHelper::Listmodule();
		MemoryHelper::Listmemory();
		if (g_hMemoryFrm) {
			::InvalidateRect(g_hMemoryFrm, nullptr, FALSE);
		}
		if (g_hModuleFrm) {
			::InvalidateRect(g_hModuleFrm, nullptr, FALSE);
		}
		
		if (!g_bHandled && g_DbgInfo.Event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
			CpuHelper::Setcpu(g_DbgInfo.Event.dwThreadId, 0, 0, 0, CPU_ASMHIST | CPU_REGAUTO);
			g_Status = STAT_STOPPED;
			::WaitForSingleObject(g_DbgInfo.hEvent, INFINITE);
			ContinueDebugEvent(g_DbgInfo.Event.dwProcessId, g_DbgInfo.Event.dwThreadId, g_DbgInfo.ContinueStatus);
		}
		else {
			::ContinueDebugEvent(g_DbgInfo.Event.dwProcessId, g_DbgInfo.Event.dwThreadId, DBG_CONTINUE);
		}
		if (g_DbgInfo.Event.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
			break;
	} while (true);

	return 0;
}

void DbgEngine::DebugEventHandler() {
	t_reg* reg = nullptr;

	reg = ThreadHelper::GetThreadCtx(g_DbgInfo.Event.dwThreadId);

	switch (g_DbgInfo.Event.dwDebugEventCode) {
	case EXCEPTION_DEBUG_EVENT:
		ExceptionHandler(reg);
		break;

	case CREATE_THREAD_DEBUG_EVENT:
		OnCreateThread();
		break;

	case CREATE_PROCESS_DEBUG_EVENT:
		OnCreateProcess();
		break;

	case EXIT_THREAD_DEBUG_EVENT:
		OnExitThread();
		break;

	case EXIT_PROCESS_DEBUG_EVENT:
		OnExitProcess();
		break;

	case LOAD_DLL_DEBUG_EVENT:
		OnLoadDll();
		break;

	case OUTPUT_DEBUG_STRING_EVENT:
		OnDebugOutputString();
		break;

	case RIP_EVENT:
		OnRip();
		break;
	}
}

void DbgEngine::OnCreateProcess() {
	t_thread t;
	g_hProcess = g_DbgInfo.Event.u.CreateProcessInfo.hProcess;
	g_hMainThread = g_DbgInfo.Event.u.CreateProcessInfo.hThread;
	g_hMainModule = (HMODULE)g_DbgInfo.Event.u.CreateProcessInfo.lpBaseOfImage;
	
	t.threadid = g_DbgInfo.Event.dwThreadId;
	t.type = TY_MAIN;
	t.handle = g_DbgInfo.Event.u.CreateProcessInfo.hThread;
	t.datablock = (ulong)g_DbgInfo.Event.u.CreateProcessInfo.lpThreadLocalBase;
	t.entry = (ulong)g_DbgInfo.Event.u.CreateProcessInfo.lpStartAddress;
	t.suspendcount = 0;
	CThreadFrame::m_ThreadTable->Addsorteddata(t);
	if (g_hThreadFrm) {
		InvalidateRect(g_hThreadFrm, nullptr, FALSE);
	}
}

void DbgEngine::OnCreateThread() {
	t_thread t;
	t.threadid = g_DbgInfo.Event.dwThreadId;
	t.type = TY_NORMAL;
	t.handle = g_DbgInfo.Event.u.CreateThread.hThread;
	t.datablock = (ulong)g_DbgInfo.Event.u.CreateThread.lpThreadLocalBase;
	t.entry = (ulong)g_DbgInfo.Event.u.CreateThread.lpStartAddress;
	t.suspendcount = 0;

	CThreadFrame::m_ThreadTable->Addsorteddata(t);
	if (g_hThreadFrm) {
		InvalidateRect(g_hThreadFrm, nullptr, FALSE);
	}
}

void DbgEngine::OnExitThread() {
	CThreadFrame::m_ThreadTable->Deletesorteddata(g_DbgInfo.Event.dwThreadId);
	if (g_hThreadFrm) {
		InvalidateRect(g_hThreadFrm, nullptr, FALSE);
	}
}

void DbgEngine::OnExitProcess() {

}

void DbgEngine::OnLoadDll() {
	
}

void DbgEngine::OnUnloadDll() {

}

void DbgEngine::OnDebugOutputString() {

}

void DbgEngine::OnRip() {

}

void DbgEngine::ExceptionHandler(t_reg* reg) {
	ulong addr;
	if (reg) {
		addr = reg->ip;
	}
	else {
		addr = (ulong)g_DbgInfo.Event.u.Exception.ExceptionRecord.ExceptionAddress;
	}
	if (g_DbgInfo.Event.dwDebugEventCode == EXCEPTION_BREAKPOINT) {
		uchar opcode;
		int bpLenth = 0;
		int size = MemoryHelper::Readmemory((void*)&opcode,addr,1, MM_RESILENT);
		if (size == 1) {
			if (opcode == 0xcc) {
				bpLenth = 1;
			}
			else if (opcode == 0x03) {
				if (MemoryHelper::Readmemory((void*)&opcode, addr-2, 1, MM_RESILENT) == 1) {
					if (opcode == 0xcd) {
						bpLenth = 2;
					}
				}
			}
		}
		addr -= bpLenth;
		int dbgType = g_DbgInfo.DbgType;
		//g_dbgType = 0; // 下次调试新进程时使用
		if (dbgType) {
			ModulesHelper::Listmodule();
			MemoryHelper::Listmemory();
			auto& info = CModuleFrame::m_ModuleTable->m_Table.data.info;
			for (int i = 0; i < CModuleFrame::m_ModuleTable->m_Table.data.n; i++) {
				info[i].type |= TY_REPORTED;
			}
			if (g_hModuleFrm) {
				::InvalidateRect(g_hModuleFrm, nullptr, FALSE);
			}
		}
		ulong passcount;
		ulong type = BreakpointHelper::Getbreakpointtypecount(addr, &passcount);
		if (!reg) {
			return;
		}
		if (type & TY_INVALID && bpLenth == 0) {
			reg->ip = addr;
			reg->modified = 1;
			return;
		}
		if (type & (TY_DISABLED | TY_ACTIVE)) {
			reg->ip = addr;
			reg->modified = 1;

		}
		if (type & TY_ONESHOT) {

		}
		if (type & TY_INVALID) {
			reg->ip = addr;
			reg->modified = 1;

			return ;
		}
		if (dbgType) {
			g_isNeedListmodule = true;
			ModulesHelper::Listmodule();
			MemoryHelper::Listmemory();
			auto& info = CModuleFrame::m_ModuleTable->m_Table.data.info;
			for (int i = 0; i < CModuleFrame::m_ModuleTable->m_Table.data.n; i++) {
				info[i].type &= ~TY_NEW;
			}
			if (g_hModuleFrm) {
				::InvalidateRect(g_hModuleFrm, nullptr, FALSE);
			}
			if (dbgType != DBG_ATTACH) { // 如果不是创建调试
				
				return;
			}
		}
	}
}

void DbgEngine::SetStatus(int status) {
	if (status != g_Status) {
		g_Status = status;
	}
}

int DbgEngine::Run() {
	MSG msg;
	// Standard Win32 message loop
	/*while (GetMessage(&msg, nullptr, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/
	bool report = false;
	do
	{
		while (true) {
			if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				break;
			}
			report = true;
			// EXCEPTION_NONCONTINUABLE
			if (g_DbgInfo.Event.u.Exception.ExceptionRecord.ExceptionFlags && g_isNeedListmodule) {

			}
			if (g_Status != STAT_RUNNING) {
				Sleep(1);
				report = false;
			}
			if (!::WaitForDebugEvent(&g_DbgInfo.Event, 0)) {
				Sleep(0);
				report = false;
			}
			if (report) {
				DebugEventHandler();
				ModulesHelper::Listmodule();
				MemoryHelper::Listmemory();
				if (g_hMemoryFrm) {
					::InvalidateRect(g_hMemoryFrm, nullptr, FALSE);
				}
				if (g_hModuleFrm) {
					::InvalidateRect(g_hModuleFrm, nullptr, FALSE);
				}

				if (!g_bHandled && g_DbgInfo.Event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
					CpuHelper::Setcpu(g_DbgInfo.Event.dwThreadId, 0, 0, 0, CPU_ASMHIST | CPU_REGAUTO);
					g_Status = STAT_STOPPED;
					//::WaitForSingleObject(g_DbgInfo.hEvent, INFINITE);
					//ContinueDebugEvent(g_DbgInfo.Event.dwProcessId, g_DbgInfo.Event.dwThreadId, g_DbgInfo.ContinueStatus);
				}
				else {
					::ContinueDebugEvent(g_DbgInfo.Event.dwProcessId, g_DbgInfo.Event.dwThreadId, DBG_CONTINUE);
				}
			}
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	} while (msg.message!=WM_QUIT);

	return msg.wParam;
}