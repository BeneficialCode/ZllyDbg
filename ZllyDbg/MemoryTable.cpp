#include "stdafx.h"
#include "MemoryTable.h"

CMemoryTable::CMemoryTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table):
	CTable(font, bars, nbar, captured, table){
	SetTableWindowInfo(nbar);
}

int CMemoryTable::ParseTableEntry(CString& s, char& mask, int& select, t_memory& info, int column) {
	switch (column) {
		case 0:// 基址
			s.Format(L"%08X", info.base);
			break;
		case 1:// 大小
			s.Format(L"%08X", info.size);
			break;
		case 2:// 状态
			s = StateToString(info.state);
			break;
		case 3:// 类型
			info.state == MEM_COMMIT ? s = MemoryTypeToString(info.memType) : s = L"";
			break;
		case 4:// 访问类型
			info.state == MEM_COMMIT ? s = ProtectionToString(info.access).c_str() : s = L"";
			break;
		case 5:// 初始访问类型
			info.state == MEM_FREE ? L"" : s = ProtectionToString(info.initAccess).c_str();
			break;
		case 6:// 映射类型
			s = GetDetails(g_hProcess, info).c_str();
			break;
		default:
			break;
	}
	if (info.type & TY_NEW) {
		select |= DRAW_HILITE;
	}
	return s.GetLength();
}

bool CMemoryTable::CompareItems(const t_memory& i1, const t_memory& i2, int col, bool asc) {
	switch (col) {
		case 0: return SortHelper::SortNumbers(i1.base, i2.base,asc);
	}
	return false;
}


const wchar_t* CMemoryTable::MemoryTypeToString(DWORD type) {
	switch (type) {
		case MEM_IMAGE:return L"Image";
		case MEM_MAPPED: return L"Mapped";
		case MEM_PRIVATE: return L"Private";
	}
	ATLASSERT(false);
	return L"";
}

const wchar_t* CMemoryTable::StateToString(DWORD state) {
	switch (state) {
		case MEM_COMMIT: return L"Committed";
		case MEM_RESERVE: return L"Reserved";
		case MEM_FREE: return L"Free";
	}
	ATLASSERT(false);
	return L"";
}

std::wstring CMemoryTable::ProtectionToString(DWORD protection) {
	static const struct {
		PCWSTR Text;
		DWORD Value;
	}prot[] = {
		{L"",0},
		{L"Execute",PAGE_EXECUTE},
		{L"Execute/Read",PAGE_EXECUTE_READ},
		{L"WriteCopy",PAGE_WRITECOPY},
		{L"Execute/Read/Write",PAGE_EXECUTE_READWRITE},
		{L"Execute/WriteCopy",PAGE_EXECUTE_WRITECOPY},
		{L"No Access",PAGE_NOACCESS},
		{L"Read",PAGE_READONLY},
		{L"Read/Write",PAGE_READWRITE},
	};

	std::wstring text = std::find_if(std::begin(prot), std::end(prot), [protection](auto& p) {
		return p.Value == (protection & 0xff);
		})->Text;

	static const struct {
		PCWSTR Text;
		DWORD Value;
	}extra[] = {
		{L"Gurad",PAGE_GUARD},
		{L"No Cache",PAGE_NOCACHE},
		{L"Write Combine",PAGE_WRITECOMBINE},
		{L"Targets Invalid",PAGE_TARGETS_INVALID},
		{L"Targets No Update",PAGE_TARGETS_NO_UPDATE},
	};

	std::for_each(std::begin(extra), std::end(extra), [&text, protection](auto& p) {
		if (p.Value & protection)
			((text += L"/") += p.Text);
	});

	return text;
}

std::wstring CMemoryTable::GetDetails(HANDLE hProcess, t_memory& mem) {
	if (mem.state != MEM_COMMIT)
		return L"";

	if (mem.memType == MEM_IMAGE || mem.memType == MEM_MAPPED) {
		wchar_t path[MAX_PATH];
		if (::GetMappedFileName(hProcess, (LPVOID)mem.base, path, sizeof(path)) > 0) {
			return GetDosNameFromNtName(path);
		}
	}
	return L"";
}

std::wstring CMemoryTable::GetDosNameFromNtName(PCWSTR name) {
	static std::vector<std::pair<std::wstring, std::wstring>> deviceNames;
	static bool first = true;
	if (first) {
		auto drives = ::GetLogicalDrives();
		int drive = 0;
		while (drives) {
			if (drives & 1) {
				// drive exists
				WCHAR driveName[] = L"X:";
				driveName[0] = (WCHAR)(drive + 'A');
				WCHAR path[MAX_PATH];
				if (::QueryDosDevice(driveName, path, MAX_PATH)) {
					deviceNames.push_back({ path,driveName });
				}
			}
			drive++;
			drives >>= 1;
		}
		first = false;
	}

	for (auto& [ntName, dosName] : deviceNames) {
		if (::_wcsnicmp(name, ntName.c_str(), ntName.size()) == 0)
			return dosName + (name + ntName.size());
	}
	return L"";
}

t_memory* CMemoryTable::Findsorteddata(ulong addr) {
	auto it = std::find_if(m_Table.data.info.begin(), m_Table.data.info.end(), [=](auto& p) {
		if (addr>=p.base && addr<p.base + p.size) {
			return true;
		}
		return false;
		});
	if (it != m_Table.data.info.end())
		return &*it;

	return nullptr;
}