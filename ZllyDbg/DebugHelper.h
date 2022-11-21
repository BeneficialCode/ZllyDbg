#pragma once
#include "Table.h"
/* 1.�ϵ�����ÿռ� 
//		����ϵ㣬���ݶϵ㣬I/O�ϵ�
// 2.�ϵ�����÷���
//		����ϵ� �ϵ�ָ��
//		Ӳ���ϵ� ���ԼĴ��� dr0-dr7

����ִ��
	���ָ�� TF
	Դ���뼶��ĵ���׷�� ���TF�͵��Է���
	��֧ DbgCtl MSR �Ĵ�����BTF Branch Trap Flag windbg tb
	ÿ��ִ��һ���߳� TSS����״̬�ε�T��־

	��־
	���������Ϣ
	�¼�׷�� Event Trace
	dump file
	ջ����
	call stack ��������ջ
*/

// ������ָ��
// rep movs dword ptr es:[edi],dword ptr [esi] 0xf3a5
// rep stos dword ptr es:[edi] 0xf3ab
// STC/CLC	STD/CLD	STI/CLI
// ״̬��־ CF��PF��AF��ZF��SF��OF
// ���Ʒ��� DF
// rdmsr wrmsr

// ��Ȩָ��
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

	�������쳣

	����״̬�Ĵ��� dr6
*/
struct DebugHelper abstract final {
	// ���������Ϣ
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