#pragma once
#include "Table.h"

class ModulesHelper final abstract{
public:
	static int Listmodule();
	static t_module* Findmodule(ulong addr);
	static uchar* Finddecode(ulong addr, ulong* psize);
	static int AnalyseModule(t_module* m);
};