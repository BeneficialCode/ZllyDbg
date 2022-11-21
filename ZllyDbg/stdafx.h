// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
//#define WINVER		0x0501
//#define _WIN32_WINNT	0x0501
//#define _WIN32_IE	0x0600
//#define _RICHEDIT_VER	0x0300

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;
extern int g_AvHighFont;
extern int g_AvWidthFont;
extern HFONT g_hOemFixedFont;

extern HPEN g_hBlackPen, g_hWhitePen, g_hDarkGrayPen;
extern HPEN g_myPen[20];

extern COLORREF g_crLightGray, g_crDarkGray, g_crBlack;
extern COLORREF g_myColor[20];

extern HBRUSH g_hLightGrayBrush;

extern HWND g_hModuleFrm, g_hWndMDIClient, g_hWndTop, g_hThreadFrm, g_hMemoryFrm, g_hCpuFrm;
extern HWND g_hBpFrm;

extern HBRUSH g_myBrush[20];

extern HANDLE g_hProcess;
extern HMODULE g_hMainModule;
extern DWORD g_mainThreadId;
extern HANDLE g_hMainThread;
extern HANDLE g_hMutex;
extern DWORD g_cpuThreadId;

extern bool g_isNeedListmodule;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atltime.h>

#include <wil/resource.h>
#include <vector>
#include <memory>
#include <bitset>
#include <string>
#include <iostream>

#include <TlHelp32.h>
#include <Psapi.h>
#include <DbgHelp.h>
#include "capstone/capstone.h"

#pragma comment(lib,"capstone.lib")

#include "SortHelper.h"

struct t_scheme { // 颜色主题
	std::string	name;          // 主题名字
	int  textcolor;            // 文本颜色
	int  hitextcolor;          // 文本高亮颜色
	int  lowcolor;             // 辅助文本颜色
	int  bkcolor;              // 背景颜色
	int  selbkcolor;           // 选中时的背景颜色
	int  linecolor;            // 分割线的颜色
	int  auxcolor;             // 辅助对象的颜色
	int  condbkcolor;          // 条件断点的颜色
};

enum t_status {	// 进程/线程状态
	STAT_NONE=0,				// 空
	STAT_STOPPED,				// 挂起
	STAT_EVENT,					// 处理调试事件，暂停
	STAT_RUNNING,				// 运行
	STAT_FINISHED,				// 进程结束
	STAT_CLOSING				// 进程请求终止
};

extern t_scheme g_myScheme[8];

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// GENERAL DECLARATIONS /////////////////////////////
typedef unsigned char  uchar;          // Unsigned character (byte)
typedef unsigned short ushort;         // Unsigned short
typedef unsigned int   uint;           // Unsigned integer
typedef unsigned long  ulong;          // Unsigned long

#define TEXTLEN        256             // Maximal length of text string
#define ARGLEN         1024            // Maximal length of argument string
#define USERLEN        4096            // Maximal length of record in .udd file
#define SHORTLEN       25               // Maximal length of short name

#define BLACK          0               // Indices of colours used by OllyDbg. In
#define BLUE           1               // syntax highlighting, use only colours
#define GREEN          2               // 0 to 15 in the least significant bits
#define CYAN           3               // of the corresponding mask byte.
#define RED            4
#define MAGENTA        5
#define BROWN          6
#define LIGHTGRAY      7
#define DARKGRAY       8
#define LIGHTBLUE      9
#define LIGHTGREEN     10
#define LIGHTCYAN      11
#define LIGHTRED       12
#define LIGHTMAGENTA   13
#define YELLOW         14
#define WHITE          15
#define MINT           16
#define SKYBLUE        17
#define IVORY          18
#define GRAY           19

#define NCOLORS        20              // Total number of defined colours

#define BKTRANSP       0x00            // Background colours in syntax hiliting
#define BKBLACK        0x10
#define BKGRAY         0x20
#define BKWHITE        0x30
#define BKCYAN         0x40
#define BKGREEN        0x50
#define BKRED          0x60
#define BKYELLOW       0x70

#define BLACKWHITE     0               // Colour schemes used by OllyDbg
#define BLUEGOLD       1
#define SKYWIND        2
#define NIGHTSTARS     3
#define SCHEME4        4
#define SCHEME5        5
#define SCHEME6        6
#define SCHEME7        7

