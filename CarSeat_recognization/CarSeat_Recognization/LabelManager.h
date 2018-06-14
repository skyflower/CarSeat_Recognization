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
	
	std::wstring GetExternalTypeByBarcode(std::wstring barcode);
	std::wstring GetExternalTypeByClassifyType(std::wstring classifyType);
	
	std::vector<std::wstring> GetBarcode();
	std::vector<std::wstring> GetInternalType();
	std::vector<std::wstring> GetClassifyType();

	/*  not implement，从服务器发过来的xml，更新条形码对照表 */
	void UpdateBarcode();


private:
	bool init();


	/*  not implement,保存到labelConfig.txt文件 */
	bool serialize();

	std::unordered_map<std::wstring, std::wstring> *m_pBarcode;
	std::unordered_map<std::wstring, std::wstring> *m_pClassifyType;
	static bool m_bInitFlag;
};

