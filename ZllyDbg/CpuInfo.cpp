#include "stdafx.h"
#include "CpuInfo.h"

t_dump CInfoWindow::_dump;

LRESULT CInfoWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	m_Table.showbar = 1;
	m_Table.hscroll = 0;
	m_Table.scheme = 0;
	m_Table.mode = TABLE_ONTOP | TABLE_CPU | TABLE_USERDEF | TABLE_APPMENU | TABLE_USERAPP| TABLE_NOHSCR;
	m_Table.font = 9;
	m_Table.hilite = 0;
	m_Table.data.name = "CPU info";
	m_Bar.font = 9;
	_dump.menutype = 0x8000;
	m_Bar.defdx[0] = 256;
	m_Bar.name[0] = "Information";
	m_Bar.mode[0] = BAR_NOSORT;
	m_Bar.nbar = 1;
	Defaultbar();
	m_Bar.captured = 0;
	return 0;
}

LRESULT CInfoWindow::OnEraseBkground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	bHandled = true;
	return 1;
}

LRESULT CInfoWindow::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	PaintTable(m_hWnd);
	return LRESULT();
}

LRESULT CInfoWindow::OnLBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	::PostMessage(GetParent().m_hWnd, WM_USER_FOCUS, 0, (LPARAM)m_hWnd);
	return 0;
}