#define FIXEDFONT      0               // Fonts used by OllyDbg. Variable-pitch
#define TERMINAL6      1               // fonts are placed at the end of this
#define FIXEDSYS       2               // table.
#define COURIERFONT    3
#define LUCIDACONS     4
#define FONT5          5
#define FONT6          6
#define FONT7          7
#define MAINFONT       8
#define SYSFONT        9
#define INFOFONT       10

#define BAR_PRESSED    0x01            // Bar segment pressed, used internally
#define BAR_DISABLED   0x02            // Bar segment disabled
#define BAR_NOSORT     0x04            // Flat bar column, supports no sorting
#define BAR_NORESIZE   0x08            // Bar column cannot be resized
#define BAR_BUTTON     0x10            // Segment sends WM_USER_BAR
#define BAR_SHIFTSEL   0x20            // Selection shifted 1/2 char to left

#define CAPT_FREE      0               // Bar and data are not captured
#define CAPT_SPLIT	   0x1
#define CAPT_BAR	   0x2
#define CAPT_DATA	   0x3


#define TABLE_DIR      0x0001          // Bottom-to-top table
#define TABLE_COPYMENU 0x0002          // Attach copy item
#define TABLE_SORTMENU 0x0004          // Attach sort menu
#define TABLE_APPMENU  0x0010          // Attach appearance menu
#define TABLE_WIDECOL  0x0020          // Attach wide columns menu item
#define TABLE_USERAPP  0x0040          // Attach user-processed appearance menu
#define TABLE_USERDEF  0x0080          // User-drawn table
#define TABLE_NOHSCR   0x0100          // Table contains no horizontal scroll
#define TABLE_SAVEPOS  0x0200          // Save position & appearance to .ini
#define TABLE_CPU      0x0400          // Table belongs to CPU window
#define TABLE_FASTSEL  0x0800          // Update when selection changes
#define TABLE_COLSEL   0x1000          // Column-wide selection
#define TABLE_SAVEAPP  0x2000          // Save multiinstance appearance to .ini
#define TABLE_HILMENU  0x4000          // Attach Syntax highlighting menu
#define TABLE_ONTOP    0x8000          // Attach Always on top menu

#define DRAW_NORMAL    0x0000          // Normal plain text
#define DRAW_GRAY      0x0001          // Grayed text
#define DRAW_HILITE    0x0002          // 绘制高亮文本
#define DRAW_UL        0x0004          // Underlined text
#define DRAW_SELECT    0x0008          // 绘制选中背景色
#define DRAW_EIP       0x0010          // Inverted normal text/background
#define DRAW_BREAK     0x0020          // 绘制断点背景色
#define DRAW_GRAPH     0x0040          // Graphical element
#define DRAW_DIRECT    0x0080          // Direct text colour index (mask only)
#define DRAW_MASK      0x0080          // Use mask to set individual colors
#define DRAW_EXTSEL    0x0100          // Extend mask till end of column
#define DRAW_UNICODE   0x0200          // Text in UNICODE
#define DRAW_TOP       0x0400          // 绘制文本上半截
#define DRAW_BOTTOM    0x0800          // 绘制文本下半截

// Please note: Although types here contain mostly unique bit assignments, it's
// not really necessary. Same bits, except for reserved general types, can be
// freely shared between different types of sorted data.
// General item types:
#define TY_NORMAL	   0x00000000		
#define TY_NEW         0x00000001      // Item is new
#define TY_CONFIRMED   0x00000002      // Item still exists
#define TY_MAIN        0x00000004      // Main item (thread or module)
#define TY_INVALID     0x00000008      // Invalid type (item does not exist)
#define TY_SELECTED    0x80000000      // Reserved for multiple selection
// Module-specific types:
#define TY_REPORTED    0x00000010      // Stop on module was reported
// Reference-specific types:
#define TY_REFERENCE   0x00000020      // Item is a real reference
#define TY_ORIGIN      0x00000040      // Item is a search origin
// Breakpoint-specific types:
#define TY_STOPAN      0x00000080      // Stop animation if TY_ONESHOT
#define TY_SET         0x00000100      // Code INT3 is in memory

