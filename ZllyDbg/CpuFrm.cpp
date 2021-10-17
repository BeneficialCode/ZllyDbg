#include "stdafx.h"
#include "resource.h"
#include "CPUFrm.h"

void CCPUFrame::OnFinalMessage(HWND) {
	// 销毁窗口,防止内存泄漏
	delete this;
	g_hCpuFrm = NULL;
}

LRESULT CCPUFrame::OnForwardMsg(UINT, WPARAM, LPARAM lParam, BOOL&) {
	LPMSG pMsg = (LPMSG)lParam;
	return CMDIChildWindowImpl<CCPUFrame>::PreTranslateMessage(pMsg);
}

void CCPUFrame::OnGetMinMaxInfo(LPMINMAXINFO lpMMI) {
	lpMMI->ptMinTrackSize.x = 24 * g_AvWidthFont + 12;
	lpMMI->ptMinTrackSize.y = 8 * g_AvHighFont + GetSystemMetrics(SM_CYSIZE) + 12;
}

void CCPUFrame::InvalidateChildWindow(bool earse) {
	if (m_hWnd) {
		InvalidateRect(nullptr,false);
		::InvalidateRect(m_Asm.m_hWnd, nullptr, false);
		::InvalidateRect(m_Stack.m_hWnd, nullptr, false);
		::InvalidateRect(m_Reg.m_hWnd, nullptr, false);
		::InvalidateRect(m_Info.m_hWnd, nullptr, false);
		::InvalidateRect(m_Dump.m_hWnd, nullptr, false);
		if (earse) {
			RedrawWindow(0, 0, RDW_ALLCHILDREN | RDW_UPDATENOW);
		}
	}
}

LRESULT CCPUFrame::OnUserChmem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	InvalidateChildWindow();
	return 0;
}

LRESULT CCPUFrame::OnUserChall(UINT, WPARAM, LPARAM lParam, BOOL&) {
	
	return LRESULT();
}

LRESULT CCPUFrame::OnUserChreg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	::InvalidateRect(m_Asm.m_hWnd, 0, false);
	::InvalidateRect(m_Info.m_hWnd, 0, false);
	::InvalidateRect(m_Stack.m_hWnd, 0, false);
	::InvalidateRect(m_Reg.m_hWnd, 0, false);

	return 0;
}

LRESULT CCPUFrame::OnCreate(UINT, WPARAM, LPARAM lParam, BOOL&) {
	_regHeight[0] = 329;
	_regHeight[1] = 341;
	_asmWidth[0] = 705;
	_asmWidth[1] = 895;
	_asmHeight[0] = 163;
	_asmHeight[1] = 275;
	_dumpWidth[0] = 661;
	_dumpWidth[1] = 751;

	_halfWidth[0] = 0;
	_halfHeight[0] = 0;
	_halfWidth[1] = 0;
	_halfHeight[1] = 0;

	m_Asm._dump.hw = m_Asm.Create(m_hWnd, 0, 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL);
	m_Dump._dump.hw = m_Dump.Create(m_hWnd, 0, 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL);
	m_Stack._dump.hw = m_Stack.Create(m_hWnd, 0, 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL);
	m_Info._dump.hw = m_Info.Create(m_hWnd, 0, 0, WS_CHILD | WS_VISIBLE);
	m_Reg._dump.hw = m_Reg.Create(m_hWnd, 0, 0, WS_CHILD | WS_VISIBLE);
	return 0;
}

