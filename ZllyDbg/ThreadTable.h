#pragma once
#include "Table.h"

class CThreadTable :public CTable<t_thread> {
public:
	CThreadTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table);
	int ParseTableEntry(CString& s, char& mask, int& select, t_thread& info, int column);
	bool CompareItems(const t_thread& i1, const t_thread& i2, int col, bool asc = true);
	CString GetErrorText(DWORD value);
	t_thread* Findsorteddata(ulong threadid);
};