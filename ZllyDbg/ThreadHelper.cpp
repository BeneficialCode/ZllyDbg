#include "stdafx.h"
#include "ThreadTable.h"
#include "resource.h"
#include "ThreadFrm.h"
#include "Disassembly.h"
#include "ThreadHelper.h"
#include "MemoryHelper.h"

t_thread* ThreadHelper::Findthread(ulong threadid) {
	return CThreadFrame::m_ThreadTable->Findsorteddata(threadid);
}

t_reg* ThreadHelper::GetThreadCtx(ulong threadid) {
	CONTEXT* pCtx = nullptr;
	char* pTag = nullptr;
	t_reg* r = nullptr;
	
	auto& threads = CThreadFrame::m_ThreadTable->m_Table.data.info;
	for (int i = 0; i < CThreadFrame::m_ThreadTable->m_Table.data.n; i++) {
		auto& t = threads[i];
		t.reg.modified = 0;
		t.reg.modifiedbyuser = 0;
		t.reg.singlestep = 0;
		pCtx = &t.context;
		t.context.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS | 
			CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS;
		if (::GetThreadContext(t.handle, pCtx)) {
			if (t.threadid == threadid) {
				r = &t.reg;
			}
			t.reg.r[0] = pCtx->Eax;
			t.reg.r[1] = pCtx->Ecx;
			t.reg.r[2] = pCtx->Edx;
			t.reg.r[3] = pCtx->Ebp;
			t.reg.r[4] = pCtx->Esp;
			t.reg.r[5] = pCtx->Ebp;
			t.reg.r[6] = pCtx->Esi;
			t.reg.r[7] = pCtx->Edi;
			t.reg.ip = pCtx->Eip;
			t.reg.flags = pCtx->EFlags;
			/**		x87 FPU ¼Ä´æÆ÷ stack (»·ÐÎ½á¹¹×éÖ¯)
			*  growth		r7   ST(4)
			*  stack		r6	 ST(3)
			*				r5	 ST(2)
			*				r4	 ST(1)
			*				r3	 ST(0)   TOP: 011B
			*				r2   ST(7)
			*				r1   ST(6)
			*				r0   ST(5)
			*/
			/*		x87 FPU register
			  15 14  13  11  10  9   8   7   6   5   4   3   2   1   0
			____________________________________________________________
			| B | C | TOP  | C | C | C | E | S | P | U | O | Z | D | I |
			|   | 3 |      | 2 | 1 | 0 | S | F | E | E | E | E | E | E |
			¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª¡ª
			*/
			t.reg.top = (pCtx->FloatSave.StatusWord >> 11) & 7;
			BYTE* pFloatRegisterArea = t.reg.FloatRegisterArea;
			pTag = t.reg.tag;
			for (int i = 0; i < 8; i++) {
				int index = (i + 8 - t.reg.top) % 8;
				*(DWORD*)pFloatRegisterArea = *(DWORD*)&pCtx->FloatSave.RegisterArea[10 * index];
				*((DWORD*)pFloatRegisterArea + 1) = *(DWORD*)&pCtx->FloatSave.RegisterArea[10 * index + 4];
				*((WORD*)pFloatRegisterArea + 4) = *(WORD*)&pCtx->FloatSave.RegisterArea[10 * index + 8];
				*pTag++ = (pCtx->FloatSave.TagWord >> (2 * index) & 3);
				pFloatRegisterArea += 10;
			}
			t.reg.fst = pCtx->FloatSave.StatusWord;
			t.reg.fcw = pCtx->FloatSave.ControlWord;
			t.reg.s[0] = pCtx->SegEs;
			t.reg.s[1] = pCtx->SegCs;
			t.reg.s[2] = pCtx->SegSs;
			t.reg.s[3] = pCtx->SegDs;
			t.reg.s[4] = pCtx->SegFs;
			t.reg.s[5] = pCtx->SegGs;

			char* pBig = t.reg.big;

			LDT_ENTRY ldtEntry;
			ulong base = 0, limit;
			char big = 1;
			for (int k = 0; k < 6; k++) {
				if (::GetThreadSelectorEntry(t.handle, t.reg.s[k], &ldtEntry)) {
					base = ldtEntry.BaseLow + ((ldtEntry.HighWord.Bits.BaseMid << 16) & 0xFF0000) +
						((ldtEntry.HighWord.Bits.BaseHi << 24) & 0xFF000000);
					limit = ldtEntry.LimitLow + ((ldtEntry.HighWord.Bits.LimitHi << 16) & 0xF0000);
					if (ldtEntry.HighWord.Bits.Granularity) {
						limit = (limit << 12) + 0xFFF;
					}
					big = ldtEntry.HighWord.Bits.Default_Big;
				}
				else {
					base = 0;
					limit = 0;
					big = 1;
				}
				t.reg.base[k] = base;
				t.reg.limit[k] = limit;
				t.reg.big[k] = big;
			}
			// ¶Ïµã¼Ä´æÆ÷
			t.reg.drlin[0] = pCtx->Dr0;
			t.reg.drlin[1] = pCtx->Dr1;
			t.reg.drlin[2] = pCtx->Dr2;
			t.reg.drlin[3] = pCtx->Dr3;
			// ×´Ì¬¼Ä´æÆ÷
			t.reg.dr6 = pCtx->Dr6;
			// ¿ØÖÆ¼Ä´æÆ÷
			t.reg.dr7 = pCtx->Dr7;

			t.reg.threadid = threadid;
			DWORD lastError;
			int size = MemoryHelper::Readmemory(&lastError, t.datablock + 0x34, 4, MM_RESILENT);
			if (size == 4) {
				t.reg.lasterror = lastError;
			}
			else {
				t.reg.lasterror = -1;
			}
			t.reg.ssevalid = 0;
			t.reg.ssemodified = 0;
			t.regvalid = 1;
			if (!t.oldregvalid) {
				memcpy(&t.oldreg, &t.reg, sizeof(t.reg));
				t.oldregvalid = true;
			}
		}
		else {
			t.regvalid = 0;
		}
	}

	return r;
}

