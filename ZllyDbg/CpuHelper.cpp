#include "stdafx.h"
#include "DbgEngine.h"
#include "CpuHelper.h"
#include "CpuAsm.h"
#include "CpuStack.h"
#include "CpuDump.h"
#include "ThreadHelper.h"
#include "ModuleHelper.h"
#include "MemoryHelper.h"
#include "CpuFrm.h"

void CpuHelper::InvalidateWindows(bool redraw) {
	if (g_hCpuFrm) {
		InvalidateRect(g_hCpuFrm, nullptr, false);
		InvalidateRect(CAsmWindow::_dump.hw, nullptr, false);
		InvalidateRect(CDumpWindow::_dump.hw, nullptr, false);
		InvalidateRect(CStackWindow::_dump.hw, nullptr, false);
		InvalidateRect(CInfoWindow::_dump.hw, nullptr, false);
		InvalidateRect(CRegWindow::_dump.hw, nullptr, false);
		if (redraw)
			RedrawWindow(g_hCpuFrm, nullptr, 0, RDW_UPDATENOW | RDW_ALLCHILDREN | RDW_INVALIDATE);
	}
}

// Updates state of panes in CPU window. If necessary, creates or restores CPU window and moves it to top.
// 
// void Setcpu(ulong threadid,ulong asmaddr,ulong dumpaddr,ulong stackaddr,int mode);
// 
// Parameters:
// 
// threadid - identified of thread to display in CPU, or 0 if thread remains unchanged. If threadid id non-zero, parameters asmaddr and stackaddr are ignored and set to contents of EIP and ESP of the specified thread. If threadid is 0 and actual thread is invalid, Setcpu automatically reswitches to main thread;
// 
// asmaddr - address to display in Disassembler, or 0 if this address remains unchanged. Ignored if threadid is not 0;
// 
// dumpaddr - address to display in CPU Dump, or 0 if this address remains unchanged;
// 
// stackaddr - address to display in Stack, or 0 if this address remains unchanged. Ignored if threadid is not 0;
// 
// mode - combination of CPU_xxx flags that select update mode:
// 
// CPU_ASMHIST Add change to Disassembler history
// CPU_ASMCENTER   Position address in the middle of Disassembler window
// CPU_ASMFOCUS    Move focus to Disassembler
// CPU_DUMPHIST    Add change to Dump history (currently not available)
// CPU_DUMPFIRST   Make dumpaddr the first byte in CPU Dump
// CPU_DUMPFOCUS   Move focus to CPU Dump
// CPU_REGAUTO Automatically change Registers mode to FPU/MMX/3DNow!
// CPU_RUNTRACE    Show run trace data at offset asmaddr
// CPU_NOCREATE    Don't create CPU window if absent
// CPU_REDRAW  Redraw CPU window immediately
// 
// OllyDbg Plugin API v1.10
void CpuHelper::Setcpu(ulong threadid, ulong asmaddr, ulong dumpaddr, ulong stackaddr, int mode) {
	t_thread* t;
	t_module* m;
	t_memory* mem;
	ulong selAddr = asmaddr;

	if (g_DbgInfo.Pid) {
		if (mode & CPU_RUNTRACE) {

		}
		else {
			CAsmWindow::_dump.runtraceoffset = 0;
		}
		if (mode & CPU_ASMHIST) {

		}
		if (mode & CPU_DUMPHIST) {

		}
		if (!threadid) {

		}
		if (threadid) {
			t = ThreadHelper::Findthread(threadid);
			if (t==nullptr) {
				threadid = g_mainThreadId;
				t = ThreadHelper::Findthread(threadid);
			}
			g_DbgInfo.EventTID = threadid;
			g_cpuThreadId = threadid;
			CAsmWindow::_dump.threadid = threadid;
			if (t && t->regvalid) {
				if (!selAddr) {
					selAddr = t->reg.ip; // ¶ÁÈ¡eipµÄÖµ
				}
			}
		}
		if (CStackWindow::_dump.hw) {

		}
		if (selAddr) {
			mem = MemoryHelper::Findmemory(selAddr);
			if (mem != nullptr) {
				if (mem->base == CAsmWindow::_dump.base && CAsmWindow::_dump.size == mem->size) {
					CAsmWindow::_dump.sel0 = selAddr;
					CAsmWindow::_dump.startsel = selAddr;
					CAsmWindow::_dump.sel1 = Disassembleforward(nullptr, CAsmWindow::_dump.base, CAsmWindow::_dump.size, selAddr, 1, 1);
				}
				else {
					CAsmWindow::_dump.addr = selAddr;
					CAsmWindow::_dump.size = mem->size;
					CAsmWindow::_dump.base = mem->base;
					CAsmWindow::_dump.sel0 = selAddr;
					CAsmWindow::_dump.sel1 = Disassembleforward(nullptr, CAsmWindow::_dump.base, CAsmWindow::_dump.size, selAddr, 1, 1);
					CAsmWindow::_dump.startsel = selAddr;
					CAsmWindow::_dump.reladdr = 0;
					CAsmWindow::_dump.backup = 0;
				}
			}
			else {
				CAsmWindow::_dump.addr = 0;
				CAsmWindow::_dump.size = 0;
				CAsmWindow::_dump.base = 0;
				CAsmWindow::_dump.sel0 = 0;
				CAsmWindow::_dump.sel1 = 0;
				CAsmWindow::_dump.startsel = 0;
				CAsmWindow::_dump.reladdr = 0;
				CAsmWindow::_dump.backup = 0;
			}
			CCPUFrame::m_Asm.SetdumpAddress(CAsmWindow::_dump);
		}
		if (!dumpaddr && !CDumpWindow::_dump.base&&g_hMainModule) {
			/*m = ModulesHelper::Findmodule((ulong)g_hMainModule);
			if (m!=nullptr) {
				dumpaddr = m->
			}*/
		}
		if (dumpaddr) {

		}
		if (stackaddr) {

		}
		if (mode & CPU_ASMHIST) {

		}
		if (mode & CPU_DUMPHIST) {

		}
		if (mode & CPU_NOCREATE) {

		}
		if (g_hCpuFrm) {
			if (mode & CPU_ASMFOCUS) {
				SendMessage(g_hCpuFrm, WM_USER_FOCUS, 0, (LPARAM)CAsmWindow::_dump.hw);
			}
			InvalidateWindows(mode & CPU_REDRAW);
		}
	}
	else {
		CAsmWindow::_dump.threadid = 0;
		CAsmWindow::_dump.addr = 0;
		CAsmWindow::_dump.base = 0;
		CAsmWindow::_dump.size = 0;
		CAsmWindow::_dump.sel1 = 0;
		CAsmWindow::_dump.sel0 = 0;
		CAsmWindow::_dump.startsel = 0;
		CAsmWindow::_dump.reladdr = 0;
		CDumpWindow::_dump.threadid = 0;
		CDumpWindow::_dump.addr = 0;
		CDumpWindow::_dump.size = 0;
		CDumpWindow::_dump.base = 0;
		CDumpWindow::_dump.sel1 = 0;
		CDumpWindow::_dump.sel0 = 0;
		CDumpWindow::_dump.reladdr = 0;
		CStackWindow::_dump.threadid = 0;
		CStackWindow::_dump.addr = 0;
		CStackWindow::_dump.base = 0;
		CStackWindow::_dump.sel1 = 0;
		CStackWindow::_dump.sel0 = 0;
		CStackWindow::_dump.reladdr = 0;
		
	}
}

