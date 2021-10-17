#include "stdafx.h"
#include "Dump.h"
#include "Disassembly.h"
#include "DbgEngine.h"
#include "MemoryHelper.h"


void CDump::Setdumptype(t_dump& dump, int dumpType) {
	// For variable-length types size is 1
	int size = dumpType & DU_SIZE;
	int count = (dumpType & DU_COUNT) >> 4;
	int type = (dumpType & DU_TYPE);

	int i1;
	int i2;
	m_Bar.mode[1] = BAR_NOSORT;
	m_Bar.mode[2] = BAR_NOSORT;
	m_Bar.mode[3] = BAR_NOSORT;
	m_Bar.defdx[0] = 9;

	switch (type) {
		case DU_HEXTEXT:
			m_Bar.defdx[1] = 3 * count;
			m_Bar.name[1] = "Hex Dump";
			m_Bar.mode[1] = BAR_SHIFTSEL | BAR_BUTTON;
			m_Bar.defdx[2] = count + 1;
			m_Bar.name[2] = "ASCII";
			m_Bar.mode[2] = BAR_BUTTON;
			m_Bar.nbar = 3;
			break;

		case DU_TEXT:
			m_Bar.defdx[1] = count + 1;
			m_Bar.mode[1] = BAR_BUTTON;
			m_Bar.name[1] = "ASCII Dump";
			m_Bar.nbar = 2;
			break;

		case DU_UNICODE:
			m_Bar.defdx[1] = count + 1;
			m_Bar.mode[1] = BAR_BUTTON;
			m_Bar.name[1] = "Unicode Dump";
			m_Bar.nbar = 2;
			break;

		case DU_INT:
		case DU_UINT:
		case DU_IHEX:
			if (size == 2) {
				dump.itemwidth = 7;
				m_Bar.name[1] = "16bit Short";
			}
			else {
				dump.itemwidth = 12;
				m_Bar.name[1] = "32bit Long";
			}
			m_Bar.defdx[1] = count * dump.itemwidth + 1;
			m_Bar.mode[1] = BAR_BUTTON | BAR_SHIFTSEL;
			m_Bar.nbar = 3;
			break;

		case DU_FLOAT:
			if (size == 4) {
				dump.itemwidth = 15;
				m_Bar.name[1] = "32bit Float";
			}
			else if (size == 8) {
				dump.itemwidth = 25;
				m_Bar.name[1] = "64bit Double";
			}
			else {
				dump.itemwidth = 31;
				m_Bar.name[1] = "80bit Long Double";
			}
			m_Bar.defdx[1] = count * dump.itemwidth + 1;
			m_Bar.mode[1] = BAR_BUTTON | BAR_SHIFTSEL;
			m_Bar.nbar = 2;
			break;

		case DU_ADDR:
			dumpType = DU_ADDR | 0x14;
			m_Bar.defdx[1] = 9;
			m_Bar.name[1] = "Value";
			m_Bar.defdx[2] = 256;
			m_Bar.name[2] = "Comment";
			m_Bar.nbar = 3;
			break;

		case DU_DISASM:
			dumpType = DU_DISASM | 0x11;
			m_Bar.defdx[1] = 17;
			m_Bar.name[1] = "Hex Dump";
			m_Bar.defdx[2] = 40;
			m_Bar.name[2] = "Disassembly";
			m_Bar.defdx[3] = 256;
			m_Bar.name[3] = "Comment";
			if (dump.dumptype != dumpType) {
				dump.commentmode = 0; // comment
			}
			if ((m_Table.mode & TABLE_CPU) != 0) {
				m_Bar.mode[3] = BAR_BUTTON;
			}
			m_Bar.nbar = 4;
			break;
		
		case DU_HEXUNI:
			m_Bar.defdx[1] = 3 * count;
			m_Bar.name[1] = "Hex Dump";
			m_Bar.mode[1] = BAR_BUTTON | BAR_SHIFTSEL;
			i1 = count & 1;
			i2 = (dumpType & DU_COUNT) >> 5;
			if (i2 < 0) {
				i2 += i1;
			}
			m_Bar.defdx[2] = i2 + 1;
			m_Bar.name[2] = "Unicode";
			m_Bar.mode[2] = BAR_BUTTON;
			m_Bar.nbar = 3;
			break;

		case DU_ADRASC:
			dumpType = DU_ADRASC | 0x41;
			m_Bar.defdx[1] = 9;
			m_Bar.name[1] = "Value";
			m_Bar.defdx[2] = 5;
			m_Bar.name[2] = "ASCII";
			m_Bar.mode[2] = BAR_BUTTON;
			m_Bar.defdx[3] = 256;
			m_Bar.name[3] = "Comment";
			m_Bar.nbar = 4;
			break;

		case DU_ADRUNI:
			dumpType = DU_ADRUNI | 0x41;
			m_Bar.defdx[1] = 9;
			m_Bar.name[1] = "Value";
			m_Bar.defdx[2] = 5;
			m_Bar.name[2] = "Unicode";
			m_Bar.mode[2] = BAR_BUTTON;
			m_Bar.defdx[3] = 256;
			m_Bar.name[3] = "Comment";
			m_Bar.nbar = 4;
			break;

		case DU_SPEC:
			dumpType = DU_SPEC | 0x11;
			m_Bar.defdx[1] = 13;
			m_Bar.name[1] = "Hex Dump";
			m_Bar.defdx[2] = 5;
			m_Bar.name[2] = "Data";
			m_Bar.mode[2] = BAR_BUTTON;
			m_Bar.defdx[3] = 256;
			m_Bar.name[3] = "Comment";
			m_Bar.nbar = 4;
			break;

		default:
			m_Bar.nbar = 1;
			break;
	}
	dump.dumptype = dumpType;
	SetdumpAddress(dump);
	Defaultbar();
}

