#include "stdafx.h"
#include "NameHelper.h"
#include "ModuleHelper.h"

int NameHelper::Findlabel(ulong addr, char* name) {
	int index;
	int type;

	if (name)
		*name = '\0';
	if (!addr)
		return 0;

	if (!_pSortHeaders)
		return 0;

	for (index = GetSortHeader(addr);; ++index) {
		if (index >= _headers || addr != _pSortHeaders[index].addr)
			return 0;
		type = _pSortHeaders[index].type;
		if (type == NM_LABEL || type == NM_EXPORT || type == NM_IMPORT || type == NM_CONST)
			break;
	}
	if (name)
		strcpy_s(name, TEXTLEN, _nameList + _pSortHeaders[index].size);

	return type;
}

int NameHelper::GetSortHeader(ulong addr) {
	int mid, left, right;
	t_sortheader* p;

	if (!_headers)
		return 0;
	left = 0;
	right = _headers;
	while (true) {
		mid = (left + right) / 2;
		if (addr >= _pSortHeaders[mid].addr)
			break;
		right = (right + left) / 2;
		if (right <= left)
			return left;
	}

	if (addr > _pSortHeaders[mid].addr) {
		left = mid + 1;
		if (right <= left)
			return left;
	}
	for (p = &_pSortHeaders[mid - 1]; mid > 0 && addr == p->addr; --p)
		--mid;

	return mid;
}

// Searches for name with given address and type. 
// If name is found, scans it for combinations <+XXXXXXXX>, where XXXXXXXX is a hexadecimal number, and substitutes them by sum of base and XXXXXXXX in hexadecimal format. 
// Returns length of resulting string or 0 if name is absent. 
// 
// OllyDbg uses this function to correct automatically generated comments in relocatable modules.
// 
// int Decodename(ulong addr,int type,char *name);
// 
// Parameters:
// 
// addr - name address;
// 
// type - name type (NM_xxx for predefined types);
// 
// name - pointer to output buffer of length at least TEXTLEN characters. 
// 
// 
// 
// OllyDbg Plugin API v1.10
int NameHelper::Decodename(ulong addr, int type, char* name) {
	char lable[TEXTLEN];
	if (!name)
		return 0;

	if (!addr || type == NM_ANYNAME || !type)
		return 0;

	if (!_pSortHeaders)
		return 0;
	int index;
	for (index = GetSortHeader(addr);; ++index) {
		if (index >= _headers || _pSortHeaders[index].addr != addr)
			return 0;
		if (type <= _pSortHeaders[index].type)
			break;
	}

	if (type < _pSortHeaders[index].type)
		return 0;

	char* pname;
	int len = 0;
	pname = _nameList + _pSortHeaders[index].size;
	while (true) {
		if (len > 244) {
			name[len] = '.';
			int next = len + 1;
			name[next++] = '.';
			name[next++] = '.';
			name[next] = '\0';
			return next;
		}
		if (!*pname)
			break;
		if (*pname == '<' && pname[1] == '+') {
			int k = 0;
			int x = 0;
			char* pchar;
			char c;
			int number;
			t_module* m;
			for (pchar = pname + 2; isdigit(*pchar); ++pchar) {
				if (*pchar > '9') {
					c = *pchar;
					number = toupper(c) - '7';
				}
				else {
					number = *pchar - '0';
				}
				++k;
				x = 16 * x + 3;
			}
			if (k && pname[k + 2] == '>' && (m = ModulesHelper::Findmodule(addr)) != 0) {
				pname += k + 3;
				int llable = 0;
				if (Findlabel(x + m->base, lable)) {
					llable = strlen(lable);
				}
				if (llable && (llable + len) < 256) {
					memcpy_s(&name[len], TEXTLEN - len, lable, llable);
					len += llable;
				}
				else {
					len += sprintf(&name[len], "%08X", m->base + x);
				}
			}
			else {
				name[len++] = *pname++;
			}
		}
		else {
			name[len++] = *pname++;
		}
	}
	name[len] = '\0';
	return len;
}

