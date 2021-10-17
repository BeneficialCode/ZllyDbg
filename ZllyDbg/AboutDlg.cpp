// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"


LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	HBITMAP hBt;
	BITMAP  BTTop;
	RECT rect;
	GetClientRect(&rect);
	
	hBt = CreateBitmap(rect.right-rect.left, rect.bottom - rect.top, 0, 24, nullptr);
	hBt = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_GIRL));
	::GetObject(hBt, sizeof(BTTop), &BTTop);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, BTTop.bmWidth, BTTop.bmHeight, SWP_NOMOVE);	//设置窗口大小	
	m_Ripple.InitRipple(m_hWnd, 1, hBt);//初始化水波参数
	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_Ripple.m_nSpeed = 0;
	EndDialog(wID);
	return 0;
}

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Ripple.DropStone(point.x, point.y, 10, 128);
}

void CAboutDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	m_Ripple.DropStone(point.x, point.y, 2, 128);
}
