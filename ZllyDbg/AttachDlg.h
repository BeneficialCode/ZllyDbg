#pragma once

class CProcessTable;
class CAttachDlg :public CDialogImpl<CAttachDlg> {
public:
	enum {
		IDD = IDD_GET_PROC
	};

	BEGIN_MSG_MAP_EX(CAttachDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		COMMAND_ID_HANDLER(IDC_ATTACH, OnAttach)
		COMMAND_ID_HANDLER(IDC_CANCEL, OnCancel)
	END_MSG_MAP()

	LRESULT OnAttach(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSysCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void CAttachDlg::OnGetMinMaxInfo(LPMINMAXINFO lpMMI) {
		lpMMI->ptMinTrackSize.x = 350;
		lpMMI->ptMinTrackSize.y = 150;
	}

public:
	int MyEnumProcess();

private:
	wil::unique_handle _hThread{ nullptr };
	CProcessTable* m_ProcTable{ nullptr };
};

