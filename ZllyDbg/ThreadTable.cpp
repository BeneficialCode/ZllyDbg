#include "stdafx.h"

#include "ThreadTable.h"
#include "DebugHelper.h"
#include "MemoryHelper.h"
#include <array>

CThreadTable::CThreadTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table) :
	CTable(font, bars, nbar, captured, table) {
	SetTableWindowInfo(nbar);
}

CString CThreadTable::GetErrorText(DWORD value) {
	CString text;
	std::array<WCHAR, 1024> buffer;
	if (::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
		::GetModuleHandle(L"ntdll"), value,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer.data(), buffer.size(), nullptr)) {
		auto len = wcslen(buffer.data());
		buffer.data()[len - 2] = L'\0';
		text.Format(L"%s: (0x%X)", buffer.data(), value);
	}
	else {
		text = L"Error number not found";
	}
	return text;
}

int CThreadTable::ParseTableEntry(CString& s, char& mask, int& select, t_thread& info, int column) {
	FILETIME create, kernel, user, exit;
	if (::GetThreadTimes(info.handle, &create, &exit, &kernel, &user)) {
		switch (column) {
			case 0:// 线程id
				s.Format(L"%08X", info.threadid);
				if (info.type & TY_MAIN) {
					s += L" main";
					select |= DRAW_HILITE;
				}
				break;
			case 1:// 线程入口点
				s.Format(L"%08X", info.entry);
				break;
			case 2:// 线程数据块
				s.Format(L"%08X", info.datablock);
				break;
			case 3:// Last Error
			{
				DWORD error = MemoryHelper::Readmemory<DWORD>(info.datablock+0x34);
				s = GetErrorText(error);
				break;
			}
			case 4:// 状态
			{
				int preCount = ::SuspendThread(info.handle);
				int diff = preCount - info.suspendcount;
				::ResumeThread(info.handle);
				if (preCount > 0 || diff > 0) {
					if (preCount > 0 || diff <= 0) {
						if (preCount <= 0 || diff > 0) {
							s = L"Suspend";
						}
						else {
							s = L"Paused";
						}
					}
					else {
						s = L"Traced";
					}
				}
				else {
					s = L"Active";
				}
				break;
			}
			case 5:// 线程优先级
			{
				DWORD priorityClass = GetPriorityClass(g_hProcess);
				DWORD threadPriority = ::GetThreadPriority(info.handle);
				if (threadPriority >= 0) {
					s.Format(L"%3i + %i", priorityClass, threadPriority);
				}
				else {
					s.Format(L"%3i - %i", priorityClass, threadPriority);
				}
				break;
			}
			case 6:// 开始时间
			{
				s = CTime(create).Format(L"%x %X");
				break;
			}
			case 7:// CPU time
			{
				DWORD cpuTime = DWORD((*(ULONGLONG*)&kernel + *(ULONGLONG*)&user) / 10000000);
				s = CTimeSpan(cpuTime).Format(L"%D:%H:%M:%S");
				break;
			}
			default:
				break;
		}
		if (info.type & TY_NEW)
			select = DRAW_HILITE;
	}

	return s.GetLength();
}

bool CThreadTable::CompareItems(const t_thread& i1, const t_thread& i2, int col, bool asc) {
	switch (col) {
		case 0:return SortHelper::SortNumbers(i1.threadid, i2.threadid, asc);
		case 1:return SortHelper::SortNumbers(i1.entry, i2.entry, asc);
		case 2:return SortHelper::SortNumbers(i1.teb, i2.teb, asc);
	}
	return false;
}

t_thread* CThreadTable::Findsorteddata(ulong threadid) {
	auto it = std::find_if(m_Table.data.info.begin(), m_Table.data.info.end(), [=](auto& p) {
		if (p.threadid == threadid) {
			return true;
		}
		return false;
		});
	if (it != m_Table.data.info.end())
		return &*it;

	return nullptr;
}