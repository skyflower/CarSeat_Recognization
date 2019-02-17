#include "BaseRegister.h"
#include "TimeRegister.h"


CBaseRegister::CBaseRegister()
{
}


CBaseRegister::~CBaseRegister()
{
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
