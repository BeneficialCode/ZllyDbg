// ChildFrm.cpp : implementation of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "ChildFrm.h"

void CChildFrame::OnFinalMessage(HWND /*hWnd*/)
{
	delete this;
}

LRESULT CChildFrame::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LPMSG pMsg = (LPMSG)lParam;

	return CMDIChildWindowImpl<CChildFrame>::PreTranslateMessage(pMsg);
}