// Set permanent (user) breakpoint or restore disabled
#define TY_ACTIVE      0x00000200      // Permanent breakpoint 

// Temporarily deactivate permanent breakpoint. 
// If TY_ACTIVE and TY_DISABLED are set simultaneously, TY_DISABLED is ignored
#define TY_DISABLED    0x00000400      // Permanent disabled breakpoint 

// Set one-shot breakpoint that will be automatically removed when hit. Doesn't interfere with active breakpoint
#define TY_ONESHOT     0x00000800      // Temporary stop

// temporary breakpoint that will be automatically removed when hit. 
// Execution continues automatically. TY_TEMP does not interfere with active breakpoint
#define TY_TEMP        0x00001000      // Temporary breakpoint
#define TY_KEEPCODE    0x00002000      // Set and keep command code

// Leave associated names of types NM_BREAK, NM_BREAKEXPR, NM_BREAKEXPL and NM_PLUGCMD unchanged.
#define TY_KEEPCOND    0x00004000      // Keep condition unchanged (0: remove)
#define TY_NOUPDATE    0x00008000      // Don't redraw breakpoint window
#define TY_RTRACE      0x00010000      // Pseudotype of run trace breakpoint
// Namelist-specific types:
#define TY_EXPORT      0x00010000      // Exported name
#define TY_IMPORT      0x00020000      // Imported name
#define TY_LIBRARY     0x00040000      // Name extracted from object file
#define TY_LABEL       0x00080000      // User-defined name
#define TY_ANYNAME     0x000F0000      // Any of the namelist flags above
#define TY_KNOWN       0x00100000      // Name of known function
// Memory-specific types:
#define TY_DEFHEAP     0x00020000      // Contains default heap
#define TY_HEAP        0x00040000      // Contains non-default heap
#define TY_SFX         0x00080000      // Contains self-extractor
#define TY_CODE        0x00100000      // Contains image of code section
#define TY_DATA        0x00200000      // Contains image of data section
#define TY_IMPDATA     0x00400000      // Memory block includes import data
#define TY_EXPDATA     0x00800000      // Memory block includes export data
#define TY_RSRC        0x01000000      // Memory block includes resources
#define TY_RELOC       0x02000000      // Memory block includes relocation data
#define TY_STACK       0x04000000      // Contains stack of some thread
#define TY_THREAD      0x08000000      // Contains data block of some thread
#define TY_HEADER      0x10000000      // COFF header
#define TY_ANYMEM      0x1FFE0000      // Any of the memory flags above
#define TY_GUARDED     0x20000000      // NT only: guarded memory block
// Procedure data-specific types:
#define TY_PURE        0x00004000      // No side effects except in stack
#define TY_PASCAL      0x00010000      // Procedure ends with RET nnn
#define TY_C           0x00020000      // ADD ESP,nnn after call to procedure
#define TY_NOTENTRY    0x00100000      // Not necessarily entry point
// Switch data-specific types.
#define TY_CHARSW      0x00100000      // ASCII switch
#define TY_WMSW        0x00200000      // Window message switch
#define TY_EXCEPTSW    0x00400000      // Exception switch
// Stack walk data-specific types.
#define TY_RELIABLE    0x01000000      // Reliable call
#define TY_GUESSED     0x02000000      // Not a real entry, just guessed
#define TY_BELONGS     0x04000000      // Not a real entry, just belongs to proc
// Call tree-specific types.
#define TY_RECURSIVE   0x00000100      // Routine calls self
#define TY_TERMINAL    0x00000200      // Leaf function, doesn't call others
#define TY_SYSTEM      0x00000400      // Function resides in system DLL
#define TY_DIRECT      0x00000800      // Called directly
#define TY_NODATA      0x00001000      // Not analyzed or outside procedure
#define TY_DUMMY       0x00002000      // Consists of single RET command
#define TY_NOSIDE      0x00004000      // No side effects except in stack

