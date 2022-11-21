// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "AttachDlg.h"

#include "ThreadTable.h"
#include "MemoryTable.h"

#include "ChildFrm.h"
#include "ThreadFrm.h"
#include "MemoryFrm.h"
#include "ModuleFrm.h"
#include "CpuFrm.h"
#include "MainFrm.h"
#include "BreakpointFrm.h"

#include "FileDlg.h"

#include "DbgEngine.h"
#include "ModuleHelper.h"
#include "DebugHelper.h"

extern DbgInfo g_DbgInfo;

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) {
	if (CMDIFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	HWND hWnd = MDIGetActive();
	if (hWnd != NULL)
		return (BOOL)::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);

	return FALSE;
}

BOOL CMainFrame::OnIdle() {
	UIUpdateToolBar();
	return FALSE;
}

void CMainFrame::InitDbgUI() {
	CModuleFrame* pModule = new CModuleFrame;
	g_hModuleFrm = pModule->CreateEx(m_hWndClient, nullptr, nullptr, CTable<t_module>::_style);

	CThreadFrame* pThread= new CThreadFrame;
	g_hThreadFrm = pThread->CreateEx(m_hWndClient, nullptr, nullptr, CTable<t_thread>::_style);

	CMemoryFrame* pMemory = new CMemoryFrame;
	g_hMemoryFrm = pMemory->CreateEx(m_hWndClient, nullptr, nullptr, CTable<t_thread>::_style);

	CBreakpointFrame* pBreakpoint = new CBreakpointFrame;
	g_hBpFrm = pBreakpoint->CreateEx(m_hWndClient, nullptr, nullptr, CTable<t_bpoint>::_style);

	CCPUFrame* pCPU = new CCPUFrame;
	g_hCpuFrm = pCPU->CreateEx(m_hWndClient,nullptr,nullptr,
		WS_CHILD|WS_GROUP|WS_TABSTOP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE|
	WS_SYSMENU|WS_THICKFRAME|WS_CAPTION);
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	int anPanes[] = { ID_DEFAULT_PANE,ID_STATUS_PANE };
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	m_hWndStatusBar = m_wndStatusBar.Create(*this);
	UIAddStatusBar(m_hWndStatusBar);

	m_wndStatusBar.SetPanes(anPanes, 2, true);

	CreateMDIClient();
	m_CmdBar.SetMDIClient(m_hWndMDIClient);
	g_hWndMDIClient = m_hWndMDIClient;

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	/*CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);*/

	InitDbgUI();
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	//m_CmdBar.AttachMenu(NULL);

	// unregister message filtering and idle updates
	/*CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);*/

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CChildFrame* pChild = new CChildFrame;
	pChild->CreateEx(m_hWndClient);
	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CFileDialogEx dlg(TRUE, L".exe", nullptr, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST|OFN_ENABLEHOOK|OFN_ENABLETEMPLATE,
		L"Executables (*.exe)\0*.exe\0AllFiles (*.*)\0*.*\0", *this);
	if (dlg.DoModal() == IDOK) {
		wcscpy_s(g_DbgInfo.ExePath, dlg.m_szFileName);
		CString curDir = dlg.m_szFileName;
		int pos = curDir.ReverseFind(L'\\') + 1;
		dlg.m_szFileName[pos] = L'\0';
		curDir = dlg.m_szFileName;

		g_DbgInfo.dbgType = DBG_CREATE;
		/*HANDLE hThread = ::CreateThread(nullptr, 0, DebugThread, nullptr, 0, nullptr);
		CloseHandle(hThread);*/
		STARTUPINFO si{ sizeof(si) };
		PROCESS_INFORMATION pi;
		if (::CreateProcess(nullptr, g_DbgInfo.ExePath, 0, 0, 0,
			// 只调试被调试进程，不调试它的子进程。被调试程序在一个新的控制台窗口中输出信息
			DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS | CREATE_DEFAULT_ERROR_MODE,
			0, curDir, &si, &pi)) {
			g_hProcess = pi.hProcess;
			g_hMainThread = pi.hThread;
			g_DbgInfo.pid = pi.dwProcessId;
			g_mainThreadId = pi.dwThreadId;
			DbgEngine::SetStatus(STAT_RUNNING);
		}
	}
	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	MDICascade();
	return 0;
}

LRESULT CMainFrame::OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	MDITile();
	return 0;
}

