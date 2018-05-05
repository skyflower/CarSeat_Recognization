#pragma once
class CNetwrokTask
{
public:
	CNetwrokTask();
	~CNetwrokTask();
	bool IsReachable(unsigned int clientIp, unsigned int serverIp);
	bool heartBlood(unsigned int serverIp);


};

