#pragma once
#include "Disassembly.h"

constexpr int NBAR = 17;

struct t_bar {
	int nbar;					// ����
	int font;					// ����
	int dx[NBAR];				// ʵ�ʿ��
	int defdx[NBAR];			// Ĭ�Ͽ��
	std::string name[NBAR];		// ����
	uchar mode[NBAR];			// ģʽ
	int captured;				// �Ƿ񱻲���
	int active;					// ��겶�����һ��
	int prevx;					// ���֮ǰ������
};

struct BarInfo {
	int defdx;
	std::string name;
	uchar mode;
};

struct TableInfo {
	int showbar;
	int hscroll;
	int mode;
	int font;
	int scheme;
	int offset;
	int xshift;
};

template<typename T>
struct t_sorted {
	std::string name;				// ����
	int n{0};						// ʵ������
	int nmax;						// �������
	int selected;					// ѡ������
	ulong seladdr;					// ѡ�еĻ���ַ
	bool sorted;					// �����Ƿ�����
	int sort;						// ���������
	int version{ 0 };				// �汾
	std::vector<int> index[NBAR];	// ����
	bool AutoSort{ false };		// �Ƿ�����Զ�����
	std::vector<T> info;
};

template<typename T>
struct t_table {
	HWND hw;			// ���ھ��
	t_sorted<T> data;	// ����
	int showbar;		// ��ʾ�����أ�ȱʡ
	short hscroll;		// ˮƽ����������ʾ������
	short colsel;		// ѡ�б��ڶ���
	int mode;			// ģʽ
	int font;			// ����
	short scheme;		// ɫ������
	short hilite;		// ��������
	int offset;			// ��ʾ����λ��
	int xshift = 0;			// λ���ƶ�
};

struct t_thread {
	ulong threadid;		// �߳�id
	HANDLE handle;		// �߳̾��
	ulong entry;		// �߳���ڵ�
	ulong teb;			// TEB
	ulong type;			// ����
	ulong datablock;	// ÿ���̵߳����ݿ�
	int	suspendcount;	// �̹߳������
	CONTEXT context;	// �߳�������
	t_reg reg;			// �Ĵ���������
	int	regvalid = 0;	// reg�Ƿ���Ч
	t_reg oldreg;		// �ɵ��߳�������
	int oldregvalid;	// oldreg�Ƿ���Ч
};

struct t_memory {
	ulong base;			// �ڴ����ʼ��ַ
	ulong size;			// ��Ĵ�С
	ulong memType;		// �ڴ�����
	ulong type;			// ��������
	ulong state;		// �ڴ�״̬
	ulong initAccess;	// ��ʼ������־
	ulong access;		// ������־
};

struct t_process {
	DWORD Pid;
	CString ProcessName;
	CString Path;
};

struct t_module {
	ulong base;		// ģ���ַ
	ulong size;		// ģ���С
	ulong entry;	// ģ����ڵ�
	CString name;	// ģ����
	CString version;// �ļ��汾
	CString path;	// �ļ�·��
	ulong type;		// ������Ϣ
	char* codedec;	// Decode code features or nullptr
	ulong codebase;	// Base address of module code block
	ulong codesize;	// Size of module code block
	int nsect;		// Number of sections in the module
	IMAGE_SECTION_HEADER* sect;// Copy of section headers from file
	ulong headersize;// Total size of headers in executable
	ulong database;	// Base address of module data block
	ulong fixupbase;// base of image in executable file
	ulong edatatable;	// base address of export data table
	ulong edatasize;	// size of export data table
	ulong idatatable;	// base address of import data table
	ulong idatabase;	// base address of data block
	ulong reloctable;	// base address of relocation table
	ulong relocsize;	// size of relocation table
	ulong resbase;		// base address of resources
	ulong ressize;		// size of resources
};

struct t_dump {
	HWND hw;				// ���ھ��
	int dimmed;				// ��0�ͻ����Ϊ��ɫ
	ulong threadid;			// ���ڽ����ͼĴ������
	int dumptype;			// ��ǰ����,DU_xxx+count+size
	int menutype;			// ��׼�˵�
	int itemwidth;			// ��ʾ���ȣ��ַ���
	int showstackframes;	// ��ʾջֵ֡
	int showstacklocals;	// ��ʾ�ֲ�����������
	int commentmode;		// 0:comment,1:source,2:profile
	CString filename;		// �����ļ���
	ulong base;				// �ڴ�鿪ʼ���ļ���ʼ
	ulong size;				// �ڴ���С���ļ���С
	ulong addr;				// չʾ�ĵ�һ���ֽ�
	ulong lastaddr;			// չʾ�����һ���ֽ�
	ulong sel0;				// ѡ��ĵ�һ���ֽ�
	ulong sel1;				// ���һ���ֽ�,������
	ulong startsel;			// ��ʼѡ��ĵ�ַ
	ulong runtraceoffset;	// ׷��ƫ��
	int captured;			// �Ƿ���겶��
	ulong reladdr;			// ��Ե�ַ
	CString relname;		// ����
	uchar* backup = nullptr;		// �����ڴ���ļ�
	uchar* filecopy = nullptr;		// �������ļ�
};

typedef struct t_bpoint {              // Description of INT3 breakpoint
	ulong          addr;                 // �ϵ��ַ
	ulong          type;                // Type of breakpoint, TY_xxx
	char           cmd;                  // Old value of command
	ulong          passcount;            // Actual pass count
} t_bpoint;

typedef struct t_hardbpoint {          // Description of hardware breakpoint
	ulong          addr;                 // Base address of hardware breakpoint
	int            size;                 // Size of hardware breakpoint
	int            type;              // Type of breakpoint, one of HB_xxx
} t_hardbpoint;

template<class T>
class CTable {
public:
	CTable() {
	}
	CTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table);

	void Defaultbar();
	void PaintBar(HDC hdc, LPRECT rect, int iColumn);
	void PaintTable(HWND hw);
	int Tablefunction(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	
	bool Sortsorteddata(int sort);
	virtual bool CompareItems(const T& i1, const T& i2, int col,bool asc=true) {
		return false;
	}

	T Getsortedbyselection(int row);

	template<class T>
	static DWORD GetSeladdr(const T& t) {
		return 0;
	}
	template<>
	static DWORD GetSeladdr(const t_thread& t) {
		return t.threadid;
	}
	template<>
	static DWORD GetSeladdr(const t_process& t) {
		return t.Pid;
	}
	template<>
	static DWORD GetSeladdr(const t_memory& t) {
		return t.base;
	}
	template<>
	static DWORD GetSeladdr(const t_module& t) {
		return t.base;
	}

	void Deletesorteddata(ulong addr);

	void Addsorteddata(T t);

	virtual T* Findsorteddata(ulong addr) {
		return nullptr;
	}

	virtual int ParseTableEntry(CString& s, char& mask, int& select, T& info, int column) {
		return 0;
	}

	virtual int ParseTableEntry(CString& s, char& mask, int& select, t_dump* info, int column) {
		return 0;
	}

	void GetSysKeyState(UINT msg, LPARAM lp, bool& pressShift, bool& pressCtrl);

	void SetTableWindowInfo(int maxColumns,int nlines = 20);

public:
	t_bar m_Bar;
	static RECT m_Rect;
	static DWORD _style;
	t_table<T> m_Table;
	bool m_bSnowFreeDrawing{ true };	// �Ƿ��ڴ��ͼ
	bool m_bHighlightSortedColumn{ false };
	int m_BkColorIndex[8] = { -1,0,7,15,11,10,12,14 };
	int _row;
	int _page{ 0 };
};

template <class T>
DWORD CTable<T>::_style = 0;

template <class T>
RECT CTable<T>::m_Rect = { 0 };


template<class T>
T CTable<T>::Getsortedbyselection(int row) {
	return m_Table.data.info[row];
}

template<class T>
void CTable<T>::GetSysKeyState(UINT msg, LPARAM lp, bool& pressShift, bool& pressCtrl) {
	if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) {
		pressShift = GetKeyState(VK_SHIFT) & 0x8000;
		pressCtrl = GetKeyState(VK_CONTROL) & 0x8000;
	}
	else {
		pressShift = LOWORD(lp);
		pressCtrl = HIWORD(lp);
	}
}

