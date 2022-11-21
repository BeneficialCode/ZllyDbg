#include "stdafx.h"
#include "DebugHelper.h"
#include "DbgEngine.h"
#include "ThreadTable.h"
#include "ThreadHelper.h"
#include "InfoHelper.h"
#include "MemoryHelper.h"
#include "Disassembly.h"
#include "ThreadFrm.h"
#include "BreakpointFrm.h"

extern DbgInfo g_DbgInfo;
bool g_enableHBrkPoint = true;
bool g_useHBrkStep = false;

void DebugHelper::Animate(int animation) {
	g_DbgInfo.animation = animation;
}

// Continues execution of the debugged program. 
// Returns -1 if continuation is impossible and 0 on success.
// 
// int Go(ulong threadid,ulong tilladdr,int stepmode,int givechance,int backupregs);
// 
// Parameters:
// 
// threadid - thread ID to continue. If threadid is 0, function assumes thread where last debugging event occured;
// 
// tilladdr - if stepmode is STEP_SKIP, function requests skipping of all commands up to tilladdr at once. Calling routine must guarantee that tilladdr is the first byte of some command and that sequence inbetween has no jumps/returns to outside. Otherwise, sets temporary breakpoint on tilladdr so that program will pause at this point (like "Run tol selection" in Disassembler).
// 
// stepmode - stepping mode, one of the following:
// 
// givechance - if debugged application was paused on exception and this parameter is not 0, passes exception to exception handler installed by application;
// 
// backupregs - if not 0, updates old thread registers (element oldreg of structure t_thread). Disassembler uses backup to highlight modified registers.
// 
// 
// 
// OllyDbg Plugin API v1.10
int DebugHelper::Go(ulong threadid, ulong tilladdr, int stepmode, int givechance, int backupregs) {
	if (!g_DbgInfo.eventType)
		return -1;
	if (!threadid)
		threadid = g_DbgInfo.expTid;
	t_thread* t;
	t = ThreadHelper::Findthread(threadid);
	if (!t)
		t = ThreadHelper::Findthread(g_mainThreadId);
	if (!t) {
		InfoHelper::Error("Don't know the thread to continue.");
		return -1;
	}
	if (stepmode) {
		if (!g_DbgInfo.animation) {
			g_DbgInfo.bpType |= HB_TEMP;
		}
		g_DbgInfo.stepmode = stepmode;
		g_DbgInfo.givechance = givechance;
	}
	else {
		stepmode = g_DbgInfo.stepmode;
		givechance = g_DbgInfo.givechance;
	}
	ulong ip = t->reg.ip;
	ulong ecx = t->reg.r[1];
	if (ip != g_DbgInfo.expIp || ecx != g_DbgInfo._ecx) {
		g_DbgInfo.bpType = HB_FREE;
		g_DbgInfo.expIp = ip;
		g_DbgInfo._ecx = ecx;
	}
	uchar cmd[16];
	ulong len = MemoryHelper::Readcommand(ip, cmd);
	t_disasm d;
	ulong bpAddr = 0;
	int stepType = 0;
	len = Disasm(cmd, len, ip, nullptr, &d, DISASM_DATA, 0);
	if (len > 0) {
		stepType = stepmode == STEP_OVER;
		int cmdType = d.cmdtype & C_TYPEMASK;
		switch (cmdType)
		{
			// 过滤串操作指令
			case C_REP:

				break;
			case C_CMD:
			case C_PSH:// push
			case C_POP:// pop
			case C_MMX:
			case C_NOW:
			case C_FLT:

				break;
			case C_JMP:
			case C_JMC:
			case C_RET:
				bpAddr = 0;
				stepType = 0;
				break;

			case C_CAL:// call

				break;

			case C_FLG:

				break;

			default:
				break;
		}
	}
	if (stepmode == STEP_OVER || stepmode == STEP_IN) {
		if (!givechance && !len) {

		}
		if (0 == stepType) {
			if ((t->reg.flags & 0x100) != 0) {

			}
			else
			{
				t->reg.singlestep |= 1;
				t->reg.modified = 1;
				_hardBpType = HB_ACCESS;
			}
		}
	}
	else {

	}
	_hardBP[0].type = _hardBpType;
	return ContinueExecute(threadid, givechance, backupregs, 0);
}

