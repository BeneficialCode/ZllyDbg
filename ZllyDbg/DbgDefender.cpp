#include "stdafx.h"
#include "DbgDefender.h"
#include "MemoryHelper.h"

#pragma comment(lib,"ntdll")

bool DbgDefender::GetProcessPeb(HANDLE hProcess, PPEB peb) {
	PROCESS_BASIC_INFORMATION info;
	if (!NT_SUCCESS(::NtQueryInformationProcess(hProcess, ProcessBasicInformation, &info, sizeof(info), nullptr)))
		return false;

	return ::ReadProcessMemory(hProcess, info.PebBaseAddress, peb, sizeof(*peb), nullptr);
}

PPEB DbgDefender::GetProcessPebAddress(HANDLE hProcess) {
	PROCESS_BASIC_INFORMATION info;
	if (!NT_SUCCESS(::NtQueryInformationProcess(hProcess, ProcessBasicInformation, &info, sizeof(info), nullptr)))
		return nullptr;

	return info.PebBaseAddress;
}