ulong CpuHelper::Findprocbegin(ulong addr) {
	t_memory* m = nullptr;
	uchar* pdecode = nullptr;
	ulong size;
	ulong rva;
	uchar* ptype;

	m = MemoryHelper::Findmemory(addr);
	if (!m)
		return 0;

	pdecode = ModulesHelper::Finddecode(m->base, &size);
	rva = addr - m->base;
	if (!pdecode || rva >= size) {
		return 0;
	}

	if ((pdecode[rva] & DEC_PBODY) == 0)
		return 0;
	
	for (ptype = &pdecode[rva];; --ptype) {
		if ((*ptype & DEC_PBODY) == 0 || (*ptype & DEC_PROCMASK) == DEC_PROC) {
			return rva + m->base;
		}
		if (!rva)
			break;
		--rva;
	}
	return 0;
}

// Returns address of the last command of the procedure that encloses addr, or 0 on error, for example, when module is not analyzed or address points to no procedure.
// 
// ulong Findprocend(ulong addr);
// 
// Parameters:
// 
// addr - address of any command within the procedure.
// 
// 
// 
// OllyDbg Plugin API v1.10
ulong CpuHelper::Findprocend(ulong addr) {
	uchar* pdecode = nullptr;
	ulong i = 0;
	ulong size = 0;

	pdecode = ModulesHelper::Finddecode(addr, &size);
	if (!pdecode)
		return 0;

	if ((*pdecode & DEC_PROCMASK) == 0)
		return 0;

	if (!size)
		return 0;

	while ((*pdecode & DEC_PROCMASK) != 0 && (*pdecode & DEC_PROCMASK) != DEC_PEND){
		++i;
		++pdecode;
		if (i >= size)
			return 0;
	}
	return i + addr;
}

