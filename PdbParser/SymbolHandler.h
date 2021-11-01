#pragma once

class SymbolHandler final {
public:
	SymbolHandler(HANDLE hProcess, bool ownHandle);

	void LoadSymbolsForModule(std::string imageName, DWORD64 dllBase = 0, HANDLE hFile = nullptr);
	void EnumSymbols(std::string mask = "*!*");

	void EnumTypes(std::string mask = "*");
	ULONG64 GetSymbolAddressFromName(std::string Name);
	DWORD GetStructMemberOffset(std::string Name, std::string memberName);

	bool GetSymbolFromName(std::string name, PSYMBOL_INFO pSymbol);
	bool GetSymbolFromIndex(DWORD64 dllBase, int index, PSYMBOL_INFO pSymbol);

private:
	HANDLE _hProcess;
	bool _ownHandle;
	DWORD64 _baseAddress = 0;
};