#include "../stdafx.h"
#include "RFIDReader.h"
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include "utils.h"
#include "Log.h"
#include "../xml/tinyxml.h"


CRFIDReader::CRFIDReader():
	m_nDefaultPort(10001),
	m_bDefaultDHCP(false),
	m_nSocket(-1)
{
	init();
}


CRFIDReader::~CRFIDReader()
{
}

bool CRFIDReader::init()
{
	memset(m_szDefaultIp, 0, sizeof(m_szDefaultIp));
	strcpy_s(m_szDefaultIp, "192.168.0.254");
	
	memset(m_szDefaultNetmask, 0, sizeof(m_szDefaultNetmask));
	strcpy_s(m_szDefaultNetmask, "255.255.255.0");

	memset(m_szDefaultGatewap, 0, sizeof(m_szDefaultGatewap));
	strcpy_s(m_szDefaultGatewap, "192.168.0.1");

	return true;
}

bool CRFIDReader::initRFID(unsigned int serverIp, int port)
{
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		err = WSAGetLastError();
		WriteError("err = %u", err);
		return false;
	}
	m_nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_nSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	sockaddr_in addr;

	//接收时限
	int value = 1000;
	setsockopt(m_nSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&value, sizeof(int));

	value = 1;
	setsockopt(m_nSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(int));



	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = htonl(serverIp);

	if (0 != connect(m_nSocket, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		err = WSAGetLastError();
		TRACE1("connect failed,err = %u\n", err);
		WriteError("connect failed, err = %u", err);
		closesocket(m_nSocket);
		m_nSocket = -1;
		WSACleanup();
		return false;
	}

	//size_t tmpLen = send(socketFD, sendMsg, sendMsgLen, 0);// MSG_DONTROUTE);
	//if (tmpLen == SOCKET_ERROR)
	//{
	//	WriteError("send Failed, msg = %s, len = %u, Err:", sendMsg, sendMsgLen, WSAGetLastError());
	//	closesocket(socketFD);
	//	WSACleanup();
	//	recvMsgLen = 0;
	//	return false;
	//}
	//tmpLen = recv(socketFD, recvMsg, recvMsgLen, 0);
	//if (SOCKET_ERROR == tmpLen)
	//{
	//	WriteError("recv Failed, Err: %u", GetLastError());
	//	closesocket(socketFD);
	//	WSACleanup();
	//	recvMsgLen = 0;
	//	return false;
	//}
	//recvMsgLen = tmpLen;
	//closesocket(socketFD);
	//socketFD = -1;

	//WSACleanup();

	return true;
}

bool CRFIDReader::isConnect(SOCKET fd)
{
	/*struct tcp_info info;
	int len = sizeof(info);
	getsockopt(fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
	if ((info.tcpi_state == TCP_ESTABLISHED))
	{

	}
	else
	{

	}*/
	return false;
}

bool CRFIDReader::hostGreetings(SOCKET fd)
{
	char greedyXML[] = "<frame>	\
		<cmd>							\
		<id>100000129</id>		\
		<hostGreetings>			\
		<readerType>SIMATIC_RF640R</readerType>	\
		<readerMode>Run</readerMode>	\
		<supportedVersions>								\
		<version>V1.0</version>			\
		</supportedVersions>							\
		</hostGreetings>								\
		</cmd>				\
		</frame>";

	send(fd, greedyXML, strlen(greedyXML), MSG_WAITALL);


	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);

	int recvLen = 0;
	recv(fd, buffer, recvLen, MSG_WAITALL);


	TiXmlDocument lconfigXML;
	//TiXmlParsingData data;
	lconfigXML.Parse(buffer);
	if (lconfigXML.Error())
	{
		TRACE0("xml Format is invalid\n");
		WriteError("recvBlood = [%s]", buffer);
		return false;
	}
	TiXmlElement *rootElement = lconfigXML.RootElement();

	if ((rootElement = nullptr) || (strncmp(rootElement->Value(), "reply", strlen("reply")) != 0))
	{
		WriteError("recvBlood get root element Failed, %s", buffer);
		return false;
	}
	if (strncmp(rootElement->Attribute("package"), "bloodheart", strlen("bloodheart") != 0))
	{
		WriteError("replay package error");
		return false;
	}
	rootElement->Clear();
	lconfigXML.Clear();



	return false;
}
