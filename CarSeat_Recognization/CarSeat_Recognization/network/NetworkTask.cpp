#include "../stdafx.h"
#include "NetworkTask.h"
#include "../common/ParamManager.h"
#include "../common/Log.h"
#include <chrono>
#include <ctime>
#include <Iphlpapi.h>
#include <IcmpAPI.h>
//#include <time.h>

CNetworkTask::CNetworkTask()
{
	m_pParamManager = CParamManager::GetInstance();
}


CNetworkTask::~CNetworkTask()
{
}

bool CNetworkTask::IsReachable(unsigned int clientIp, unsigned int serverIp)
{
	HANDLE IcmpHandle = IcmpCreateFile();
	char reply[200] = { 0 };
	PIP_OPTION_INFORMATION info = NULL;
	WriteInfo("icmp echo clientIp = 0x%X, serverIp = 0x%X", clientIp, serverIp);
	
	if (0 == IcmpSendEcho2Ex(IcmpHandle, NULL, NULL, NULL, \
		htonl(clientIp), htonl(serverIp), "icmp", strlen("icmp"), \
		info, reply, sizeof(reply), 100))
	{
		WriteError("icmpSendEcho failed clientIp = 0x%X, ServerIp = 0x%X", clientIp, serverIp);
		IcmpCloseHandle(IcmpHandle);
		WSACleanup();
		return false;
	}
	
	IcmpCloseHandle(IcmpHandle);
	return true;
}

bool CNetworkTask::heartBlood(unsigned int serverIp, unsigned int port)
{
	unsigned int localIp = m_pParamManager->GetLocalIP();
	if ((localIp == 0) || (localIp == -1))
	{
		return false;
	}

	char *str = "<?xml version=\"1.0\" encoding=\"gbk\"?>	\
		<bloodheart ip=\"%s\"  status=\"%s\" pcName=\"%s\" time =\"%s\"/>";

	char ipLocalStr[20] = { 0 };
	snprintf(ipLocalStr, sizeof(ipLocalStr), "%d.%d.%d.%d", \
		((localIp & 0xFF000000) >> 24), ((localIp & 0xFF0000) >> 16), \
		((localIp & 0xFF00) >> 8), (localIp & 0xFF));

	char bloodheart[200];
	memset(bloodheart, 0, sizeof(bloodheart));

	std::chrono::system_clock::time_point curPoint = std::chrono::system_clock::now();
	
	std::time_t t = std::time(nullptr);
	std::tm time;
	localtime_s(&time, &t);
	char timeStr[20] = { 0 };
	snprintf(timeStr, sizeof(timeStr), "%04d%02d%02d-%02d%02d%02d", \
		time.tm_year, time.tm_mon, time.tm_mday,		\
		time.tm_hour, time.tm_min, time.tm_sec);
	
	snprintf(bloodheart, sizeof(bloodheart), str, ipLocalStr, "success", m_pParamManager->GetLocalName().c_str(), timeStr);


	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		err = WSAGetLastError();
		WriteError("err = %u", err);
		return false;
	}
	SOCKET socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFD == -1)
	{
		TRACE("create socket failed\n");
		WSACleanup();
		return false;
	}

	sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = htonl(serverIp);

	if (0 != connect(socketFD, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		err = WSAGetLastError();
		TRACE1("connect failed,err = %u\n", err);
		WriteError("connect failed, err = %u", err);
		WSACleanup();
		closesocket(socketFD);
		return false;
	}

	send(socketFD, bloodheart, strlen(bloodheart) + 1, MSG_DONTROUTE);
	char tmpRecv[1024] = { 0 };
	recv(socketFD, tmpRecv, sizeof(tmpRecv), MSG_PEEK);
	TRACE1("link test recv = %s\n", tmpRecv);
	closesocket(socketFD);
	socketFD = -1;

	WSACleanup();

	return true;
}

void CNetworkTask::operator()()
{

}
