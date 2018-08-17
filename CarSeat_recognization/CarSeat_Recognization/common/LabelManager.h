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
	

	/*
	条形码转换接口，将其中有效的6-8位提取出来
	*/
	std::wstring GetInternalTypeByBarcode(std::wstring barcode);

	/*
	根据从识别模块拿到的类型，转换成内部类型
	*/
	std::wstring GetInternalTypeByClassifyType(std::wstring type);
	
	/*
	根据从rfid拿到的条形码转换成外部类型，方便用户查看的类型
	*/
	std::wstring GetExternalTypeByBarcode(std::wstring barcode);
	
	/*
	根据识别模块的分类类型，转换成方便用户查看的外部类型
	*/
	std::wstring GetExternalTypeByClassifyType(std::wstring classifyType);
	
	/*
	根据外部类型抓化成识别模块的分类类型
	*/
	std::wstring GetClassifyTypeByExternal(std::wstring externalType);
	

	std::vector<std::wstring> GetBarcode();
	
	std::vector<std::wstring> GetExternalType();

	std::vector<std::wstring> GetClassifyType();

	const wchar_t *GetLoginUsrName()const;
	const wchar_t *GetLoginPasswd()const;
	bool SetLoginUsrName(const wchar_t *name);
	bool SetLoginPasswd(const wchar_t *passwd);
	bool GetLoginAutoSave();
	void SetLoginAutoSave(bool autoSave);

	int GetExposureTimeMax();
	int GetExposureTimeMin();

	/*  not implement，从服务器发过来的xml，更新条形码对照表 */
	void UpdateBarcode(const char *xmlContent, size_t len);

	

private:
	bool init();


	bool serialize();

	bool m_bSerialize;

	wchar_t m_strLoginName[MAX_USR_NAME_AND_PASSWD];
	wchar_t m_strLoginPasswd[MAX_USR_NAME_AND_PASSWD];

	int m_nExposureTimeMax;
	int m_nExposureTimeMin;
	bool m_bLoginAutoSave;
	
	/*
	内部类型转换外部类型的条形码对照表,条形码位截取其中有效字段组成
	*/
	std::unordered_map<std::wstring, std::wstring> *m_pBarcode;
	
	/*
	识别模块的类型转化成内部类型的对照表
	*/
	std::unordered_map<std::wstring, std::wstring> *m_pClassifyType;
	
	static bool m_bInitFlag;
};

