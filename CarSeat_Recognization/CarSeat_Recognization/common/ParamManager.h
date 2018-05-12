#pragma once
#include <iostream>
#include <vector>

class CParamManager
{
public:
	CParamManager();
	~CParamManager();

	static CParamManager* GetInstance();

	unsigned int GetLocalIP();

	int GetServerIP();
	int GetServerPort();
	int GetTestServerPort();
	int GetTestClientPort();

	std::wstring GetLocalName() { return m_strLocalName; }


private:

	static CParamManager *m_pInstance;

	void Init();
	bool parseVector(const char *content, const char*name, std::vector<std::wstring> *pVector);
	bool parseLineSegment(const char *name, size_t, std::vector<std::wstring> *pVec);

	int parseServerIp(const char *content, const char *name);
	bool getValueByName(const char *content, const char*name, char*value);

	unsigned int __auxLocalIP();

	size_t m_nServerIp;
	int m_nServerPort;
	size_t m_nLocalIp;

	int m_nTestServerPort;
	int m_nTestClientPort;

	std::wstring m_strLocalName;
	std::vector<std::wstring> *m_pColor;
	std::vector<std::wstring> *m_pOutline;
	std::vector<std::wstring> *m_pTexture;
};

