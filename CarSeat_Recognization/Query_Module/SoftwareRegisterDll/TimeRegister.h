#pragma once
#include "BaseRegister.h"
class CTimeRegister :
	public CBaseRegister
{
public:
	CTimeRegister();
	~CTimeRegister();

	virtual bool CheckRegisterCode(char *crypto, int length);
	virtual char *GenerateRegisterCode(registerNode * code, int *len);
};

