#include "stdafx.h"
#include "ModuleTable.h"
#include "ModuleHelper.h"
#include "resource.h"
#include "ModuleFrm.h"
#include "PEParser.h"

bool g_isNeedListmodule = false;

int ModulesHelper::Listmodule() {
	std::unique_ptr<HMODULE[]> hModules;
	int maxCount = 256;
	int count = 0;
	t_module t;

	if (!g_isNeedListmodule)
		return -1;

	if (g_hProcess) {
		for (;;) {
			hModules = std::make_unique<HMODULE[]>(maxCount);
			DWORD actualSize;
			if (!::EnumProcessModules(g_hProcess, hModules.get(), maxCount * sizeof(HMODULE),&actualSize))
				break;

			count = actualSize / sizeof(HMODULE);
			if (count < maxCount)
				break;
			maxCount *= 2;
		}

		for (int i = 0; i < count; i++) {
			HMODULE hModule = hModules[i];
			MODULEINFO moduleInfo;
			if (GetModuleInformation(g_hProcess, hModule, &moduleInfo, sizeof(MODULEINFO))) {
				WCHAR path[260];
				if (GetModuleFileNameEx(g_hProcess, hModule, path, MAX_PATH)) {
					t.path = path;
					t.name = t.path.Mid(t.path.ReverseFind(L'\\') + 1);
					t.base = reinterpret_cast<ulong>(moduleInfo.lpBaseOfDll);
					t.size = moduleInfo.SizeOfImage;
					t.entry = reinterpret_cast<ulong>(moduleInfo.EntryPoint);
					t.type = TY_NORMAL;
					AnalyseModule(&t);
					CModuleFrame::m_ModuleTable->Addsorteddata(t);
				}
			}
		}
	}
	return 0;
}

t_module* ModulesHelper::Findmodule(ulong addr) {
	return CModuleFrame::m_ModuleTable->Findsorteddata(addr);
}

uchar* ModulesHelper::Finddecode(ulong addr, ulong* psize) {
	t_module* m = nullptr;

	m = Findmodule(addr);
	if (m && m->codedec && addr >= m->codebase && addr < m->codesize + m->codebase) {
		if (psize) {
			*psize = m->codesize + m->codebase - addr;
		}
		return (uchar*)&m->codedec[addr - m->codebase];
	}
	else {
		if (psize)
			*psize = 0;
		return 0;
	}
}

int ModulesHelper::AnalyseModule(t_module* m) {
	int error = 0;
	ulong base = m->base;

	//m->type = TY_CONFIRMED|TY_NEW;
	if (m->base == (ulong)g_hMainModule)
		m->type |= TY_MAIN;
	
	PEParser parser(m->path);

	error = parser.IsValid() ? 0 : 2;

	if (!error) {
		bool isPE64 = parser.IsPe64();
		m->codebase = base + (isPE64 ? parser.GetOptionalHeader64().BaseOfCode : parser.GetOptionalHeader32().BaseOfCode);
		m->codesize = isPE64 ? parser.GetOptionalHeader64().SizeOfCode : parser.GetOptionalHeader32().SizeOfCode;
		m->nsect = parser.GetSectionCount();
		if (m->nsect > 0 && m->nsect < 0x2000)
			m->sect = (IMAGE_SECTION_HEADER*)VirtualAlloc(nullptr, sizeof(IMAGE_SECTION_HEADER) * m->nsect, MEM_COMMIT,
				PAGE_READWRITE);
		if (m->sect) {
			memcpy_s(m->sect, sizeof(IMAGE_SECTION_HEADER) * m->nsect, parser.GetSections(), sizeof(IMAGE_SECTION_HEADER) * m->nsect);
		}
		m->headersize = parser.GetOptionalHeader32().SizeOfHeaders;
		m->database = base + parser.GetOptionalHeader32().BaseOfData;
		m->fixupbase = parser.GetOptionalHeader32().ImageBase;

		auto dir = parser.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_EXPORT);
		m->edatatable = dir->VirtualAddress + base;
		m->edatasize = dir->Size;
		dir = parser.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_IMPORT);
		m->idatatable = dir->VirtualAddress + base;
		dir = parser.GetDataDirectory(IMAGE_DIRECTORY_ENTRY_BASERELOC);
		m->reloctable = dir->VirtualAddress + base;
		m->relocsize = dir->Size;
	}
	return error;
}