LRESULT CCPUFrame::OnUserSize(UINT, WPARAM, LPARAM lParam, BOOL&) {
	RECT rect;
	GetClientRect(&rect);
	bool zoomed = IsZoomed() != 0;
	int halfWidth = rect.right / 2;
	int halfHeight = rect.bottom / 2;
	if (!_halfWidth[zoomed]) {
		_halfWidth[zoomed] = halfWidth;
		_halfHeight[zoomed] = halfHeight;
	}
	if (zoomed && !_threeFourthWidth) {
		_threeFourthWidth = 3 * rect.right / 4;
		_twoThirdHeight = 2 * rect.bottom / 3;
	}
	_asmWidth[zoomed] += halfWidth - _halfWidth[zoomed];
	_dumpWidth[zoomed] += halfWidth - _halfWidth[zoomed];
	_halfWidth[zoomed] = halfWidth;
	_asmHeight[zoomed] += halfHeight - _halfHeight[zoomed];
	_regHeight[zoomed] += halfHeight - _halfHeight[zoomed];
	_halfHeight[zoomed] += halfHeight - _halfHeight[zoomed];
	int maxWidth = rect.right - 6;
	int minWidth = 6;
	if (_asmWidth[zoomed] >= maxWidth) {
		_asmWidth[zoomed] = maxWidth;
	}
	if (_asmWidth[zoomed] < minWidth) {
		_asmWidth[zoomed] = minWidth;
	}
	if (_dumpWidth[zoomed] >= maxWidth) {
		_dumpWidth[zoomed] = maxWidth;
	}
	if (_dumpWidth[zoomed] < minWidth) {
		_dumpWidth[zoomed] = minWidth;
	}
	int maxHeight = rect.bottom - 6;
	int minHeight = 12;
	if (_regHeight[zoomed] >= maxHeight) {
		_regHeight[zoomed] = maxHeight;
	}
	if (_regHeight[zoomed] < minHeight) {
		_regHeight[zoomed] = minHeight;
	}
	maxHeight = rect.bottom - 12;
	if (_asmHeight[zoomed] >= maxHeight) {
		_asmHeight[zoomed] = maxHeight;
	}
	minHeight = 6;
	if (_asmHeight[zoomed] < minHeight) {
		_asmHeight[zoomed] = minHeight;
	}
	int width, height, x, y;
	x = 2, y = 2;
	width = _asmWidth[zoomed] - 3;
	height = _asmHeight[zoomed] - 3;
	m_Asm.MoveWindow(2, 2, width,height, false);
	
	x = 2;
	y = _asmHeight[zoomed] + 1;
	width = _asmWidth[zoomed] - 3;
	height = _regHeight[zoomed] - _asmHeight[zoomed] - 2;
	m_Info.MoveWindow(x, y, width, height, false);

	x = _asmWidth[zoomed] + 1;
	y = 2;
	width = rect.right - _asmWidth[zoomed] - 3;
	height = _regHeight[zoomed] - 3;
	m_Reg.MoveWindow(x, y, width, height, false);

	x = 2;
	y = _regHeight[zoomed] + 1;
	width = _dumpWidth[zoomed] - 3;
	height = rect.bottom - _regHeight[zoomed] - 3;
	m_Dump.MoveWindow(2, y, width, height, false);

	x = _dumpWidth[zoomed] + 1;
	y = _regHeight[zoomed] + 1;
	width = rect.right - _dumpWidth[zoomed] - 3;
	height = rect.bottom - _regHeight[zoomed] - 3;
	m_Stack.MoveWindow(x, y, width, height,false);
	RedrawWindow(0, 0, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return 0;
}


LRESULT CCPUFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	bHandled = true;
	if (!IsIconic() && wParam != 1) {
		SendMessage(WM_USER_SIZE, wParam, lParam);
	}
	return DefMDIChildProc(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CCPUFrame::OnLBtnDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	bHandled = true;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	RECT rect;
	GetClientRect(&rect);
	bool zoomed = IsZoomed();
	int line = -1;
	if (m_hWnd != GetCapture() || !_line) {
		if (pt.x >= rect.left && pt.x < rect.right
			&& pt.y >= rect.top && pt.y < rect.bottom) {	// 在客户区内
			if (_regHeight[zoomed] - 1 == pt.y || pt.y == _regHeight[zoomed]) {
				line = 1;
				_shift = pt.y - _regHeight[zoomed];
			}
			else if (_regHeight[zoomed] > pt.y && (_asmWidth[zoomed] - 1 == pt.x || _asmWidth[zoomed] == pt.x)) {
				line = 2;// reg 与 asm 分割线
				_shift = pt.x - _asmWidth[zoomed];
			}
			else if (_asmWidth[zoomed] > pt.x && (_asmHeight[zoomed] - 1 == pt.y || _asmHeight[zoomed] == pt.y)) {
				line = 3;
				_shift = pt.y - _asmHeight[zoomed];
			}
			else if (_regHeight[zoomed] <= pt.y && (_dumpWidth[zoomed] - 1 == pt.x || _dumpWidth[zoomed] == pt.x)) {
				line = 4;
				_shift = pt.x - _dumpWidth[zoomed];
			}
			else {
				line = 0;
			}
		}
		else {
			line = -1;
		}
	}
	if (line > 0) {
		_line = line;
		SetCapture();
	}

	return DefMDIChildProc(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CCPUFrame::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	if (LOWORD(lParam) != 1) {
		return DefMDIChildProc(m_hWnd, uMsg, wParam, lParam);
	}

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	RECT rect;
	GetClientRect(&rect);
	bool zoomed = IsZoomed();
	int line = 0;
	if (m_hWnd != GetCapture() || !_line) {
		if (pt.x >= rect.left && pt.x < rect.right
			&& pt.y >= rect.top && pt.y < rect.bottom) {	// 在客户区内
			if (_regHeight[zoomed] - 1 == pt.y || pt.y == _regHeight[zoomed]) {
				line = 1;	// reg 与 stack 分割线
				_shift = pt.y - _regHeight[zoomed];
			}
			else if (_regHeight[zoomed] > pt.y && (_asmWidth[zoomed] - 1 == pt.x || _asmWidth[zoomed] == pt.x)) {
				line = 2;	// reg 与 asm 分割线
				_shift = pt.x - _asmWidth[zoomed];
			}
			else if (_asmWidth[zoomed] > pt.x && (_asmHeight[zoomed] - 1 == pt.y || _asmHeight[zoomed] == pt.y)) {
				line = 3;	// asm 与 info 分割线
				_shift = pt.y - _asmHeight[zoomed];
			}
			else if (_regHeight[zoomed] <= pt.y && (_dumpWidth[zoomed] - 1 == pt.x || _dumpWidth[zoomed] == pt.x)) {
				line = 4;	// dump 与 stack 分割线
				_shift = pt.x - _dumpWidth[zoomed];
			}
			else {
				line = 0;
			}
		}
		else {
			line = -1;
		}
	}
	
	HCURSOR hCursor;
	if (m_hWnd == GetCapture() && _line > 0) {
		line = _line;
	}
	if (line == 1 || line == 3) {
		hCursor = LoadCursor(NULL, IDC_SIZENS);
		SetCursor(hCursor);
	}
	else if (line == 2 || line == 4) {
		hCursor = LoadCursor(NULL, IDC_SIZEWE);
		SetCursor(hCursor);
	}
	else {
		if (line) {
			return DefMDIChildProc(m_hWnd, uMsg, wParam, lParam);
		}
		hCursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hCursor);
	}
	return 1;
}

LRESULT CCPUFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	bHandled = true;
	if (m_hWnd == GetCapture() && _line > 0) {
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		RECT rect;
		GetClientRect(&rect);
		bool zoomed = IsZoomed();
		int infoHeight;
		int maxHeight;
		int minHeight;
		int height, width;
		int maxWidth, minWidth;
		switch (_line) {
			case 1:
				infoHeight = _regHeight[zoomed] - _asmHeight[zoomed];
				_regHeight[zoomed] = pt.y - _shift;
				maxHeight = rect.bottom - 6;
				if (_regHeight[zoomed] >= maxHeight) {
					_regHeight[zoomed] = maxHeight;
				}
				if (_regHeight[zoomed] < 12) {
					_regHeight[zoomed] = 12;
				}
				_asmHeight[zoomed] = _regHeight[zoomed] - infoHeight;
				if (_asmHeight[zoomed] <= 3) {
					_asmHeight[zoomed] = 3;
				}
				height = _regHeight[zoomed] - 4 * g_AvHighFont - 2;
				if (_asmHeight[zoomed] <= height) {
					_asmHeight[zoomed] = height;
				}
				break;

			case 2:
				maxWidth = rect.right - 6;
				width = pt.x - _shift;
				if (width >= maxWidth) {
					_asmWidth[zoomed] = maxWidth;
				}
				else {
					_asmWidth[zoomed] = width;
				}
				if (_asmWidth[zoomed] < 6) {
					_asmWidth[zoomed] = 6;
				}
				break;
			
			case 3:
				height = pt.y - _shift;
				if (height <= 3) {
					_asmHeight[zoomed] = 3;
				}
				else {
					_asmHeight[zoomed] = height;
				}
				height = _regHeight[zoomed] - 4 * g_AvHighFont - 2;
				if (_asmHeight[zoomed] <= height) {
					_asmHeight[zoomed] = height;
				}
				height = _regHeight[zoomed] - 3;
				if (_asmHeight[zoomed] >= height) {
					_asmHeight[zoomed] = height;
				}
				break;

			case 4:
				maxWidth = rect.right - 6;
				width = pt.x - _shift;
				if (width >= maxWidth) {
					_dumpWidth[zoomed] = maxWidth;
				}
				else {
					_dumpWidth[zoomed] = width;
				}
				if (_dumpWidth[zoomed] < 6) {
					_dumpWidth[zoomed] = 6;
				}
				break;
		}
		SendMessage(WM_USER_SIZE);
	}
	return 0;
}

