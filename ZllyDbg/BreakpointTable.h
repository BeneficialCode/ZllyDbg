#pragma once
#include "Table.h"

class CBreakpointTable :public CTable<t_bpoint> {
public:
	CBreakpointTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table);
	int ParseTableEntry(CString& s, char& mask, int& select, t_bpoint& info, int column);
	t_bpoint* Findsorteddata(ulong addr);
};