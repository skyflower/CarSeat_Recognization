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
	std::string GetInternalTypeByBarcode(std::string barcode);

	/*
	根据从识别模块拿到的类型，转换成内部类型
	*/
	std::string GetInternalTypeByClassifyType(std::string type);
	
	/*
	根据从rfid拿到的条形码转换成外部类型，方便用户查看的类型
	*/
	std::string GetExternalTypeByBarcode(std::string barcode);
	
	/*
	根据识别模块的分类类型，转换成方便用户查看的外部类型
	*/
	std::string GetExternalTypeByClassifyType(std::string classifyType);
	
	/*
	根据外部类型抓化成识别模块的分类类型
	*/
	std::string GetClassifyTypeByExternal(std::string externalType);
	

	std::vector<std::string> GetBarcode();
	
	std::vector<std::string> GetExternalType();

	std::vector<std::string> GetClassifyType();

	const char *GetLoginUsrName()const;
	const char *GetLoginPasswd()const;
	bool SetLoginUsrName(const char *name);
	bool SetLoginPasswd(const char *passwd);
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

	char m_strLoginName[MAX_USR_NAME_AND_PASSWD];
	char m_strLoginPasswd[MAX_USR_NAME_AND_PASSWD];

	int m_nExposureTimeMax;
	int m_nExposureTimeMin;
	bool m_bLoginAutoSave;
	
	/*
	内部类型转换外部类型的条形码对照表,条形码位截取其中有效字段组成
	*/
	std::unordered_map<std::string, std::string> *m_pBarcode;
	
	/*
	识别模块的类型转化成内部类型的对照表
	*/
	std::unordered_map<std::string, std::string> *m_pClassifyType;

	char m_configFile[MAX_CHAR_LENGTH];
	
	static bool m_bInitFlag;
};

