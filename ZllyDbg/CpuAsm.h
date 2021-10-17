#pragma once
#include "Dump.h"

class CAsmWindow : public CWindowImpl<CAsmWindow>,public CDump {
public:
	DECLARE_WND_CLASS(nullptr);

	BEGIN_MSG_MAP(CAsmWindow)
		MESSAGE_HANDLER(WM_ERASEBKGND,OnEraseBkground)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_PAINT,OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLBtnDown)
		MESSAGE_HANDLER(WM_LBUTTONUP,OnLBtnUp)
		MESSAGE_HANDLER(WM_SETFOCUS,OnSetFocus)
		//MESSAGE_HANDLER(WM_NCLBUTTONDOWN,OnNcLBtnDown)
		//MESSAGE_HANDLER(WM_NCRBUTTONDOWN,OnNcRBtnDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE,OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		MESSAGE_HANDLER(WM_USER_VABS,OnUserVabs)
		MESSAGE_HANDLER(WM_USER_VREL,OnUserVrel)
		MESSAGE_HANDLER(WM_USER_VBYTE,OnUserVbyte)
		MESSAGE_HANDLER(WM_USER_STS,OnUserSts)
		MESSAGE_HANDLER(WM_USER_CHGS,OnUserChgs)
		MESSAGE_HANDLER(WM_USER_CNTS,OnUserCnts)
		MESSAGE_HANDLER(WM_USER_SCR,OnUserScr)
		MESSAGE_HANDLER(WM_USER_DBLCLK,OnUserDblClk)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED,OnWindowPosChanged)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnNcLBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnNcRBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnLBtnUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserVabs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserVbyte(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserVrel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserSts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserChgs(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserCnts(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserScr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

public:
	static t_dump _dump;
};