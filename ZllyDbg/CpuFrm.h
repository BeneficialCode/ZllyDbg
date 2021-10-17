#pragma once
#include "CpuAsm.h"
#include "CpuInfo.h"
#include "CpuStack.h"
#include "CpuDump.h"
#include "CpuReg.h"
#include "resource.h"


class CCPUFrame : public CMDIChildWindowImpl<CCPUFrame> {
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MDICHILD_CPU)

	virtual void OnFinalMessage(HWND /*hWnd*/);


	BEGIN_MSG_MAP_EX(CCPUFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		MESSAGE_HANDLER(WM_USER_CHALL, OnUserChall)
		MESSAGE_HANDLER(WM_USER_SIZE, OnUserSize)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLBtnDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLBtnUp)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_USER_FOCUS, OnUserFocus)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_USER_CHMEM,OnUserChmem)
		MESSAGE_HANDLER(WM_USER_CHREG,OnUserChreg)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED,OnWindowPosChanged)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		CHAIN_MSG_MAP(CMDIChildWindowImpl<CCPUFrame>)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);

	LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserChall(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnLBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnLBtnUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserChmem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserChreg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);


	void GetCapturedWindow();
	void InvalidateChildWindow(bool earse = false);


	static inline CAsmWindow m_Asm;
	static inline CStackWindow m_Stack;
	static inline CDumpWindow m_Dump;
	static inline CInfoWindow m_Info;
	static inline CRegWindow m_Reg;
private:
	int _halfWidth[2];
	int _halfHeight[2];
	int _threeFourthWidth;
	int _twoThirdHeight;
	int _asmWidth[2];
	int _dumpWidth[2];
	int _asmHeight[2];
	int _regHeight[2];

private:
	int _line = 0; 
	int _shift;
	HWND _hwnd = 0; // 当前获得焦点的窗口
};