// Simplified (old) version of Setbreakpointext, kept for compatibility reasons. Equivalent to call Setbreakpointext(addr,type,cmd,0).
// 
// int Setbreakpoint(ulong addr,ulong type,uchar cmd);
// 
// Parameters:
// 
// addr - address of breakpoint. If address points to data or in the middle of the command, OllyDbg will ask you for confirmation;
// 
// type - combination of bits TY_xxx that specify requested actions and type of breakpoint, see description of Setbreakpointext;
// 
// cmd - original command that will be saved to descriptor if bit TY_KEEPCODE is set. Otherwise, this parameter is ignored and command is read from the memory.
// 
// OllyDbg Plugin API v1.10
int DebugHelper::Setbreakpoint(ulong addr, ulong type, uchar cmd) {
	return Setbreakpointtext(addr, type, cmd, 0);
}

// Sets new INT3 breakpoint or changes type of existing breakpoint at specified address. 
// Returns 0 on success and -1 on error (i.e. breakpoint was neither set nor restored). 
// If bit TY_KEEPCOND in type is set, condition, explanation and expression associated with breakpoint (explained here) remain unchanged, otherwise they are removed. 
// If bit TY_SETCOUNT is set or breakpoint is absent, sets specified pass count, otherwise pass count remains unchanged.
// 
// int Setbreakpointext(ulong addr,ulong type,uchar cmd,ulong passcount);
// 
// Parameters:
// 
// addr - address of breakpoint. If address points to data or in the middle of the command, OllyDbg will ask you for confirmation;
// 
// type - combination of bits TY_xxx that specify requested actions and type of breakpoint:
// 
// 
// Flag    Meaning
// TY_ACTIVE   Set permanent (user) breakpoint or restore disabled
// TY_DISABLED Temporarily deactivate permanent breakpoint. If TY_ACTIVE and TY_DISABLED are set simultaneously, TY_DISABLED is ignored
// TY_ONESHOT  Set one-shot breakpoint that will be automatically removed when hit. Doesn't interfere with active breakpoint
// TY_TEMP Set temporary breakpoint that will be automatically removed when hit. Execution continues automatically. TY_TEMP does not interfere with active breakpoint
// TY_STOPAN   Stop animation if breakpoint is hit
// TY_KEEPCODE Force original command (parameter cmd)
// TY_SETCOUNT Force pass count even if breakpoint already exists
// TY_KEEPCOND Leave associated names of types NM_BREAK, NM_BREAKEXPR, NM_BREAKEXPL and NM_PLUGCMD unchanged. If this bit is not set, breakpoints of types TY_ACTIVE and TY_DISABLED clear these names
// cmd - original command that will be saved to descriptor if bit TY_KEEPCODE is set. Otherwise, this parameter is ignored and command is read from the memory;
// 
// passcount - pass count, i.e. the number of times this breakpoint should be skipped. If breakpoint already exists and flag TY_SETCOUNT is not set, this parameter is ignored and pass count remains unchanged.
// 
// 
// To set conditional breakpoint, consider use of Manualbreakpoint. If breakpoint must be set automatically (i.e. without user's interference), please do the following:
// 
// ?If debugged program is still running, call Suspendprocess to make following operations atomic;
// ?Call Setbreakpointext(addr,TY_ACTIVE,0,passcount), thus setting INT3 breakpoint and related pass count. This is enough for ordinary (unconditional) breakpoint;
// ?If necessary, set condition by call to Insertname(addr,NM_BREAK,condition). This is enough for conditional breakpoint;
// ?To set conditional logging breakpoint, you must additionally prepare control byte,  expression and explanation and set them calling Insertname(NM_BREAKEXPR) and Insertname(NM_BREAKEXPL);
// 
// ?If necessary, resume execution (Go).
// 
// OllyDbg Plugin API v1.10
int DebugHelper::Setbreakpointtext(ulong addr, ulong type, char cmd, ulong passcount) {
	t_bpoint bp;
	bp.addr = addr;
	bp.type = TY_NORMAL;
	bp.cmd = 0xcc;
	bp.passcount = passcount;
	CBreakpointFrame::m_BreakpointTable->Addsorteddata(bp);
	int bpType = type & (TY_DISABLED | TY_ACTIVE | TY_TEMP | TY_STOPAN | TY_ONESHOT);
	if ((bpType & TY_ACTIVE) != 0) { // 启用断点
		bp.type = (bp.type & ~(TY_DISABLED | TY_ACTIVE | TY_TEMP | TY_ONESHOT)) | TY_ACTIVE;
	}
	else if ((bpType & TY_DISABLED) != 0) {// 禁用断点
		bp.type = (bp.type & ~(TY_DISABLED | TY_ACTIVE | TY_TEMP | TY_ONESHOT)) | TY_DISABLED;
	}
	else {
		bp.type |= bpType;
	}
	if (g_Status == STAT_RUNNING) {
		int n = ThreadHelper::GetThreadCount();
		if (n > 0) {
			auto threads = ThreadHelper::GetThreads();
			for (int i = 0; i < n; i++) {
				::SuspendThread(threads[i].handle);
			}
		}
	}

	if ((bp.type & TY_SET) != 0) {
		if ((bp.type & (TY_ACTIVE | TY_ONESHOT)) == 0) {

		}
	}
	return -1;
}