void CDump::SetdumpAddress(t_dump& dump) {
	if ((dump.menutype & 0x8000) == 0) {
		m_Bar.mode[0] = BAR_NOSORT;
		m_Bar.name[0] = "Address";
		dump.dumptype &= 0xFFFEFFFF;
	}
	else {
		m_Bar.name[0] = "Address";
	}
}

int CDump::Dumpfunction(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	t_dump* dump = (t_dump*)GetWindowLong(hw, GWL_USERDATA);
	ulong totalSize;
	ulong maxAddress;
	int type = dump->dumptype & DU_TYPE;
	ulong curAddress = 0;
	ulong bytes;
	ulong leftBytes;
	ulong fix;
	ulong newSel0 = dump->sel0;
	ulong newSel1 = dump->sel1;
	int usedec = 0;

	if (msg > WM_USER_VABS) {
		switch (msg) {
			case WM_USER_VREL: // by percent
			{
				if (!dump->size) {
					return 0;
				}
				totalSize = (dump->dumptype & DU_SIZE) * ((dump->dumptype & DU_COUNT) >> 4);
				if (!totalSize) {
					totalSize = 1;
				}
				maxAddress = dump->size + dump->base;
				if (type == DU_DISASM) {
					curAddress = dump->base + MulDiv(dump->size, lp, MAXTRACK);
					if (curAddress > maxAddress) {
						curAddress = maxAddress;
					}
					// 反汇编
					curAddress = Disassembleback(dump->filecopy, dump->base, dump->size, curAddress, 1, dump->threadid != 0);
				}
				else if (type == DU_SPEC) {
					curAddress = dump->base + MulDiv(dump->base, lp, MAXTRACK);
					if (curAddress > maxAddress) {
						curAddress = maxAddress;
					}
					// 解析pe
				}
				else {
					if (lp) {
						leftBytes = (dump->addr - dump->base) % totalSize;
					}
					else {
						leftBytes = 0;
					}
					bytes = leftBytes;
					if (leftBytes) {
						fix = 0;
					}
					else {
						fix = totalSize;
					}
					curAddress = bytes + dump->base + totalSize * (MulDiv(dump->size - fix, lp, MAXTRACK) / totalSize);
					if (bytes + maxAddress - totalSize < curAddress) {
						curAddress = bytes + maxAddress - totalSize;
					}
					if (bytes) {
						totalSize = 0;
					}
				}
				if (curAddress != dump->addr) {
					dump->addr = curAddress;
					return MulDiv(curAddress - dump->base, MAXTRACK, dump->size - totalSize);
				}
				return -1;
			}
			case WM_USER_CHGS:
			case WM_USER_VBYTE:	// 0x46a bytes
			{
				if (!dump->size) {
					return 0;
				}
				curAddress = dump->addr;
				totalSize = (dump->dumptype & DU_SIZE) * ((dump->dumptype & DU_COUNT) >> 4);
				if (!totalSize) {
					totalSize = 1;
				}
				maxAddress = dump->size + dump->base;
				if (msg == WM_USER_VBYTE || type != DU_DISASM) {
					if (msg == WM_USER_VBYTE || type != DU_SPEC) {
						if (msg == WM_USER_CHGS && lp == MOVETOP) {
							curAddress = dump->base;
						}
						else if (msg == WM_USER_CHGS && lp == MOVEBOTTOM) {
							curAddress = maxAddress - totalSize;
						}
						else if (lp < 0) {
							if (msg == WM_USER_VBYTE) {
								bytes = lp;
							}
							else {
								bytes = totalSize + lp;
							}
							if (dump->base - bytes <= curAddress) {
								curAddress += bytes;
							}
							else {
								curAddress = dump->base;
							}
						}
						else if (msg == WM_USER_VBYTE) {
							curAddress += lp;
							if (maxAddress - 1 < curAddress) {
								curAddress = maxAddress - 1;
							}
						}
						else if (curAddress + totalSize < maxAddress) {
							curAddress += totalSize + lp;
							if (curAddress + totalSize > maxAddress) {
								curAddress = maxAddress - totalSize;
							}
						}
						if (curAddress - dump->base % totalSize) {
							totalSize = 0;
						}
					}
					else if (msg == WM_USER_CHGS && lp == MOVETOP) {
						curAddress = dump->base;
					}
					else {
						if (msg == WM_USER_CHGS && lp == MOVEBOTTOM) {

						}
						if (lp < 0) {

						}
						if (lp > 0) {

						}
					}
				}
				else if (msg == WM_USER_CHGS && lp == MOVETOP) {
					curAddress = dump->base;
				}
				else {
					if (msg == WM_USER_CHGS && lp == MOVEBOTTOM) {
						curAddress = Disassembleback(dump->filecopy, dump->base, dump->size, maxAddress, 1, dump->threadid != 0);
						if (lp && curAddress == dump->addr) {
							return -1;
						}
						dump->addr = curAddress;
						return MulDiv(curAddress - dump->base, MAXTRACK, dump->size - totalSize);
					}
					if (lp < 0) {
						curAddress = Disassembleback(dump->filecopy, dump->base, dump->size, curAddress, -lp, dump->threadid != 0);
						if (lp && curAddress == dump->addr) {
							return -1;
						}
						dump->addr = curAddress;
						return MulDiv(curAddress - dump->base, MAXTRACK, dump->size - totalSize);
					}
					if (lp > 0) {
						curAddress = Disassembleforward(dump->filecopy, dump->base, dump->size, curAddress, lp, dump->threadid != 0);
						if (curAddress >= maxAddress) {
							curAddress = Disassembleback(dump->filecopy, dump->base, dump->size, maxAddress, 1, dump->threadid != 0);
						}
					}
				}

				if (lp && curAddress == dump->addr) {
					return -1;
				}
				dump->addr = curAddress;
				return MulDiv(curAddress - dump->base, 0x4000, dump->size - totalSize);
			}
			case WM_USER_CNTS:
			case WM_USER_STS:	// start select
			{
				int ret;
				if (dump->dumptype & DU_BACKUP) {
					return 0;
				}
				curAddress = dump->size + dump->base;
				if (type == DU_DISASM) {
					usedec = dump->threadid != 0;
					newSel0 = Disassembleforward(dump->filecopy, dump->base, dump->size, dump->addr, lp, usedec);
					if (msg == WM_USER_STS) {
						newSel1 = Disassembleforward(dump->filecopy, dump->base, dump->size, newSel0, 1, usedec);
					}
					else if (dump->startsel <= newSel0) {
						newSel1 = Disassembleforward(dump->filecopy, dump->base, dump->size, newSel0, 1, usedec);
						newSel0 = dump->startsel;
					}
					else {
						newSel1 = Disassembleforward(dump->filecopy, dump->base, dump->size, dump->startsel, 1, usedec);
					}
				}
				else if (type == DU_SPEC) {

				}
				else {
					if (type != DU_ADDR && type != DU_ADRASC && type != DU_ADRUNI && HIWORD(wp) <= 0) {
						if (msg == WM_USER_STS) {
							return -1;
						}
						else {
							return 0;
						}
					}
					if (type == DU_ADDR || type == DU_ADRASC || type == DU_ADRUNI) {

					}
					else {

					}
					if (type == DU_HEXTEXT || type == DU_HEXUNI) {

					}
					else if (type == DU_TEXT || type == DU_UNICODE) {

					}
					else if ((type == DU_INT || type == DU_UINT || type == DU_IHEX) && dump->itemwidth) {

					}
					else {

					}
				}
				if (newSel0 >= curAddress) {
					newSel0 = curAddress;
				}
				if (newSel1 >= curAddress) {
					newSel1 = curAddress;
				}
				if (msg == WM_USER_STS) {
					dump->startsel = newSel0;
				}
				if (dump->sel0 == newSel0 && dump->sel1 == newSel1) {
					return 0;
				}
				dump->sel0 = newSel0;
				dump->sel1 = newSel1;
				ret = 1;
				return ret;
			}
			case WM_USER_BAR:
			{
				InvalidateRect(hw, nullptr, FALSE);
				return DefMDIChildProc(hw, msg, wp, lp);
			}
			case WM_USER_DBLCLK:
			{

				InvalidateRect(hw, nullptr, FALSE);
				return 1;
			}
			case WM_USER_FILE:
			{

				return STOP_BROADCAST;
			}
			case WM_USER_CHALL:
				InvalidateRect(hw, nullptr, FALSE);
				return 0;
			case WM_USER_CHMEM:
				if (!dump->filecopy && dump->base<lp && dump->size + dump->base>wp) {
					InvalidateRect(hw, nullptr, FALSE);
				}
				return 0;
			case WM_USER_CHREG:
				if (!dump->filecopy && dump->threadid) {
					InvalidateRect(hw, nullptr, FALSE);
				}
				return 0;

			case WM_USER_KEY:
			{

				return 1;
			}

			case WM_USER_SYSKEY:
			{

				return DefMDIChildProc(hw, msg, wp, lp);
			}
			default:
				return DefMDIChildProc(hw, msg, wp, lp);
		}
	}
	

	if(msg == WM_USER_VABS)	// 0x468 lp ==> lines
	{
		if (!dump->size) {
			return 0;
		}
		curAddress = dump->addr;
		totalSize = (dump->dumptype & DU_SIZE) * ((dump->dumptype & DU_COUNT) >> 4);
		if (!totalSize) {
			totalSize = 1;
		}
		maxAddress = dump->size + dump->base;
		if (msg == WM_USER_VBYTE || type != DU_DISASM) {
			if (msg == WM_USER_VBYTE || type != DU_SPEC) {
				if (msg == WM_USER_CHGS && lp == MOVETOP) {
					curAddress = dump->base;
				}
				else if (msg == WM_USER_CHGS && lp == MOVEBOTTOM) {
					curAddress = maxAddress - totalSize;
				}
				else if (lp < 0) {
					if (msg == WM_USER_VBYTE) {
						bytes = lp;
					}
					else {
						bytes = totalSize + lp;
					}
					if (dump->base - bytes <= curAddress) {
						curAddress += bytes;
					}
					else {
						curAddress = dump->base;
					}
				}
				else if (msg == WM_USER_VBYTE) {
					curAddress += lp;
					if (maxAddress - 1 < curAddress) {
						curAddress = maxAddress - 1;
					}
				}
				else if (curAddress + totalSize < maxAddress) {
					curAddress += totalSize + lp;
					if (curAddress + totalSize > maxAddress) {
						curAddress = maxAddress - totalSize;
					}
				}
				if (curAddress - dump->base % totalSize) {
					totalSize = 0;
				}
			}
			else if (msg == WM_USER_CHGS && lp == MOVETOP) {
				curAddress = dump->base;
			}
			else {
				if (msg == WM_USER_CHGS && lp == MOVEBOTTOM) {

				}
				if (lp < 0) {

				}
				if (lp > 0) {

				}
			}
		}
		else if (msg == WM_USER_CHGS && lp == MOVETOP) {
			curAddress = dump->base;
		}
		else {
			if (msg == WM_USER_CHGS && lp == MOVEBOTTOM) {
				curAddress = Disassembleback(dump->filecopy, dump->base, dump->size, maxAddress, 1, dump->threadid != 0);
				if (lp && curAddress == dump->addr) {
					return -1;
				}
				dump->addr = curAddress;
				return MulDiv(curAddress - dump->base, MAXTRACK, dump->size - totalSize);
			}
			if (lp < 0) {
				curAddress = Disassembleback(dump->filecopy, dump->base, dump->size, curAddress, -lp, dump->threadid != 0);
				if (lp && curAddress == dump->addr) {
					return -1;
				}
				dump->addr = curAddress;
				return MulDiv(curAddress - dump->base, MAXTRACK, dump->size - totalSize);
			}
			if (lp > 0) {
				curAddress = Disassembleforward(dump->filecopy, dump->base, dump->size, curAddress, lp, dump->threadid != 0);
				if (curAddress >= maxAddress) {
					curAddress = Disassembleback(dump->filecopy, dump->base, dump->size, maxAddress, 1, dump->threadid != 0);
				}
			}
		}

		if (lp && curAddress == dump->addr) {
			return -1;
		}
		dump->addr = curAddress;
		return MulDiv(curAddress - dump->base, 0x4000, dump->size - totalSize);
	}
	if (msg > WM_SYSKEYDOWN) {

	}
	if (msg == WM_SYSKEYDOWN) {

	}
	if (msg < WM_WINDOWPOSCHANGED) {
		switch (msg)
		{
			case WM_WINDOWPOSCHANGED:
				if (dump) {
					return Tablefunction(dump->hw, msg, wp, lp);
				}
				break;
			case WM_DESTROY:
				if (dump) {
					Tablefunction(hw, WM_DESTROY, wp, lp);
				}
				break;
			case WM_PAINT:
				PaintTable(hw);
				return 0;

			case WM_CLOSE:
				
				break;
			default:
				break;
		}
		return DefMDIChildProc(hw, msg, wp, lp);
	}
	if (msg == WM_KEYDOWN) {

	}
	if (msg != WM_CHAR) {
		return DefMDIChildProc(hw, msg, wp, lp);
	}

	return 0;
}

