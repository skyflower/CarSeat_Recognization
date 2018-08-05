#pragma once
#include <iostream>
#include <cstring>
#include <unordered_map>
#include "./utils.h"



class CLabelManager
{
public:
	CLabelManager();
	~CLabelManager();
	std::wstring GetInternalTypeByBarcode(std::wstring barcode);
	std::wstring GetInternalTypeByClassifyType(std::wstring type);
	
	std::wstring GetExternalTypeByBarcode(std::wstring barcode);
	std::wstring GetExternalTypeByClassifyType(std::wstring classifyType);
	std::wstring GetClassifyTypeByExternal(std::wstring externalType);
	
	std::vector<std::wstring> GetBarcode();
	std::vector<std::wstring> GetExternalType();
	//std::vector<std::wstring> GetInternalType();
	std::vector<std::wstring> GetClassifyType();

	const wchar_t *GetLoginUsrName()const;
	const wchar_t *GetLoginPasswd()const;
	bool SetLoginUsrName(const wchar_t *name);
	bool SetLoginPasswd(const wchar_t *passwd);
	bool GetLoginAutoSave();
	void SetLoginAutoSave(bool autoSave);

	/*  not implement，从服务器发过来的xml，更新条形码对照表 */
	void UpdateBarcode(const char *xmlContent, size_t len);


private:
	bool init();


	bool serialize();

	bool m_bSerialize;

	wchar_t m_strLoginName[MAX_USR_NAME_AND_PASSWD];
	wchar_t m_strLoginPasswd[MAX_USR_NAME_AND_PASSWD];
	bool m_bLoginAutoSave;
	

	std::unordered_map<std::wstring, std::wstring> *m_pBarcode;
	std::unordered_map<std::wstring, std::wstring> *m_pClassifyType;
	static bool m_bInitFlag;
};

