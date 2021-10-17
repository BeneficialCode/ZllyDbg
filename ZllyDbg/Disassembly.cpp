#include "stdafx.h"
#include "Disassembly.h"
#include "MemoryHelper.h"

ulong   Disasm(uchar* src, ulong srcsize, ulong srcip, uchar* srcdec,
	t_disasm* disasm, int disasmmode, ulong threadid) {
	int size = 0;
	csh handle;
	cs_insn* insn;
	if (cs_open(CS_ARCH_X86, CS_MODE_32, &handle)) {
		printf("Error: Failed to initialize engine!");
		return 0;
	}

	cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
	auto count = cs_disasm(handle, src, srcsize, srcip, 1, &insn);

	if (count > 0) {
		for (int i = 0; i < insn->detail->groups_count; i++) {
			if (insn->detail->groups[i] == CS_GRP_JUMP) {
				if(!strcmp(insn->mnemonic, "jmp"))
					disasm->cmdtype = C_JMP;
				else
					disasm->cmdtype = C_JMC;
			}
			if (insn->detail->groups[i] == CS_GRP_RET) {
				disasm->cmdtype = C_RET;
			}
		}
		disasm->ip = srcip;
		// 反汇编的结果
		USES_CONVERSION;
		std::wstring str = A2CW(insn->mnemonic);
		str += L" ";
		str += A2CW(insn->op_str);
		disasm->result = str.c_str();
		// 反汇编的机器码
		WCHAR hex[15];
		disasm->dump = L"";
		for (int i = 0; i < insn->size; i++) {
			wsprintf(hex, L" %02X", src[i]);
			CString s = hex;
			disasm->dump += s;
		}
		size = insn->size;
		cs_free(insn, 1);
	}
	cs_close(&handle);
	return size;
}

ulong Disasmback(uchar* src, ulong srcsize, ulong srcip, uchar* srcdec,
	t_disasm* disasm, int disasmmode, ulong threadid, int n) {
	int size = 0;
	csh handle;
	cs_insn* insn;
	if (cs_open(CS_ARCH_X86, CS_MODE_32, &handle)) {
		printf("Error: Failed to initialize engine!");
		return 0;
	}

	cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
	auto count = cs_disasm(handle, src, srcsize, srcip, n, &insn);
	if (count > 0) {
		for (int i = 0; i < count; i++) {
			size += insn[i].size;
		}
		cs_free(insn, count);
	}
	cs_close(&handle);
	return size;
}

// Calculates address of assembler instruction which is n instructions (maximally 127) back from instruction at specified address. 
// Returns address of found instruction. 
// In case of error, it may be less than n instructions apart. 
// 
// 80x86 commands have variable length. Disassembleback use heuristical methods to separate commands and in some (astoundingly rare!) cases may return invalid answer. To avoid risks of invaling backward walking, or correctly walk through constants and strings, use results of code analysis.
// 
// ulong Disassembleback(char *block,ulong base,ulong size,ulong ip,int n,int usedec);
// 
// Parameters:
// 
// block - pointer to copy of code. If block is NULL, Disassembleback assumes memory of debugged process and if necessary reads it;
// 
// base - address of first byte of code block;
// 
// size - size of code block;
// 
// ip - address of current instruction;
// 
// n - number of instructions to walk back;
// 
// usedec - flag indicating whether Disassembleback should try to use decoding data.
// 
// OllyDbg Plugin API v1.10
ulong   Disassembleback(uchar* block, ulong base, ulong size,
	ulong ip, int n, int usedec) {
	uchar* pDecInfo = nullptr;
	t_disasm disasm;
	uchar buf[2096];
	uchar* pCode = nullptr;
	ulong address[131] = { 0 };

	if (n >= 0) {
		if (n > 127) {
			n = 127;
		}
	}
	else {
		n = 0;
	}
	// 代码块的大小
	if (size + base < ip) {
		ip = size + base;
	}
	if (!n)
		return ip;
	if (n + base >= ip) {
		return base;
	}
	ulong back = 16 * n + 48;
	if (back + base > ip) {
		back = ip - base;
	}
	ulong codeStart = ip - back;
	if (usedec) {

	}
	else {
		pDecInfo = nullptr;
	}
	
	if (block) {
		pCode = block + (codeStart - base);
	}
	else {
		if (back != MemoryHelper::Readmemory(&buf, codeStart, back, 3)) {
			return ip - n;
		}
		pCode = buf;
	}
	ulong backAddress = 0;
	ulong asmSize;

	int i = 0;
	for (i = 0; codeStart < ip; ++i) {
		address[i % 128] = codeStart;
		//asmSize = Disasm(pCode, back, codeStart, nullptr, &disasm, 0, 0);
		asmSize = Disasmback(pCode, back, codeStart, nullptr, &disasm, 0, 0, n);
		// 如果反汇编的地址+汇编指令字节数==ip,说明反汇编正确
		if (codeStart + asmSize == ip)
			break;
		if (asmSize == 0) {
			pCode += 1;
		}
		pCode += asmSize;
		codeStart += asmSize;
		back -= asmSize;
	}
	if (i >= n) {
		return address[(i - n + 128) % 128];
	}
	else {
		return address[0];
	}

}


