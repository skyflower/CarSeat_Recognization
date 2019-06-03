﻿#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
  
/*

解析参数文件config.txt
此文件中的参数不会动态改变

*/

class CParamManager
{
public:
	CParamManager();
	~CParamManager();

	static CParamManager* GetInstance();

	unsigned int GetLocalIP();

	unsigned int GetServerIP();
	unsigned int GetServerPort();
	unsigned int GetTestServerPort();
	unsigned int GetTestClientPort();

	unsigned int GetKepServerIp();
	unsigned int GetKepServerPort();

	unsigned int GetEdsImageQuality();


	//std::vector<std::wstring> *GetFtpParameter();
	//std::string FindCameraByLineID(std::string lineID);

	unsigned int GetBarcodeIp();
	unsigned int GetBarcodePort();

	const char* GetGraphFile() const;
	const char* GetLabelFile() const;
	const char* GetImageDirectory() const;
	const char* GetLineName() const;
	const char* GetCameraName() const;
	const char* GetPatternImagePath() const;

	//临时存储识别模块的图像路径
	const char* GetCacheImagePath() const;

	size_t GetBarcodeTime() const;

	const char* GetBarcodeResetParam() const;

	const char* GetSendFailedCache() const;

	int GetServerImagePort() const;

	std::string GetLocalName() { return m_strLocalName; }

	RECT GetImageROI();


private:

	static CParamManager *m_pInstance;

	void Init();
	

	unsigned int __auxLocalIP();

	// serverip， 服务器Ip
	unsigned int m_nServerIp;

	// serverport 服务器通信端口号
	int m_nServerPort;

	//发送图像数据的端口号
	int m_nServerImagePort;

	// 本机和服务器通信的Ip
	unsigned int m_nLocalIp;

	// 和服务器测试连通的服务器端口号
	int m_nTestServerPort;

	// 和服务器测试连通的客户端端口号
	int m_nTestClientPort;

	// 获取条形码的ip
	unsigned int m_nBarcodeIp;

	//获取条形码的端口号
	unsigned int m_nBarcodePort;

	

	char m_szBarcodeResetParam[100];

	unsigned int m_nEdsImageQuality;


	//从获取条形码到拍照的延迟时间
	size_t m_nBarcodeTime;

	// kepServer模块通信ip地址
	unsigned int m_nKepServerIp;

	// kepServer模块通信端口
	unsigned int m_nKepServerPort;

	//拍照图像本地保存路径
	char m_szImagePath[256];

	//模板图像路径
	char m_szPatternImagePath[256];

	//传送失败的文件路径
	char m_szSendFailedCache[256];

	//产线名称编号
	char m_szLineName[20];

	//相机名字或者序列号等
	char m_szCameraName[100];

	//识别模块临时存储的图像路径
	char m_szCacheImagePath[256];

	// 本机名
	std::string m_strLocalName;

	//ftp用户名密码以及图像的存储路径
	//std::vector<std::wstring> *m_pFtp;

	//graph
	std::string m_szGraphFile;

	std::string m_szLabelFile;

	//产线和相机的对应关系
	//std::unordered_map<std::string, std::string> *m_pLineCamera;

	///图像ROI区域
	std::unordered_map<std::string, int> *m_pImageROI;
};

