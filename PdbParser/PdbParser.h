#pragma once

#include <string>
#include <vector>
#include <list>

// 调试符号
enum class ProcessAccessMask : unsigned {
	Query = 0x400
};

enum class BasicType {
	NoType = 0,
	Void = 1,
	Char = 2,
	WChar = 3,
	Int = 6,
	UInt = 7,
	Float = 8,
	BCD = 9,
	Bool = 10,
	Long = 13,
	ULong = 14,
	Currency = 25,
	Date = 26,
	Variant = 27,
	Complex = 28,
	Bit = 29,
	BSTR = 30,
	Hresult = 31
};

enum class UdtKind {
	Struct,
	Class,
	Union,
	Interface,
	Unknown = 99
};

enum class SymbolTag{
	Null,
	Exe,
	Compiland,
	CompilandDetails,
	CompilandEnv,
	Function,		// 函数
	Block,
	Data,			// 变量，函数实参，复合结构字段，枚举值
	Annotation,
	Label,
	PublicSymbol,
	UDT,			// 用户定义类型，例如struct,class和union
	Enum,			// 枚举类型
	FunctionType,	// 函数类型
	PointerType,	// 指针类型
	ArrayType,		// 数组类型
	BaseType,		// 基本类型
	Typedef,		// typedef 类型
	BaseClass,		// 基类类型
	Friend,			// 友元类型
	FunctionArgType,// 函数形参类型
	FuncDebugStart,
	FuncDebugEnd,
	UsingNamespace,
	VTableShape,
	VTable,
	Custom,
	Thunk,
	CustomType,
	ManagedType,
	Dimension,
	CallSite,
	InlineSite,
	BaseInterface,
	VectorType,
	MatrixType,
	HLSLType,
	Caller,
	Callee,
	Export,
	HeapAllocationSite,
	CoffGroup,
	Max
};
DEFINE_ENUM_FLAG_OPERATORS(SymbolTag);

enum class SymbolTypeInfo {
	Tag,
	Name,
	Length,
	DbgType,
	TypeId,
	BaseType,
	ArrayIndexTypeId,
	FindChildren,
	DataKind,
	AddressOffset,
	Offset,
	Value,
	Count,
	ChildrenCount,
	BitPosition,
	VirtualBaseClass,
	VIRTUALTABLESHAPEID,
	VIRTUALBASEPOINTEROFFSET,
	ClassParentId,
	Nested,
	SymIndex,
	LexicalParent,
	Address,
	ThisAdjust,
	UdtKind,
	IsEquivalentTo,
	CallingConvention,
	IsCloseEquivalentTo,
	GTIEX_REQS_VALID,
	VirtualBaseOffset,
	VirtualBaseDispIndex,
	IsReference,
	IndirectVirtualBaseClass
};

enum class SymbolFlags : unsigned {
	None = 0,
	ClrToken = 0x40000,
	Constant = 0x100,
	Export = 0x200,
	Forwarder = 0x400,
	FrameRelative = 0x20,
	Function = 0x800,
	ILRelative = 0x10000,
	Local = 0x80,
	Metadata = 0x20000,
	Parameter = 0x40,
	Register = 0x8,
	RegisterRelative = 0x10,
	Slot = 0x8000,
	Thunk = 0x2000,
	TLSRelative = 0x4000,
	ValuePresent = 1,
	Virtual = 0x1000,
	Null = 0x80000,
	FunctionNoReturn = 0x100000,
	SyntheticZeroBase = 0x200000,
	PublicCode = 0x400000
};

enum class SymbolOptions : DWORD {
	None = 0,
	AllowAbsoluteSymbols = SYMOPT_ALLOW_ABSOLUTE_SYMBOLS,
	AllowZeroAddress = SYMOPT_ALLOW_ZERO_ADDRESS,
	AutoPublic = SYMOPT_AUTO_PUBLICS,
	CaseInsensitive = SYMOPT_CASE_INSENSITIVE,
	Debug = SYMOPT_DEBUG,
	DeferredLoads = SYMOPT_DEFERRED_LOADS,
	DisableSymSrvAutoDetect = SYMOPT_DISABLE_SYMSRV_AUTODETECT,
	ExactSymbols = SYMOPT_EXACT_SYMBOLS,
	FailCriticalErrors = SYMOPT_FAIL_CRITICAL_ERRORS,
	FavorCompressed = SYMOPT_FAVOR_COMPRESSED,
	FlatDirectory = SYMOPT_FLAT_DIRECTORY,
	IgnoreCodeViewRecord = SYMOPT_IGNORE_CVREC,
	IgnoreImageDir = SYMOPT_IGNORE_IMAGEDIR,
	IgnoreNTSymbolPath = SYMOPT_IGNORE_NT_SYMPATH,
	Include32BitModules = SYMOPT_INCLUDE_32BIT_MODULES,
	LoadLines = SYMOPT_LOAD_LINES,
	NoCPP = SYMOPT_NO_CPP,
	NoImageSearch = SYMOPT_NO_IMAGE_SEARCH,
	NoPrompts = SYMOPT_NO_PROMPTS,
	NoPublics = SYMOPT_NO_PUBLICS,
	NoUnqualifiedLoads = SYMOPT_NO_UNQUALIFIED_LOADS,
	Overwrite = SYMOPT_OVERWRITE,
	PublicsOnly = SYMOPT_PUBLICS_ONLY,
	Secure = SYMOPT_SECURE,
	UndecorateNames = SYMOPT_UNDNAME
};

struct SymbolInfo : SYMBOL_INFO {
	// member name cannot be larger than this (docs says 2000, but seems wasteful in practice)
	//const int MaxSymbolLen = 500;

};