// Calculates address of assembler instruction which is n instructions forward from instruction at specified address. If copy of code is not supplied, Disassembleforward guarantees correct results up to n=127 (typically 300). 
// 
// Returns address of found instruction. In case of error, it may be less than n instructions apart. 
// 
// If you want to correctly walk through constants and strings, use results of code analysis.
// 
// ulong Disassembleforward(char *block,ulong base,ulong size,ulong ip,int n,int usedec);
// 
// Parameters:
// 
// block - pointer to copy of code. If block is NULL, Disassembleforward assumes memory of debugged process and if necessary reads it;
// 
// base - address of first byte of code block;
// 
// size - size of code block;
// 
// ip - address of current instruction;
// 
// n - number of instructions to walk forward;
// 
// usedec - flag indicating whether Disassembleforward should try to use decoding data.
// 
// 
// 
// OllyDbg Plugin API v1.10
ulong Disassembleforward(uchar* block, ulong base, ulong size,
	ulong ip, int n, int usedec) {
	ulong address;
	uchar buf[2048];
	char* p = nullptr;
	uchar* pInfo = nullptr;
	uint decType = 0;
	uchar* pType;
	int cmdSize;
	t_disasm disasm;

	address = ip;
	if (ip < base) {
		address = base;
	}

	ulong blockEnd = size + base;
	if (address <= blockEnd) {
		if (n > 0) {
			if (block) {
				block += address - base;
			}
			int blockSize = size - (address - base);
			p = (char*)address;
			if (usedec) {

			}
			else {
				pInfo = nullptr;
			}
			int k = 0;
			int nSize = 0;
			while (n > 0 && blockSize > k) {
				if (pInfo) {
					decType = pInfo[k] & DEC_TYPEMASK;
				}
				else {
					decType = DEC_UNKNOWN;
				}
				if ((decType == DEC_WORD || decType == DEC_BYTE) ||
					(decType < 0xF&&decType!=DEC_UNKNOWN) || decType == DEC_BYTESW) {
					if (pInfo != nullptr) {
						for (pType = &pInfo[++k]; blockSize > k && (*pType & DEC_TYPEMASK) == DEC_NEXTDATA; ++pType) {
							++k;
						}
					}
				}
				else if (decType < DEC_PROC&&decType!=DEC_UNKNOWN) {
					for (pType = &pInfo[++k]; blockSize > k && (*pType & DEC_TYPEMASK) == DEC_NEXTCODE; ++pType) {
						++k;
					}
				}
				else {
					if (block) {
						cmdSize = Disasm(&block[k], blockSize - k, (ulong)&p[k], nullptr, &disasm, DISASM_SIZE, 0);
					}
					else {
						if (k + 16 > nSize && blockSize > nSize) {
							p += k;
							blockSize -= k;
							if (pInfo) {
								pInfo += k;
							}
							k = 0;
							nSize = 16 * (n >= 128 ? 128 : n);
							if (blockSize < nSize) {
								nSize = blockSize;
							}
							int readBytes = MemoryHelper::Readmemory(buf, (ulong)p, nSize, MM_RESILENT);
							if (readBytes != nSize) {
								return (ulong)&p[n];
							}
						}
						cmdSize = Disasm(&buf[k], nSize - k, (ulong)&p[k], nullptr, &disasm, DISASM_SIZE, 0);
					}
					if (cmdSize == 0) {
						k += 1;
					}
					k += cmdSize;
				}
				--n;
			}
			return (ulong)&p[k];
		}
		else {
			return address;
		}
	}
	return blockEnd;
}