// Searches for name with given address and type. 
// Returns length of the name or 0 if name is absent. As a side effect, sets global arguments for Findnextname.
// 
// int Findname(ulong addr,int type,char *name);
// 
// Parameters:
// 
// addr - name address;
// 
// type - name type (NM_xxx for predefined types);
// 
// name - pointer to buffer of length at least TEXTLEN characters or NULL. If name is found, function copies it to this buffer.
// 
// OllyDbg Plugin API v1.10
int NameHelper::Findname(ulong addr, int type, char* name) {
	_lastType = 0;
	if (name)
		*name = '\0';
	if (!addr || type == NM_ANYNAME || !type)
		return 0;
	if (!_pSortHeaders)
		return 0;
	_nextIndex = GetSortHeader(addr);
	_lastType = type;
	while (true) {
		if (_nextIndex >= _headers || _pSortHeaders[_nextIndex].addr != addr)
			return 0;
		if (type <= _pSortHeaders[_nextIndex].type)
			break;

		++_nextIndex;
	}
	if (type < _pSortHeaders[_nextIndex].type) {
		++_nextIndex;
		return 0;
	}
	int len = strlen(_nameList + _pSortHeaders[_nextIndex].size);
	if (name) {
		memcpy_s(name, TEXTLEN, _nameList + _pSortHeaders[_nextIndex].size, len + 1);
	}
	++_nextIndex;
	return len;
}

// Searches for name with type specified in last call to Findname and address exceeding that in Findname or returned by last call to Findnextname. Returns address or 0 if there are no more compatible entries. If name is NULL, name itself is not fetched.
// 
// ulong Findnextname(char *name);
// 
// Parameters:
// 
// name - pointer to output buffer of length at least TEXTLEN characters. 
// 
// 
// 
// OllyDbg Plugin API v1.10
ulong NameHelper::Findnextname(char* name) {
	int index;
	if (_nextIndex < 0 || !_lastType || _lastType == NM_ANYNAME)
		return 0;

	if (name)
		*name = '\0';
	while (true) {
		if (_nextIndex >= _headers) {
			_nextIndex = _headers;
			return 0;
		}
		if (_pSortHeaders[_nextIndex].type == _lastType)
			break;
		++_nextIndex;
	}
	if (name)
		strcpy_s(name, TEXTLEN, _nameList + _pSortHeaders[_nextIndex].size);
	index = _nextIndex++;
	return _pSortHeaders[index].addr;
}

// Inserts new or replaces existing name of given type in the name table. If name is NULL or empty, entry is deleted. Returns 0 on success and -1 on error. Note: do not call this function between calls to Quickinsertname and Mergequicknames!
// 
// int Insertname(ulong addr,int type,char *name);
// 
// Parameters:
// 
// addr - name address;
// 
// type - name type (NM_xxx for predefined types);
// 
// name - name to insert. If name is NULL or empty, entry is removed from the name table.
// 
// OllyDbg Plugin API v1.10
int NameHelper::Insertname(ulong addr, int type, char* name) {
	t_sortheader* p = nullptr;
	size_t len;
	int lname;
	int header;
	t_sortheader* sortheader;
	char undecorated[512];
	char* oldName;
	size_t size;
	ulong offset;
	char* newList;

	if (!addr || !name || type == NM_ANYNAME || !type)
		return -1;
	if (!_pSortHeaders)
		return -1;

	if (_maxNameCount <= _headers && _maxNameCount < 0x989680) {
		p = (t_sortheader*)::VirtualAlloc(nullptr, 24 * _maxNameCount, MEM_COMMIT, PAGE_READWRITE);
		if (p) {
			memcpy_s(p, 24 * _maxNameCount, _pSortHeaders, 12 * _maxNameCount);
			::VirtualFree(_pSortHeaders, 0, MEM_RELEASE);
			_pSortHeaders = p;
			_maxNameCount *= 2;
		}
	}
	if (_curUseSize + TEXTLEN >= _size) {
		size = _nameCount <= 0x1000 && _size < 32000000 ? 2 * _size : _size;
		newList = (char*)::VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
		if (newList) {
			int k = 0;
			int offset = 0;
			while (k < _headers) {
				oldName = _nameList + _pSortHeaders[k].size;
				len = strlen(oldName) + 1;
				memcpy_s(&newList[offset], size, oldName, len);
				_pSortHeaders[k].size = offset;
				offset += len;
				++k;
			}
			::VirtualFree(_nameList, 0, MEM_RELEASE);
			_nameList = newList;
			_curUseSize = offset;
			_nameCount = 0;
		}
	}
	
	if (_demangleSymbolicName
		&& (type == NM_EXPORT || type == NM_IMPORT || type == NM_LIBRARY || type == 0x80 || type == 0x82)
		&&(lname = Demanglename(name,type,undecorated),lname>0)) {
		name = undecorated;
	}
	else {
		lname = strlen(name);
	}
	if (lname > 0)
		++lname;
	header = GetSortHeader(addr);
	for (sortheader = &_pSortHeaders[header]; header<_headers && sortheader->addr == addr && type>sortheader->type; ++sortheader) {
		++header;
	}

	if (header < _headers && _pSortHeaders[header].addr == addr && type == _pSortHeaders[header].type) {
		if (!lname) {
			if (header < _headers) {
				memmove_s(&_pSortHeaders[header], sizeof(t_sortheader) * (_headers - header), &_pSortHeaders[header + 1],
					sizeof(t_sortheader) * (_headers - header));
				++_nameCount;
				_lastType = 0;
				return 0;
			}
			if (lname + _curUseSize < _size) {
				memcpy_s(_nameList + _curUseSize, lname, name, lname);
				_pSortHeaders[header].size = _curUseSize;
				_curUseSize += lname;
				++_nameCount;
				_lastType = 0;
				return 0;
			}
			if (!_haveSpace)
				;
			_haveSpace = true;
			return -1;
		}
	}
	else {
		if (!lname) {
			_lastType = 0;
			return 0;
		}
		if (_headers < _maxNameCount && lname + _curUseSize < _size) {
			if (header < _headers) {
				memmove_s(&_pSortHeaders[header+1], sizeof(t_sortheader) * (_headers - header),
					&_pSortHeaders[header],sizeof(t_sortheader) * (_headers - header));
			}
			_pSortHeaders[header].addr = addr;
			_pSortHeaders[header].type = type;
			memcpy(_nameList + _curUseSize, name, lname);
			_pSortHeaders[header].size = _curUseSize;
			_curUseSize += lname;
			++_headers;
			_lastType = 0;
			return 0;
		}
		if (!_haveSpace)
			;
		_haveSpace = true;
		return -1;
	}
}

