#pragma once

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// DISASSEMBLY FUNCTIONS /////////////////////////////

#define REG_EAX        0               // Indexes of general-purpose registers
#define REG_ECX        1               // in t_reg.
#define REG_EDX        2
#define REG_EBX        3
#define REG_ESP        4
#define REG_EBP        5
#define REG_ESI        6
#define REG_EDI        7

#define SEG_UNDEF     -1
#define SEG_ES         0               // Indexes of segment/selector registers
#define SEG_CS         1               // in t_reg.
#define SEG_SS         2
#define SEG_DS         3
#define SEG_FS         4
#define SEG_GS         5

// Selected items in register window.
#define RS_NONE        0x0000          // No selection
#define RS_INT         0x0010          // General-purpose 32-bit registers
#define RS_EIP         0x0020          // EIP (instruction pointer)
#define RS_FLG         0x0030          // 1-bit decoded flags
#define RS_SEG         0x0040          // Segment (selector) registers
#define RS_EFL         0x0050          // 32-bit flag register
#define RS_TAG         0x0060          // FPU register tag
#define RS_FPU         0x0070          // 80-bit FPU registers
#define RS_FST         0x0080          // FPU status
#define RS_FCO         0x0090          // FPU condition bits
#define RS_FER         0x00A0          // FPU error bits
#define RS_FCW         0x00B0          // FPU control word
#define RS_FPR         0x00C0          // FPU precision fields
#define RS_FEM         0x00D0          // FPU error mask bits
#define RS_MMX         0x00E0          // MMX registers
#define RS_3DN         0x00F0          // 3DNow! registers
#define RS_SSE         0x0100          // SSE registers
#define RS_CSR         0x0110          // SSE MXCSR register
#define RS_CSB         0x0120          // SSE MXCSR bits
#define RS_CPR         0x0130          // SSE rounding control
#define RS_ERR         0x0140          // Last thread error

#define RS_GROUP       0x01F0          // Mask to extract group of registers
#define RS_INDEX       0x000F          // Mask to extract index of register

#define NREGSTACK      32              // Length of stack trace buffer
#define MAXCALSIZE     8               // Max length of CALL without prefixes

#define INT3           0xCC            // Code of 1-byte breakpoint
#define NOP            0x90            // Code of 1-byte NOP command
#define TRAPFLAG       0x00000100      // Trap flag in CPU flag register

#define C_TYPEMASK     0xF0            // Mask for command type
#define   C_CMD        0x00            // Ordinary instruction
#define   C_PSH        0x10            // PUSH instruction
#define   C_POP        0x20            // POP instruction
#define   C_MMX        0x30            // MMX instruction
#define   C_FLT        0x40            // FPU instruction
#define   C_JMP        0x50            // JUMP instruction
#define   C_JMC        0x60            // Conditional JUMP instruction
#define   C_CAL        0x70            // CALL instruction
#define   C_RET        0x80            // RET instruction
#define   C_FLG        0x90            // Changes system flags
#define   C_RTF        0xA0            // C_JMP and C_FLG simultaneously
#define   C_REP        0xB0            // Instruction with REPxx prefix
#define   C_PRI        0xC0            // Privileged instruction
#define   C_SSE        0xD0            // SSE instruction
#define   C_NOW        0xE0            // 3DNow! instruction
#define   C_BAD        0xF0            // Unrecognized command
#define C_RARE         0x08            // Rare command, seldom used in programs
#define C_SIZEMASK     0x07            // MMX data size or special flag
#define   C_EXPL       0x01            // (non-MMX) Specify explicit memory size

#define C_DANGER95     0x01            // Command is dangerous under Win95/98
#define C_DANGER       0x03            // Command is dangerous everywhere
#define C_DANGERLOCK   0x07            // Dangerous with LOCK prefix

#define DEC_TYPEMASK   0x1F            // Type of memory byte
#define   DEC_UNKNOWN  0x00            // Unknown type
#define   DEC_BYTE     0x01            // Accessed as byte
#define   DEC_WORD     0x02            // Accessed as short
#define   DEC_NEXTDATA 0x03            // Subsequent byte of data
#define   DEC_DWORD    0x04            // Accessed as long
#define   DEC_FLOAT4   0x05            // Accessed as float
#define   DEC_FWORD    0x06            // Accessed as descriptor/long pointer
#define   DEC_FLOAT8   0x07            // Accessed as double
#define   DEC_QWORD    0x08            // Accessed as 8-byte integer
#define   DEC_FLOAT10  0x09            // Accessed as long double
#define   DEC_TBYTE    0x0A            // Accessed as 10-byte integer
#define   DEC_STRING   0x0B            // Zero-terminated ASCII string
#define   DEC_UNICODE  0x0C            // Zero-terminated UNICODE string
#define   DEC_3DNOW    0x0D            // Accessed as 3Dnow operand
#define   DEC_SSE      0x0E            // Accessed as SSE operand
#define   DEC_TEXT     0x10            // For use in t_result only
#define   DEC_BYTESW   0x11            // Accessed as byte index to switch
#define   DEC_NEXTCODE 0x13            // Subsequent byte of command
#define   DEC_COMMAND  0x1D            // First byte of command
#define   DEC_JMPDEST  0x1E            // Jump destination
#define   DEC_CALLDEST 0x1F            // Call (and maybe jump) destination
#define DEC_PROCMASK   0x60            // Procedure analysis
#define   DEC_PROC     0x20            // Start of procedure
#define   DEC_PBODY    0x40            // Body of procedure
#define   DEC_PEND     0x60            // End of procedure
#define DEC_CHECKED    0x80            // Byte was analysed
#define DEC_SIGNED     0x100           // For use in t_result only

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

