#pragma once

// ÏûÏ¢¹ýÂË
class CMainFilter : public CMessageFilter {
	BOOL PreTranslateMessage(MSG* pMsg) {
		if (pMsg->message == WM_ERASEBKGND) {
			return TRUE;
		}
	}
};