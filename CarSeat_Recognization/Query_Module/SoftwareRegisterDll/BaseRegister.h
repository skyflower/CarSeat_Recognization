#pragma once

#ifdef REDISTER_DLL_EXPORTS
#define REDISTER_DLL_API __declspec(dllexport)
#else
#define REDISTER_DLL_API __declspec(dllimport)
#endif

struct registerNode
{
	char m_szMachineCode[200];
	char m_szBeginTime[20];
	char m_szEndTime[20];
	char m_szRandomKey[20];
	unsigned long long m_nRegisterLimit;
};


class CBaseRegister
{
public:
	CBaseRegister();
	~CBaseRegister();
	virtual bool CheckRegisterCode(char *crypto, int length) = 0;
	virtual char *GenerateRegisterCode(registerNode * code, int *len) = 0;
};

extern "C"
{
	REDISTER_DLL_API  CBaseRegister *createRegisterManager();
	REDISTER_DLL_API void destroyRegisterManager(CBaseRegister* pRegister);
}