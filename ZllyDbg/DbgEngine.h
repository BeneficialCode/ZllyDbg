#pragma once
#include "Internals.h"
#include "Disassembly.h"

// Reasons why debugged application was paused, as a first argument in call to
// ODBG_Paused(), ODBG_Pausedex() and ODBG_Plugincmd().
#define		PP_MAIN              0x0003  // Mask to extract main reason
#define		PP_EVENT             0x0000  // Paused on debugging event
#define		PP_PAUSE             0x0001  // Paused on user's request
#define		PP_TERMINATED        0x0002  // Application terminated
// Extended reasons in ODBG_Pausedex().
#define PP_BYPROGRAM           0x0004  // Debugging event caused by program
#define PP_INT3BREAK           0x0010  // INT3 breakpoint
#define PP_MEMBREAK            0x0020  // Memory breakpoint
#define PP_HWBREAK             0x0040  // Hardware breakpoint
#define PP_SINGLESTEP          0x0080  // Single-step trap
#define PP_EXCEPTION           0x0100  // Exception, like division by 0
#define PP_ACCESS              0x0200  // Access violation
#define PP_GUARDED             0x0400  // Guarded page

struct DbgInfo {
	DWORD Pid;
	int DbgType;
	int EventTID;
	DEBUG_EVENT Event;
    int BpType;
    int StepMode;
    ulong ECX;
    ulong ExpAddr;
	int GiveChance;
	HANDLE hEvent;
	DWORD ContinueStatus;
	wchar_t Name[MAX_PATH];
	wchar_t CurDir[MAX_PATH];
	wchar_t ExePath[MAX_PATH];
	wchar_t Arguments[4096];
};

extern DbgInfo g_DbgInfo;

extern int g_Status;


struct DbgEngine final abstract {
	static void DebugEventHandler();
	static void ExceptionHandler(t_reg* reg);
	static void OnCreateThread();
	static void OnCreateProcess();
	static void OnExitThread();
	static void OnExitProcess();
	static void OnLoadDll();
	static void OnUnloadDll();
	static void OnDebugOutputString();
	static void OnRip();
	static void SetStatus(int status);
	static int Run();
	static DWORD WINAPI DebugThread(void* param);
};
