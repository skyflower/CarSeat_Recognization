#include "../stdafx.h"
#include "ParamManager.h"
#include <fstream>
#include "Log.h"
#include "utils.h"
#include <functional>
#include "../network/NetworkTask.h"


CParamManager *CParamManager::m_pInstance = nullptr;

CParamManager::CParamManager() :
m_pFtp(nullptr),
m_pLineVec(nullptr),
m_nLocalIp(-1),
m_nServerIp(-1),
m_nServerPort(-1),
m_pMethodType(nullptr),
m_pSeatType(nullptr),
mAutoSaveFlag(false)
{
	memset(m_szConfigFile, 0, sizeof(m_szConfigFile));
	strcat_s(m_szConfigFile, sizeof(m_szConfigFile), "./queryConfig.txt");
	Init();
}


CParamManager::~CParamManager()
{
	serialization();

	if (m_pFtp != nullptr)
	{
		m_pFtp->clear();
		delete m_pFtp;
		m_pFtp = nullptr;
	}
	if (m_pLineVec != nullptr)
	{
		m_pLineVec->clear();
		delete m_pLineVec;
		m_pLineVec = nullptr;
	}
	if (m_pSeatType != nullptr)
	{
		m_pSeatType->clear();
		delete m_pSeatType;
		m_pSeatType = nullptr;
	}
	if (m_pMethodType != nullptr)
	{
		m_pMethodType->clear();
		delete m_pMethodType;
		m_pMethodType = nullptr;
	}
}


CParamManager* CParamManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CParamManager();
	}
	return m_pInstance;
}

unsigned int CParamManager::GetLocalIP()
{
	if ((m_nLocalIp == 0) || (m_nLocalIp == -1))
	{
		unsigned int tmp = __auxLocalIP();
		if ((tmp == 0) || (tmp == -1))
		{
			return 0;
		}
		m_nLocalIp = tmp;
		return m_nLocalIp;
	}
	return m_nLocalIp;
}

unsigned int CParamManager::__auxLocalIP()
{
	clock_t startTime = clock();
	//WORD wVersionRequested = MAKEWORD(2, 2);

	/*WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		return 0;
	}*/

	char               buf[100];
	int                ret = 0;
	struct addrinfo    hints;
	struct addrinfo    *res = nullptr, *curr = nullptr;
	struct sockaddr_in *sa = nullptr;


	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;
	if (gethostname(buf, sizeof(buf)) < 0)
	{
		WriteError("get local name Failed");
		return 0;
	}

	{
		//std::string tmpLocalName(buf);
		m_strLocalName = std::string(buf);// utils::StrToWStr(tmpLocalName);
	}
	
	if ((ret = getaddrinfo(buf, NULL, &hints, &res)) != 0)
	{
		WriteError("getaddrinfo: %s\n", gai_strerror(ret));
		return 0;
	}
	
	curr = res;
	
	unsigned int nIp = 0;
	while (curr)
	{
		sa = (struct sockaddr_in *)curr->ai_addr;
		memset(buf, 0, sizeof(buf));
		inet_ntop(AF_INET, &sa->sin_addr.S_un.S_addr, buf, sizeof(buf));
		TRACE1("IP = 0x%X\n", sa->sin_addr.S_un.S_addr);
		int tmpIp = (sa->sin_addr.S_un.S_un_b.s_b1 << 24) | \
			(sa->sin_addr.S_un.S_un_b.s_b2 << 16) | \
			(sa->sin_addr.S_un.S_un_b.s_b3 << 8) | \
			(sa->sin_addr.S_un.S_un_b.s_b4);
		if (true == CNetworkTask::IsReachable(tmpIp, m_nServerIp))
		{
			nIp = tmpIp;
			break;
		}
		curr = curr->ai_next;
	}

	//WSACleanup();
	clock_t endTime = clock();
	TRACE1("get localIp cost Time = %u\n", endTime - startTime);

	return nIp;
}

