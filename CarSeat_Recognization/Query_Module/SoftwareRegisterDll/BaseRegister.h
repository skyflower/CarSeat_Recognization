#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include <IPHlpApi.h>

#ifdef REGISTER_DLL_EXPORTS
#define REGISTER_DLL_API __declspec(dllexport)
#else
#define REGISTER_DLL_API __declspec(dllimport)
#endif

#pragma comment(lib, "Iphlpapi.lib")


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

char *getDiskID();
bool GetMACaddress(char *macAddress, int len);


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
	REGISTER_DLL_API char *getDeviceMachineCode();

	REGISTER_DLL_API  CBaseRegister *createRegisterManager();

	REGISTER_DLL_API void destroyRegisterManager(CBaseRegister* pRegister);
}