LRESULT CCPUFrame::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	bHandled = true;
	return DefMDIChildProc(m_hWnd, uMsg, wParam, lParam);
}


LRESULT CCPUFrame::OnLBtnUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	if (m_hWnd == GetCapture()) {
		ReleaseCapture();
	}
	_line = -1;
	return 0;
}

LRESULT CCPUFrame::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	bHandled = true;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(&ps);
	RECT rect;
	RECT clientRect;
	GetClientRect(&rect);
	CopyRect(&clientRect, &rect);
	if (_hwnd) {
		POINT pt;
		::GetWindowRect(_hwnd, &rect);
		pt.x = rect.left;
		pt.y = rect.top;
		::ScreenToClient(m_hWnd, &pt);
		rect.left = pt.x - 2;
		rect.top = pt.y - 2;
		pt.x = rect.right;
		pt.y = rect.bottom;
		::ScreenToClient(m_hWnd, &pt);
		rect.right = pt.x + 2;
		rect.bottom = pt.y + 2;
		FillRect(hdc, &rect, g_myBrush[g_myScheme[0].auxcolor]);
		// 建立一个新的裁剪区域
		IntersectClipRect(hdc, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
		// 从裁剪区域中去除一个矩形
		ExcludeClipRect(hdc, rect.left, rect.top, rect.right, rect.bottom);
		FillRect(hdc, &clientRect, g_myBrush[g_myScheme[0].lowcolor]);
		// 删除设备上下文裁剪区域
		SelectClipRgn(hdc, 0);
	}
	else {
		FillRect(hdc, &rect, g_myBrush[g_myScheme[0].lowcolor]);
	}
	EndPaint(&ps);
	return 0;
}

