#include "stdafx.h"
#include "CpuStack.h"

t_dump CStackWindow::_dump;

LRESULT CStackWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	_dump.dimmed = 1;
	m_Table.showbar = 1;
	m_Table.hscroll = 1;
	m_Table.scheme = 0;
	m_Table.mode = TABLE_ONTOP | TABLE_CPU | TABLE_USERDEF | TABLE_APPMENU | TABLE_USERAPP;
	m_Table.font = 9;
	m_Table.hilite = 0;
	m_Table.data.name = "CPU stack";
	m_Bar.font = 9;
	_dump.menutype = 0x8000;
	m_Bar.mode[0] = BAR_BUTTON;
	Setdumptype(_dump, DU_ADDR | 0x14);
	SetWindowLong(GWL_USERDATA, (LONG)&_dump);
	ShowScrollBar(SB_HORZ, m_Table.hscroll);
	return 0;
}

LRESULT CStackWindow::OnEraseBkground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	bHandled = true;
	return 1;
}

LRESULT CStackWindow::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	PaintTable(m_hWnd);
	return 0;
}

LRESULT CStackWindow::OnLBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	::PostMessage(GetParent().m_hWnd, WM_USER_FOCUS, 0, (LPARAM)m_hWnd);
	return 0;
}
