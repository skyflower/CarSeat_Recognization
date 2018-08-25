#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "utils.h"

/*

���������ļ�config.txt
���ļ��еĲ������ᶯ̬�ı�

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

	//�Զ������־
	bool mAutoSaveFlag;

	// serverip�� ������Ip
	size_t m_nServerIp;

	// serverport ������ͨ�Ŷ˿ں�
	int m_nServerPort;

	// �����ͷ�����ͨ�ŵ�Ip
	size_t m_nLocalIp;

	// ������
	std::string m_strLocalName;

	std::string m_strUsrName;
	std::string m_strPasswd;

	//ftp�û��������Լ�ͼ��Ĵ洢·��
	std::vector<std::string> *m_pFtp;

	//���߱��
	std::vector<std::string> *m_pLineVec;

	//methodType
	std::vector<std::string> *m_pMethodType;

	//seatType
	std::vector<std::string> *m_pSeatType;
};