unsigned int CParamManager::GetServerIP()
{
	return m_nServerIp;
}

unsigned int CParamManager::GetServerPort()
{
	return m_nServerPort;
}

const std::vector<std::string>* CParamManager::GetFtpParameter() const
{
	return m_pFtp;
	
}

const std::vector<std::string>* CParamManager::GetMethodType() const
{
	return m_pMethodType;
	
}

const std::vector<std::string>* CParamManager::GetSeatType() const
{
	return m_pSeatType;
	
}

const std::vector<std::string>* CParamManager::GetLineNo() const
{
	return m_pLineVec;
	
}

bool CParamManager::GetAutoSaveFlag()
{
	return mAutoSaveFlag;
}

void CParamManager::SetAutoSaveFalg(bool flag)
{
	mAutoSaveFlag = flag;
}

bool CParamManager::SetLoginUserName(std::string tmpUserName)
{
	mAutoSaveFlag = true;
	m_strUsrName = tmpUserName;
	return true;
}

bool CParamManager::SetLoginPasswd(std::string tmpPasswd)
{
	mAutoSaveFlag = true;
	m_strPasswd = tmpPasswd;
	return true;
}


void CParamManager::Init()
{
	FILE *fp = nullptr;
	fopen_s(&fp, m_szConfigFile, "rb");
	bool ret = true;
	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		size_t length = ftell(fp);
		char *content = new char[length + 1];
		memset(content, 0, sizeof(char) * (length + 1));
		fseek(fp, 0, SEEK_SET);
		fread_s(content, length, 1, length, fp);
		
		if (m_pFtp == nullptr)
		{
			m_pFtp = new std::vector<std::string>;
		}
		ret = utils::parseVector(content, "ftpLogin", m_pFtp);
		if(ret == false)
		{
			WriteError("ftp Parameter init Failed");
		}
		if (m_pLineVec == nullptr)
		{
			m_pLineVec = new std::vector<std::string>;
		}
		utils::parseVector(content, "line", m_pLineVec);
		if ((m_pLineVec == nullptr) || (m_pLineVec->size() == 0))
		{
			WriteError("line No. init Failed");
		}

		if (m_pMethodType == nullptr)
		{
			m_pMethodType = new std::vector<std::string>;
		}
		utils::parseVector(content, "methodType", m_pMethodType);
		if ((m_pMethodType == nullptr) || (m_pMethodType->size() == 0))
		{
			WriteError("method type init Failed");
		}


		if (m_pSeatType == nullptr)
		{
			m_pSeatType = new std::vector<std::string>;
		}
		utils::parseVector(content, "seatType", m_pSeatType);
		if ((m_pSeatType == nullptr) || (m_pSeatType->size() == 0))
		{
			WriteError("method type init Failed");
		}

		unsigned int tmpLocal = utils::parseIp(content, "serverip");
		if (tmpLocal == 0)
		{
			TRACE0("get ServerIp Failed\n");
		}
		m_nServerIp = tmpLocal;


		char tmpStr[MAX_CHAR_LENGTH] = { 0 };
		if (utils::getValueByName(content, "serverport", tmpStr) == true)
		{
			m_nServerPort = atoi(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (utils::getValueByName(content, "username", tmpStr) == true)
		{
			if (strlen(tmpStr) > 0)
			{
				//m_strUsrName = std::wstring(tmpStr);
				std::string tmp(tmpStr);
				m_strUsrName = tmp;
			}
			
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (utils::getValueByName(content, "passwd", tmpStr) == true)
		{
			if (strlen(tmpStr) > 0)
			{
				std::string tmp(tmpStr);
				m_strPasswd = tmp;
			}
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (content != nullptr)
		{
			delete[]content;
			content = nullptr;
		}
	}
	unsigned int tmpLocal = __auxLocalIP();
	if ((tmpLocal != 0) && (tmpLocal != -1))
	{
		m_nLocalIp = tmpLocal;
	}
	fclose(fp);
	fp = nullptr;
}

void CParamManager::serialization()
{
	if (mAutoSaveFlag == false)
	{
		return;
	}
	FILE *fp = nullptr;
	fopen_s(&fp, m_szConfigFile, "wb");
	if (fp == nullptr)
	{
		WriteError("serialization queryConfig.txt Failed");
		return;
	}
	const size_t Length = 2000;
	char *tmpStr = new char[Length];
	memset(tmpStr, 0, sizeof(char) * Length);

	if ((m_pLineVec != nullptr) && (m_pLineVec->size() > 0))
	{
		strcat_s(tmpStr, Length, "line={");
		size_t tmpLength = m_pLineVec->size();
		for (size_t i = 0; i < tmpLength - 1; ++i)
		{
			sprintf_s(tmpStr, Length, "%s\"%s\",", tmpStr, m_pLineVec->at(i).c_str());
			//StrCatW(tmpStr, m_pLineVec->at(i).c_str());
		}
		sprintf_s(tmpStr, Length, "%s\"%s\"}\n", tmpStr, m_pLineVec->at(tmpLength - 1).c_str());
	}
	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(char) * Length);


	if ((m_pSeatType != nullptr) && (m_pSeatType->size() > 0))
	{
		strcat_s(tmpStr, Length, "seatType={");
		size_t tmpLength = m_pSeatType->size();
		for (size_t i = 0; i < tmpLength - 1; ++i)
		{
			sprintf_s(tmpStr, Length, "%s\"%s\",\n", tmpStr, m_pSeatType->at(i).c_str());
			//StrCatW(tmpStr, m_pLineVec->at(i).c_str());
		}
		sprintf_s(tmpStr, Length, "%s\"%s\"}\n", tmpStr, m_pSeatType->at(tmpLength - 1).c_str());
	}
	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(char) * Length);

	sprintf_s(tmpStr, Length, "serverip=\"%d.%d.%d.%d\"\nserverport=\"%d\"\n", \
		((m_nServerIp >> 24) & 0xFF), ((m_nServerIp >> 16) & 0xFF), \
		((m_nServerIp >> 8) & 0xFF), (m_nServerIp & 0xFF), m_nServerPort);

	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(char) * Length);


	if ((m_pFtp != nullptr) && (m_pFtp->size() > 0))
	{
		strcat_s(tmpStr, Length, "ftpLogin={");
		size_t tmpLength = m_pFtp->size();
		for (size_t i = 0; i < tmpLength - 1; ++i)
		{
			sprintf_s(tmpStr, Length, "%s\"%s\",", tmpStr, m_pFtp->at(i).c_str());
			//StrCatW(tmpStr, m_pLineVec->at(i).c_str());
		}
		sprintf_s(tmpStr, Length, "%s\"%s\"}\n", tmpStr, m_pFtp->at(tmpLength - 1).c_str());
	}
	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(char) * Length);

	if ((m_pMethodType != nullptr) && (m_pMethodType->size() > 0))
	{
		strcat_s(tmpStr, Length, "methodType={");
		size_t tmpLength = m_pMethodType->size();
		for (size_t i = 0; i < tmpLength - 1; ++i)
		{
			sprintf_s(tmpStr, Length, "%s\"%s\",", tmpStr, m_pMethodType->at(i).c_str());
			//StrCatW(tmpStr, m_pLineVec->at(i).c_str());
		}
		sprintf_s(tmpStr, Length, "%s\"%s\"}\n", tmpStr, m_pMethodType->at(tmpLength - 1).c_str());
	}
	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(char) * Length);





	sprintf_s(tmpStr, Length, "username=\"%s\"\npasswd=\"%s\"\n", m_strUsrName.c_str(),
		m_strPasswd.c_str());

	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(char) * Length);

	delete[]tmpStr;
	tmpStr = nullptr;

	fclose(fp);
	fp = nullptr;
}