LRESULT CCPUFrame::OnUserFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	if ((HWND)lParam == _hwnd) {
		return 0;
	}
	else {
		HWND hwnd = _hwnd;
		_hwnd = (HWND)lParam;
		if (hwnd == m_Asm.m_hWnd) {
			m_Asm._dump.dimmed = 1;
		}
		if (hwnd == m_Dump.m_hWnd) {
			m_Dump._dump.dimmed = 1;
		}
		if (hwnd == m_Stack.m_hWnd) {
			m_Stack._dump.dimmed = 1;
		}
		if (hwnd) {
			::InvalidateRect(hwnd, nullptr, false);
		}
		::SetFocus(_hwnd);
		if (_hwnd == m_Asm.m_hWnd) {
			m_Asm._dump.dimmed = 0;
		}
		if (_hwnd == m_Dump.m_hWnd) {
			m_Dump._dump.dimmed = 0;
		}
		if (_hwnd == m_Stack.m_hWnd) {
			m_Stack._dump.dimmed = 0;
		}
		::InvalidateRect(_hwnd, nullptr, false);
		if (hwnd == m_Asm.m_hWnd || _hwnd == m_Asm.m_hWnd) {
			::InvalidateRect(m_Info.m_hWnd, nullptr, false);
		}
		InvalidateRect(nullptr, false);
		return 0;
	}
}

LRESULT CCPUFrame::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	bHandled = true;
	if (!_hwnd) {
		_hwnd = m_Asm.m_hWnd;
	}
	::SetFocus(_hwnd);
	return DefMDIChildProc(m_hWnd,uMsg,wParam,lParam);
}
