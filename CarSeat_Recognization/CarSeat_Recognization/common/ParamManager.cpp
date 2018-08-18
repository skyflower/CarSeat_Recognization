#include "../stdafx.h"
#include "ParamManager.h"
#include <fstream>
#include "Log.h"
#include "utils.h"
#include <functional>
#include "../network/NetworkTask.h"
#include <cstdlib>
#include <io.h>

CParamManager *CParamManager::m_pInstance = nullptr;

CParamManager::CParamManager() :
//m_pFtp(nullptr),
m_pLineCamera(nullptr),
m_nLocalIp(-1),
m_nServerIp(-1),
m_nServerPort(-1),
m_nTestClientPort(-1),
m_nTestServerPort(-1),
m_nBarcodeIp(-1),
m_nBarcodePort(-1),
m_nKepServerIp(-1),
m_nKepServerPort(-1),
m_nBarcodeTime(1)
{
	
	memset(m_szImagePath, 0, sizeof(m_szImagePath));
	memset(m_szLineName, 0, sizeof(m_szLineName));
	memset(m_szCameraName, 0, sizeof(m_szCameraName));
	memset(m_szPatternImagePath, 0, sizeof(m_szPatternImagePath));
	memset(m_szCacheImagePath, 0, sizeof(m_szCacheImagePath));
	//m_szBarcodeResetParam
	memset(m_szBarcodeResetParam, 0, sizeof(m_szBarcodeResetParam));
	
	Init();


	/// 建立日期目录存储图像文件
	time_t  time1 = time(NULL);//获取系统时间，单位为秒;
	struct tm tmpTime;
	localtime_s(&tmpTime, &time1);//转换成tm类型的结构体;

	char logFileName[100] = { 0 };
	memset(logFileName, 0, sizeof(logFileName));

	sprintf_s(logFileName, "%s\\%04d%02d%02d", \
		m_szImagePath, tmpTime.tm_year + 1900, tmpTime.tm_mon + 1, tmpTime.tm_mday);

	memset(m_szImagePath, 0, sizeof(m_szImagePath));
	memcpy(m_szImagePath, logFileName, strlen(logFileName));
	//判断目录是否存在，
	if (_access(m_szImagePath, 0) != 0)
	{
		wchar_t *tmpPath = utils::CharToWchar(m_szImagePath);
		if (tmpPath != nullptr)
		{
			_wmkdir(tmpPath);
			delete tmpPath;
			tmpPath = nullptr;
		}
		
	}
}


CParamManager::~CParamManager()
{
	/*if (m_pFtp != nullptr)
	{
		m_pFtp->clear();
		delete m_pFtp;
		m_pFtp = nullptr;
	}*/
	if (m_pLineCamera != nullptr)
	{
		//m_pLineCamera->clear();
		delete m_pLineCamera;
		m_pLineCamera = nullptr;
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
	WORD wVersionRequested = MAKEWORD(2, 2);

	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		return 0;
	}

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
		std::string tmpLocalName(buf);
		m_strLocalName = utils::StrToWStr(tmpLocalName);
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
		WriteInfo("IP = 0x%X", sa->sin_addr.S_un.S_addr);
		int tmpIp = (sa->sin_addr.S_un.S_un_b.s_b1 << 24) | \
			(sa->sin_addr.S_un.S_un_b.s_b2 << 16) | \
			(sa->sin_addr.S_un.S_un_b.s_b3 << 8) | \
			(sa->sin_addr.S_un.S_un_b.s_b4);
		if (true == CNetworkTask::IsReachable(tmpIp, m_nServerIp))
		{
			nIp = tmpIp;
			break;
		}
		WriteError("ip reach failed");
		curr = curr->ai_next;
	}

	WSACleanup();
	clock_t endTime = clock();
	TRACE1("get localIp cost Time = %u", endTime - startTime);
	WriteInfo("get localIp cost Time = %u", endTime - startTime);


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

unsigned int CParamManager::GetTestServerPort()
{
	return m_nTestServerPort;
}

unsigned int CParamManager::GetTestClientPort()
{
	return m_nTestClientPort;
}

unsigned int CParamManager::GetKepServerIp()
{
	return m_nKepServerIp;
}

unsigned int CParamManager::GetKepServerPort()
{
	return m_nKepServerPort;
}

//std::vector<std::wstring>* CParamManager::GetFtpParameter()
//{
//	return m_pFtp;
//}


std::wstring CParamManager::FindCameraByLineID(std::wstring lineID)
{
	if (m_pLineCamera != nullptr)
	{
		std::unordered_map < std::wstring, std::wstring>::const_iterator iter = \
			m_pLineCamera->find(lineID);
		if (iter == m_pLineCamera->end())
		{
			return std::wstring();
		}
		return iter->second;
	}
	return std::wstring();
}

unsigned int CParamManager::GetBarcodeIp()
{
	return m_nBarcodeIp;
}

