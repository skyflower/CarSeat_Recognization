#include "stdafx.h"
#include "ParamManager.h"
#include <fstream>
#include <functional>


CParamManager *CParamManager::m_pInstance = nullptr;

CParamManager* CParamManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CParamManager();
	}
	return m_pInstance;
}

int CParamManager::GetLocalIP()
{
	WORD wVersionRequested = MAKEWORD(2, 2);

	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		return 0;
	}
	char local[255] = { 0 };
	gethostname(local, sizeof(local));
	hostent* ph = gethostbyname(local);
	if (ph == NULL)
	{
		return 0;
	}
	in_addr addr;
	memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr)); // 这里仅获取第一个ip  

	std::string localIP;
	localIP.assign(inet_ntoa(addr));

	WSACleanup();

	TRACE1("localIp = %s", localIP.c_str());
	unsigned int one = 0;
	unsigned int two = 0;
	unsigned int three = 0;
	unsigned int four = 0;
	sscanf_s(localIP.c_str(), "%d.%d.%d.%d", one, two, three, four);
	unsigned int tmpServerIp = (one << 24) | (two << 16) | (three < 8) | four;

	return tmpServerIp;
}

void CParamManager::Init()
{
	FILE *fp = nullptr;
	fopen_s(&fp, "./config.txt", "r");
	bool ret = true;
	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		size_t length = ftell(fp);
		char *content = new char[length + 1];
		memset(content, 0, sizeof(char) * (length + 1));
		fread_s(content, length, 1, length, fp);
		char *p = strstr(content, "color");
		if (m_pColor == nullptr)
		{
			m_pColor = new std::vector<std::wstring>;
		}
		ret = parseVector(content, "color", m_pColor);
		if (ret == false)
		{
			TRACE0("init color Failed");
		}
		if (m_pOutline == nullptr)
		{
			m_pOutline = new std::vector<std::wstring>;
		}
		ret = parseVector(content, "outline", m_pOutline);
		if (ret == false)
		{
			TRACE0("out line init Failed");
		}
		if (m_pTexture == nullptr)
		{
			m_pTexture = new std::vector<std::wstring>;
		}
		ret = parseVector(content, "texture", m_pTexture);
		if (ret == false)
		{
			TRACE0("texture init Failed");
		}
	}
}

bool CParamManager::parseVector(const char *content, const char * name, std::vector<std::wstring>* pVector)
{
	if ((content == nullptr) || (name == nullptr) || (pVector == nullptr))
	{
		return false;
	}
	char *p = const_cast<char*>(strstr(content, name));
	if (p == NULL)
	{
		char *line = strchr(p, '=');
		char *end = strchr(p, '\n');
		if ((line != NULL) && (end != NULL))
		{
			if (m_pColor == nullptr)
			{
				m_pColor = new std::vector<std::wstring>;
			}
			if (parseLineSegment(line + 1, end - line - 1, m_pColor) == true)
			{
				for (auto &k : *m_pColor)
				{
					TRACE1("color = %s", k);
				}
			}
		}
	}
	return false;
}

bool CParamManager::parseLineSegment(const char * pContent, size_t length, std::vector<std::wstring>* pData)
{

	if ((pContent == nullptr) || (length < 1) || (pData == nullptr))
	{
		return false;
	}

	char *p = const_cast<char*>(pContent);
	char tmpStr[100];
	while (p < pContent + length)
	{
		char* begin = strchr(p, '"');
		if (begin == NULL)
		{
			break;
		}
		char* end = strchr(begin + 1, '"');
		if (end == NULL)
		{
			break;
		}
		p = end + 1;
		if (end - begin - 1 != 0)
		{
			memset(tmpStr, 0, sizeof(tmpStr));
			memcpy(tmpStr, begin + 1, end - begin - 1);
			wchar_t *wchar = CharToWchar(tmpStr);
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

int CParamManager::parseServerIp(const char * content, const char * name)
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
	sscanf_s(str, "%d.%d.%d.%d", one, two, three, four);
	unsigned int tmpServerIp = (one << 24) | (two << 16) | (three < 8) | four;

	return tmpServerIp;
}


CParamManager::CParamManager() :m_pColor(nullptr),
m_pOutline(nullptr),
m_pTexture(nullptr),
m_nLocalIp(-1),
m_nServerIp(-1)
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
}

char* CParamManager::WcharToChar(wchar_t* wc)
{

	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	char *m_char = new char[len + 1];
	memset(m_char, 0, sizeof(char) * (len + 1));

	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
wchar_t* CParamManager::CharToWchar(char* c)
{

	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	wchar_t *m_wchar = new wchar_t[len + 1];
	memset(m_wchar, 0, sizeof(wchar_t) * (len + 1));

	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}
std::wstring CParamManager::StrToWStr(const std::string str)
{
	//wchar_t* CharToWchar(char* c);
	wchar_t *pWChar = CharToWchar(const_cast<char*>(str.c_str()));
	std::wstring tmpStr(pWChar);
	delete[]pWChar;
	pWChar = nullptr;
	return tmpStr;
}
std::string CParamManager::WStrToStr(const std::wstring wstr)
{
	char *pChar = WcharToChar(const_cast<wchar_t*>(wstr.c_str()));

	std::string tmpStr(pChar);
	delete[]pChar;
	pChar = nullptr;

	return tmpStr;
}
