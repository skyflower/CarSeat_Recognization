#include "BaseRegister.h"
#include "TimeRegister.h"


CBaseRegister::CBaseRegister()
{
}


CBaseRegister::~CBaseRegister()
{
}

extern "C" char * getDeviceMachineCode()
{
	char szMacAddress[100];
	memset(szMacAddress, 0, sizeof(szMacAddress));
	GetMACaddress(szMacAddress, sizeof(szMacAddress));

	char *szDiskID = getDiskID();
	static char tmpCode[1024];
	memset(tmpCode, 0, sizeof(tmpCode));
	sprintf_s(tmpCode, sizeof(tmpCode) / sizeof(tmpCode[0]), "%s%s", szMacAddress, szDiskID);

	return tmpCode;
}

CBaseRegister * createRegisterManager()
{
	return new CTimeRegister();
}

void destroyRegisterManager(CBaseRegister * pRegister)
{
	if (pRegister != nullptr)
	{
		delete pRegister;
		pRegister = nullptr;
	}
}


bool GetMACaddress(char *macAddress, int len)
{
	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo);

	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (dwStatus != ERROR_SUCCESS)
		return false;

	sprintf_s(macAddress, len, "%02X%02X%02X%02X%02X%02X", AdapterInfo->Address[0],
		AdapterInfo->Address[1], AdapterInfo->Address[2], AdapterInfo->Address[3],
		AdapterInfo->Address[4], AdapterInfo->Address[5]);
	return true;
}

char *getDiskID()
{
	try
	{
		static char id[1024];
		wchar_t Name[MAX_PATH];
		DWORD serno;
		DWORD length;
		DWORD FileFlag;
		wchar_t FileName[MAX_PATH];
		BOOL Ret;
		Ret = GetVolumeInformation(L"C:\\", Name, MAX_PATH, &serno, &length, &FileFlag, FileName, MAX_PATH);
		if (Ret)
		{
			sprintf_s(id, "%08X", serno);
		}
		return id;
	}
	catch (...)
	{
		return "";
	}
}


