#pragma once
#include "Table.h"
#include "Disassembly.h"
#include "CpuHelper.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// DUMP /////////////////////////////////////

// Standard dump types.
#define DU_ESCAPABLE   0x20000L        // 创建可回退的dump窗口
#define DU_BACKUP      0x10000L        // Bit indicates that backup is displayed
#define DU_TYPE        0x0F000L        // Mask for dump type
#define DU_COUNT       0x00FF0L        // Mask for number of items/line
#define DU_SIZE        0x0000FL        // Mask for size of single item

#define DU_UNDEF       0x00000L        // Undefined dump type
#define DU_HEXTEXT     0x01000L        // Hexadecimal dump with ASCII text
#define DU_TEXT        0x02000L        // Character dump
#define DU_UNICODE     0x03000L        // Unicode dump
#define DU_INT         0x04000L        // Integer signed dump
#define DU_UINT        0x05000L        // Integer unsigned dump
#define DU_IHEX        0x06000L        // Integer hexadecimal dump
#define DU_FLOAT       0x07000L        // Floating-point dump
#define DU_ADDR        0x08000L        // Address dump
#define DU_DISASM      0x09000L        // Disassembly
#define DU_HEXUNI      0x0A000L        // Hexadecimal dump with UNICODE text
#define DU_ADRASC      0x0B000L        // Address dump with ASCII text
#define DU_ADRUNI      0x0C000L        // Address dump with UNICODE text
#define DU_SPEC        0x0D000L        // Special decoding

// Standard menu types.
#define MT_BACKUP      0x0001          // Backup, Undo
#define MT_COPY        0x0002          // Copy to clipboard
#define MT_EDIT        0x0004          // Edit, Modify, Assemble
#define MT_SEARCH      0x0008          // Search, Next
#define MT_DHISTORY    0x0010          // Previous, Next in history

class CDump :public CTable<t_dump> {
public:
	int Dumpfunction(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	virtual int ParseTableEntry(CString& s, char& mask, int& select, t_dump* info, int column);
	void Setdumptype(t_dump& dump,int dumpType);
	void SetdumpAddress(t_dump& dump);
	
private:
	ulong _firstRow;
	ulong _curAddress;
	ulong _curRow;
	ulong _dumpAddress;
	ulong _size;
	ulong _highlight;
	HGLOBAL _block = nullptr;
	ulong _blockSize;
	HGLOBAL _profile = nullptr;
	ulong _profileSize;
	t_module* _module = nullptr;
	ulong _targetAddress;
	t_disasm _disasm;
};