#define WM_USER_MENU   (WM_USER+101)   // 激活上下文菜单	// 0x465
#define WM_USER_SCR    (WM_USER+102)   // Redraw scroll(s)	0x466
#define WM_USER_SAVE   (WM_USER+103)   // Save data to disk			0x467
#define WM_USER_VABS   (WM_USER+104)   // Scroll contents of window by lines	0x468
#define WM_USER_VREL   (WM_USER+105)   // Scroll contents of window by percent	0x469
#define WM_USER_VBYTE  (WM_USER+106)   // Scroll contents of window by bytes	0x46a
#define WM_USER_STS    (WM_USER+107)   // Start selection in window		0x46b
#define WM_USER_CNTS   (WM_USER+108)   // Continue selection in window	0x46c
#define WM_USER_CHGS   (WM_USER+109)   // 移动单行选择		0x46d
#define WM_USER_BAR    (WM_USER+110)   // Message from bar segment as button	0x46e
#define WM_USER_DBLCLK (WM_USER+111)   // Doubleclick in column			0x46f
#define WM_USER_SIZE   (WM_USER+112)   // Resize children in CPU window	0x470
#define WM_USER_FOCUS  (WM_USER+113)   // Set focus to child of CPU window	0x471
#define WM_USER_FILE   (WM_USER+114)   // Change state of file dump		0x472
#define WM_USER_HERE   (WM_USER+115)   // Query presence list		0x473
#define WM_USER_CHALL  (WM_USER+116)   // Redraw (almost) everything		0x474
#define WM_USER_CHMEM  (WM_USER+117)   // Range of debuggee's memory changed	0x475
#define WM_USER_CHREG  (WM_USER+118)   // Debuggee's register(s) changed	// 0x476
#define WM_USER_CHNAM  (WM_USER+119)   // Redraw name tables		//	0x477
#define WM_USER_MOUSE  (WM_USER+120)   // Check mouse coordinates	// 0x478
#define WM_USER_KEY    (WM_USER+121)   // Emulate WM_KEYDOWN		// 0x479
#define WM_USER_SYSKEY (WM_USER+122)   // Emulate WM_SYSKEYDOWN		// 0x47A

// Constants used for scrolling and selection.
#define MAXTRACK       16384           // Maximal scroll of user-drawn table
#define MOVETOP        0x7FFFFFFFL     // Move selection to top of table
#define MOVEBOTTOM     0x7FFFFFFEL     // Move selection to bottom of table

#define CONT_BROADCAST 0x0000          // Continue sending msg to other windows
#define STOP_BROADCAST 0x1234          // Stop sending message to other windows

// Symbolic names for graphical characters. Any other character is displayed
// as space. Use only characters in range [1..0x7F]!
#define D_SPACE        'N'             // Space
#define D_SEP          ' '             // 分割线
#define D_BEGIN        'B'             // 函数头
#define D_BODY         'I'             // 函数体
#define D_ENTRY        'J'             // Loop entry point
#define D_LEAF         'K'             // Intermediate leaf on a tree
#define D_END          'E'             // 函数尾
#define D_SINGLE       'S'             // Single-line scope
#define D_ENDBEG       'T'             // End and begin of stack scope
#define D_POINT        '.'             // Point
#define D_JMPUP        'U'             // Jump upstairs
#define D_JMPOUT       '<'             // 跳转到模块外，或者同一个地方
#define D_JMPDN        'D'             // 向下跳转
#define D_PATHUP       'u'             // Jump path upstairs (highlighted)
#define D_GRAYUP       'v'             // Jump path upstairs (grayed)
#define D_PATH         'i'             // Jump path through text (highlighted)
#define D_GRAYPATH     'j'             // Jump path through text (grayed)
#define D_PATHDN       'd'             // Jump path downstairs (highlighted)
#define D_GRAYDN       'e'             // Jump path downstairs (grayed)
#define D_PATHUPEND    'r'             // End of path upstairs (highlighted)
#define D_GRAYUPEND    's'             // End of path upstairs (grayed)
#define D_PATHDNEND    'f'             // End of path downstairs (highlighted)
#define D_GRAYDNEND    'g'             // End of path downstairs (grayed)
#define D_SWTOP        't'             // switch 开始
#define D_SWBODY       'b'             // switch 体
#define D_CASE         'c'             // Intermediate switch case
#define D_LASTCASE     'l'             // Last switch case

