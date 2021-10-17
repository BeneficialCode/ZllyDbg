#include "stdafx.h"
#include "BreakpointTable.h"
#include "ThreadHelper.h"
#include "ModuleHelper.h"

CBreakpointTable::CBreakpointTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table):
	CTable(font,bars,nbar,captured,table){
	SetTableWindowInfo(nbar);
}

int CBreakpointTable::ParseTableEntry(CString& s, char& mask, int& select, t_bpoint& info, int column) {
	switch (column)
	{
		case 0: // 断点地址
		{
			s.Format(L"%08X", info.addr);
			if (g_cpuThreadId) {
				t_thread* t = ThreadHelper::Findthread(g_cpuThreadId);
				if (t) {
					if (t->regvalid && t->reg.ip == info.addr) {
						select = DRAW_EIP;
					}
				}
			}
			break;
		}
		
		case 1: // 所在模块
		{
			t_module* m = ModulesHelper::Findmodule(info.addr);
			s = m->name;
		}

		case 2: // Active
		{

			break;
		}

		case 3: // 反汇编
		{

			break;
		}

		case 4: // 注解
		{

			break;
		}
		default:
			break;
	}

	return s.GetLength();
}

t_bpoint* CBreakpointTable::Findsorteddata(ulong addr) {
	auto it = std::find_if(m_Table.data.info.begin(), m_Table.data.info.end(), [=](auto& p) {
		if (p.addr == addr) {
			return true;
		}
		return false;
		});
	if (it != m_Table.data.info.end())
		return &*it;

	return nullptr;
}