template<class T>
CTable<T>::CTable(int font, BarInfo bars[], int nbar, int captured, TableInfo& table) {
	ATLASSERT(nbar < NBAR&& nbar>0);
	ATLASSERT(font > 0 && font < 11);

	m_Table.showbar = table.showbar;
	m_Table.hscroll = table.hscroll;
	m_Table.mode = table.mode;
	m_Table.font = table.font;
	m_Table.scheme = table.scheme;
	m_Table.offset = table.offset;
	m_Table.xshift = table.xshift;

	m_Bar.font = font;
	m_Bar.nbar = nbar;
	m_Bar.captured = captured;

	for (int i = 0; i < nbar; i++) {
		m_Bar.defdx[i] = bars[i].defdx;
		m_Bar.name[i] = bars[i].name;
		m_Bar.mode[i] = bars[i].mode;
	}

	Defaultbar();
}

template<class T>
void CTable<T>::Defaultbar() {
	int i = 0;
	while (i < m_Bar.nbar) {
		m_Bar.dx[i] = g_AvWidthFont * m_Bar.defdx[i];
		++i;
	}
}

template<class T>
void CTable<T>::PaintBar(HDC hdc, LPRECT client, int iColumn) {
	ATLASSERT(hdc && client);
	int font;
	RECT bar, col;

	if (client->left < client->right && client->top < client->bottom) {
		font = m_Bar.font;
		SetTextAlign(hdc, TA_LEFT | TA_BOTTOM);
		SetBkColor(hdc, g_crLightGray);
		SelectObject(hdc, g_hOemFixedFont);
		memcpy(&bar, client, sizeof(bar));
		client->top += g_AvHighFont + 4;
		bar.bottom = client->top - 1;
		SelectObject(hdc, g_hBlackPen);
		MoveToEx(hdc, bar.left, bar.bottom, NULL);
		LineTo(hdc, bar.right, bar.bottom);	// �����б���Ŀ�

		int i = 0;
		while (m_Bar.nbar > i) {
			if (m_Bar.dx[i] > 0) {
				if (client->right <= bar.left)
					break;
				memcpy(&col, &bar, sizeof(col));
				bar.left = col.left + m_Bar.dx[i];
				col.right = bar.left - 1;
				SelectObject(hdc, g_hBlackPen);
				MoveToEx(hdc, col.right, col.top, NULL);
				LineTo(hdc, col.right, col.bottom);	// �ұ߽�Ϊ��ɫ
				int x = col.left + g_AvWidthFont / 2;
				int y = col.bottom - 1;
				// ���ñ߿�ָ�
				if (m_Bar.mode[i] & BAR_NOSORT) {	// ���������ϱ߽�Ϊ����
					SelectObject(hdc, g_hWhitePen);
					MoveToEx(hdc, col.left, col.top, NULL);
					LineTo(hdc, col.right, col.top);
					++col.top;
				}
				else if ((m_Bar.mode[i] & BAR_PRESSED) == BAR_PRESSED) {	// ��ѹʱ,��߽���ϱ߽�Ϊ�ڻ�ɫ
					SelectObject(hdc, g_hDarkGrayPen);
					MoveToEx(hdc, col.left, col.bottom - 1, NULL);
					LineTo(hdc, col.left, col.top);
					LineTo(hdc, col.right, col.top);
					++col.left;
					++col.top;
					++x;
					++y;
				}
				else {
					--col.bottom;
					SelectObject(hdc, g_hWhitePen);				// ��߽�Ϊ��ɫ
					MoveToEx(hdc, col.left, col.bottom, NULL);
					LineTo(hdc, col.left, col.top);
					LineTo(hdc, col.right, col.top);
					++col.left;
					--col.right;
					SelectObject(hdc, g_hDarkGrayPen);			// �ұ߽�Ϊ�ڻ�ɫ
					MoveToEx(hdc, col.left, col.bottom, NULL);
					LineTo(hdc, col.right, col.bottom);
					LineTo(hdc, col.right, col.top);
					++col.top;
				}
				// ����������ɫ�ͱ�����ɫ
				if (m_Bar.mode[i] & BAR_DISABLED) {
					SetTextColor(hdc, g_crDarkGray);
					SetBkColor(hdc, g_crLightGray);
				}
				else if (m_Bar.mode[i] & BAR_NOSORT || !m_bHighlightSortedColumn || i != iColumn) {
					SetTextColor(hdc, g_crBlack);
					SetBkColor(hdc, g_crLightGray);
				}
				else {
					SetTextColor(hdc, g_crBlack);
					SetBkColor(hdc, g_myColor[18]);
				}

				// ����б��� column captions
				int len = 0;
				len = m_Bar.name[i].length();
				ExtTextOutA(hdc, x, y, ETO_CLIPPED | ETO_OPAQUE, &col, m_Bar.name[i].c_str(), len, NULL);
			}
			i++;
		}
		if (bar.left < bar.right) {	// δ����������ϱ߽�Ϊ��ɫ
			SelectObject(hdc, g_hWhitePen);
			MoveToEx(hdc, bar.left, bar.top, NULL);
			LineTo(hdc, bar.right, bar.top);
			++bar.top;
			FillRect(hdc, &bar, g_hLightGrayBrush);
		}
	}
}

template<class T>
bool CTable<T>::Sortsorteddata(int sort) {

	if (m_Table.data.sorted && sort == m_Table.data.sort) {
		return false;
	}
	std::sort(m_Table.data.info.begin(), m_Table.data.info.end(), [=](const auto& i1, const auto& i2) {
		return CompareItems(i1, i2, sort);
		});
	m_Table.data.sort = sort;
	return true;
}

template<class T>
void CTable<T>::Deletesorteddata(ulong addr) {
	::WaitForSingleObject(g_hMutex, INFINITE);
	// remove_if �� erase���ʹ�ò�������ɾ��Ԫ��
	auto it = std::remove_if(m_Table.data.info.begin(), m_Table.data.info.end(), [=](auto& p) {
		return GetSeladdr(p) == addr;
		});
	m_Table.data.info.erase(it);
	m_Table.data.n--;
	::ReleaseMutex(g_hMutex);
}

template<class T>
void CTable<T>::Addsorteddata(T t) {
	// acquire the mutex
	::WaitForSingleObject(g_hMutex, INFINITE);
	m_Table.data.info.push_back(std::move(t));
	m_Table.data.n++;
	::ReleaseMutex(g_hMutex);
}

