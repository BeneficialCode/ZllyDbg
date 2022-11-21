#include "stdafx.h"
#include "resource.h"
#include "ProcessTable.h"
#include "DbgEngine.h"
#include "AttachDlg.h"

#pragma comment(lib,"psapi.lib")

#define IDC_VIEW_PROCESS	0xDAD

DbgInfo g_DbgInfo;

LRESULT CAttachDlg::OnAttach(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	int row = m_ProcTable->m_Table.data.selected;
	auto process = m_ProcTable->Getsortedbyselection(row);
	g_DbgInfo.pid = process.Pid;
	g_DbgInfo.dbgType = DBG_ATTACH;
	
	// _hThread.reset(::CreateThread(nullptr, 0, DebugThread, &g_DbgInfo, 0, nullptr));
	::DebugActiveProcess(g_DbgInfo.pid);
	g_Status = STAT_RUNNING;
	g_isNeedListmodule = true;

	EndDialog(0);

	return 0;
}

LRESULT CAttachDlg::OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	EndDialog(-1);
	return 0;
}

LRESULT CAttachDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	BarInfo bars[] = {
		{9,"Process",0},
		{35,"Name",0},
		{256,"Path",0}
	};

	TableInfo table = {
		1,1,TABLE_SORTMENU | TABLE_COPYMENU | TABLE_APPMENU,9,0,0,0
	};

	m_ProcTable = new CProcessTable(9, bars, 3, 0, table);
	RECT rect { -1000,-1000,10,10 };
	HWND hWndAttachProc = m_ProcTable->Create(m_hWnd, rect, nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_BORDER,
		0, IDC_VIEW_PROCESS);

	if (hWndAttachProc) {
		GetClientRect(&rect);
		int iHorizontalUnit = LOWORD(GetDialogBaseUnits()) / 2;
		int iVerticalUnit = LOWORD(GetDialogBaseUnits()) / 4;
		HWND hAttachBtn = GetDlgItem(IDC_ATTACH);
		RECT attachRect;
		::GetWindowRect(hAttachBtn, &attachRect);
		int height = attachRect.bottom - attachRect.top;
		int width = attachRect.right - attachRect.left;
		int iY = rect.bottom - height - iVerticalUnit;
		int iX = rect.right - 2 * width - 2 * iHorizontalUnit;
		::MoveWindow(hAttachBtn, iX, iY, width, height, false);
		iX = rect.right - width - iHorizontalUnit;
		HWND hCancelBtn = GetDlgItem(IDC_CANCEL);
		::MoveWindow(hCancelBtn, iX, iY, width, height, false);
		height = rect.bottom - height - 3 * iHorizontalUnit;
		width = rect.right - 2 * iHorizontalUnit;
		HWND hWndChild = GetDlgItem(IDC_VIEW_PROCESS);
		::MoveWindow(hWndChild, iHorizontalUnit, iVerticalUnit, width, height, true);
		::ShowScrollBar(hWndAttachProc, SB_HORZ, table.hscroll);
		::SetFocus(hWndAttachProc);
		MyEnumProcess();
		::InvalidateRect(hWndAttachProc, nullptr, false);
	}
	return TRUE;
}

LRESULT CAttachDlg::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	RECT rect, attachRect;
	GetClientRect(&rect);
	int iHorizontalUnit = LOWORD(GetDialogBaseUnits());
	int iVerticalUnit = HIWORD(GetDialogBaseUnits());
	GetDlgItem(IDC_ATTACH).GetWindowRect(&attachRect);
	int height = attachRect.bottom - attachRect.top;
	int width = attachRect.right - attachRect.left;
	int iY = rect.bottom - height - iVerticalUnit;
	int iX = rect.right - 2 * width - 2 * iHorizontalUnit;
	HWND hWndAttachBtn = GetDlgItem(IDC_ATTACH);
	::MoveWindow(hWndAttachBtn, iX, iY, width, height, false);
	HWND hWndCancelBtn = GetDlgItem(IDC_CANCEL);
	iX = rect.right - width - iHorizontalUnit;
	::MoveWindow(hWndCancelBtn, iX, iY, width, height, false);
	height = rect.bottom - height - 3 * iVerticalUnit;
	width = rect.right - 2 * iHorizontalUnit;
	HWND hWndChild = GetDlgItem(IDC_VIEW_PROCESS);
	::MoveWindow(hWndChild, iHorizontalUnit, iVerticalUnit, width, height, true);
	RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
	return TRUE;
}



LRESULT CAttachDlg::OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
	if ((wParam & 0xFFF0) == SC_CLOSE) {
		EndDialog(-1);
	}
	bHandled = false;
	return 0;
}

int CAttachDlg::MyEnumProcess() {
	int status = 0;
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	DWORD curPID = GetCurrentProcessId();
	wil::unique_handle hSnapshot;
	hSnapshot.reset(::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));
	if (INVALID_HANDLE_VALUE == hSnapshot.get())
		status = -1;
	WCHAR fullPath[MAX_PATH];

	if (!status) {
		Process32First(hSnapshot.get(), &pe);
		while (Process32Next(hSnapshot.get(), &pe)) {
			if (pe.th32ProcessID != curPID) {
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pe.th32ProcessID);
				if (hProcess != NULL) {
					t_process element;
					element.Pid = pe.th32ProcessID;
					::GetModuleFileNameEx(hProcess, NULL, fullPath, MAX_PATH);
					element.Path = fullPath;
					element.ProcessName = pe.szExeFile;
					// m_ProcTable->m_Table.data.info.push_back(element);
					m_ProcTable->Addsorteddata(element);
					CloseHandle(hProcess);
					hProcess = NULL;
				}
			}
		}
		m_ProcTable->m_Table.data.n = m_ProcTable->m_Table.data.info.size();
		status = m_ProcTable->m_Table.data.n;
	}
	return status;
}
