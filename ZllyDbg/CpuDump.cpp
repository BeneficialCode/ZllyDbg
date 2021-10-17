#include "stdafx.h"
#include "CpuDump.h"

t_dump CDumpWindow::_dump;

LRESULT CDumpWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	_dump.dimmed = 0;
	m_Table.showbar = 1;
	m_Table.hscroll = 1;
	m_Table.scheme = 0;
	m_Table.mode = TABLE_ONTOP | TABLE_HILMENU | TABLE_CPU | TABLE_USERDEF | TABLE_APPMENU | TABLE_USERAPP;
	m_Table.font = 9;
	m_Table.hilite = 0;
	m_Table.data.name = "CPU dump";
	m_Bar.font = 9;
	_dump.menutype = MT_DHISTORY | MT_SEARCH | MT_EDIT | MT_COPY | MT_BACKUP;
	Setdumptype(_dump, DU_HEXTEXT | 0x81);
	SetWindowLong(GWL_USERDATA, (LONG)&_dump);
	ShowScrollBar(SB_HORZ, m_Table.hscroll);
	return 0;
}

LRESULT CDumpWindow::OnEraseBkground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	bHandled = true;
	return 1;
}


LRESULT CDumpWindow::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	PaintTable(m_hWnd);
	return LRESULT();
}

LRESULT CDumpWindow::OnLBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	::PostMessage(GetParent().m_hWnd, WM_USER_FOCUS, 0, (LPARAM)m_hWnd);
	return 0;
}
