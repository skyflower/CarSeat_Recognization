#include "../stdafx.h"
#include "ParamManager.h"
#include <fstream>
#include "Log.h"
#include "utils.h"
#include <functional>
#include "../network/NetworkTask.h"


CParamManager *CParamManager::m_pInstance = nullptr;

CParamManager::CParamManager() :
m_pColor(nullptr),
m_pOutline(nullptr),
m_pTexture(nullptr),
m_pFtp(nullptr),
m_pLineCamera(nullptr),
m_nLocalIp(-1),
m_nServerIp(-1),
m_nServerPort(-1),
m_nTestClientPort(-1),
m_nTestServerPort(-1),
m_nBarcodeIp(-1),
m_nBarcodePort(-1)
{
	Init();
}


CParamManager::~CParamManager()
{
	if (m_pColor != nullptr)
	{
		m_pColor->clear();
		delete m_pColor;
		m_pColor = nullptr;
	}
	if (m_pTexture != nullptr)
	{
		m_pTexture->clear();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	if (m_pOutline != nullptr)
	{
		m_pOutline->clear();
		delete m_pOutline;
		m_pOutline = nullptr;
	}
	if (m_pFtp != nullptr)
	{
		m_pFtp->clear();
		delete m_pFtp;
		m_pFtp = nullptr;
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

	WSACleanup();
	clock_t endTime = clock();
	TRACE1("get localIp cost Time = %u", endTime - startTime);

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

std::vector<std::wstring>* CParamManager::GetFtpParameter()
{
	return m_pFtp;
}

std::vector<std::wstring>* CParamManager::GetOutlineParameter()
{
	return m_pOutline;
}

std::vector<std::wstring>* CParamManager::GetTextureParameter()
{
	return m_pTexture;
}

std::vector<std::wstring>* CParamManager::GetColorParameter()
{
	return m_pColor;
}

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
		
		if (m_pColor == nullptr)
		{
			m_pColor = new std::vector<std::wstring>;
		}
		ret = parseVector(content, "color", m_pColor);
		if (ret == false)
		{
			TRACE0("init color Failed\n");
		}
		if (m_pOutline == nullptr)
		{
			m_pOutline = new std::vector<std::wstring>;
		}
		ret = parseVector(content, "outline", m_pOutline);
		if (ret == false)
		{
			TRACE0("out line init Failed\n");
		}
		if (m_pTexture == nullptr)
		{
			m_pTexture = new std::vector<std::wstring>;
		}
		ret = parseVector(content, "texture", m_pTexture);
		if (ret == false)
		{
			TRACE0("texture init Failed\n");
		}
		if (m_pFtp == nullptr)
		{
			m_pFtp = new std::vector<std::wstring>;
		}
		ret = parseVector(content, "ftpLogin", m_pFtp);
		if(ret == false)
		{
			WriteError("ftp Parameter init Failed");
		}
		if (m_pLineCamera == nullptr)
		{
			m_pLineCamera = new std::unordered_map<std::wstring, std::wstring>;
		}
		ret = parseMap(content, "lineCamera", m_pLineCamera);
		if (ret == false)
		{
			WriteError("line Camera init Failed");
		}

		unsigned int tmpLocal = parseIp(content, "serverip");
		if (tmpLocal == 0)
		{
			TRACE0("get ServerIp Failed\n");
		}
		m_nServerIp = tmpLocal;

		tmpLocal = parseIp(content, "barcodeIp");
		if (tmpLocal == 0)
		{
			TRACE0("get barcodeIp Failed\n");
		}
		m_nBarcodeIp = tmpLocal;

		tmpLocal = parseIp(content, "barcodePort");
		if (tmpLocal == 0)
		{
			TRACE0("get barcodeIp Failed\n");
		}
		m_nBarcodePort = tmpLocal;


		char tmpStr[20] = { 0 };
		if (getValueByName(content, "serverport", tmpStr) == true)
		{
			m_nServerPort = atoi(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));
		if (getValueByName(content, "testClientPort", tmpStr) == true)
		{
			m_nTestClientPort = atoi(tmpStr);
		}
		memset(tmpStr, 0, sizeof(tmpStr));

		if (getValueByName(content, "testServerPort", tmpStr) == true)
		{
			m_nTestServerPort = atoi(tmpStr);
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
}

bool CParamManager::parseVector(const char *content, const char * name, std::vector<std::wstring>* pVector)
{
	if ((content == nullptr) || (name == nullptr) || (pVector == nullptr))
	{
		return false;
	}
	char *p = const_cast<char*>(strstr(content, name));
	if (p != NULL)
	{
		char *line = strchr(p, '=');
		char *end = strchr(p, '\n');
		if ((line != NULL) && (end != NULL))
		{

#if _DEBUG

			char tmpStr[100] = { 0 };
			memcpy(tmpStr, line + 1, end - line - 1);
			//setlocale(LC_ALL, "chs");
			TRACE2("name = %s, tmpStr = %s\n", name, tmpStr);

#endif
			if (parseLineSegment(line + 1, end - line - 1, pVector) == true)
			{
#if _DEBUG
				for (auto &k : *pVector)
				{
					TRACE2("name = %s, value = %s\n", name, k);
				}
#endif // _DEBUG

				
			}
		}
	}
	return true;
}

bool CParamManager::parseLineSegment(const char * pContent, size_t length, std::vector<std::wstring>* pData)
{
	if ((pContent == nullptr) || (length < 1) || (pData == nullptr))
	{
		return false;
	}

	char *p = const_cast<char*>(pContent);
	char tmpStr[100];
	char c = '\"';
	while (p < pContent + length)
	{
		char* begin = strchr(p, c);
		if ((begin == NULL) || (begin >= pContent + length))
		{
			break;
		}
		char* end = strchr(begin + 1, c);
		if ((end == NULL) || (end >= pContent + length))
		{
			break;
		}
		p = end + 1;
		if (end - begin - 1 != 0)
		{
			memset(tmpStr, 0, sizeof(tmpStr));
			memcpy(tmpStr, begin + 1, sizeof(char)*(end - begin - 1));
			wchar_t *wchar = utils::CharToWchar(tmpStr);
			if (wchar == nullptr)
			{
				continue;
			}
			std::wstring wStr(wchar);
			pData->push_back(wStr);
			if (wchar != nullptr)
			{
				delete[]wchar;
				wchar = nullptr;
			}
		}
	}
	return true;
}

int CParamManager::parseIp(const char * content, const char * name)
{
	if ((content == nullptr) || (name == nullptr))
	{
		return 0;
	}
	char *p = const_cast<char*>(strstr(content, name));
	if (p == nullptr)
	{
		return 0;
	}
	const char *quote = strchr(content, '=');
	if (quote == nullptr)
	{
		return 0;
	}
	const char *endline = strchr(quote + 1, '\n');
	char str[100];
	memset(str, 0, sizeof(str));
	memcpy(str, quote + 1, endline - quote - 1);
	TRACE1("ServerIp = %s", str);
	unsigned int one = 0;
	unsigned int two = 0;
	unsigned int three = 0;
	unsigned int four = 0;
	sscanf_s(str, "%d.%d.%d.%d", &one, &two, &three, &four);
	unsigned int tmpServerIp = (one << 24) | (two << 16) | (three << 8) | four;

	return tmpServerIp;
}

int CParamManager::parseMap(const char * content, const char * name, std::unordered_map<std::wstring, std::wstring>* pMap)
{
	if ((content == nullptr) || (name == nullptr) || (pMap == nullptr))
	{
		return 0;
	}
	const char *p = strstr(content, name);
	if (p == nullptr)
	{
		return 0;
	}
	const char *endLine = strchr(p, '\n');
	if (endLine == nullptr)
	{
		return 0;
	}
	char tmpStr[20] = { 0 };
	while (p < endLine)
	{
		const char *first = strchr(p, '\"');
		if ((first > endLine) || (first == nullptr))
		{
			return pMap->size();
		}
		const char *second = strchr(first + 1, '\"');
		if ((second > endLine) || (second == nullptr))
		{
			return pMap->size();
		}
		memset(tmpStr, 0, sizeof(tmpStr));
		memcpy(tmpStr, first, second - first);
		std::string keyChar(tmpStr);
		std::wstring keyWChar = utils::StrToWStr(keyChar);

		p = second + 1;

		first = strchr(p, '\"');
		if ((first > endLine) || (first == nullptr))
		{
			return pMap->size();
		}
		second = strchr(first + 1, '\"');
		if ((second > endLine) || (second == nullptr))
		{
			return pMap->size();
		}
		memset(tmpStr, 0, sizeof(tmpStr));
		memcpy(tmpStr, first, second - first);
		std::string valueChar(tmpStr);
		std::wstring valueWChar = utils::StrToWStr(valueChar);

		pMap->insert(std::make_pair(keyWChar, valueWChar));

		p = second + 1;

	}


	return pMap->size();
}

bool CParamManager::getValueByName(const char *content, const char * name, char * value)
{
	if ((name == nullptr) || (value == nullptr) || (content == nullptr))
	{
		return false;
	}

	char *p = strstr(const_cast<char*>(content), name);
	if (p == NULL)
	{
		return false;
	}
	char *lineEnd = strstr(p, "\n");
	if (lineEnd == NULL)
	{
		return false;
	}
	char *begin = strstr(p + 1, "=");
	if ((begin == NULL) || (begin >= lineEnd))
	{
		return false;
	}
	memcpy_s(value, MAX_CHAR_LENGTH, begin + 1, lineEnd - begin - 1);

	return true;
}


