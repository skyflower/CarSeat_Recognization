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

	std::vector<std::wstring> *GetFtpParameter();
	std::vector<std::wstring> *GetOutlineParameter();
	std::vector<std::wstring> *GetTextureParameter();
	std::vector<std::wstring> *GetColorParameter();
	std::wstring FindCameraByLineID(std::wstring lineID);

	unsigned int GetBarcodeIp();
	unsigned int GetBarcodePort();

	std::wstring GetLocalName() { return m_strLocalName; }


private:

	static CParamManager *m_pInstance;

	void Init();
	bool parseVector(const char *content, const char*name, std::vector<std::wstring> *pVector);
	bool parseLineSegment(const char *name, size_t, std::vector<std::wstring> *pVec);
	int parseIp(const char *content, const char *name);
	int parseMap(const char*content, const char*name, std::unordered_map<std::wstring, std::wstring>*pMap);

	bool getValueByName(const char *content, const char*name, char*value);

	unsigned int __auxLocalIP();

	// serverip， 服务器Ip
	size_t m_nServerIp;

	// serverport 服务器通信端口号
	int m_nServerPort;

	// 本机和服务器通信的Ip
	size_t m_nLocalIp;

	// 和服务器测试连通的服务器端口号
	int m_nTestServerPort;

	// 和服务器测试连通的客户端端口号
	int m_nTestClientPort;

	// 获取条形码的ip
	size_t m_nBarcodeIp;

	//获取条形码的端口号
	size_t m_nBarcodePort;

	// 本机名
	std::wstring m_strLocalName;

	// 颜色选项
	std::vector<std::wstring> *m_pColor;

	// 汽车座椅外形
	std::vector<std::wstring> *m_pOutline;

	//汽车座椅纹理类型
	std::vector<std::wstring> *m_pTexture;

	// ftp用户名密码以及图像的存储路径
	std::vector<std::wstring> *m_pFtp;

	//产线和相机的对应关系
	std::unordered_map<std::wstring, std::wstring> *m_pLineCamera;
};

