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

	unsigned int GetServerIP();
	unsigned int GetServerPort();
	unsigned int GetTestServerPort();
	unsigned int GetTestClientPort();

	std::vector<std::wstring> *GetFtpParameter();
	std::vector<std::wstring> *GetOutlineParameter();
	std::vector<std::wstring> *GetTextureParameter();
	std::vector<std::wstring> *GetColorParameter();
	unsigned int GetBarcodeIp();

	std::wstring GetLocalName() { return m_strLocalName; }


private:

	static CParamManager *m_pInstance;

	void Init();
	bool parseVector(const char *content, const char*name, std::vector<std::wstring> *pVector);
	bool parseLineSegment(const char *name, size_t, std::vector<std::wstring> *pVec);

	int parseIp(const char *content, const char *name);
	bool getValueByName(const char *content, const char*name, char*value);

	unsigned int __auxLocalIP();

	size_t m_nServerIp;
	int m_nServerPort;
	size_t m_nLocalIp;

	int m_nTestServerPort;
	int m_nTestClientPort;

	size_t m_nBarcodeIp;

	std::wstring m_strLocalName;
	std::vector<std::wstring> *m_pColor;
	std::vector<std::wstring> *m_pOutline;
	std::vector<std::wstring> *m_pTexture;
	std::vector<std::wstring> *m_pFtp;
};

