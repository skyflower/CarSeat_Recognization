#pragma once
#include <iostream>
#include <cstring>
#include <unordered_map>

class CLabelManager
{
public:
	CLabelManager();
	~CLabelManager();
	std::wstring GetInternalTypeByBarcode(std::wstring barcode);
	std::wstring GetInternalTypeByClassifyType(std::wstring type);
	std::vector<std::wstring> GetBarcode();
	std::vector<std::wstring> GetInternalType();
	std::vector<std::wstring> GetClassifyType();


private:
	bool init();

	std::unordered_map<std::wstring, std::wstring> *m_pBarcode;
	std::unordered_map<std::wstring, std::wstring> *m_pClassifyType;
	static bool m_bInitFlag;
};