// Sets temporary or one-shot breakpoint on execution. If possible, sets hardware breakpoint, otherwise INT3. OllyDbg automatically removes temporary and one-shot breakpoints.
// 
// void Tempbreakpoint(ulong addr,int mode);
// 
// Parameters:
// 
// addr - code address where temporary breakpoint should be set;
// 
// mode - type of breakpoint to set:
// 
// TY_ONESHOT|TY_KEEPCOND  Set one-shot breakpoint. OllyDbg automatically removes one-shot breakpoint when hit and pauses debugged application
// TY_ONESHOT|TY_KEEPCOND|TY_STOPAN    Same as above, additionally stops any kind of trace or animation when hit
// TY_TEMP|TY_KEEPCOND Set temporary breakpoint. OllyDbg automatically removes temporary breakpoint when hit and immediately continues execution
// Any other combination   Sets INT3 breakpoint of specified type
// 
// OllyDbg Plugin API v1.10
void DebugHelper::Tempbreakpoint(ulong addr, int mode) {
	if (g_enableHBrkPoint && g_useHBrkStep && mode == (TY_ONESHOT | TY_KEEPCOND) && !Sethardbreakpoint(addr, 1, HB_ONESHOT)
		|| mode == (TY_ONESHOT | TY_KEEPCOND | TY_STOPAN) && !Sethardbreakpoint(addr, 1, HB_STOPAN)
		|| mode == (TY_TEMP | TY_KEEPCOND) && !Sethardbreakpoint(addr, 1, HB_TEMP))
		g_DbgInfo.bpType &= ~1;
	else
		Setbreakpointtext(addr, mode, 0, 0);
}

