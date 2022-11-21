#pragma once
#include "Table.h"

// Mode bits recognized by Readmemory() and Writememory().
#define MM_RESTORE     0x01            // Restore or remove INT3 breakpoints
#define MM_SILENT      0x02            // Don't display error message
#define MM_DELANAL     0x04            // Delete analysis from the memory

#define MM_RESILENT    (MM_RESTORE|MM_SILENT)

// 观察和修改数据的功能，变量，堆栈
class MemoryHelper final abstract{
public:
	template <typename val>
	static val Readmemory(uintptr_t addr) {
		val x;
		::ReadProcessMemory(g_hProcess, (LPBYTE*)addr, &x, sizeof(x), nullptr);
		return x;
	}

	template <typename val>
	static val Writememory(uintptr_t addr,val x) {
		DWORD old;
		//::VirtualProtect((LPBYTE*)addr, sizeof(x), PAGE_READWRITE, &old);
		::WriteProcessMemory(g_hProcess, (LPBYTE*)addr, &x, sizeof(x), nullptr);
		//::VirtualProtect((LPBYTE*)addr, sizeof(x), old, nullptr);
		return x;
	}

	static ulong Readmemory(void* buf, ulong addr, ulong size, int mode);

	static int Listmemory();
	static t_memory* Findmemory(ulong addr);
	static ulong Readcommand(ulong ip, uchar* cmd);
private:
	static inline ulong _ip = 0;
	static inline ulong _len = 0;
	static inline uchar _cmd[16];
};