unsigned int CParamManager::GetBarcodePort()
{

	return m_nBarcodePort;
}

const char * CParamManager::GetGraphFile() const
{
	return m_szGraphFile.c_str();
}

const char * CParamManager::GetLabelFile() const
{
	return m_szLabelFile.c_str();
}

const char * CParamManager::GetImageDirectory() const
{
	return m_szImagePath;
}

const char * CParamManager::GetLineName() const
{
	return m_szLineName;
}

const char *CParamManager::GetCameraName() const
{
	return m_szCameraName;
}

const char * CParamManager::GetPatternImagePath() const
{
	return m_szPatternImagePath;
}

const char * CParamManager::GetCacheImagePath() const
{
	return m_szCacheImagePath;
}

size_t CParamManager::GetBarcodeTime() const
{
	return m_nBarcodeTime;
}

const char * CParamManager::GetBarcodeResetParam() const
{
	return m_szBarcodeResetParam;
}

void CParamManager::Init()
{
	FILE *fp = nullptr;
	fopen_s(&fp, "./config.txt", "rb");
	bool ret = true;
	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		size_t length = ftell(fp);
		char *content = new char[length + 1];
		memset(content, 0, sizeof(char) * (length + 1));
		fseek(fp, 0, SEEK_SET);
		fread_s(content, length, 1, length, fp);
		
		/*if (m_pFtp == nullptr)
		{
			m_pFtp = new std::vector<std::wstring>;
		}*/
		/*ret = utils::parseVector(content, "ftpLogin", m_pFtp);
		if(ret == false)
		{
			WriteError("ftp Parameter init Failed");
		}*/
		if (m_pLineCamera == nullptr)
		{
			m_pLineCamera = new std::unordered_map<std::wstring, std::wstring>;
		}
		utils::parseMap(content, "lineCamera", m_pLineCamera);
		if ((m_pLineCamera == nullptr) || (m_pLineCamera->size() == 0))
		{
			WriteError("line Camera init Failed");
		}

		unsigned int tmpLocal = utils::parseIp(content, "serverip");
		if (tmpLocal == 0)
		{
			TRACE0("get ServerIp Failed\n");
		}
		m_nServerIp = tmpLocal;

		tmpLocal = utils::parseIp(content, "barcodeIp");
		if (tmpLocal == 0)
		{
			TRACE0("get barcodeIp Failed\n");
		}
		m_nBarcodeIp = tmpLocal;

		tmpLocal = utils::parseIp(content, "kepServerIp");
		if (tmpLocal == 0)
		{
			TRACE0("get kepServerIp Failed\n");
		}
		m_nKepServerIp = tmpLocal;


		char tmpStr[MAX_CHAR_LENGTH] = { 0 };
		if (utils::getValueByName(content, "serverport", tmpStr) == true)
		{
			m_nServerPort = atoi(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (utils::getValueByName(content, "kepServerPort", tmpStr) == true)
		{
			m_nKepServerPort = atoi(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));


		if (utils::getValueByName(content, "testClientPort", tmpStr) == true)
		{
			m_nTestClientPort = atoi(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (utils::getValueByName(content, "barcodePort", tmpStr) == true)
		{
			m_nBarcodePort = atoi(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (utils::getValueByName(content, "barcodeTimeout", tmpStr) == true)
		{
			m_nBarcodeTime = atoi(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (utils::getValueByName(content, "testServerPort", tmpStr) == true)
		{
			m_nTestServerPort = atoi(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));



		if (utils::getValueByName(content, "graphFile", tmpStr) == true)
		{
			m_szGraphFile = std::string(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (utils::getValueByName(content, "labelFile", tmpStr) == true)
		{
			m_szLabelFile = std::string(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (utils::getValueByName(content, "imagePath", m_szImagePath) != true)
		{
			//////
			WriteError("init config.txt imagePath error");
		}
		//m_szCacheImagePath
		if (utils::getValueByName(content, "cacheImagePath", m_szCacheImagePath) != true)
		{
			//////
			WriteError("init config.txt cacheImagePath error");
		}

		if (utils::getValueByName(content, "lineName", m_szLineName) == false)
		{
			//////
			WriteError("init config.txt lineName error");
		}
		if (utils::getValueByName(content, "cameraName", m_szCameraName) == false)
		{
			//////
			WriteError("init config.txt cameraName error");
		}
		//m_szPatternImagePath
		if (utils::getValueByName(content, "patternImagePath", m_szPatternImagePath) == false)
		{
			//////
			WriteError("init config.txt patternImagePath error");
		}

		//m_szPatternImagePath
		if (utils::getValueByName(content, "barcodeResetParam", m_szBarcodeResetParam) == false)
		{
			//////
			WriteError("init config.txt barcodeResetParam error");
		}

		//WriteInfo("cameraName = %s", m_szCameraName);
		//memset(tmpStr, 0, sizeof(tmpStr));


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
}

