#pragma once

#include <iostream>
#include <Windows.h>
#include <unordered_map>


struct RecogResult
{
	char m_szBarcode[32];		//	完整条形码
	char m_szTime[32];			//	结果形成的时间戳，包含本地日期和时间
	char m_szTypeByRecog[32];	//	识别结果的外部类型
	char m_szMaterial[12];		//	根据识别结果拿到的材料类型
	char m_szColor[12];			//	根据识别结果得到的座椅颜色
	bool m_bIsCorrect;			//	识别结果是否一致
	char m_szRecogMethod[8];	//	识别方法，目前只是"auto"
	char m_szCameraName[20];	//	拍照相机的名字
	char m_szLineName[8];		//	产线名称
	char m_szUsrName[20];		//	产线管理员用户名
	char m_szImagePath[256];	//	照片路径
};



class CRecogResultManager
{
public:
	
	~CRecogResultManager();

	static CRecogResultManager *GetInstance();

	const RecogResult* findByBarcode(const char *barcode)const;

private:

	CRecogResultManager();

	void init();


	std::hash<const char*> m_pHashFunc;

	static CRecogResultManager *m_pInstance;

	std::unordered_map<size_t, RecogResult> *m_pRecogResult;

};