int DebugHelper::ContinueExecute(ulong threadid, int givechance, int backupregs, int bypass) {
	MemoryHelper::Readcommand(0, nullptr);
	FlushInstructionCache(g_hProcess, nullptr, 0);
	/*if ((g_DbgInfo.BpType & HB_ACCESS) == 0 && !bypass) {
		return -1;
	}*/
	DWORD status;
	int ret = -1;
	if (g_DbgInfo.eventType == 1 || g_DbgInfo.eventType == 2 || g_DbgInfo.eventType == 3) {
		WriteModifiedRegister(backupregs);
		if (!bypass && (givechance && 
			g_DbgInfo.eventType == 2 || g_DbgInfo.eventType == 3) 
			&& g_DbgInfo.Event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
			status = DBG_EXCEPTION_NOT_HANDLED;
		}
		else {
			status = DBG_CONTINUE;
		}
		if (threadid) {
			if (threadid != -1) {
				ThreadHelper::Runsinglethread(threadid);
			}
		}
		else {
			ThreadHelper::Restroeallthreads();
		}
		bool success = ::ContinueDebugEvent(g_DbgInfo.Event.dwProcessId, g_DbgInfo.Event.dwThreadId, status);
		
		// g_DbgInfo.DbgType = DBG_NONE;
		if (success && !bypass) {
			DbgEngine::SetStatus(STAT_RUNNING);
		}
		ret = 0;
		if (!success)
			return -1;
	}
	else if (g_DbgInfo.eventType == 4) {

	}
	else {
		if (!bypass) {

		}
		ret = -1;
	}
	return ret;
}

int DebugHelper::WriteModifiedRegister(int backup) {
	int n = CThreadFrame::m_ThreadTable->m_Table.data.n;
	if (n <= 0)
		return -1;
	int ret = 0;
	CONTEXT* pCtx;
	auto& threads = CThreadFrame::m_ThreadTable->m_Table.data.info;

	for (int i = 0; i < n; i++) {
		auto& t = threads[i];
		if (!t.regvalid)
			continue;
		if (backup) {
			memcpy(&t.oldreg, &t.reg, sizeof(t.reg));
			t.oldregvalid = 1;
		}
		if (t.reg.modified) {
			pCtx = &t.context;
			t.context.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS |
				CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS;
			t_reg* r = &t.reg;
			t.context.Eax = t.reg.r[0];
			t.context.Ecx = t.reg.r[1];
			t.context.Edx = t.reg.r[2];
			t.context.Ebx = t.reg.r[3];
			t.context.Esp = t.reg.r[4];
			t.context.Ebp = t.reg.r[5];
			t.context.Esi = t.reg.r[6];
			t.context.Edi = t.reg.r[7];
			t.context.Eip = t.reg.ip;
			t.context.EFlags = t.reg.flags;
			if ((t.reg.singlestep & 1) != 0) {
				t.context.EFlags |= 0x100;
				t.reg.dr6 &= 0xFFFFFFF0;
			}
			t.context.FloatSave.StatusWord = ((t.reg.top & 7) << 11) | t.context.FloatSave.StatusWord & 0xFFFFC7FF;
			BYTE* pFloatRegisterArea = t.context.FloatSave.RegisterArea;
			char* pTag = t.reg.tag;
			int index;
			for (int i = 0; i < 8; i++) {
				index = (i + t.reg.top) % 8;
				t.context.FloatSave.TagWord = ((*pTag & 3) << (2 * i)) | t.context.FloatSave.TagWord & ~(3 << (2 * i));
				*(DWORD*)pFloatRegisterArea = *(DWORD*)&r->FloatRegisterArea[10 * index];
				*((DWORD*)pFloatRegisterArea + 1) = *(DWORD*)&r->FloatRegisterArea[10 * index + 4];
				*((DWORD*)pFloatRegisterArea + 4) = *(DWORD*)&r->FloatRegisterArea[10 * index + 8];
				pFloatRegisterArea += 10;
				++pTag;
			}
			t.context.FloatSave.StatusWord = t.reg.fst;
			t.context.FloatSave.ControlWord = t.reg.fcw;
			t.context.SegEs = t.reg.s[0];
			t.context.SegCs = t.reg.s[1];
			t.context.SegSs = t.reg.s[2];
			t.context.SegDs = t.reg.s[3];
			t.context.SegFs = t.reg.s[4];
			t.context.SegGs = t.reg.s[5];
			t.context.Dr6 = t.reg.dr6;
		}
		else {
			if ((t.reg.singlestep & 1) == 0) {
				continue;
			}
			pCtx = &t.context;
			t.context.ContextFlags = CONTEXT_CONTROL| CONTEXT_DEBUG_REGISTERS;
			t.context.Eip = t.reg.ip;
			t.context.EFlags |= 0x100;
			t.reg.dr6 &= 0xFFFFFFF0;
		}
		if (g_enableHBrkPoint) {
			// 调试控制寄存器 dr7 定义断点的中断条件
			DWORD dr7 = 0x400;
			// 调试地址寄存器 dr0~dr3
			pCtx->Dr0 = _hardBP[0].addr;
			pCtx->Dr1 = _hardBP[1].addr;
			pCtx->Dr2 = _hardBP[2].addr;
			pCtx->Dr3 = _hardBP[3].addr;
			int RW;
			for (int i = 0; i < 4; i++) {
				if (_hardBP[i].type && (g_DbgInfo.bpType & 1) == 0) {
					int L = (1 << (2 * i)) | dr7;
					switch (_hardBP[i].type)
					{
						case HB_CODE:
						case HB_ONESHOT:
						case HB_STOPAN:
						case HB_TEMP:
							RW = 0;// 仅当执行对应地址的指令时中断
							if (i - 1 > 0)
								_hardBP[i - 1].type = HB_CODE;
							break;

						case HB_ACCESS:
							RW = 3;// 当向对应地址读写数据时都中断，但是从地址读取指令除外
							L |= 0x100;// LE 局部断点 486开始的处理器忽略这个设置
							break;

						case HB_WRITE:
							RW = 1;// 仅当向地址写数据时中断
							L |= 0x100;
							break;

						case HB_IO:
							RW = 2;// 当向相应地址进行I/O读写时中断
							L |= 0x100;
							break;
						default:
							RW = 0;
							break;
					}
					if (i > 1) {
						if (_hardBP[i - 1].type == HB_ACCESS)
							RW |= 4;
						else if (_hardBP[i - 1].type == HB_IO)
							RW |= 0xc;
					}
					dr7 = (RW << (4 * i + 16)) | L;
				}
			}
			pCtx->Dr7 = dr7;
		}
		if (!::SetThreadContext(t.handle, pCtx)) {
			ret = -1;
		}
	}
	return ret;
}

