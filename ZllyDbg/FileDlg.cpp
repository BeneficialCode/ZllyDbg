#include "stdafx.h"
#include "FileDlg.h"
#include "resource.h"
#include "DbgEngine.h"

extern DbgInfo g_DbgInfo;

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
	LPCTSTR lpszDefExt,
	LPCTSTR lpszFileName ,
	DWORD dwFlags,
	LPCTSTR lpszFilter, HWND hWndParent) :CFileDialogImpl<CFileDialogEx>(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags,
		lpszFilter, hWndParent) {
	m_ofn.lpfnHook = OFNHookProc;
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_OFNHOOK_ARG);
}

UINT_PTR CALLBACK OFNHookProc(
	_In_ HWND hdlg,
	_In_ UINT uiMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	switch (uiMsg) {
		case WM_INITDIALOG:
		{
			SendDlgItemMessage(hdlg, IDC_COMBO, CB_SETEXTENDEDUI, TRUE, 0);
			SendDlgItemMessage(hdlg, IDC_COMBO, CB_LIMITTEXT, 0xFFF, 0);
			break;
		}
		case WM_NOTIFY:
		{
			auto nmhdr = (NMHDR*)lParam;
			if (nmhdr->code == CDN_FILEOK) {
				SendDlgItemMessage(hdlg, IDC_COMBO, WM_GETTEXT, sizeof(g_DbgInfo.Arguments), (LPARAM)g_DbgInfo.Arguments);
			}
			break;
		}
		case WM_COMMAND:
		{

			break;
		}
	}
	return 0;
}
