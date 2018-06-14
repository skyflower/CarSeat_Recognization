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

	/*  not implement���ӷ�������������xml��������������ձ� */
	void UpdateBarcode();


private:
	bool init();


	/*  not implement,���浽labelConfig.txt�ļ� */
	bool serialize();

	std::unordered_map<std::wstring, std::wstring> *m_pBarcode;
	std::unordered_map<std::wstring, std::wstring> *m_pClassifyType;
	static bool m_bInitFlag;
};

