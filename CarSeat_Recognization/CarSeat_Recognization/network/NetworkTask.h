#pragma once

#include <iostream>
#include <Windows.h>
#include <winsock.h>
#include "../common/ParamManager.h"

class CNetworkTask
{
public:
	CNetworkTask();
	~CNetworkTask();
	static bool IsReachable(unsigned int clientIp, unsigned int serverIp);
	bool heartBlood(unsigned int serverIp, unsigned int port);
	void operator()();

private:
	CParamManager *m_pParamManager;
	
};

