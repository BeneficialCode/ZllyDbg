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
#include "DebugHelper.h"
#include "DbgDefender.h"

HANDLE g_hMutex;

// c++?

int g_Status = STAT_NONE;

int g_refreshMem = false;

int DbgEngine::DebugEventHandler() {

	_reg = ThreadHelper::GetThreadCtx(g_DbgInfo.Event.dwThreadId);

	switch (g_DbgInfo.Event.dwDebugEventCode) {
		case EXCEPTION_DEBUG_EVENT:
			return ExceptionHandler();

		case CREATE_THREAD_DEBUG_EVENT:
			OnCreateThread();
			return 1;

		case CREATE_PROCESS_DEBUG_EVENT:
			OnCreateProcess();
			return 1;

		case EXIT_THREAD_DEBUG_EVENT:
			OnExitThread();
			return 1;

		case EXIT_PROCESS_DEBUG_EVENT:
			OnExitProcess();
			return -1;

		case LOAD_DLL_DEBUG_EVENT:
			OnLoadDll();
			return 1;

		case UNLOAD_DLL_DEBUG_EVENT:

			return 1;

		case OUTPUT_DEBUG_STRING_EVENT:
			OnDebugOutputString();
			return 1;

		case RIP_EVENT:
			OnRip();
			return 0;
	}

	return 0;
}

