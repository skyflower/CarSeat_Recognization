#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "utils.h"

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
	
	const std::vector<std::string> *GetFtpParameter()const;
	const std::vector<std::string> *GetMethodType()const;
	const std::vector<std::string> *GetSeatType()const;
	const std::vector<std::string> *GetLineNo()const;


	std::string GetLocalName() { return m_strLocalName; }
	std::string GetLoginUserName() { return m_strUsrName; }
	std::string GetLoginPasswd() { return m_strPasswd; }

	bool SetLoginUserName(std::string tmpUserName);
	bool SetLoginPasswd(std::string tmpPasswd);


private:

	static CParamManager *m_pInstance;

	void Init();

	void serialization();
	

	unsigned int __auxLocalIP();

	char m_szConfigFile[MAX_CHAR_LENGTH];

	//自动保存标志
	bool mAutoSaveFlag;

	// serverip， 服务器Ip
	size_t m_nServerIp;

	// serverport 服务器通信端口号
	int m_nServerPort;

	// 本机和服务器通信的Ip
	size_t m_nLocalIp;

	// 本机名
	std::string m_strLocalName;

	std::string m_strUsrName;
	std::string m_strPasswd;

	//ftp用户名密码以及图像的存储路径
	std::vector<std::string> *m_pFtp;

	//产线编号
	std::vector<std::string> *m_pLineVec;

	//methodType
	std::vector<std::string> *m_pMethodType;

	//seatType
	std::vector<std::string> *m_pSeatType;
};

