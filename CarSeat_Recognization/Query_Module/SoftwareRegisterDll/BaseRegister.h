#pragma once

#ifdef REDISTER_DLL_EXPORTS
#define REDISTER_DLL_API __declspec(dllexport)
#else
#define REDISTER_DLL_API __declspec(dllimport)
#endif

enum registerType
{
	SINGLE_DEVICE,
	MULTI_DEVICE
};

struct registerNode
{
	unsigned int m_nVersion;
	char m_szMachineCode[200];
	unsigned int m_nBeginTime;
	unsigned int m_szEndTime;
	char m_szRandomKey[20];
	registerType m_type;
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