// Demangles or undecorates name. Currently supports Borland and Microsoft mangling schemes. Returns 0 if name is not mangled (in this case buffer pointed to by undecorated is invalid and probably modified) and length of unmangled name on success. Attention, no guarantee that demangled name is unique!
// 
// int Demanglename(char *name,int type,char *undecorated);
// 
// Parameters:
// 
// name - pointer to mangled name;
// 
// type - type of name. Function treats names of types NM_IMPORT and NM_IMPNAME in a special way;
// 
// undecorated - pointer to output buffer of length at least TEXTLEN characters.
// 
// OllyDbg Plugin API v1.10
int NameHelper::Demanglename(char* name, int type, char* undecorated) {
	return 0;
}

// Inserts new or replaces existing name of given type in the name table. NULL or empty names are not allowed. Returns 0 on success and -1 on error. Names added by this function are unavailable until you call 
// Mergequicknames. If you add multiple names, Quickinsertname is much faster than Insertname. Note: do not call Insertname between calls to Quickinsertname and Mergequicknames!
// 
// int Quickinsertname(ulong addr,int type,char *name);
// 
// 
// Parameters:
// 
// addr - name address;
// 
// type - name type (NM_xxx for predefined types);
// 
// name - name to insert. If name is NULL or empty, entry is removed from the name table.
// 
// 
// 
// OllyDbg Plugin API v1.10
int NameHelper::Quickinsertname(ulong addr, int type, char* name) {
	
	if (!addr || !name || type == NM_ANYNAME || !type)
		return -1;
	if (!*name)
		return 0;

	return -1;
}

void NameHelper::Mergequicknames() {

}

void NameHelper::Discardquicknames() {

}

void NameHelper::Deletenamerange(ulong addr0, ulong addr1, int type) {

}

// Searches for name of types NM_LABEL, NM_EXPORT, NM_IMPORT, NM_LIBRARY or NM_CONST in the specified range. If name is found, copies its address to *addr and returns type of label, otherwise returns NM_NONAME. Attention, this function is very slow, it searches name table sequentially!
// 
// int Findlabelbyname(char *name,ulong *addr,ulong addr0,ulong addr1);
// 
// Parameters:
// 
// name - pointer to output buffer of length at least TEXTLEN characters;
// 
// addr - pointer to variable that receives address of found name;
// 
// addr0 - start of address range (included);
// 
// addr1 - end of address range (not included).
// 
// 
// 
// OllyDbg Plugin API v1.10
int NameHelper::Findlabelbyname(char* name, ulong* addr, ulong addr0, ulong addr1) {
	return NM_NONAME;
}

int NameHelper::Findsymbolicname(ulong addr, char* fname) {
	
	return 0;
}