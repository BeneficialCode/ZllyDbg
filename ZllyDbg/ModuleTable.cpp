#include "stdafx.h"
#include "ModuleTable.h"

CModuleTable::CModuleTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table) :
	CTable(font,bars,nbar,captured,table){
	SetTableWindowInfo(20, m_Bar.nbar);
}

int CModuleTable::ParseTableEntry(CString& s, char& mask, int& select, t_module& info, int column) {
	switch (column) {
		case 0:// Base
			if (info.type & TY_MAIN)
				select = DRAW_HILITE;
			s.Format(L"%08X", info.base);
			break;
		case 1:// Size
			s.Format(L"%08X", info.size);
			break;
		case 2:// Entry
			if (info.entry) {
				s.Format(L"%08X", info.entry);
			}
			break;
		case 3:// Name (system)
			s = info.name;
			break;
		case 4:// File version
			s = info.version;
			break;
		case 5:// Path
			s = info.path;
			break;
		default:
			return 0;
	}
	if (info.type & TY_NEW) {
		select = DRAW_HILITE;
	}

	return s.GetLength();
}

bool CModuleTable::CompareItems(const t_module& i1, const t_module& i2, int col, bool asc) {
	switch (col) {
		case 0: return SortHelper::SortNumbers(i1.base, i2.base, asc);
		case 3: return SortHelper::SortStrings(i1.name, i2.name, asc);
		case 5: return SortHelper::SortStrings(i1.path, i2.path, asc);
	}
	return false;
}

t_module* CModuleTable::Findsorteddata(ulong addr) {
	auto it = std::find_if(m_Table.data.info.begin(), m_Table.data.info.end(), [=](auto& p) {
		if (p.base >= addr && p.base + p.size > addr) {
			return true;
			}
		return false;
		});
	if (it != m_Table.data.info.end())
		return &*it;

	return nullptr;
}