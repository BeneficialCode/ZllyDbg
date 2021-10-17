#include "stdafx.h"
#include "CpuAsm.h"
#include "CpuInfo.h"

t_dump CAsmWindow::_dump;

LRESULT CAsmWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	_dump.dimmed = 0;
	_dump.size = 0;
	m_Table.showbar = 1;
	m_Table.hscroll = 1;
	m_Table.mode = TABLE_COPYMENU | TABLE_ONTOP | TABLE_HILMENU | TABLE_CPU | TABLE_USERDEF | TABLE_APPMENU | TABLE_USERAPP;
	m_Table.font = 9;
	m_Table.scheme = 0;
	m_Table.hilite = 0;
	m_Table.data.name = "CPU disasm";
	m_Bar.font = 9;
	Setdumptype(_dump, DU_DISASM | 0x11);
	SetWindowLong(GWL_USERDATA, (LONG)&_dump);
	ShowScrollBar(SB_HORZ, m_Table.hscroll);
	return 0;
}

// 双缓冲加去除BeginPaint带来的EraseBkground闪烁问题
LRESULT CAsmWindow::OnEraseBkground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	bHandled = true;
	return 1;
}

LRESULT CAsmWindow::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	PaintTable(m_hWnd);
	return 0;
}

LRESULT CAsmWindow::OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	// 让父窗口激活当前窗口
	::PostMessage(GetParent().m_hWnd, WM_USER_FOCUS, 0, (LPARAM)m_hWnd);
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CAsmWindow::OnLBtnUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CAsmWindow::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	bHandled = true;
	if (LOWORD(lParam) == 1) {
		return 1;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

LRESULT CAsmWindow::OnNcLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	SendMessage(GetParent().m_hWnd, WM_USER_FOCUS, 0, (LPARAM)m_hWnd);
	return 0;
}

LRESULT CAsmWindow::OnNcRBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	SendMessage(GetParent().m_hWnd, WM_USER_FOCUS, 0, (LPARAM)m_hWnd);
	return 0;
}

LRESULT CAsmWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CAsmWindow::OnUserVabs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	ulong sel0 = _dump.sel0;
	ulong ret = Dumpfunction(m_hWnd, uMsg, wParam, lParam);
	if (sel0 != _dump.sel0) {
		HWND hWnd = CInfoWindow::_dump.hw;
		::InvalidateRect(hWnd, nullptr, false);
	}
	return ret;
}

LRESULT CAsmWindow::OnUserVbyte(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	ulong sel0 = _dump.sel0;
	ulong ret = Dumpfunction(m_hWnd, uMsg, wParam, lParam);
	if (sel0 != _dump.sel0) {
		HWND hWnd = CInfoWindow::_dump.hw;
		::InvalidateRect(hWnd, nullptr, false);
	}
	return ret;
}

LRESULT CAsmWindow::OnUserVrel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	ulong sel0 = _dump.sel0;
	ulong ret = Dumpfunction(m_hWnd, uMsg, wParam, lParam);
	if (sel0 != _dump.sel0) {
		HWND hWnd = CInfoWindow::_dump.hw;
		::InvalidateRect(hWnd, nullptr, false);
	}
	return ret;
}

LRESULT CAsmWindow::OnUserSts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	ulong sel0 = _dump.sel0;
	ulong ret = Dumpfunction(m_hWnd, uMsg, wParam, lParam);
	if (sel0 != _dump.sel0) {
		HWND hWnd = CInfoWindow::_dump.hw;
		::InvalidateRect(hWnd, nullptr, false);
	}
	return ret;
}

LRESULT CAsmWindow::OnUserChgs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	ulong sel0 = _dump.sel0;
	ulong ret = Dumpfunction(m_hWnd, uMsg, wParam, lParam);
	if (sel0 != _dump.sel0) {
		HWND hWnd = CInfoWindow::_dump.hw;
		::InvalidateRect(hWnd, nullptr, false);
	}
	return ret;
}

LRESULT CAsmWindow::OnUserCnts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	ulong sel0 = _dump.sel0;
	ulong ret = Dumpfunction(m_hWnd, uMsg, wParam, lParam);
	if (sel0 != _dump.sel0) {
		HWND hWnd = CInfoWindow::_dump.hw;
		::InvalidateRect(hWnd, nullptr, false);
	}
	return ret;
}

LRESULT CAsmWindow::OnUserDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	if ((_dump.dumptype & DU_BACKUP) == 0
		&& _dump.sel0 >= _dump.base && _dump.size + _dump.base > _dump.sel0) {

	}
	return TRUE;
}

LRESULT CAsmWindow::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	return Tablefunction(m_hWnd, WM_VSCROLL, zDelta >= 0 ? 0 : 1, 0);
}

LRESULT CAsmWindow::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CAsmWindow::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CAsmWindow::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CAsmWindow::OnUserScr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return Tablefunction(m_hWnd, uMsg, wParam, lParam);
}