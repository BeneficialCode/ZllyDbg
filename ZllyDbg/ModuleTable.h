#pragma once

#include "Table.h"

class CModuleTable :public CTable<t_module> {
public:
	CModuleTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table);
	int ParseTableEntry(CString& s, char& mask, int& select, t_module& info, int column);
	bool CompareItems(const t_module& i1, const t_module& i2, int col, bool asc = true);
	t_module* Findsorteddata(ulong addr);
};