template<class T>
void CTable<T>::PaintTable(HWND hw) {
	HDC hdc, hdcMem;
	PAINTSTRUCT ps;
	RECT client, row, entry, txt;
	int column;
	int fontHeight;
	HBITMAP hBitmap;
	HGDIOBJ hgdiMem;
	T info;
	t_dump* dump;

	Tablefunction(hw, WM_USER_SCR, 0, 0);	// �ػ������
	hdc = BeginPaint(hw, &ps);
	GetClientRect(hw, &client);

	if (m_Table.mode & TABLE_USERDEF || m_Bar.nbar > 0) {
		client.left = -m_Table.xshift;
		if (m_Table.showbar == 1) { // ��ʾ
			column = -1;
			PaintBar(hdc, &client, column);
		}

		fontHeight = g_AvHighFont;
		if (m_bSnowFreeDrawing) {
			hdcMem = CreateCompatibleDC(hdc);
			if (hdcMem) {
				hBitmap = CreateCompatibleBitmap(hdc, client.right, fontHeight);
			}
			else {
				hBitmap = NULL;
			}
			if (hBitmap) {
				hgdiMem = SelectObject(hdcMem, hBitmap);
			}
			else {
				hdcMem = hdc;
				m_bSnowFreeDrawing = false;
			}
		}
		else {
			hdcMem = hdc;
		}
		SelectObject(hdcMem, g_hOemFixedFont);
		SetTextAlign(hdcMem, TA_LEFT | TA_BOTTOM);
		SetBkMode(hdcMem, TRANSPARENT);
		if (m_Table.mode & TABLE_USERDEF) {
			m_Table.data.n = (client.bottom - client.top - 1) / fontHeight + 1;
			m_Table.offset = 0;							// ������ʾ����
		}
		else {
			if (m_Table.data.n <= m_Table.offset) {
				m_Table.offset = m_Table.data.n - 1;	// ��ʾ���һ��
			}
			if (m_Table.offset < 0) {
				m_Table.offset = 0;
			}
		}

		memcpy(&row, &client, sizeof(row));
		::WaitForSingleObject(g_hMutex, INFINITE);
		for (int i = m_Table.offset; m_Table.mode & TABLE_USERDEF || i < m_Table.data.n; ++i) {	// ѭ������Ϊ������
			if (m_Table.mode & TABLE_DIR) {
				if (client.top >= client.bottom)
					break;
				row.top = row.bottom - fontHeight;
				if (client.top > row.bottom - fontHeight)
					row.top = client.top;
			}
			else {
				if (row.top > client.bottom)
					break;
				row.bottom = fontHeight + row.top;
			}
			memcpy(&entry, &row, sizeof(entry));
			if (m_bSnowFreeDrawing) {
				entry.top = 0;
				entry.bottom = fontHeight;
			}
		
			if (m_Table.mode & TABLE_USERDEF) {
				m_Table.offset = i;
				dump = (t_dump*)GetWindowLong(hw, GWL_USERDATA);
			}
			else {
				info = m_Table.data.info[i];
			}
			int scheme = m_Table.scheme;
			int iTxColor=0, iBkColor;
			int col;
			int len;
			for (col = 0; col < m_Bar.nbar;) {	// ѭ������Ϊ������
				int width = m_Bar.dx[col];
				if (entry.left > row.right)
					break;
				entry.right = entry.left + width - 1;
				if (width > 3) {
					if (entry.right >= row.left) {
						CString s;
						char mask = 0;
						int select = 0;
						int symName = 0;
						if (m_Table.mode & TABLE_USERDEF) {
							len = ParseTableEntry(s, mask, select, dump, col);
						}
						else {
							len = ParseTableEntry(s, mask, select, info, col);
						}
						USES_CONVERSION;
						std::string t = W2CA(s.GetString());
						HBRUSH hBkBrush;
						if (!(m_Table.mode & TABLE_USERDEF)	// ���Ի�
							&& i==m_Table.data.selected
							&& !(select & DRAW_BREAK)	// ���Ƕϵ�
							&& (!(m_Table.mode & TABLE_COLSEL) || m_Table.colsel == col)) {	// �����ڸı������
							select |= DRAW_SELECT;
						}
						int bottom;
						if (select & DRAW_DIRECT) {
							int drawMask = DRAW_NORMAL;
							memcpy(&txt, &entry, sizeof(txt));
							if (m_Bar.mode[col] & BAR_SHIFTSEL) {
								txt.right = txt.left;
							}
							else {
								txt.right = txt.left + g_AvWidthFont / 2;
							}
							int j = 0;
							int k = 0;
							while (j < len && txt.left < entry.right) {
								if (k >= len || drawMask != mask || txt.right > entry.right) {
									if (k != len || drawMask & DRAW_DIRECT || drawMask & (DRAW_UL | DRAW_SELECT)
										&& !(select & DRAW_EXTSEL)) {
										if (entry.right < txt.right) {
											txt.right = entry.right;
										}
									}
									else {
										txt.right = entry.right;
									}
									if (txt.right > txt.left) {
										if (drawMask & DRAW_DIRECT) {
											iTxColor = drawMask & 0xf;
											if (iTxColor > 20) {
												iTxColor = g_myScheme[m_Table.scheme].textcolor;
											}
											iBkColor = m_BkColorIndex[((drawMask) >> 4) & 7];
											if (iBkColor < 0) {
												if (select & DRAW_EIP) {
													iBkColor = g_myScheme[m_Table.scheme].textcolor;
												}
												else if (select & DRAW_BREAK) {
													iBkColor = g_myScheme[m_Table.scheme].hitextcolor;
												}
												else if (select & DRAW_SELECT) {
													iBkColor = g_myScheme[m_Table.scheme].selbkcolor;
												}
												else {
													iBkColor = g_myScheme[m_Table.scheme].bkcolor;
												}
											}
										}
										else {
											if (drawMask & DRAW_HILITE) {
												iTxColor = g_myScheme[m_Table.scheme].hitextcolor;
											}
											else if ((drawMask | select) & DRAW_GRAY) {
												iTxColor = g_myScheme[m_Table.scheme].lowcolor;
											}
											if ((drawMask | select) & DRAW_EIP) {
												if (!(drawMask & DRAW_HILITE)) {
													iTxColor = g_myScheme[m_Table.scheme].bkcolor;
												}
												iBkColor = g_myScheme[m_Table.scheme].textcolor;
											}
											else if ((drawMask | select) & DRAW_BREAK) {
												iTxColor = g_myScheme[m_Table.scheme].textcolor;
												iBkColor = g_myScheme[m_Table.scheme].hitextcolor;
											}
											else if ((drawMask ^ select) & DRAW_SELECT) {
												iBkColor = g_myScheme[m_Table.scheme].selbkcolor;
											}
											else {
												iBkColor = g_myScheme[m_Table.scheme].bkcolor;
											}
										}
										SetTextColor(hdcMem, g_myColor[iTxColor]);
										SetBkColor(hdcMem, g_myColor[iBkColor]);
										bottom = entry.bottom;
										if (select & DRAW_TOP) {
											txt.bottom -= fontHeight / 2;
											if (select & DRAW_SELECT) {
												hBkBrush = g_myBrush[g_myScheme[m_Table.scheme].selbkcolor];
											}
											else {
												hBkBrush = g_myBrush[g_myScheme[m_Table.scheme].bkcolor];
											}
											FillRect(hdcMem, &txt, hBkBrush);
											txt.top += fontHeight / 2;
											txt.bottom += fontHeight / 2;
											bottom += fontHeight / 2;
										}
										else if (select & DRAW_BOTTOM) {
											txt.bottom -= fontHeight / 2;
											bottom -= fontHeight - fontHeight / 2;
										}
										if (select & DRAW_UNICODE) {
											ExtTextOutW(hdcMem, entry.left + g_AvWidthFont / 2, bottom, ETO_CLIPPED | ETO_OPAQUE,
												&txt, s.GetString(), len, NULL);
										}
										else {
											ExtTextOutA(hdcMem, entry.left + g_AvWidthFont / 2, bottom, ETO_CLIPPED | ETO_OPAQUE,
												&txt, t.c_str(), len, NULL);
										}
										if (drawMask & DRAW_UL) {
											SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].lowcolor]);
											MoveToEx(hdcMem, txt.left, bottom - 1, NULL);
											LineTo(hdcMem, txt.right, bottom - 1);
										}
										if (select & DRAW_TOP) {
											txt.top -= fontHeight / 2;
										}
										else if (select & DRAW_BOTTOM) {
											txt.bottom += fontHeight / 2;
											txt.top += fontHeight;
											if (select & DRAW_SELECT) {
												hBkBrush = g_myBrush[g_myScheme[m_Table.scheme].selbkcolor];
											}
											else {
												hBkBrush = g_myBrush[g_myScheme[m_Table.scheme].bkcolor];
											}
											FillRect(hdcMem, &txt, hBkBrush);
											txt.top -= fontHeight / 2;
										}
										switch (symName) {
											case D_SEP:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].auxcolor]);
												MoveToEx(hdcMem, g_AvWidthFont + txt.left - 1, txt.top, NULL);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, txt.bottom);
												break;

											case D_POINT:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												MoveToEx(hdcMem, txt.left, txt.top + fontHeight / 2, NULL);
												LineTo(hdcMem, txt.left, txt.top + fontHeight / 2 + 2);
												MoveToEx(hdcMem, txt.left + 1, txt.top + fontHeight / 2, 0);
												LineTo(hdcMem, txt.left + 1, txt.top + fontHeight / 2 + 2);
												break;

											case D_JMPOUT:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												MoveToEx(hdcMem, txt.left, (txt.bottom + txt.top) / 2, 0);
												LineTo(hdcMem, txt.left + 2 * g_AvWidthFont / 3 + 1, (txt.bottom + txt.top) / 2);
												break;

											case D_BEGIN:
											case D_SWTOP:
												if (symName == D_BEGIN) {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												}
												else {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].condbkcolor]);
												}
												MoveToEx(hdcMem, g_AvWidthFont + txt.left - 2, txt.top + fontHeight / 2 - 1, 0);
												LineTo(hdcMem, txt.left, txt.top + fontHeight / 2 - 1);
												LineTo(hdcMem, txt.left, txt.bottom);
												MoveToEx(hdcMem, g_AvWidthFont + txt.left - 2, txt.top + fontHeight / 2 - 2, 0);
												LineTo(hdcMem, txt.left + 1, txt.top + fontHeight / 2 - 2);
												LineTo(hdcMem, txt.left + 1, txt.bottom);
												break;

											case D_JMPDN:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												MoveToEx(hdcMem, txt.left, txt.bottom - g_AvWidthFont / 3 - 3, 0);
												LineTo(hdcMem, txt.left + g_AvWidthFont / 3, txt.bottom - 3);
												LineTo(hdcMem, txt.left + 2 * (g_AvWidthFont / 3) + 1, txt.bottom - g_AvWidthFont / 3 - 4);
												break;

											case D_END:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												MoveToEx(hdcMem, txt.left, txt.top, 0);
												LineTo(hdcMem, txt.left, txt.top + fontHeight / 2 + 1);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, txt.top + fontHeight / 2 + 1);
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, txt.top + fontHeight / 2 + 2);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, txt.top + fontHeight / 2 + 2);
												break;

											case D_BODY:
											case D_SWBODY:
												if (symName == D_BODY) {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												}
												else {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].condbkcolor]);
												}
												MoveToEx(hdcMem, txt.left, txt.top, 0);
												LineTo(hdcMem, txt.left, txt.bottom);
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, txt.bottom);
												break;

											case D_ENTRY:
											case D_CASE:
												if (symName == D_ENTRY) {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												}
												else {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].condbkcolor]);
												}
												MoveToEx(hdcMem, txt.left, txt.top, 0);
												LineTo(hdcMem, txt.left, txt.bottom);
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, txt.bottom);
												MoveToEx(hdcMem, txt.left + 2, txt.top + fontHeight / 2, 0);
												LineTo(hdcMem, txt.left + 5, txt.top + fontHeight / 2);
												MoveToEx(hdcMem, txt.left + 2, txt.top + fontHeight / 2 - 2, 0);
												LineTo(hdcMem, txt.left + 2, txt.top + fontHeight / 2 + 3);
												MoveToEx(hdcMem, txt.left + 3, txt.top + fontHeight / 2 - 1, 0);
												LineTo(hdcMem, txt.left + 3, txt.top + fontHeight / 2 + 2);
												break;

											case D_LEAF:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												MoveToEx(hdcMem, txt.left, txt.top, 0);
												LineTo(hdcMem, txt.left, txt.bottom);
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, txt.bottom);
												MoveToEx(hdcMem, txt.left + 1, txt.top + fontHeight / 2 + 1, 0);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, txt.top + fontHeight / 2 + 1);
												MoveToEx(hdcMem, txt.left + 1, txt.top + fontHeight / 2 + 2, 0);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, txt.top + fontHeight / 2 + 2);
												break;

											case D_SINGLE:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												MoveToEx(hdcMem, g_AvWidthFont + txt.left - 2, txt.top + 1, 0);
												LineTo(hdcMem, txt.left + 1, txt.top + 1);
												LineTo(hdcMem, txt.left + 1, txt.bottom - 2);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, txt.bottom - 2);
												MoveToEx(hdcMem, g_AvWidthFont + txt.left - 2, txt.top + 2, 0);
												LineTo(hdcMem, txt.left, txt.top + 2);
												LineTo(hdcMem, txt.left, txt.bottom - 3);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, txt.bottom - 3);
												break;

											case D_ENDBEG:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												MoveToEx(hdcMem, g_AvWidthFont + txt.left - 2, txt.bottom - 3, 0);
												LineTo(hdcMem, txt.left, txt.bottom - 3);
												LineTo(hdcMem, txt.left, txt.bottom);
												MoveToEx(hdcMem, g_AvWidthFont + txt.left - 2, txt.bottom - 4, 0);
												LineTo(hdcMem, txt.left + 1, txt.bottom - 4);
												LineTo(hdcMem, txt.left + 1, txt.bottom);
												MoveToEx(hdcMem, txt.left, txt.top, 0);
												LineTo(hdcMem, txt.left, txt.top + 2);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, txt.top + 2);
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, txt.top + 3);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, txt.top + 3);
												break;

											case D_JMPUP:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].textcolor]);
												MoveToEx(hdcMem, txt.left, txt.top + g_AvWidthFont / 3 + 2, 0);
												LineTo(hdcMem, txt.left + g_AvWidthFont / 3, txt.top + 2);
												LineTo(hdcMem, txt.left + 2 * (g_AvWidthFont / 3) + 1, txt.top + g_AvWidthFont / 3 + 3);
												break;

											case 'a':
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].hitextcolor]);
												MoveToEx(hdcMem, txt.left + 2 * (g_AvWidthFont / 3), (txt.bottom + txt.top) / 2, 0);
												LineTo(hdcMem, txt.left + 3, (txt.bottom + txt.top) / 2);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 - 2);
												LineTo(hdcMem, txt.left + 1, txt.top - 1);
												MoveToEx(hdcMem, txt.left + 1, txt.bottom - 1, 0);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 - 1);
												break;

											case D_PATHDN:
											case D_GRAYDN:
												if (symName == D_PATHDN) {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].hitextcolor]);
												}
												else {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].lowcolor]);
												}
												MoveToEx(hdcMem, txt.left + 2 * (g_AvWidthFont / 3), (txt.bottom + txt.top) / 2, 0);
												LineTo(hdcMem, txt.left + 3, (txt.bottom + txt.top) / 2);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 + 2);
												LineTo(hdcMem, txt.left + 1, txt.bottom);
												break;

											case D_PATHDNEND:
											case D_GRAYDNEND:
												if (symName == D_PATHDNEND) {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].hitextcolor]);
												}
												else {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].lowcolor]);
												}
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 - 1);
												LineTo(hdcMem, txt.left + 2, (txt.bottom + txt.top) / 2);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, (txt.bottom + txt.top) / 2);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 4, (txt.bottom + txt.top) / 2 - 3);
												MoveToEx(hdcMem, g_AvWidthFont + txt.left - 1, (txt.bottom + txt.top) / 2, 0);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 4, (txt.bottom + txt.top) / 2 + 3);
												break;

											case 'h':
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].hitextcolor]);
												MoveToEx(hdcMem, txt.left + 2 * (g_AvWidthFont / 3), (txt.bottom + txt.top) / 2, 0);
												LineTo(hdcMem, txt.left + 3, (txt.bottom + txt.top) / 2);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 + 2);
												LineTo(hdcMem, txt.left + 1, txt.bottom);
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 + 1);
												break;

											case D_PATH:
											case D_GRAYPATH:
												if (symName == D_PATH) {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].hitextcolor]);
												}
												else {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].lowcolor]);
												}
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, txt.bottom);
												break;

											case D_LASTCASE:
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].condbkcolor]);
												MoveToEx(hdcMem, txt.left, txt.top, 0);
												LineTo(hdcMem, txt.left, txt.top + fontHeight / 2 + 1);
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, txt.top + fontHeight / 2 + 2);
												MoveToEx(hdcMem, txt.left + 2, txt.top + fontHeight / 2, 0);
												LineTo(hdcMem, txt.left + 5, txt.top + fontHeight / 2);
												MoveToEx(hdcMem, txt.left + 2, txt.top + fontHeight / 2 - 2, 0);
												LineTo(hdcMem, txt.left + 2, txt.top + fontHeight / 2 + 3);
												MoveToEx(hdcMem, txt.left + 3, txt.top + fontHeight / 2 - 1, 0);
												LineTo(hdcMem, txt.left + 3, txt.top + fontHeight / 2 + 2);
												break;

											case D_PATHUPEND:
											case D_GRAYUPEND:
												if (symName == D_PATHUPEND) {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].hitextcolor]);
												}
												else {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].lowcolor]);
												}
												MoveToEx(hdcMem, txt.left + 2 * (g_AvWidthFont / 3), (txt.bottom + txt.top) / 2, 0);
												LineTo(hdcMem, txt.left + 3, (txt.bottom + txt.top) / 2);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 - 2);
												LineTo(hdcMem, txt.left + 1, txt.top - 1);
												break;

											case D_PATHUP:
											case D_GRAYUP:
												if (symName == D_PATHUP) {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].hitextcolor]);
												}
												else {
													SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].lowcolor]);
												}
												MoveToEx(hdcMem, txt.left + 2 * (g_AvWidthFont / 3), (txt.bottom + txt.top) / 2, 0);
												LineTo(hdcMem, txt.left + 3, (txt.bottom + txt.top) / 2);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 - 2);
												LineTo(hdcMem, txt.left + 1, txt.top - 1);
												break;

											case 'z':
												SelectObject(hdcMem, g_myPen[g_myScheme[m_Table.scheme].hitextcolor]);
												MoveToEx(hdcMem, txt.left + 1, txt.top, 0);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 - 1);
												LineTo(hdcMem, txt.left + 2, (txt.bottom + txt.top) / 2);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 1, (txt.bottom + txt.top) / 2);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 4, (txt.bottom + txt.top) / 2 - 3);
												MoveToEx(hdcMem, txt.left + 1, txt.bottom - 1, 0);
												LineTo(hdcMem, txt.left + 1, (txt.bottom + txt.top) / 2 + 1);
												LineTo(hdcMem, txt.left + 2, (txt.bottom + txt.top) / 2);
												MoveToEx(hdcMem, g_AvWidthFont + txt.left - 1, (txt.bottom + txt.top) / 2, 0);
												LineTo(hdcMem, g_AvWidthFont + txt.left - 4, (txt.bottom + txt.top) / 2 + 3);
												break;

											default:
												break;
										}
									}
									symName = 0;
									j = k;
									txt.left = txt.right;	// ��һ���ı�λ��
								}
								if (txt.left >= entry.right)
									break;
								if (mask & DRAW_GRAPH) {
									if (select & DRAW_UNICODE) {
										symName = s[j];
									}
									else {
										symName = t.at(j);
									}
								}
								drawMask = mask;
								if (!(drawMask & DRAW_MASK)) {
									drawMask &= ~DRAW_GRAPH;
								}
								k++;
								txt.right += g_AvWidthFont;
							}
							if (entry.right > txt.left) {
								txt.right = entry.right;
								if (select & DRAW_SELECT) {
									iBkColor = g_myScheme[m_Table.scheme].selbkcolor;
								}
								else {
									iBkColor = g_myScheme[m_Table.scheme].bkcolor;
								}
								FillRect(hdcMem, &txt, g_myBrush[iBkColor]);
							}
						}
						else {
							int opt;
							if (select & DRAW_GRAY) {
								iTxColor = g_myScheme[m_Table.scheme].lowcolor;
							}
							else if (select & DRAW_HILITE) {
								iTxColor = g_myScheme[m_Table.scheme].hitextcolor;
							}
							else {
								iTxColor = g_myScheme[m_Table.scheme].textcolor;
							}
							if (select & DRAW_EIP) {
								if (select & DRAW_BREAK) {
									iTxColor = g_myScheme[m_Table.scheme].hitextcolor;
								}
								else {
									iTxColor = g_myScheme[m_Table.scheme].bkcolor;
								}
								iBkColor = g_myScheme[m_Table.scheme].textcolor;
							}
							else if (select & DRAW_BREAK) {
								iTxColor = g_myScheme[m_Table.scheme].textcolor;
								iBkColor = g_myScheme[m_Table.scheme].hitextcolor;
							}
							else if (select & DRAW_SELECT) {
								iBkColor = g_myScheme[m_Table.scheme].selbkcolor;
							}
							else {
								iBkColor = g_myScheme[m_Table.scheme].bkcolor;
							}
							SetTextColor(hdcMem, g_myColor[iTxColor]);
							SetBkColor(hdcMem, g_myColor[iBkColor]);
							bottom = entry.bottom;
							if (select & DRAW_TOP) {
								entry.bottom -= fontHeight / 2;
								FillRect(hdcMem, &entry, g_myBrush[g_myScheme[m_Table.scheme].bkcolor]);
								entry.top += fontHeight;
								entry.bottom += fontHeight / 2;
								bottom += fontHeight / 2;
							}
							else if (select & DRAW_BOTTOM) {
								entry.bottom -= fontHeight - fontHeight / 2;
								bottom += fontHeight / 2;
							}
							if ((select & (DRAW_BREAK | DRAW_SELECT)) == (DRAW_BREAK | DRAW_SELECT)) {
								FillRect(hdcMem, &entry, g_myBrush[g_myScheme[m_Table.scheme].condbkcolor]);
								opt = ETO_CLIPPED;
							}
							else {
								opt = ETO_CLIPPED | ETO_OPAQUE;
							}
							if (select & DRAW_UNICODE) {
								ExtTextOutW(hdcMem, entry.left + g_AvWidthFont / 2, bottom, opt, &entry, s.GetString(), len, NULL);
							}
							else {
								ExtTextOutA(hdcMem, entry.left + g_AvWidthFont / 2, bottom, opt, &entry, t.c_str(), t.length(), NULL);
							}
							if (select & DRAW_TOP) {
								entry.top -= fontHeight / 2;
							}
							else if (select & DRAW_BOTTOM) {
								entry.bottom += fontHeight - fontHeight / 2;
								entry.top += fontHeight - fontHeight / 2;
								FillRect(hdcMem, &entry, g_myBrush[1]);
								entry.top -= fontHeight - fontHeight / 2;
							}
						}
					}
				}
				else {
					FillRect(hdcMem, &entry, g_myBrush[g_myScheme[scheme].auxcolor]);
				}
				SelectObject(hdcMem, g_myPen[g_myScheme[scheme].linecolor]);
				MoveToEx(hdcMem, entry.right, entry.top, 0);
				LineTo(hdcMem, entry.right, entry.bottom);	// �̵ķָ���
				entry.left = entry.right + 1;	// ��һ������λ��
				col++;	// ��һ��entry
			}
			if (row.right > entry.left) {
				entry.right = row.right;
				FillRect(hdcMem, &entry, g_myBrush[g_myScheme[m_Table.scheme].bkcolor]);
			}
			int height;
			if (m_Table.mode & TABLE_DIR) {
				if (m_bSnowFreeDrawing) {
					height = row.bottom - row.top;
					BitBlt(hdc, row.left, row.top, row.right - row.left, row.bottom - row.top, hdcMem, row.left,
						fontHeight - (row.bottom - row.top),
						SRCCOPY);
				}
				row.bottom = row.top;
			}
			else {
				if (m_bSnowFreeDrawing) {
					BitBlt(hdc, row.left, row.top, row.right - row.left, fontHeight, hdcMem, row.left, 0, SRCCOPY);
				}
				row.top = row.bottom;
			}
		}
		::ReleaseMutex(g_hMutex);

		if (!(m_Table.mode & TABLE_DIR) && row.top<client.bottom || m_Table.mode & TABLE_DIR
			&& row.bottom>client.top) {
			SelectObject(hdc, g_myPen[g_myScheme[m_Table.scheme].linecolor]);
			if (m_Table.mode & TABLE_DIR) {
				row.top = client.top;
			}
			else {
				row.bottom = client.bottom;
			}

			for (int m = 0; m < m_Bar.nbar; m++) {
				int dx = m_Bar.dx[m];
				row.right = row.left + dx - 1;
				if (dx > 3) {
					FillRect(hdc, &row, g_myBrush[g_myScheme[m_Table.scheme].bkcolor]);
				}
				else {
					FillRect(hdc, &row, g_myBrush[g_myScheme[m_Table.scheme].auxcolor]);
				}
				MoveToEx(hdc, row.right, row.top, NULL);
				LineTo(hdc, row.right, row.bottom);
				row.left = row.right + 1;
			}
			if (client.right > row.left) { // ���������
				row.right = client.right;
				FillRect(hdc, &row, g_myBrush[g_myScheme[m_Table.scheme].bkcolor]);
			}
		}
		if (m_bSnowFreeDrawing) {
			SelectObject(hdcMem, hgdiMem);
			DeleteObject(hBitmap);
			DeleteDC(hdcMem);
		}
		EndPaint(hw, &ps);
	}
	else {
		FillRect(hdc, &client, g_myBrush[g_myScheme[m_Table.scheme].bkcolor]);
		EndPaint(hw, &ps);
	}
}

