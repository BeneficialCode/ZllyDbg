#pragma once

typedef struct t_sortheader {          // Header of sorted data field
	ulong          addr;                 // Base address of the entry
	ulong          size;                 // Size address of the entry
	ulong          type;                 // Entry type, TY_xxx
} t_sortheader;

struct NameHelper abstract final {
	static int Insertname(ulong addr, int type, char* name);
	static int Findlabel(ulong addr, char* name);
	static int Decodename(ulong addr, int type, char* name);
	static int Findname(ulong addr, int type, char* name);
	static ulong Findnextname(char* name);
	static int Demanglename(char* name, int type, char* undecorated);
	static int Quickinsertname(ulong addr, int type, char* name);
	static void Mergequicknames();
	static void Discardquicknames();
	static void Deletenamerange(ulong addr0, ulong addr1, int type);
	static int Findlabelbyname(char* name, ulong* addr, ulong addr0, ulong addr1);
	static int Findsymbolicname(ulong addr, char* fname);

private:
	static int GetSortHeader(ulong addr);

	static inline t_sortheader* _pSortHeaders;
	static inline int _headers;
	static inline char* _nameList;
	static inline size_t _size;
	static inline int _nameCount;
	static inline int _lastType;
	static inline int _nextIndex;
	static inline int _maxNameCount = 0x8000;
	static inline int _curUseSize;
	static inline bool _demangleSymbolicName = true;
	static inline bool _haveSpace;
};