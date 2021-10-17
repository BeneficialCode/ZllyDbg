#pragma once

class CFileDialogEx : public CFileDialogImpl<CFileDialogEx> {
public:
	CFileDialogEx(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		HWND hWndParent = NULL);

	BEGIN_MSG_MAP(CFileDialogEx)
		CHAIN_MSG_MAP(CFileDialogImpl<CFileDialogEx>)
	END_MSG_MAP()
private:
	CEdit m_Edit;
};

UINT_PTR CALLBACK OFNHookProc(
	_In_ HWND hdlg,
	_In_ UINT uiMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);