template<class T>
void CTable<T>::SetTableWindowInfo(int maxColumns,int nlines) {
	ATLASSERT(nlines > 5 && maxColumns > 2);

	m_Rect.right = GetSystemMetrics(SM_CXVSCROLL) - 1;
	int j = 0;
	while (m_Bar.nbar > j && j < maxColumns) {
		if (m_Bar.nbar - 1 != j || m_Bar.defdx[j] != 256) {
			m_Rect.right += m_Bar.defdx[j];
		}
		else {
			m_Rect.right += 50 * g_AvWidthFont;
		}
		++j;
	}
	m_Rect.bottom = nlines * g_AvHighFont;
	if (m_Table.hscroll == 1) {
		m_Rect.bottom += GetSystemMetrics(SM_CYHSCROLL);
		m_Rect.bottom += g_AvHighFont + 4;
	}

	AdjustWindowRect(&m_Rect, WS_CHILD | WS_GROUP | WS_TABSTOP | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION, false);
	m_Rect.right -= m_Rect.left;
	m_Rect.bottom -= m_Rect.top;
	RECT client;
	GetClientRect(g_hWndMDIClient, &client);

	_style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_CLIPSIBLINGS
		| WS_VISIBLE | WS_VSCROLL | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION;
	m_Rect.top = CW_USEDEFAULT;
	m_Rect.left = CW_USEDEFAULT;

	if (!(m_Table.mode & TABLE_NOHSCR)) {
		_style = WS_CHILD | WS_GROUP | WS_TABSTOP | WS_CLIPSIBLINGS
			| WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION;
	}
	if (client.right < m_Rect.right) {
		m_Rect.right = client.right;
	}
	if (client.bottom < m_Rect.bottom) {
		m_Rect.bottom = client.bottom;
	}
}