int CDump::ParseTableEntry(CString& s, char& mask, int& select, t_dump* info, int column) {
	if (!info||!info->addr) {
		return 0;
	}
	int type = info->dumptype & DU_TYPE;
	int count = (info->dumptype & DU_COUNT) >> 4;
	int size = info->dumptype & DU_SIZE;
	t_thread* t;
	

	select = 0;
	if (m_Table.data.n > 0) {
		if (type == DU_DISASM || type == DU_SPEC) {
			size = 0x10;
		}
		if (type == DU_SPEC) {
			_dumpAddress = info->base;
			_size = info->size;
		}
		else {
			_dumpAddress = info->addr;
			_size = m_Table.data.n * count * size;
			if (info->size < _size) {
				_size = info->size;
			}
			if (info->size + info->base >= _dumpAddress) {
				if (info->size + info->base < _dumpAddress + _size) {
					_size = info->size + info->base - _dumpAddress;
				}
			}
			else {
				_size = 0;
			}
		}
		// 高亮
		if (type != DU_DISASM || (info->dumptype & DU_BACKUP)!=0) {
			_highlight = 0;
		}
		else {
			_highlight = m_Table.hilite;
			if (_highlight < 0 || _highlight >= 5) {
				_highlight = 0;
			}
		}
		if (!_block || _size > _blockSize || _blockSize > 0x10000) {
			if (_block) {
				GlobalFree(_block);
				_block = nullptr;
				_blockSize = 0;
			}
			_block = GlobalAlloc(GMEM_FIXED, _size);
			if (_block) {
				_blockSize = _size;
			}
		}
		if (!_profile && (_profileSize < _size || _profileSize>0x10000)) {
			GlobalFree(_profile);
			_profile = nullptr;
			_profileSize = 0;
		}
		// _profile与runtrace相关
		if (!_profile && type == DU_DISASM && info->commentmode == 2) {
			_profile = GlobalAlloc(GMEM_FIXED, 4 * _size);
			if (_profile) {
				_profileSize = _size * 4;
			}
		}
		_module = nullptr;
		if (info->dumptype & DU_BACKUP) {
			memcpy(_block, (char*)_dumpAddress + (ulong)info->backup - info->base, _size);
		}
		else if (info->filecopy) {
			memcpy(_block, (char*)_dumpAddress + (ulong)info->filecopy, _size);
		}
		else {	
			// 1.读取内存
			_size = MemoryHelper::Readmemory(_block, _dumpAddress, _size, MM_RESILENT);
			// 2.修复重定位

			if (type == DU_ADDR || type == DU_ADRASC || type == DU_ADRUNI) {

			}
			else if (type == DU_DISASM && info->threadid) {
				
			}
		}
		// profile
		if (_profile && type == DU_DISASM && info->commentmode == 2) {

		}
		m_Table.data.n = 0;
		_curAddress = info->addr;
		_curRow = _curAddress;
		_firstRow = -1;
		if (info->filecopy || !info->threadid) {
			t = nullptr;
		}
		else {
			
		}
		// 内存窗口
		if ((type == DU_ADDR || type == DU_ADRASC || type == DU_ADRUNI)) {
			
		}
		else {

		}
		if (type == DU_DISASM) {
			// 解析跳转
		}
	}
	if (_firstRow != m_Table.offset) {
		if (type == DU_DISASM) {
			_curRow = _curAddress;
			if (_dumpAddress + _size <= _curAddress) {
				_disasm.dump = L"";
				_disasm.result = L"";
				_disasm.comment = L"";
			}
			else if (!info->threadid||info->runtraceoffset<=0||info->filecopy) {
				_curAddress += Disasm((uchar*)((char*)_block + (_curRow - _dumpAddress)),
					_blockSize - (_curRow - _dumpAddress),
					_curRow,
					nullptr,
					&_disasm,
					(_highlight << 16) | DISASM_FILE,
					info->threadid); // first step
			}
			else {
				_curAddress += Disasm((uchar*)((char*)_block + _curRow - _dumpAddress),
					_blockSize - (_curRow - _dumpAddress),
					_curRow,
					nullptr,
					&_disasm,
					(_highlight << 16) | DISASM_RTRACE,
					info->threadid);
			}
		}
		else if (type == DU_SPEC) {

		}
		else {
			_curRow = _dumpAddress + size * count * m_Table.offset;
		}
		_firstRow = m_Table.offset;
		info->lastaddr = _curAddress;
	}
	if (column) {
		if (type == DU_HEXTEXT) {

		}
		if (type == DU_INT) {

		}
		if (type == DU_FLOAT) {

		}
		if (type != DU_ADDR && type != DU_ADRASC && type != DU_ADRUNI) {
			if (type == DU_DISASM || type == DU_SPEC && column == 1) {
				select = DRAW_NORMAL;
				if (_curAddress <= info->sel0 || _curRow >= info->sel1) {
					select = DRAW_DIRECT;
				}
				else {
					select = DRAW_DIRECT | DRAW_SELECT;
				}
				if (column == 1) {
					s = _disasm.dump;	// Hex dump value
				}
				else if (column == 2) {
					s = _disasm.result; // Disassembly code
				}
				else if (column != 3 || info->filecopy) {

				}
				else {

				}
				
			}
			else if (type == DU_ADRASC) {

			}

			if (column > 0 && (info->dumptype & DU_BACKUP) || info->dimmed) {
				select |= DRAW_GRAY;
			}
			return s.GetLength();
		}
	}
	if (info->filecopy) {
		column = 0;
	}
	else {

	}
	if (((type == DU_DISASM || type == DU_SPEC) && _curAddress > info->sel0
		|| (type == DU_ADDR || type == DU_ADRASC || type == DU_ADRUNI) && _curRow + size > info->sel0)
		&& (_curRow < info->sel1)) {
		select = DRAW_SELECT;
	}
	else {
		select = DRAW_GRAY;
	}

	if (column > 0 && (info->dumptype & DU_BACKUP) || info->dimmed) {
		select |= DRAW_GRAY;
	}

	if (info->size + info->base > _curRow) {
		if (info->reladdr) {

		}
		else {
			s.Format(L"%08lX", _curRow);
		}
	}
	return s.GetLength();
}




