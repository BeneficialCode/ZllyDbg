#include "stdafx.h"
#include "resource.h"

#include "ThreadTable.h"
#include "ThreadFrm.h"

CThreadTable*  CThreadFrame::m_ThreadTable;

void CThreadFrame::OnFinalMessage(HWND /*hWnd*/) {
	delete this;
	g_hThreadFrm = NULL;
}

LRESULT CThreadFrame::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	LPMSG pMsg = (LPMSG)lParam;

	return CMDIChildWindowImpl<CThreadFrame>::PreTranslateMessage(pMsg);
}

LRESULT CThreadFrame::OnPaint(UINT, WPARAM, LPARAM, BOOL&) {
	m_ThreadTable->PaintTable(m_hWnd);
	return 0;
}

CThreadFrame::CThreadFrame() {
	BarInfo bars[] = {
		{9,"线程id",0},
		{9,"入口点",0},
		{9,"数据块",0},
		{27,"错误信息",BAR_NOSORT},
		{10,"状态",BAR_NOSORT},
		{9,"优先级",BAR_NOSORT},
		{20,"开始时间",BAR_NOSORT},
		{12,"cpu时间",BAR_NOSORT}
	};

	TableInfo table = {
		1,false,TABLE_ONTOP|TABLE_SAVEPOS|TABLE_APPMENU|TABLE_SORTMENU|TABLE_COPYMENU,9,0,0,0
	};

	m_ThreadTable = new CThreadTable(9, bars, 8, 0, table);
}

LRESULT CThreadFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CThreadFrame::OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CThreadFrame::OnLBtnUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CThreadFrame::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CThreadFrame::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CThreadFrame::OnUserSts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CThreadFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	SetTimer(2, 500, nullptr);
	return 0;
}

LRESULT CThreadFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	KillTimer(2);
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CThreadFrame::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if (wParam == 2) {
		::InvalidateRect(g_hThreadFrm, nullptr, false);
	}
	return 0;
}

LRESULT CThreadFrame::OnUserVabs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}		
		
LRESULT CThreadFrame::OnUserVrel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}		
		
LRESULT CThreadFrame::OnUserChgs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ThreadTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}		

LRESULT CThreadFrame::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	return m_ThreadTable->Tablefunction(m_hWnd, WM_VSCROLL, zDelta >= 0 ? 0 : 1, lParam);
}