void DbgEngine::OnCreateProcess() {
	t_thread t;
	g_hProcess = g_DbgInfo.Event.u.CreateProcessInfo.hProcess;
	g_hMainThread = g_DbgInfo.Event.u.CreateProcessInfo.hThread;
	g_hMainModule = (HMODULE)g_DbgInfo.Event.u.CreateProcessInfo.lpBaseOfImage;

	PEB peb;
	DbgDefender::GetProcessPeb(g_hProcess, &peb);
	if (peb.BeingDebugged) {
		PPEB pPEB = DbgDefender::GetProcessPebAddress(g_hProcess);
		uintptr_t addr = reinterpret_cast<uintptr_t>(&pPEB->BeingDebugged);
		MemoryHelper::Writememory(addr, 0);
	}
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

int DbgEngine::FilterException(DWORD code, int ignore) {
	return 0;
}

// 忽略异常返回2，需要处理为0,
int DbgEngine::ExceptionHandler() {
	ulong addr;
	int type = DebugHelper::_hardBpType;
	DebugHelper::_hardBpType = HB_FREE;
	ulong ecx;
	auto pExceptionInfo = &g_DbgInfo.Event.u.Exception;
	if (_reg) {
		addr = _reg->ip;
		ecx = _reg->r[1];
		if (type && (_reg->flags & 0x100) != 0) {
			_reg->flags &= ~0x100;
			_reg->modified = 1;
		}
	}
	else {
		ecx = 0;
		addr = (ulong)g_DbgInfo.Event.u.Exception.ExceptionRecord.ExceptionAddress;
	}
	int bpLenth = 0;
	if (pExceptionInfo->ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT) {
		uchar opcode;
		int size = MemoryHelper::Readmemory((void*)&opcode, addr - 1, 1, MM_RESILENT);
		if (size == 1) {
			if (opcode == 0xcc) {
				bpLenth = 1;
			}
			else if (opcode == 0x03) {
				if (MemoryHelper::Readmemory((void*)&opcode, addr - 2, 1, MM_RESILENT) == 1) {
					if (opcode == 0xcd) {
						bpLenth = 2;
					}
				}
			}
		}
	}
	addr -= bpLenth;
	if (g_DbgInfo.expIp != addr || ecx != g_DbgInfo._ecx) {
		g_DbgInfo.bpType = HB_FREE;
		g_DbgInfo.expIp = addr;
		g_DbgInfo._ecx = ecx;
	}
	DWORD code = pExceptionInfo->ExceptionRecord.ExceptionCode;

	switch (code)
	{
		case STATUS_INTEGER_OVERFLOW:			// Integer overflow
		case STATUS_PRIVILEGED_INSTRUCTION:		// Privileged instruction
		case STATUS_STACK_OVERFLOW:				// Stack overflow
		case STATUS_INTEGER_DIVIDE_BY_ZERO:		// Integer division by zero
		case STATUS_FLOAT_INVALID_OPERATION:	// Invalid floating-point operation
		case STATUS_FLOAT_OVERFLOW:				// Floating-point overflow
		case STATUS_FLOAT_STACK_CHECK:			// FPU stack error
		case STATUS_FLOAT_UNDERFLOW:			// Floating-point underflow
		case STATUS_FLOAT_INEXACT_RESULT:		// Inexact floating-point result
		case STATUS_NONCONTINUABLE_EXCEPTION:	// Exception is not continuable
		case STATUS_FLOAT_DIVIDE_BY_ZERO:		// Floating-point division by zero
		case STATUS_FLOAT_DENORMAL_OPERAND:		// Denormalized floating-point operand
		case STATUS_ARRAY_BOUNDS_EXCEEDED:		// Array bounds exceeded
		case STATUS_ILLEGAL_INSTRUCTION:		// Illegal instruction
		{
			g_DbgInfo.reason |= (PP_EXCEPTION | PP_BYPROGRAM);
			g_DbgInfo.exception = code;
			g_DbgInfo.eventType = 2;
			DebugHelper::Animate(ANIMATE_OFF);
			return 0;
		}
	}

	if (code == STATUS_GUARD_PAGE_VIOLATION) {
		g_DbgInfo.reason |= PP_GUARDED;
		DebugHelper::Animate(ANIMATE_OFF);
		return 0;
	}

	if (code == STATUS_BREAKPOINT) {
		g_DbgInfo.reason |= PP_INT3BREAK;
		int dbgType = g_DbgInfo.dbgType;
		g_DbgInfo.dbgType = 0;
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

		ulong bpType = BreakpointHelper::Getbreakpointtypecount(addr, &passcount);
		if (!_reg) {
			return 0;
		}
		if ((bpType & TY_INVALID) && bpLenth == 0) {
			_reg->ip = addr;
			_reg->modified = 1;
			return 1;
		}
		if (bpType & (TY_DISABLED | TY_ACTIVE)) {
			_reg->ip = addr;
			_reg->modified = 1;
			g_DbgInfo.bpType |= TY_MAIN;

			DebugHelper::Animate(ANIMATE_OFF);
			return 0;
		}
		if (bpType & TY_ONESHOT) {
			DebugHelper::Animate(ANIMATE_OFF);
			return 0;
		}
		if ((bpType & TY_INVALID) == 0) {
			_reg->ip = addr;
			_reg->modified = 1;

			return 1;
		}
		if (dbgType) {
			g_DbgInfo.refresh = 1;
			ModulesHelper::Listmodule();
			MemoryHelper::Listmemory();
			auto& info = CModuleFrame::m_ModuleTable->m_Table.data.info;
			for (int i = 0; i < CModuleFrame::m_ModuleTable->m_Table.data.n; i++) {
				info[i].type &= ~TY_NEW;
			}
			if (g_hModuleFrm) {
				::InvalidateRect(g_hModuleFrm, nullptr, FALSE);
			}
			if (dbgType != DBG_CREATE) { // 如果不是创建调试
				if (g_DbgInfo.pid) {
					g_DbgInfo.eventType = 2;
					return 1;
				}
				// Attached process paused at ntdll.DbgBreakpoint
				g_DbgInfo.eventType = 2;
				return 0;
			}
			// System startup breakpoint
			return 0;
		}

		return 0;
	}

	if (code == STATUS_ACCESS_VIOLATION) {
		g_DbgInfo.reason |= PP_ACCESS | PP_BYPROGRAM;
		DebugHelper::Animate(ANIMATE_OFF);
		return 0;
	}

	if (code == STATUS_SINGLE_STEP) {
		int L = 0;
		if (DebugHelper::_hardBP[0].type) {
			L |= 1;
		}
		if (DebugHelper::_hardBP[1].type) {
			L |= 2;
		}
		if (DebugHelper::_hardBP[2].type) {
			L |= 4;
		}
		if (DebugHelper::_hardBP[3].type) {
			L |= 8;
		}
		if (_reg && (L & _reg->dr6) != 0) {
			g_DbgInfo.reason |= PP_HWBREAK;

		}
		g_DbgInfo.reason |= PP_SINGLESTEP;
		if (_reg && (_reg->dr6 & 0xF) != 0) {
			_reg->dr6 &= 0xFFFFFFF0;
			_reg->modified = 1;
		}
		if (type == HB_ACCESS) {
			if (_reg) {
				_reg->dr6 &= ~0x4000;
				_reg->modified = 1;
			}
			return 0;
		}
		else {
			if (type != HB_CODE) {

			}
			if (_reg) {
				_reg->dr6 &= ~0x4000;
				_reg->modified = 1;
			}
			return 1;
		}
		return 0;
	}
}

void DbgEngine::SetStatus(int status) {
	if (status != g_Status) {
		g_Status = status;
	}
}

void DbgEngine::UpdateDumps() {

}

int DbgEngine::Run() {
	MSG msg;
	// Standard Win32 message loop
	/*while (GetMessage(&msg, nullptr, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/

	DWORD status;
	int givechance;
	do
	{
		while (true) {
			if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
				break;
			}
			// EXCEPTION_NONCONTINUABLE
			if (g_DbgInfo.Event.u.Exception.ExceptionRecord.ExceptionFlags && g_isNeedListmodule) {
				ModulesHelper::Listmodule();
				MemoryHelper::Listmemory();
				auto& threads = CThreadFrame::m_ThreadTable->m_Table.data.info;
				for (int i = 0; i < CThreadFrame::m_ThreadTable->m_Table.data.n; i++) {
					auto& t = threads[i];
					::ResumeThread(t.handle);
				}
				g_isNeedListmodule = false;
			}

			do
			{
				if (g_Status != STAT_RUNNING) {
					Sleep(1);
					break;
				}
				if (!::WaitForDebugEvent(&g_DbgInfo.Event, 0)) {
					Sleep(0);
					break;
				}

				if (g_DbgInfo.Event.dwProcessId == g_DbgInfo.pid) {
					int oldStatus = g_Status;
					g_Status = STAT_EVENT;
					g_DbgInfo.eventType = 1;
					g_DbgInfo.reason = PP_EVENT;
					if (g_refreshMem) {
						ModulesHelper::Listmodule();
						MemoryHelper::Listmemory();
					}

					int ret = DebugEventHandler();
					int lcmd = 0;
					g_Status = oldStatus;
					uchar cmd[4];
					if (g_DbgInfo.animation == ANIMATE_RET
						&& _reg
						&& (lcmd = MemoryHelper::Readcommand(_reg->ip, cmd) != 0)
						&& ((lcmd = cmd[0] & 0xF6, lcmd == 0xc2) || cmd[0] == 0xcf))
					{

					}
					else {
						if (g_DbgInfo.animation == ANIMATE_USER) {

						}
						if (ret == 2 && g_DbgInfo.animation != ANIMATE_STOP) {
							DebugHelper::Go(g_DbgInfo.Event.dwThreadId, 0, 0, 0, 0);
							break;
						}
						if (ret == 1 && (!g_DbgInfo.animation || g_DbgInfo.Event.dwDebugEventCode != EXCEPTION_DEBUG_EVENT)) {
							givechance = g_DbgInfo.eventType == 2 || g_DbgInfo.eventType == 3;
							DebugHelper::ContinueExecute(-1, givechance, 0, 0);
							break;
						}
						if (ret == -1) {
							CpuHelper::Setcpu(g_DbgInfo.Event.dwThreadId, 0, 0, 0, CPU_ASMHIST | CPU_REGAUTO);
							SetStatus(STAT_FINISHED);
							break;
						}
						if (g_DbgInfo.animation == ANIMATE_RET// 执行到返回
							|| g_DbgInfo.animation == ANIMATE_SKPRET// 跳过返回指令
							|| g_DbgInfo.animation == ANIMATE_USER// 执行到用户代码
							|| g_DbgInfo.animation == ANIMATE_TRIN// trace in
							|| g_DbgInfo.animation == ANIMATE_TROVER) {

						}
						else {
							CpuHelper::Setcpu(g_DbgInfo.Event.dwThreadId, 0, 0, 0, CPU_ASMHIST | CPU_REGAUTO);
						}

						if (g_DbgInfo.animation == ANIMATE_OFF || g_DbgInfo.animation == ANIMATE_STOP) {
							if (g_DbgInfo.animation == ANIMATE_STOP) {
								DebugHelper::Animate(ANIMATE_OFF);
							}
							if (!g_DbgInfo.animation) {
								DbgEngine::SetStatus(STAT_STOPPED);
							}
						}
					}

				}
				else {
					if (g_DbgInfo.Event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT && g_DbgInfo.Event.u.Exception.dwFirstChance) {
						status = DBG_EXCEPTION_NOT_HANDLED;
					}
					else {
						status = DBG_CONTINUE;
					}
					::ContinueDebugEvent(g_DbgInfo.Event.dwProcessId, g_DbgInfo.Event.dwThreadId, status);
				}
			} while (false);


		}

		do
		{
			if (::TranslateMDISysAccel(g_hWndMDIClient, &msg))
				break;
			if (msg.message == WM_SYSKEYDOWN || msg.message == WM_KEYDOWN) {
				int shift = GetKeyState(VK_SHIFT) & 0x8000;
				int ctrl = GetKeyState(VK_CONTROL) & 0x8000;
				int alt = msg.wParam & 0x20000000;
				OnKeyDown(msg.message, msg.wParam, alt, shift, ctrl);
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		} while (false);

	} while (msg.message != WM_QUIT);

	return msg.wParam;
}

bool DbgEngine::OnKeyDown(UINT uMsg, WPARAM wParam, int alt, int shift, int ctrl) {
	// 单步步过
	if (uMsg == WM_KEYDOWN && wParam == VK_F8) {
		DebugHelper::Animate(ANIMATE_OFF);
		DebugHelper::Go(0, 0, STEP_OVER, 0, 1);
		return true;
	}
	// Run
	if (uMsg == WM_KEYDOWN && wParam == VK_F9) {
		DebugHelper::Animate(ANIMATE_OFF);
		DebugHelper::Go(0, 0, STEP_RUN, 0, 1);
	}
	// Pause
	if (uMsg == WM_KEYDOWN && wParam == VK_F12) {
		DebugHelper::Animate(ANIMATE_OFF);
		if (g_Status == STAT_RUNNING) {
			DebugHelper::Suspendprocess(1);
			return true;
		}
	}
	return true;
}