// Warnings issued by Disasm():
#define DAW_FARADDR    0x0001          // Command is a far jump, call or return
#define DAW_SEGMENT    0x0002          // Command loads segment register
#define DAW_PRIV       0x0004          // Privileged command
#define DAW_IO         0x0008          // I/O command
#define DAW_SHIFT      0x0010          // Shift constant out of range 1..31
#define DAW_PREFIX     0x0020          // Superfluous prefix
#define DAW_LOCK       0x0040          // Command has LOCK prefix
#define DAW_STACK      0x0080          // Unaligned stack operation
#define DAW_DANGER95   0x1000          // May mess up Win95/98 if executed
#define DAW_DANGEROUS  0x3000          // May mess up any OS if executed

#define RST_INVALID    0               // Register undefined
#define RST_VALUE      1               // Register contains regdata
#define RST_VFIXUP     2               // Reg contains regdata that is fixup
#define RST_INDIRECT   3               // Register contains [regdata]

#define NREGSTACK      32              // Length of stack trace buffer

typedef struct t_reg {                 // Excerpt from context
	int            modified;             // Some regs modified, update context
	int            modifiedbyuser;       // Among modified, some modified by user
	int            singlestep;           // Type of single step, SS_xxx
	ulong          r[8];                 // EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI
	ulong          ip;                   // Instruction pointer (EIP)
	ulong          flags;                // Flags
	int            top;                  // Index of top-of-stack
    long double    f[8];                 // Float registers, f[top] - top of stack
	BYTE           FloatRegisterArea[SIZE_OF_80387_REGISTERS];
	char           tag[8];               // Float tags (0x3 - empty register)
	ulong          fst;                  // FPU status word
	ulong          fcw;                  // FPU control word
	ulong          s[6];                 // Segment registers ES,CS,SS,DS,FS,GS
	ulong          base[6];              // Segment bases
	ulong          limit[6];             // Segment limits
	char           big[6];               // Default size (0-16, 1-32 bit)
	ulong          dr6;                  // Debug register DR6
	ulong          threadid;             // ID of thread that owns registers
	ulong          lasterror;            // Last thread error or 0xFFFFFFFF
	int            ssevalid;             // Whether SSE registers valid
	int            ssemodified;          // Whether SSE registers modified
	char           ssereg[8][16];        // SSE registers
	ulong          mxcsr;                // SSE control and status register
	int            selected;             // Reports selected register to plugin
	ulong          drlin[4];             // Debug registers DR0..DR3
	ulong          dr7;                  // Debug register DR7
} t_reg;

typedef struct t_operand {             // Full decription of command's operand
	char           optype;               // DEC_xxx (mem) or DECR_xxx (reg,const)
	char           opsize;               // Size of operand
	char           regscale[8];          // Scales of registers
	char           seg;                  // Segment register
	ulong          opconst;              // Constant
} t_operand;

typedef struct t_disasm {              // Results of disassembling
	ulong          ip;                   // Instrucion pointer
	CString        dump;				 // Hexadecimal dump of the command
	CString        result;				 // Disassembled command
	CString        comment;				 // Brief comment
	char           opinfo[3][TEXTLEN];   // Comments to command's operands
	int            cmdtype;              // One of C_xxx
	int            memtype;              // Type of addressed variable in memory
	int            nprefix;              // Number of prefixes
	int            indexed;              // Address contains register(s)
	ulong          jmpconst;             // Constant jump address
	ulong          jmptable;             // Possible address of switch table
	ulong          adrconst;             // Constant part of address
	ulong          immconst;             // Immediate constant
	int            zeroconst;            // Whether contains zero constant
	int            fixupoffset;          // Possible offset of 32-bit fixups
	int            fixupsize;            // Possible total size of fixups or 0
	ulong          jmpaddr;              // Destination of jump/call/return
	int            condition;            // 0xFF:unconditional, 0:false, 1:true
	int            error;                // Error while disassembling command
	int            warnings;             // Combination of DAW_xxx
	int            optype[3];            // Type of operand (extended set DEC_xxx)
	int            opsize[3];            // Size of operand, bytes
	int            opgood[3];            // Whether address and data valid
	ulong          opaddr[3];            // Address if memory, index if register
	ulong          opdata[3];            // Actual value (only integer operands)
	t_operand      op[3];                // Full description of operand
	ulong          regdata[8];           // Registers after command is executed
	int            regstatus[8];         // Status of registers, one of RST_xxx
	ulong          addrdata;             // Traced memory address
	int            addrstatus;           // Status of addrdata, one of RST_xxx
	ulong          regstack[NREGSTACK];  // Stack tracing buffer
	int            rststatus[NREGSTACK]; // Status of stack items
	int            nregstack;            // Number of items in stack trace buffer
	ulong          reserved[29];         // Reserved for plugin compatibility
} t_disasm;

// 反汇编

// 编译器填充 nop或者int 3等单字节指令
// 加壳软件填充0，迷惑反汇编器
// 32条跳转指令
// 4种寻址方式

ulong Disasm(uchar* src, ulong srcsize, ulong srcip, uchar* srcdec,
	t_disasm* disasm, int disasmmode, ulong threadid);
ulong Disasmback(uchar* src, ulong srcsize, ulong srcip, uchar* srcdec,
	t_disasm* disasm, int disasmmode, ulong threadid,int n);

ulong Disassembleback(uchar* block, ulong base, ulong size,
	ulong ip, int n, int usedec);
ulong Disassembleforward(uchar* block, ulong base, ulong size,
	ulong ip, int n, int usedec);
//int     cdecl Issuspicious(char* cmd, ulong size, ulong ip,
//	ulong threadid, t_reg* preg, char* s);
//int     cdecl Isfilling(ulong offset, char* data, ulong size, ulong align);
