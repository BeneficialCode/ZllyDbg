#include "stdafx.h"
#include "resource.h"

#include "MemoryTable.h"
#include "MemoryFrm.h"

CMemoryTable* CMemoryFrame::m_MemoryTable;
void CMemoryFrame::OnFinalMessage(HWND /*hWnd*/) {
	delete this;
	g_hMemoryFrm = NULL;
}

LRESULT CMemoryFrame::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	LPMSG pMsg = (LPMSG)lParam;

	return CMDIChildWindowImpl<CMemoryFrame>::PreTranslateMessage(pMsg);
}

LRESULT CMemoryFrame::OnPaint(UINT, WPARAM, LPARAM, BOOL&) {
	m_MemoryTable->PaintTable(m_hWnd);
	return LRESULT();
}

CMemoryFrame::CMemoryFrame() {
	BarInfo bars[] = {
		{9,"地址",0},
		{9,"大小",0},
		{12,"状态",BAR_NOSORT},
		{9,"类型",BAR_NOSORT},
		{18,"访问属性",BAR_NOSORT},
		{20,"初始访问属性",BAR_NOSORT},
		{256,"映射关系",BAR_NOSORT},
	};

	TableInfo table = {
		1,true,TABLE_ONTOP | TABLE_SAVEPOS | TABLE_APPMENU | TABLE_SORTMENU | TABLE_COPYMENU,9,0,0,0
	};

	m_MemoryTable = new CMemoryTable(9, bars, _countof(bars),0,table);
}

LRESULT CMemoryFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	return m_MemoryTable->Tablefunction(m_hWnd, WM_VSCROLL, zDelta >= 0 ? 0 : 1, lParam);
}

LRESULT CMemoryFrame::OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnLBtnUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnUserSts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnUserScr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnUserVrel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnUserVabs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnUserChgs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CMemoryFrame::OnUserDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	int selAddr = m_MemoryTable->m_Table.data.selected;
	int i = 0;
	if (selAddr > 0) {
		t_memory t = m_MemoryTable->Getsortedbyselection(selAddr);
		switch (t.memType) {
			default:
				break;
		}
	}
	return 1;
}

LRESULT CMemoryFrame::OnLBtnDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	return m_MemoryTable->Tablefunction(m_hWnd, uMsg, wParam, lParam);
}

