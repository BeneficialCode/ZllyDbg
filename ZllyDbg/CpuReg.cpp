#include "stdafx.h"
#include "CpuReg.h"

RegisterMode CRegWindow::_regMode = RegisterMode::Fpu;

t_dump CRegWindow::_dump;

LRESULT CRegWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	m_Table.showbar = 1;
	m_Table.hscroll = 0;
	m_Table.scheme = 0;
	m_Table.mode = TABLE_ONTOP | TABLE_CPU | TABLE_USERDEF | TABLE_APPMENU | TABLE_USERAPP | TABLE_NOHSCR;
	m_Table.font = 9;
	m_Table.hilite = 0;
	m_Table.data.name = "CPU registers";
	m_Bar.font = 9;
	_dump.menutype = 0x8000;
	m_Bar.defdx[0] = 256;
	SetRegisterMode(_regMode);
	m_Bar.mode[0] = BAR_BUTTON|BAR_SHIFTSEL;
	m_Bar.nbar = 1;
	Defaultbar();
	m_Bar.captured = 0;
	return 0;
}

void CRegWindow::SetRegisterMode(RegisterMode mode) {
	switch (mode) {
		case RegisterMode::MMX:
			m_Bar.name[0] = ">>> Register <MMX>";
			break;
		case RegisterMode::ThreeDNow:
			m_Bar.name[0] = ">>> Reister <3D Now!>";
			break;
		case RegisterMode::DebugRegister:
			m_Bar.name[0] = ">>> Debug Registers";
			break;
		default:
			m_Bar.name[0] = ">>> Register <Fpu>";
			break;
	}
	std::string name = "         <        <        <        <        <        <        <    ";
	m_Bar.name[0] += name;
	_regMode = mode;
	m_Table.xshift = 0;
}

LRESULT CRegWindow::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	PaintTable(m_hWnd);
	return LRESULT();
}

LRESULT CRegWindow::OnLBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	::PostMessage(GetParent().m_hWnd, WM_USER_FOCUS, 0, (LPARAM)m_hWnd);
	return 0;
}

LRESULT CRegWindow::OnEraseBkground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	bHandled = true;
	return 1;
}