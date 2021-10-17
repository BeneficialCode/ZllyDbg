#include "stdafx.h"
#include "MemoryTable.h"
#include "MemoryHelper.h"
#include "resource.h"
#include "MemoryFrm.h"

int MemoryHelper::Listmemory() {
	t_memory mem;
	BYTE* address = nullptr;
	MEMORY_BASIC_INFORMATION mbi;

	for (;;) {
		if (0 == ::VirtualQueryEx(g_hProcess, address, &mbi, sizeof(mbi)))
			break;
		mem.base = reinterpret_cast<ulong>(mbi.BaseAddress);
		mem.initAccess = mbi.AllocationProtect;
		mem.memType = mbi.Type;
		mem.size = mbi.RegionSize;
		mem.access = mbi.Protect;
		mem.state = mbi.State;

		CMemoryFrame::m_MemoryTable->Addsorteddata(mem);

		address += mbi.RegionSize;
	}

	return 0;
}

ulong MemoryHelper::Readmemory(void* buf, ulong addr, ulong size, int mode) {
	bool success = ::ReadProcessMemory(g_hProcess, (LPBYTE*)addr, buf, size, nullptr);
	if (!success || GetLastError() == ERROR_PROCESS_ABORTED) {
		return 0;
	}
	return size;
}

t_memory* MemoryHelper::Findmemory(ulong addr) {
	return CMemoryFrame::m_MemoryTable->Findsorteddata(addr);
}

// Reads command from the memory of debugged process and restored breakpoints. 
// Returns length of the read code (at most MAXCMDSIZE bytes) or 0 if memory can't be read.
// 
// Note: Any access to the memory in different process is extremely time-expensive. As in many cases different parts of OllyDbg access same command several times, Readcommand maintains small 1-command cache significantly improves the wholesave productivity of OllyDbg. If you need to access several compactly placed commands, Readmemory is usually much faster.
// 
// ulong Readcommand(ulong ip,char *cmd);
// 
// Parameters:
// 
// ip - address of the command in the memory space of debugged process. If ip is 0, function invalidates cache and returns 0;
// 
// cmd - buffer of length at least MAXCMDSIZE bytes that receives command.
// 
// 
// 
// OllyDbg Plugin API v1.10
ulong MemoryHelper::Readcommand(ulong ip, uchar* cmd) {
	ulong len = 0;
	if (ip) {
		if (cmd) {
			if (ip == _ip && _len) {
				memcpy_s(cmd, 16, _cmd, _len);
				return _len;
			}
			else {
				for (_len = 16; _len && Readmemory(_cmd, ip, _len, MM_RESTORE | MM_SILENT) != _len; --_len)
					;
				if (_len) {
					_ip = ip;
					memcpy_s(cmd, 16, _cmd, _len);
				}
				else {
					_ip = 0;
				}
				return _len;
			}
		}
		else {
			return len;
		}
	}
	else {
		_ip = ip;
		_len = 0;
	}
	return len;
}