#define DISASM_SIZE    0               // Determine command size only
#define DISASM_DATA    1               // Determine size and analysis data
#define DISASM_TRACE   2               // Trace integer registers
#define DISASM_FILE    3               // Disassembly, no symbols/registers
#define DISASM_CODE    4               // Disassembly, registers undefined
#define DISASM_ALL     5               // Complete disassembly
#define DISASM_RTRACE  6               // Disassemble with run-trace registers

#define DISASM_MODE    0x0000000F      // Mask to extract disassembling mode
#define DISASM_HILITE  0x000F0000      // Mask to extract highlighting mode
#define DISASM_HLSHIFT 16              // Shift to extract highlighting mode

#define DBG_NONE	(0x0)
#define DBG_ATTACH	(0x2)
#define DBG_CREATE	(0x1)


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// NAME FUNCTIONS ////////////////////////////////

// Types of names used in name functions. Note that higher-priority types have
// smaller identifiers!
#define NM_NONAME      0x00            // Undefined name
#define NM_ANYNAME     0xFF            // Name of any type
// Names saved in the data file of module they appear.
#define NM_PLUGCMD     0x30            // Plugin commands to execute at break
#define NM_LABEL       0x31            // User-defined label
#define NM_EXPORT      0x32            // Exported (global) name
#define NM_IMPORT      0x33            // Imported name
#define NM_LIBRARY     0x34            // Name from library or object file
#define NM_CONST       0x35            // User-defined constant
#define NM_COMMENT     0x36            // User-defined comment
#define NM_LIBCOMM     0x37            // Comment from library or object file
#define NM_BREAK       0x38            // Condition related with breakpoint
#define NM_ARG         0x39            // Arguments decoded by analyzer
#define NM_ANALYSE     0x3A            // Comment added by analyzer
#define NM_BREAKEXPR   0x3B            // Expression related with breakpoint
#define NM_BREAKEXPL   0x3C            // Explanation related with breakpoint
#define NM_ASSUME      0x3D            // Assume function with known arguments
#define NM_STRUCT      0x3E            // Code structure decoded by analyzer
#define NM_CASE        0x3F            // Case description decoded by analyzer
// Names saved in the data file of main module.
#define NM_INSPECT     0x40            // Several last inspect expressions
#define NM_WATCH       0x41            // Watch expressions
#define NM_ASM         0x42            // Several last assembled strings
#define NM_FINDASM     0x43            // Several last find assembler strings
#define NM_LASTWATCH   0x48            // Several last watch expressions
#define NM_SOURCE      0x49            // Several last source search strings
#define NM_REFTXT      0x4A            // Several last ref text search strings
#define NM_GOTO        0x4B            // Several last expressions to follow
#define NM_GOTODUMP    0x4C            // Several expressions to follow in Dump
#define NM_TRPAUSE     0x4D            // Several expressions to pause trace
// Pseudonames.
#define NM_IMCALL      0xFE            // Intermodular call

#define NMHISTORY      0x40            // Converts NM_xxx to type of init list

// Status of animation or trace.
#define ANIMATE_OFF    0               // No animation
#define ANIMATE_IN     1               // Animate into
#define ANIMATE_OVER   2               // Animate over
#define ANIMATE_RET    3               // Execute till RET
#define ANIMATE_SKPRET 4               // Skip RET instruction
#define ANIMATE_USER   5               // Execute till user code
#define ANIMATE_TRIN   6               // Run trace in
#define ANIMATE_TROVER 7               // Run trace over
#define ANIMATE_STOP   8               // Gracefully stop animation

// Parameter stepmode in call to Go().
#define STEP_SAME      0               // Same action as on previous call
#define STEP_RUN       1               // Run program
#define STEP_OVER      2               // Step over
#define STEP_IN        3               // Step in
#define STEP_SKIP      4               // Skip sequence

#define HB_FREE        0               // Breakpoint is not used
#define HB_CODE        1               // Active on command execution
#define HB_ACCESS      2               // Active on read/write access
#define HB_WRITE       3               // Active on write access
#define HB_IO          4               // Active on port I/O
#define HB_ONESHOT     5               // One-shot on command execution
#define HB_STOPAN      6               // One-shot on command and stop
#define HB_TEMP        7               // Temporary on command execution

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
