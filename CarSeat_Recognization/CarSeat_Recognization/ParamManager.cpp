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

void CParamManager::Init()
{
	std::function<bool(char *, size_t, std::vector<std::wstring>*)> lambda = \
		[&](char *pContent, size_t length, std::vector<std::wstring> *pData)->bool
	{
		if ((pContent == nullptr) || (length < 1))
		{
			return false;
		}
		
		char *p = pContent;
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
	};
	FILE *fp = nullptr;
	fopen_s(&fp, "./config.txt", "r");
	if (fp != nullptr)
	{
		fseek(fp, 0, SEEK_END);
		size_t length = ftell(fp);
		char *content = new char[length + 1];
		memset(content, 0, sizeof(char) * (length + 1));
		fread_s(content, length, 1, length, fp);
		char *p = strstr(content, "color");
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
				if(lambda(line + 1, end - line - 1, m_pColor) == true)
				{
					for (auto &k : *m_pColor)
					{
						TRACE1("color = %s", k);
					}
				}
			}
		}
	}
}


CParamManager::CParamManager():m_pColor(nullptr),
m_pOutline(nullptr),
m_pTexture(nullptr),
m_nLocalIp(-1),
m_nServerIp(-1)
{
	Init();
}


CParamManager::~CParamManager()
{
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