// Suspends all threads except for specified, and resumes specified thread even if it was suspended. If threadid is 0 or invalid, suspends all threads. 
// Returns thread ID of the thread that was the only one running, 
// thread ID of the main thread if there were none/more than 1 active threads, and 0 on error. 
// 
// To reverse effect of this function, call Restoreallthreads. Improper use of this function may bring OllyDbg in unstable or undefined state.
// 
// ulong Runsinglethread(ulong threadid);
// 
// Parameters:
// 
// threadid - identifier (not handle!) of thread to run, or 0 to suspend all threads.
// 
// 
// 
// OllyDbg Plugin API v1.10
ulong ThreadHelper::Runsinglethread(ulong threadid) {
	DWORD tid = g_mainThreadId;
	int n = CThreadFrame::m_ThreadTable->m_Table.data.n;
	bool refresh = false;
	if (n <= 0)
		return 0;
	auto& threads = CThreadFrame::m_ThreadTable->m_Table.data.info;
	int k = 0;
	for (int i = 0; i <n; i++) {
		auto& t = threads[i];
		if (t.threadid == threadid) {
			while (true) {
				DWORD count = ResumeThread(t.handle);
				if (count <= 0) // Ê§°Ü
					break;
				--t.suspendcount;
				refresh = true;
			}
			tid = t.threadid;
			k++;
		}
		else if (t.suspendcount <= 0) {
			if (!SuspendThread(t.handle)) {
				tid = t.threadid;
				k++;
			}
			++t.suspendcount;
			refresh = true;
		}
	}
	if (refresh && g_hThreadFrm) {
		InvalidateRect(g_hThreadFrm, nullptr, false);
	}
	if (k > 1) {
		return g_mainThreadId;
	}
	return tid;
}

void ThreadHelper::Restroeallthreads() {
	DWORD tid = g_mainThreadId;
	int n = CThreadFrame::m_ThreadTable->m_Table.data.n;
	bool refresh = false;
	if (n <= 0)
		return;
	int count = 0;
	int prev;
	auto& threads = CThreadFrame::m_ThreadTable->m_Table.data.info;
	for (int i = 0; i < n; i++) {
		auto& t = threads[i];
		refresh = false;
		if (t.suspendcount >= 0) {
			if (t.suspendcount <= 0) {
				prev = ::SuspendThread(t.handle);
				::ResumeThread(t.handle);
				if (!prev)
					++count;
			}
			else {
				while (t.suspendcount > 0) {
					prev = ::ResumeThread(t.handle);
					--t.suspendcount;
					refresh = true;
				}
				if (prev <= 1)
					++count;
			}
		}
		else {
			while (t.suspendcount < 0) {
				::SuspendThread(t.handle);
				++t.suspendcount;
				refresh = true;
			}
		}
		if (refresh && g_hThreadFrm) {
			::InvalidateRect(g_hThreadFrm, nullptr, false);
		}
		if (!count) {
			if (n == 1) {
				// Your program is suspended and can't run. Please resume main thread
			}
			else if (n > 1) {
				// All threads of debugged program are suspended. Please resume some
				// if you want the program to run.
			}
		}
	}
}