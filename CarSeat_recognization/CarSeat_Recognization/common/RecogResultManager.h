#pragma once

#include <iostream>
#include <Windows.h>
#include <list>
#include "utils.h"


class RecogResult
{
public:
	char m_szBarcode[32];		//	完整条形码
	char m_szTime[32];			//	结果形成的时间戳，包含本地日期和时间
	char m_szTypeByRecog[32];	//	识别结果的外部类型
	char m_szTypeByBarcode[32];	//	根据条形码拿到的外部类型
	char m_szTypeByUsrInput[32];	//当识别错误的时候，产线管理员输入的外部类型
	char m_szRecogMethod[8];	//	识别方法，目前只是"auto"
	char m_szCameraName[20];	//	拍照相机的名字
	char m_szLineName[8];		//	产线名称
	char m_szUsrName[20];		//	产线管理员用户名
	char m_szImagePath[256];	//	照片路径

	bool m_bIsCorrect;			//	识别结果是否一致


};

class RecogResultW
{
public:
	wchar_t m_szBarcode[32];		//	完整条形码
	wchar_t m_szTime[32];			//	结果形成的时间戳，包含本地日期和时间
	wchar_t m_szTypeByRecog[32];	//	识别结果的外部类型
	wchar_t m_szTypeByBarcode[32];	//	根据条形码拿到的外部类型
	wchar_t m_szTypeByUsrInput[32];	//当识别错误的时候，产线管理员输入的外部类型
	wchar_t m_szRecogMethod[8];		//	识别方法，目前只是"auto"
	wchar_t m_szCameraName[20];		//	拍照相机的名字
	wchar_t m_szLineName[8];		//	产线名称
	wchar_t m_szUsrName[20];		//	产线管理员用户名
	wchar_t m_szImagePath[256];		//	照片路径
	bool m_bIsCorrect;				//	识别结果是否一致
};


class CRecogResultManager
{
public:
	
	~CRecogResultManager();

	static CRecogResultManager *GetInstance();

	const RecogResult* findByBarcode(const char *barcode)const;
	bool add(const RecogResult &a);

private:

	CRecogResultManager();

	void init();
	bool serialize();

	bool parseLine(char *line, RecogResult &a);

	void output(RecogResult &a);

	char m_szName[256];

	bool m_bAutoSave;

	static CRecogResultManager *m_pInstance;

	std::list<RecogResult> *m_pRecogResult;

};