template<class T>
int CTable<T>::Tablefunction(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	RECT client, bar;
	int bottomRow;
	int xPos=0, yPos = 0;
	WPARAM col = -1;
	int newdx;
	bool pressShiftKey, pressCtrlKey;
	int ret = 0;
	int selRow;

	GetClientRect(hw, &client);
	memcpy(&bar, &client, sizeof(bar));
	if (m_Table.showbar == 1) {
		client.top = g_AvHighFont + 4;
		bar.bottom = g_AvHighFont + 4;
	}
	else {
		bar.bottom = bar.top;
	}
	int rows = (client.bottom - client.top) / g_AvHighFont;
	if (rows <= 0) {
		rows = 1;
	}
	if (m_Table.mode & TABLE_DIR) {
		bottomRow = -1;
	}
	else {
		bottomRow = 1;
	}
	bool onBar = false;
	int xshift;
	int iHScrollPos, iVScrollPos;
	int lines;
	int iSplitLine = -1, left=0, x;

	if (msg == WM_MOUSEMOVE || WM_LBUTTONDOWN == msg || msg == WM_LBUTTONDBLCLK || msg == WM_LBUTTONUP
		|| msg == WM_RBUTTONDOWN || (msg == WM_TIMER && wp == 1)) {
		if (msg == WM_TIMER) {
			xPos = m_Bar.prevx;
			yPos = -1;
		}
		else {
			xPos = GET_X_LPARAM(lp);
			yPos = GET_Y_LPARAM(lp);
		}
		onBar = yPos >= bar.top && yPos < bar.bottom;
		if (bottomRow < 0) {
			yPos = client.bottom + client.top - yPos - 1;
		}
		int j = 0;
		xshift = -m_Table.xshift - 1;
		while (j < m_Bar.nbar && j < NBAR) {
			x = xshift + m_Bar.dx[j];
			if (abs(xPos - x) <= 1 && !(m_Bar.mode[j] & BAR_NORESIZE)) {
				iSplitLine = j;
			}
			if (xPos >= xshift && x > xPos) {
				col = j;
				left = xPos - xshift - 1;
				if (!(m_Bar.mode[j] & BAR_SHIFTSEL)) {
					left -= g_AvWidthFont / 2;
				}
				if (left < 0) {
					left = 0;
				}
			}
			xshift = x;
			j++;
		}
	}
	switch (msg) {
		case WM_HSCROLL:
		{
			newdx = -client.right;
			int i = 0;
			while (i < m_Bar.nbar) {
				newdx += m_Bar.dx[i];
				++i;
			}
			iHScrollPos = m_Table.xshift;
			switch (LOWORD(wp)) {
				case SB_LINELEFT:
					iHScrollPos -= g_AvWidthFont;
					break;
				case SB_LINERIGHT:
					iHScrollPos += g_AvWidthFont;
					break;
				case SB_PAGELEFT:
					iHScrollPos -= 8 * g_AvWidthFont;
					break;
				case SB_LEFT:
					iHScrollPos = 0;
					break;
				case SB_RIGHT:
					iHScrollPos = newdx;
					break;
				case SB_THUMBTRACK:
					iHScrollPos = MulDiv(HIWORD(wp), newdx, MAXTRACK);
					break;
				default:
					break;
			}
			if (iHScrollPos > newdx) {
				iHScrollPos = newdx;
			}
			if (iHScrollPos < 0) {
				iHScrollPos = 0;
			}
			if (iHScrollPos != m_Table.xshift) {
				m_Table.xshift = iHScrollPos;
				InvalidateRect(hw, nullptr, false);
			}
			return 0;
		}
		case WM_USER_SCR:
		{
			LONG style = GetWindowLong(hw, GWL_STYLE);
			if (style & WS_VSCROLL) {
				SetScrollRange(hw, SB_VERT, 0, MAXTRACK, false);
				ret = SendMessage(hw, WM_USER_VABS, rows, 0);
				if (bottomRow < 0)
					ret = MAXTRACK - ret;
				if (ret != GetScrollPos(hw, SB_VERT)) {
					SetScrollPos(hw, SB_VERT, ret, true);
				}
			}
			if (style & WS_HSCROLL) {
				SetScrollRange(hw, SB_HORZ, 0, MAXTRACK, false);
				newdx = -client.right;
				int i = 0;
				while (i < m_Bar.nbar) {
					newdx += m_Bar.dx[i];
					++i;
				}
				int nPos;
				nPos = MulDiv(m_Table.xshift, MAXTRACK, newdx);
				SetScrollPos(hw, SB_HORZ, nPos, true);
			}
			return 0;
		}
		case WM_DESTROY:
		{
			if (hw == g_hWndTop)
				g_hWndTop = 0;
			if (m_Table.mode & TABLE_SAVEPOS) {

			}
			return 0;
		}
		case WM_USER_VABS:
		{
			selRow = lp + m_Table.offset;
			// �����з���������ص��ײ�
			if (selRow > (signed int)(m_Table.data.n - wp)) {
				selRow = m_Table.data.n - wp;
			}
			if (selRow < 0) {
				selRow = 0;
			}
			if (lp && selRow == m_Table.offset)
				return -1;
			m_Table.offset = selRow;
			if (selRow) {
				return MulDiv(selRow, MAXTRACK, m_Table.data.n - wp);
			}
			else {
				return 0;
			}
		}
		case WM_USER_VREL:
		{
			if (m_Table.data.n > wp) {
				selRow = MulDiv(m_Table.data.n - wp, lp, MAXTRACK);
			}
			else {
				selRow = 0;
			}
			if (selRow > (signed int)(m_Table.data.n - wp)) {
				selRow = m_Table.data.n - wp;
			}
			if (selRow < 0) {
				selRow = 0;
			}
			if (lp && selRow == m_Table.offset) {
				return -1;
			}
			else {
				m_Table.offset = selRow;
				if (selRow) {
					return MulDiv(selRow, MAXTRACK, m_Table.data.n - wp);
				}
				else {
					return 0;
				}
			}
		}
		case WM_VSCROLL:
		{
			pressShiftKey = GetKeyState(VK_SHIFT) & 0x8000;
			switch (LOWORD(wp)) {
				case SB_LINEDOWN:
					if (pressShiftKey && m_Table.mode & TABLE_USERDEF) {
						ret = SendMessage(hw, WM_USER_VBYTE, rows, bottomRow);
					}
					else {
						ret = SendMessage(hw, WM_USER_VABS, rows, bottomRow);
					}
					break;
				case SB_LINEUP:
					if (pressShiftKey && m_Table.mode & TABLE_USERDEF) {
						ret = SendMessage(hw, WM_USER_VBYTE, rows, -bottomRow);
					}
					else {
						ret = SendMessage(hw, WM_USER_VABS, rows, -bottomRow);
					}
					break;
				case SB_PAGEUP:
					lines = rows - 1;
					if (lines <= 0)
						lines = 1;
					ret = SendMessage(hw, WM_USER_VABS, rows, bottomRow * -lines);
					break;
				case SB_PAGEDOWN:
					lines = rows - 1;
					if (lines <= 0)
						lines = 1;
					ret = SendMessage(hw, WM_USER_VABS, rows, bottomRow * lines);
					break;
				case SB_THUMBTRACK:
					iVScrollPos = HIWORD(wp);
					if (bottomRow < 0)
						iVScrollPos = 0x3fff - iVScrollPos;
					ret = SendMessage(hw, WM_USER_VREL, rows, iVScrollPos);
					break;
				default:
					ret = -1;
					break;
			}
			if (ret >= 0) {
				if (m_Table.mode & TABLE_FASTSEL) {
					RedrawWindow(hw, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				}
				else {
					InvalidateRect(hw, NULL, false);
				}
			}
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			HCURSOR hCursor;
			if (m_Bar.captured && GetCapture() == hw) {
				switch (m_Bar.captured) {
					case CAPT_SPLIT:
						hCursor = LoadCursor(NULL, IDC_SIZEWE);
						SetCursor(hCursor);
						newdx = xPos + m_Bar.dx[m_Bar.active] - m_Bar.prevx;
						if (newdx < 3) {
							int distance = 3 - newdx;
							newdx = 3;
							xPos += distance;
						}
						if (m_Bar.dx[m_Bar.active] != newdx) {
							m_Bar.dx[m_Bar.active] = newdx;
							InvalidateRect(hw, nullptr, false);
						}
						break;
					case CAPT_BAR:
						hCursor = ::LoadCursor(NULL, IDC_ARROW);
						SetCursor(hCursor);
						if (!onBar, m_Bar.active != col || m_Bar.mode[m_Bar.active] & BAR_PRESSED) {
							m_Bar.mode[m_Bar.active] &= ~BAR_PRESSED;
							InvalidateRect(hw, &bar, false);
						}
						else {
							m_Bar.mode[m_Bar.active] |= BAR_PRESSED;
							InvalidateRect(hw, &bar, false);
						}
						break;
					case CAPT_DATA:
						hCursor = LoadCursor(NULL, IDC_ARROW);
						SetCursor(hCursor);
						if (yPos < client.top || yPos >= client.bottom) {
							if (yPos >= client.bottom) {
								if (m_Bar.active <= 0) {
									SetTimer(hw, 1, 50, NULL);
								}
								m_Bar.active = 1;
								if (client.bottom + 48 < yPos) {
									m_Bar.active += (yPos - client.bottom - 48) / 16;
								}
							}
							else {
								if (m_Bar.active >= 0) {
									SetTimer(hw, 1, 50, NULL);
								}
								m_Bar.active = -1;
								// ��������ԭ����д����<����
								if (client.top - 48 > yPos) {
									m_Bar.active -= (client.top - 48 - yPos) / 16;
								}
							}
						}
						else {
							KillTimer(hw, 1);
							m_Bar.active = 0;
							if (SendMessage(hw, WM_USER_CNTS, LOWORD(col) << 16 | LOWORD(left / g_AvWidthFont),
								(yPos - client.top) / g_AvHighFont)) {
								if (m_Table.mode & TABLE_FASTSEL) {
									::RedrawWindow(hw, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
								}
								else {
									::InvalidateRect(hw, NULL, false);
								}
							}
						}
						break;
				}
				m_Bar.prevx = xPos;
			}
			else if (iSplitLine < 0) {
				hCursor = LoadCursor(NULL, IDC_ARROW);
				SetCursor(hCursor);
			}
			else {
				hCursor = LoadCursor(NULL, IDC_SIZEWE);
				SetCursor(hCursor);
			}
			return 0;
		}
		case WM_LBUTTONDBLCLK: {
			bool handle = onBar || iSplitLine >= 0 ||
				SendMessage(hw, WM_USER_DBLCLK, ((uint16_t)col << 16) | (uint16_t)(left / g_AvWidthFont), (yPos - client.top) / g_AvHighFont) != 1;
			if (!handle)
				break;
		}
		case WM_LBUTTONDOWN:
		{
			SetFocus(hw);
			if (iSplitLine < 0) {
				if (!onBar || (col & 0x80000000) != 0 || m_Bar.mode[col] & (BAR_NOSORT | BAR_DISABLED)) {
					if (!onBar || (col & 0x80000000) != 0) {
						pressShiftKey = GetKeyState(VK_SHIFT) & 0x800;
						UINT uMsg = WM_USER_CNTS;
						if (!pressShiftKey) {
							uMsg = WM_USER_STS;
						}
						int value = SendMessage(hw, uMsg, col << 16 | LOWORD(left / g_AvWidthFont),
							(yPos - client.top) / g_AvHighFont);
						if (value > 0) {
							if (m_Table.mode & TABLE_FASTSEL) {
								RedrawWindow(hw, nullptr, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
							}
							else {
								InvalidateRect(hw, nullptr, false);
							}
						}
						if (value >= 0) {
							SetCapture(hw);
							m_Bar.captured = CAPT_DATA;
							m_Bar.active = 0;
						}
					}
				}
				else {
					SetCapture(hw);
					m_Bar.captured = CAPT_BAR;
					m_Bar.mode[col] |= BAR_PRESSED;
					m_Bar.active = col;
					InvalidateRect(hw, &bar, false);
				}
			}
			else {
				SetCapture(hw);
				m_Bar.captured = CAPT_SPLIT;
				m_Bar.active = iSplitLine;
				m_Bar.prevx = xPos;
			}
			return 0;
		}
		case WM_LBUTTONUP:
		{
			if (GetCapture() == hw) {
				ReleaseCapture();
				if (m_Bar.captured == CAPT_BAR) {
					if (m_Bar.mode[m_Bar.active] & BAR_PRESSED) {
						m_Bar.mode[m_Bar.active] &= ~BAR_PRESSED;
						if (m_Bar.mode[m_Bar.active] & BAR_BUTTON) {
							InvalidateRect(hw, &bar, false);
							SendMessage(hw, WM_USER_BAR, m_Bar.active, 0);
						}
						else if (Sortsorteddata(m_Bar.active)) {
							InvalidateRect(hw, nullptr, false);// ���������ˣ���ˢ����������
						}
						else {
							InvalidateRect(hw, &bar, false);
						}
					}
				}
				else if (m_Bar.captured == CAPT_DATA) {
					KillTimer(hw, 1);
				}
				m_Bar.captured = CAPT_FREE;
			}
			return 0;
		}
		case WM_USER_CNTS:
		case WM_USER_STS:
		{
			if (m_Table.mode & TABLE_USERDEF) {
				return 0;
			}
			int selRow = lp + m_Table.offset;
			if (selRow >= m_Table.data.n) {
				selRow = m_Table.data.n - 1;
			}
			if (selRow == m_Table.data.selected && (!(m_Table.mode & TABLE_COLSEL)
				|| HIWORD(lp) == m_Table.colsel)) {
				return 0;
			}
			if (selRow > 0) {
				auto item = Getsortedbyselection(selRow);
				m_Table.data.seladdr = GetSeladdr(item);
			}
			m_Table.data.selected = selRow;
			if (m_Table.mode & TABLE_COLSEL) {
				m_Table.colsel = HIWORD(wp);
			}
			else {
				m_Table.colsel = 0;
			}
			return 1;
		}
		case WM_WINDOWPOSCHANGED:
		{
			if (*(HWND*)lp != g_hWndTop && g_hWndTop) {
				SetWindowPos(g_hWndTop, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOMOVE);
			}
			return DefMDIChildProc(g_hWndTop, WM_WINDOWPOSCHANGED, wp, lp);
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			GetSysKeyState(msg, lp, pressShiftKey, pressCtrlKey);
			int count = lp & 0x7FFF;
			if (count != 0) {
				if (count > 0x10) {
					count = 16;
				}
			}
			else {
				count = 1;
			}
			int shift;
			if (wp == VK_LEFT) {
				int colsel = m_Table.colsel;
				if (pressCtrlKey) {
					xshift = 0;
					for (int i = 0; i < m_Bar.nbar && xshift + m_Bar.dx[i] < m_Table.xshift; i++) {
						xshift += m_Bar.dx[i];
					}
				}
				else if (m_Table.mode & TABLE_COLSEL) {
					--colsel;
					xshift = m_Table.xshift;
					int width;
					do {
						width = m_Bar.dx[colsel];
						--colsel;
					} while (colsel >= 0 && width <= 3);

					if (colsel < 0) {
						colsel = m_Table.colsel;
					}
					else {
						xshift = 0;
						for (int i = 0; colsel > i; i++) {
							xshift += m_Bar.dx[i];
						}
						if (xshift > m_Table.xshift) {
							xshift = m_Table.xshift;
						}
					}
				}
				else {
					xshift = m_Table.xshift - count * g_AvWidthFont;
				}
				if (xshift < 0) {
					xshift = 0;
				}
				if (xshift != m_Table.xshift || colsel != m_Table.colsel) {
					m_Table.xshift = xshift;
					m_Table.colsel = colsel;
					InvalidateRect(hw, nullptr, NULL);
				}
				return 1;
			}
			else if (wp == VK_RIGHT) {
				int col = m_Table.colsel;
				int newdx = 0;
				if (pressCtrlKey) {
					shift = 0;
					for (int i = 0; i < m_Bar.nbar; i++) {
						if (m_Table.xshift >= newdx && i < m_Bar.nbar - 1) {
							shift += m_Bar.dx[i];
						}
						newdx += m_Bar.dx[i];
					}
				}
				else if (m_Table.mode & TABLE_COLSEL) {
					int tmp;
					do {
						tmp = m_Bar.dx[col];
						col++;
					} while (col < m_Bar.nbar && tmp <= 3);
					if (col >= m_Bar.nbar) {
						for (int i = 0; i < m_Bar.nbar; i++) {
							newdx += m_Bar.dx[i];
						}
						col = m_Table.colsel;
						shift = m_Table.xshift;
					}
					else {
						shift = 0;

						for (int i = 0; col > i; i++) {
							shift += m_Bar.dx[i];
						}
						newdx = shift + m_Bar.dx[col];
					}
				}
				else {
					for (int i = 0; i < m_Bar.nbar; i++) {
						newdx += m_Bar.dx[i];
					}
					shift = count * g_AvWidthFont + m_Table.xshift;
				}
				newdx -= client.right;
				if (shift > newdx) {
					shift = newdx;
				}
				if (shift < 0)
					shift = 0;
				if (shift != m_Table.xshift || col != m_Table.colsel) {
					m_Table.xshift = shift;
					m_Table.colsel = col;
					InvalidateRect(hw, nullptr, false);
				}
				return 1;
			}
			else {
				lines = count * bottomRow;
				switch (wp) {
					case VK_PRIOR:
					{
						int totalRow = rows - 1;
						if (totalRow <= 0) {
							totalRow = 1;
						}
						_page = lines * -totalRow;
						LPARAM lParam;
						if (pressCtrlKey) {
							lParam = 0x7FFFFFFF;
						}
						else {
							lParam = _page;
						}
						ret = SendMessage(hw, WM_USER_CHGS, rows, lParam);
						break;
					}
					case VK_NEXT:
					{
						int totalRow = rows - 1;
						if (totalRow <= 0) {
							totalRow = 1;
						}
						_page = lines * totalRow;
						LPARAM lParam;
						if (pressCtrlKey) {
							lParam = 0x7FFFFFFF;
						}
						else {
							lParam = _page;
						}
						ret = SendMessage(hw, WM_USER_CHGS, rows, lParam);
						break;
					}
					case VK_END:
					{
						ret = SendMessage(hw, WM_USER_CHGS, rows, 0x7FFFFFFE);
						break;
					}
					case VK_HOME:
					{
						ret = SendMessage(hw, WM_USER_CHGS, rows, 0x7FFFFFFF);
						break;
					}
					case VK_UP:
					{
						if (pressCtrlKey && m_Table.mode & TABLE_USERDEF) {
							ret = SendMessage(hw, WM_USER_VBYTE, rows, -lines);
						}
						else {
							ret = SendMessage(hw, WM_USER_CHGS, rows, -lines);
						}
						break;
					}
					case VK_DOWN:
					{
						if (pressCtrlKey && m_Table.mode & TABLE_USERDEF) {
							ret = SendMessage(hw, WM_USER_VBYTE, rows, lines);
						}
						else {
							ret = SendMessage(hw, WM_USER_CHGS, rows, lines);
						}
						break;
					}
					default:
						break;
				}
				if (ret >= 0) {
					if (m_Table.mode & TABLE_FASTSEL) {
						RedrawWindow(hw, nullptr, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
					}
					else {
						InvalidateRect(hw, nullptr, false);
					}
				}
				return 1;
			}
		}
		case WM_USER_CHGS:
		{
			if (lp == 0x7FFFFFFF) {
				if (bottomRow <= 0) {
					selRow = m_Table.data.n - 1;
				}
				else {
					selRow = 0;
				}
			}
			else if (lp == 0x7FFFFFFE) {
				if (bottomRow <= 0) {
					selRow = 0;
				}
				else {
					selRow = m_Table.data.n - 1;
				}
			}
			else {
				selRow = lp + m_Table.data.selected;
			}
			if (selRow >= m_Table.data.n) {
				selRow = m_Table.data.n - 1;
			}
			if (selRow < 0)
				selRow = 0;
			if (selRow == m_Table.data.selected)
				return -1;
			auto item = Getsortedbyselection(selRow);
			ulong addr = GetSeladdr(item);
			if (!addr) {
				return -1;
			}
			m_Table.data.selected = selRow;
			m_Table.data.seladdr = addr;
			int offset = _page + m_Table.offset;
			if (selRow >= offset + wp) {
				offset = selRow - wp + 1;
			}
			if (selRow < offset)
				offset = selRow;
			if (offset > (signed int)(m_Table.data.n - wp)) {
				offset = m_Table.data.n - wp;
			}
			if (offset < 0)
				offset = 0;
			m_Table.offset = offset;
			if (offset) {
				return MulDiv(offset, 0x4000, m_Table.data.n - wp);
			}
			else {
				return 0;
			}
		}
		case WM_TIMER:
		{
			if (!m_Bar.active) {
				::KillTimer(hw, 1);
				return 0;
			}
			ret = ::SendMessage(hw, WM_USER_VABS, rows, m_Bar.active);
			int msglp;
			if (m_Bar.active >= 0) {
				msglp = rows - 1;
			}
			else {
				msglp = 0;
			}
			int cnts = ::SendMessage(hw, WM_USER_CNTS, (LOWORD(col) << 16) | LOWORD(left / g_AvWidthFont), msglp);
			if (ret < 0 &&!cnts) {
				return 0;
			}
			if (m_Table.mode & TABLE_FASTSEL) {
				::RedrawWindow(hw, NULL, NULL, RDW_VALIDATE | RDW_UPDATENOW);
			}
			else {
				::InvalidateRect(hw, NULL, false);
			}
			return 0;
		}
	}
	return 1;
}


