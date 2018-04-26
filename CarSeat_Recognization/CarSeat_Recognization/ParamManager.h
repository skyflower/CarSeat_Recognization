#pragma once
#include <iostream>
#include <vector>

class CParamManager
{
public:
	CParamManager();
	~CParamManager();

	static CParamManager* GetInstance();

private:

	static CParamManager *m_pInstance;

	void Init();

	char* WcharToChar(wchar_t* wc);
	wchar_t* CharToWchar(char* c);
	std::wstring StrToWStr(const std::string str);
	std::string WStrToStr(const std::wstring wstr);


	size_t m_nServerIp;
	size_t m_nLocalIp;
	std::vector<std::wstring> *m_pColor;
	std::vector<std::wstring> *m_pOutline;
	std::vector<std::wstring> *m_pTexture;
};