// Windows 常用消息及含义
// https://www.jianshu.com/p/e77677a860c5 
LRESULT CMainFrame::OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	MDIIconArrange();
	return 0;
}

LRESULT CMainFrame::OnViewModule(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// TODO: add code to initialize document

	if (g_hModuleFrm != NULL) {
		::InvalidateRect(g_hModuleFrm, NULL, FALSE);
	}
	else {
		CModuleFrame* pChild = new CModuleFrame;
		g_hModuleFrm = pChild->CreateEx(m_hWndClient, nullptr, nullptr, CTable<t_module>::_style);
	}
	if (g_hModuleFrm) {
		if(g_hProcess!=INVALID_HANDLE_VALUE)
			ModulesHelper::Listmodule();
		::SendMessage(m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)g_hModuleFrm, 0);
		if (!::IsZoomed(g_hModuleFrm)) {
			::ShowWindow(g_hModuleFrm, SW_RESTORE);
		}
		::SetFocus(g_hModuleFrm);
	}
	return 0;
}

LRESULT CMainFrame::OnFileAttach(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CAttachDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnViewThread(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if (g_hThreadFrm != NULL) {
		::InvalidateRect(g_hThreadFrm, NULL, FALSE);
	}
	else {
		CThreadFrame* pChild = new CThreadFrame;
		g_hThreadFrm = pChild->CreateEx(m_hWndClient, nullptr, nullptr, CTable<t_thread>::_style);
	}
	if (g_hThreadFrm) {
		::SendMessage(m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)g_hThreadFrm, 0);
		if (!::IsZoomed(g_hThreadFrm)) {
			::ShowWindow(g_hThreadFrm, SW_RESTORE);
		}
		::SetFocus(g_hThreadFrm);
	}
	return 0;
}

LRESULT CMainFrame::OnViewMemory(WORD, WORD, HWND, BOOL&) {
	if (g_hMemoryFrm != NULL) {
		::InvalidateRect(g_hMemoryFrm, NULL, FALSE);
	}
	else {
		CMemoryFrame* pChild = new CMemoryFrame;
		g_hMemoryFrm = pChild->CreateEx(m_hWndClient, nullptr, nullptr, CTable<t_thread>::_style);
	}
	if (g_hMemoryFrm) {
		::SendMessage(m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)g_hMemoryFrm, 0);
		if (!::IsZoomed(g_hMemoryFrm)) {
			::ShowWindow(g_hMemoryFrm, SW_RESTORE);
		}
		::SetFocus(g_hMemoryFrm);
	}
	return LRESULT();
}

LRESULT CMainFrame::OnViewCpu(WORD, WORD, HWND, BOOL&) {
	if (g_hCpuFrm != NULL) {
		::InvalidateRect(g_hCpuFrm, NULL, FALSE);
	}
	else {
		CCPUFrame* pChild = new CCPUFrame;
		g_hCpuFrm = pChild->CreateEx(m_hWndClient);
	}
	if (g_hCpuFrm) {
		::SendMessage(m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)g_hCpuFrm, 0);
		::PostMessage(g_hCpuFrm, WM_USER_SIZE, 0, 0);
		if (!::IsZoomed(g_hCpuFrm)) {
			::ShowWindow(g_hCpuFrm, SW_RESTORE);
		}
		::SetFocus(g_hCpuFrm);
	}
	return LRESULT();
}

LRESULT CMainFrame::OnViewBreakpoint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if (g_hBpFrm != NULL) {
		::InvalidateRect(g_hBpFrm, NULL, FALSE);
	}
	else {
		CBreakpointFrame* pChild = new CBreakpointFrame;
		g_hBpFrm = pChild->CreateEx(m_hWndClient);
	}
	if (g_hBpFrm) {
		::SendMessage(m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)g_hBpFrm, 0);
		::PostMessage(g_hBpFrm, WM_USER_SIZE, 0, 0);
		if (!::IsZoomed(g_hBpFrm)) {
			::ShowWindow(g_hBpFrm, SW_RESTORE);
		}
		::SetFocus(g_hBpFrm);
	}
	return LRESULT();
}

LRESULT CMainFrame::OnDbgStepOver(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	DebugHelper::Animate(ANIMATE_OFF);
	DebugHelper::Go(0, 0, STEP_OVER, 0, 1);
	return LRESULT();
}