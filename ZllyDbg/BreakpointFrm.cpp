#include "stdafx.h"
#include "resource.h"
#include "BreakpointFrm.h"

CBreakpointTable* CBreakpointFrame::m_BreakpointTable;

void CBreakpointFrame::OnFinalMessage(HWND /*hWnd*/) {
	delete this;
	g_hThreadFrm = NULL;
}

LRESULT CBreakpointFrame::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	LPMSG pMsg = (LPMSG)lParam;

	return CMDIChildWindowImpl<CBreakpointFrame>::PreTranslateMessage(pMsg);
}

LRESULT CBreakpointFrame::OnPaint(UINT, WPARAM, LPARAM, BOOL&) {
	m_BreakpointTable->PaintTable(m_hWnd);
	return 0;
}

CBreakpointFrame::CBreakpointFrame() {
	BarInfo bars[] = {
		{9,"¶ÏµãµØÖ·",BAR_NOSORT},
		{9,"Ä£¿é",BAR_NOSORT},
		{25,"Active",BAR_NOSORT},
		{30,"·´»ã±à",BAR_NOSORT},
		{40,"×¢ÊÍ",BAR_NOSORT},
	};

	TableInfo table = {
		1,false,TABLE_ONTOP | TABLE_SAVEPOS | TABLE_APPMENU | TABLE_SORTMENU | TABLE_COPYMENU,9,0,0,0
	};


	m_BreakpointTable = new CBreakpointTable(9, bars, _countof(bars), 0, table);
}

LRESULT CBreakpointFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnLBtnUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnUserSts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	SetTimer(2, 500, nullptr);
	return 0;
}

LRESULT CBreakpointFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	KillTimer(2);
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if (wParam == 2) {
		::InvalidateRect(g_hThreadFrm, nullptr, false);
	}
	return 0;
}

LRESULT CBreakpointFrame::OnUserVabs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnUserVrel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnUserChgs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_BreakpointTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CBreakpointFrame::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	return m_BreakpointTable->Tablefunction(m_hWnd, WM_VSCROLL, zDelta >= 0 ? 0 : 1, wParam);
}