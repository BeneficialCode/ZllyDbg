#pragma once
#include "Dump.h"

enum class RegisterMode {
	MMX, ThreeDNow, DebugRegister, Fpu
};

class CRegWindow : public CWindowImpl<CRegWindow>, public CDump {
public:
	DECLARE_WND_CLASS(nullptr);

	BEGIN_MSG_MAP(CRegWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkground)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLBtnDown)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLBtnDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnEraseBkground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void SetRegisterMode(RegisterMode mode);

public:
	static t_dump _dump;
	static RegisterMode _regMode;
};