// Sets hardware breakpoint and activates it. 80x86 compatible processors support 4 hardware breakpoints. If all available slots are in use, function asks user to delete one of active breakpoints. Returns 0 on success and -1 on error or if user cancelled action. It is allowed to call Sethardwarebreakpoint "on the fly", i.e. when debugged application is running.
// 
// Note that hardware breakpoints are not supported by Windows 95 and Windows 98. To assure that you can use this function, call Plugingetvalue(VAL_HARDBP).
// 
// int Sethardwarebreakpoint(ulong addr,int size,int type);
// 
// Parameters:
// 
// addr - address of breakpoint;
// 
// size - size of memory covered by hardware breakpoint (1, 2 or 4 bytes). addr must be aligned on the corresponding boundary. This parameter must be 1 in case of breakpoint on execution;
// 
// type - type of hardware breakpoint:
// 
// HB_CODE Active on command execution
// HB_ACCESS   Active on read/write access
// HB_WRITE    Active on write access
// 
// OllyDbg Plugin API v1.10
int DebugHelper::Sethardbreakpoint(ulong addr, int size, int type) {
	ulong bpAddress;

	if (!g_enableHBrkPoint) {
		return -1;
	}
	if (type == HB_STOPAN || type == HB_CODE || type == HB_ONESHOT || type == HB_TEMP) {
		size = 1;
	}
	else if (type == HB_IO) {
		bpAddress = addr;
	}
	else if (type && (addr & (size - 1)) != 0)
		return -1;
	if (size != 1 && size != 2 && size != 4)
		return -1;

	bool find = false;
	for (int i = 0; i < 4; i++) {
		int bpType = _hardBP[i].type;
		if (bpType && type == bpType) {
			if (bpAddress >= _hardBP[i].addr && _hardBP[i].size + _hardBP[i].addr < bpAddress + size) {
				_hardBP[i].addr = bpAddress;
				_hardBP[i].size = size;
				find = true;
			}
		}
	}
	if (!find) {
		if (type == HB_ONESHOT || type == HB_STOPAN || type == HB_TEMP)
			return -1;
		int i = 0;
		do
		{
			int bpType = _hardBP[i].type;
			if (!bpType)
				break;
			i++;
		} while (i<4);
		if (i >= 4) {
			// There is no free slot for a new hardware breakpoint.
			return -1;
		}
		_hardBP[i].addr = bpAddress;
		_hardBP[i].size = size;
		_hardBP[i].type = type;
	}
	if (g_Status == STAT_RUNNING && type != HB_ONESHOT && type != HB_STOPAN && type != HB_TEMP) {
		int n = ThreadHelper::GetThreadCount();
		if (!n) {
			// Internal error: don't know how to set hardware breakpoint.
			return -1;
		}
		auto threads = ThreadHelper::GetThreads();
		for (int i = 0; i < n; i++) {
			t_thread t = threads[i];
			SuspendThread(t.handle);
		}
		for (int i = 0; i < n; i++) {
			t_thread t = threads[i];
			CONTEXT ctx;
			ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
			if (::GetThreadContext(t.handle, &ctx)) {
				ctx.Dr0 = _hardBP[0].addr;
				ctx.Dr1 = _hardBP[1].addr;
				ctx.Dr2 = _hardBP[2].addr;
				ctx.Dr3 = _hardBP[3].addr;
				int RW;
				DWORD dr7 = 0x400;
				for (int i = 0; i < 4; i++) {
					if (_hardBP[i].type && (g_DbgInfo.bpType & 1) == 0) {
						int L = (1 << (2 * i)) | dr7;
						switch (_hardBP[i].type)
						{
							case HB_CODE:
							case HB_ONESHOT:
							case HB_STOPAN:
							case HB_TEMP:
								RW = 0;// 仅当执行对应地址的指令时中断
								if (i - 1 > 0)
									_hardBP[i - 1].type = HB_CODE;
								break;

							case HB_ACCESS:
								RW = 3;// 当向对应地址读写数据时都中断，但是从地址读取指令除外
								L |= 0x100;// LE 局部断点 486开始的处理器忽略这个设置
								break;

							case HB_WRITE:
								RW = 1;// 仅当向地址写数据时中断
								L |= 0x100;
								break;

							case HB_IO:
								RW = 2;// 当向相应地址进行I/O读写时中断
								L |= 0x100;
								break;
							default:
								RW = 0;
								break;
						}
						if (i > 1) {
							if (_hardBP[i - 1].type == HB_ACCESS)
								RW |= 4;
							else if (_hardBP[i - 1].type == HB_IO)
								RW |= 0xc;
						}
						dr7 = (RW << (4 * i + 16)) | L;
					}
				}
				ctx.Dr7 = dr7;
				::SetThreadContext(t.handle, &ctx);
			}
		}

		for (int i = 0; i < n; i++) {
			t_thread t = threads[i];
			::ResumeThread(t.handle);
		}
	}
	return 0;
}

// Suspends all threads of the process being debugged. 
// It may happen (especially when logging breakpoints are set or hit trace is active) that threads will be suspended after some breakpoint is executed but corresponding debug event is not processed. 
// If you want OllyDbg to process events before returning from Suspendprocess, call it with processevents=1. 
// Returns 0 on success and -1 in case of any error. 
// To resume execution, call Go. This function is slow on Win95-bases systems.
// 
// int Suspendprocess(int processevents);
// 
// Parameters:
// 
// processevents - process pending debugging events before return.
// 
// 
// 
// OllyDbg Plugin API v1.10
int DebugHelper::Suspendprocess(int processevents) {
	ulong tid;

	tid = ThreadHelper::Runsinglethread(0);
	g_DbgInfo.eventType = 4;
	g_DbgInfo.expIp = 0;
	g_DbgInfo.bpType = 0;
	g_DbgInfo.enableMemBp = 0;
	g_DbgInfo.expTid = 0;


	return 0;
}