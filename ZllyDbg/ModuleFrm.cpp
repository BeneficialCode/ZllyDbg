#include "stdafx.h"
#include "resource.h"
#include "ModuleTable.h"
#include "ModuleFrm.h"

CModuleTable* CModuleFrame::m_ModuleTable;

void CModuleFrame::OnFinalMessage(HWND) {
	delete this;
	g_hModuleFrm = NULL;
}

LRESULT CModuleFrame::OnForwardMsg(UINT, WPARAM, LPARAM lParam, BOOL&) {
	LPMSG pMsg = (LPMSG)lParam;
	return CMDIChildWindowImpl<CModuleFrame>::PreTranslateMessage(pMsg);
}

LRESULT CModuleFrame::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&) {
	m_ModuleTable->PaintTable(m_hWnd);
	return LRESULT();
}

LRESULT CModuleFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	return m_ModuleTable->Tablefunction(m_hWnd, WM_VSCROLL, zDelta >= 0 ? 0 : 1, lParam);
}

LRESULT CModuleFrame::OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnLBtnUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnUserSts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnUserScr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnUserVrel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnUserVabs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnUserChgs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CModuleFrame::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_ModuleTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

CModuleFrame::CModuleFrame() {
	BarInfo bars[] = {
		{9,"基址",0},
		{9,"大小",0},
		{19,"入口点",BAR_NOSORT},
		{25,"名称",0},
		{16,"文件版本",BAR_NOSORT},
		{256,"路径",0}
	};

	TableInfo table = {
			1,1,TABLE_ONTOP | TABLE_SAVEPOS | TABLE_APPMENU | TABLE_SORTMENU | TABLE_COPYMENU,9,0,0,0
	};

	m_ModuleTable = new CModuleTable(9, bars, _countof(bars), 0, table);
}