// Returns start address of the procedure that precedes or encloses addr, or 0 on error, for example, when module is not analyzed or address doesn't point to executable code.
// 
// ulong Findprevproc(ulong addr);
// 
// Parameters:
// 
// addr - address of reference command.
// 
// 
// 
// OllyDbg Plugin API v1.10
ulong CpuHelper::Findprevproc(ulong addr) {
	t_memory* m = nullptr;
	uchar* pdecode = nullptr;
	ulong rva;
	uchar* ptype = nullptr;
	ulong size;

	m = MemoryHelper::Findmemory(addr);
	if (!m)
		return 0;

	if (addr == m->base)
		return 0;

	pdecode = ModulesHelper::Finddecode(m->base, &size);
	rva = addr - m->base;
	if (!pdecode || rva >= size)
		return 0;

	ptype = &pdecode[rva];
	do
	{
		--rva;
		if ((*--ptype & DEC_PROCMASK) == DEC_PROC)
			return m->base + rva;
	} while (rva);

	return 0;
}

// Returns start address of the procedure that is next to addr, or 0 on error, for example, when module is not analyzed or address doesn't point to executable code.
// 
// ulong Findnextproc(ulong addr);
// 
// Parameters:
// 
// addr - address of reference command.
// 
// 
// 
// OllyDbg Plugin API v1.10
ulong CpuHelper::Findnextproc(ulong addr) {
	uchar* pdecode = nullptr;
	ulong offset;
	uchar* pNextDecode = nullptr;
	ulong size;

	pdecode = ModulesHelper::Finddecode(addr, &size);
	if (!pdecode)
		return 0;

	offset = 1;
	pNextDecode = pdecode + 1;
	if (size <= 1)
		return 0;
	while ((*pNextDecode & DEC_PROCMASK) != DEC_PROC) {
		++offset;
		++pNextDecode;
		if (offset >= size)
			return 0;
	}
	return addr + offset;
}

// addr - address of any command within the procedure;
// 
// start - pointer to variable that receives start address of the procedure;
// 
// end - pointer to variable that receives address of the last command in the procedure.
// 
// OllyDbg Plugin API v1.10
int CpuHelper::Getproclimits(ulong addr, ulong* start, ulong* end) {
	ulong procBegin, procEnd;
	uchar* pdecode;
	uchar* ptype;

	procBegin = Findprocbegin(addr);
	procEnd = Findprocend(addr);
	if (!procBegin || !procEnd)
		return -1;

	ulong size;
	pdecode = ModulesHelper::Finddecode(procEnd, &size);
	if (!pdecode)
		return -1;
	int i = 1;
	for (ptype = pdecode + 1; i < size; ++ptype) {
		if ((*ptype & DEC_TYPEMASK) != DEC_NEXTCODE)
			break;
		++i;
	}
	if (start)
		*start = procBegin;
	if (end)
		*end = procEnd + i;
	return 0;
}