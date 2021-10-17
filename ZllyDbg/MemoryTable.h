#pragma once

#include "Table.h"

class CMemoryTable :public CTable<t_memory> {
public:
	CMemoryTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table);
	int ParseTableEntry(CString& s, char& mask, int& select, t_memory& info, int column);
	bool CompareItems(const t_memory& i1, const t_memory& i2, int col, bool asc = true);
	t_memory* Findsorteddata(ulong addr);

	const wchar_t* MemoryTypeToString(DWORD type);
	const wchar_t* StateToString(DWORD state);
	std::wstring ProtectionToString(DWORD protection);
	std::wstring GetDetails(HANDLE hProcess, t_memory& mem);
	std::wstring GetDosNameFromNtName(PCWSTR name);
};