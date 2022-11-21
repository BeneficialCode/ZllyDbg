#pragma once
#include "Table.h"
/* 1.断点的设置空间 
//		代码断点，数据断点，I/O断点
// 2.断点的设置方法
//		软件断点 断点指令
//		硬件断点 调试寄存器 dr0-dr7

单步执行
	汇编指令 TF
	源代码级别的单步追踪 多次TF和调试符号
	分支 DbgCtl MSR 寄存器的BTF Branch Trap Flag windbg tb
	每次执行一个线程 TSS任务状态段的T标志

	日志
	输出调试信息
	事件追踪 Event Trace
	dump file
	栈回溯
	call stack 函数调用栈
*/

// 串操作指令
// rep movs dword ptr es:[edi],dword ptr [esi] 0xf3a5
// rep stos dword ptr es:[edi] 0xf3ab
// STC/CLC	STD/CLD	STI/CLI
// 状态标志 CF、PF、AF、ZF、SF、OF
// 控制方向 DF
// rdmsr wrmsr

// 特权指令
/* 
	CLTS
	HLT
	INVD
	WBINVD
	INVLPG
	LGDT
	LIDT
	LMSW
	LTR
	Mov to/from CRn
	Mov to/from Drn
	RDMSR
	WRMSR
	RDPMC
	RDTSC

	into
	int n
	bound

	陷阱类异常

	调试状态寄存器 dr6
*/
struct DebugHelper abstract final {
	// 输出调试信息
	static int Go(ulong threadid, ulong tilladdr, int stepmode, int givechance, int backupregs);
	static int Setbreakpoint(ulong addr, ulong type, uchar cmd);
	static int Setbreakpointtext(ulong addr, ulong type, char cmd, ulong passcount);
	static int Sethardbreakpoint(ulong addr, int size, int type);
	static void Tempbreakpoint(ulong addr, int mode);
	static void Animate(int animation);
	static int ContinueExecute(ulong threadid, int givechance, int backupregs, int bypass);
	static int WriteModifiedRegister(int backup);
	static int Suspendprocess(int processevents);

	static inline int _hardBpType;
	static inline t_hardbpoint _hardBP[4];
};