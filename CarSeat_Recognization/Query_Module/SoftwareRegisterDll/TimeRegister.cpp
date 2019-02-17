#include "TimeRegister.h"
#include <iostream>


CTimeRegister::CTimeRegister()
{
}


CTimeRegister::~CTimeRegister()
{
}


bool CTimeRegister::CheckRegisterCode(char *crypto, int length)
{

	return true;
}

char *CTimeRegister::GenerateRegisterCode(registerNode * code, int *len)
{
	/*struct registerNode
	{
		char m_szMachineCode[200];
		char m_szBeginTime[20];
		char m_szEndTime[20];
		char m_szRandomKey[20];
		unsigned long long m_nRegisterLimit;
	};*/
	if ((code == nullptr) || (len == nullptr))
	{
		return nullptr;
	}

	char *byte = (char *)code;
	char *buffer = new char[sizeof(registerNode)];
	
	std::memset(buffer, 0, sizeof(char) * sizeof(registerNode));
	for (size_t i = 0; i < sizeof(registerNode) - 1; ++i)
	{
		buffer[i] = byte[i] ^ byte[i + 1];
	}
	buffer[sizeof(registerNode) - 1] = byte[sizeof(registerNode) - 1];
	*len = sizeof(registerNode);

	return buffer;
}