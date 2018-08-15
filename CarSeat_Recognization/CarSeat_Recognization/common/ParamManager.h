#pragma once
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


	std::vector<std::wstring> *GetFtpParameter();
	std::wstring FindCameraByLineID(std::wstring lineID);

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


	std::wstring GetLocalName() { return m_strLocalName; }


private:

	static CParamManager *m_pInstance;

	void Init();
	

	unsigned int __auxLocalIP();

	// serverip， 服务器Ip
	unsigned int m_nServerIp;

	// serverport 服务器通信端口号
	int m_nServerPort;

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

	// kepServer模块通信ip地址
	unsigned int m_nKepServerIp;

	// kepServer模块通信端口
	unsigned int m_nKepServerPort;

	//拍照图像本地保存路径
	char m_szImagePath[256];

	//模板图像路径
	char m_szPatternImagePath[256];

	//产线名称编号
	char m_szLineName[20];

	//相机名字或者序列号等
	char m_szCameraName[100];

	//识别模块临时存储的图像路径
	char m_szCacheImagePath[256];

	// 本机名
	std::wstring m_strLocalName;

	//ftp用户名密码以及图像的存储路径
	std::vector<std::wstring> *m_pFtp;

	//graph
	std::string m_szGraphFile;

	std::string m_szLabelFile;

	//产线和相机的对应关系
	std::unordered_map<std::wstring, std::wstring> *m_pLineCamera;
};

