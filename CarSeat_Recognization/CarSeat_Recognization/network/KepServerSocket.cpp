#include "../stdafx.h"
#include "KepServerSocket.h"
#include "../common/Log.h"
#include "../xml/tinyxml.h"
#include <ctime>



CKepServerSocket::CKepServerSocket(unsigned int ip, unsigned int port):
	m_nIp(ip),
	m_nPort(port),
	m_nSocket(-1)
{
	//m_nSocket = initSocket(ip, port);
}


CKepServerSocket::~CKepServerSocket()
{
	if (m_nSocket != 0)
	{
		closesocket(m_nSocket);
		m_nSocket = 0;
	}
	WSACleanup();
}

bool CKepServerSocket::SetError()
{
	return SetValue(-1);
}

bool CKepServerSocket::SetWarning()
{
	return SetValue(1);
}

bool CKepServerSocket::SetCorrect()
{
	return SetValue(0);
}

void CKepServerSocket::HeartBlood()
{
	static clock_t preTime = clock();
	if (clock() > preTime + 20 * CLOCKS_PER_SEC)
	{
		SetValue(INT_MAX);
		preTime = clock();
	}
	
}

bool CKepServerSocket::SetValue(int value)
{
	char tmpXml[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>	\
		<write>%d</write>";

	const size_t XmlLength = 200;

	char valueXml[XmlLength];
	memset(valueXml, 0, sizeof(valueXml));

	sprintf_s(valueXml, sizeof(valueXml), tmpXml, value);

	char recvXml[XmlLength] = { 0 };
	memset(recvXml, 0, sizeof(recvXml));
	int length = XmlLength;
	int i = 0;
	bool ret = false;
	while (i < 5)
	{
		ret = SendMessageToServer(valueXml, strlen(valueXml), recvXml, length);
		if (ret == true)
		{
			break;
		}
		++i;
	}
	if (ret == true)
	{
		// 解析服务器返回xml
		TiXmlDocument lconfigXML;
		//TiXmlParsingData data;
		lconfigXML.Parse(recvXml);
		if (lconfigXML.Error())
		{
			WriteError("recvBlood = [%s]", recvXml);
			return false;
		}
		TiXmlElement *rootElement = lconfigXML.RootElement();

		if ((rootElement = nullptr) || (strncmp(rootElement->Value(), "reply", strlen("reply")) != 0))
		{
			WriteError("recvBlood get root element Failed, %s", recvXml);
			if (rootElement != nullptr)
			{
				rootElement->Clear();
			}
			lconfigXML.Clear();
			return false;
		}
		if (strncmp(rootElement->GetText(), "ok", strlen("ok")) == 0)
		{
			ret = true;
		}
		rootElement->Clear();
		lconfigXML.Clear();
		return ret;
	}
	return false;
}

bool CKepServerSocket::SendMessageToServer(char * msg, int len, char * recvBuffer, int &recvLen)
{
	if (m_nSocket == 0)
	{
		resetConnect();
	}
	if (m_nSocket == 0)
	{
		return false;
	}
	size_t tmpLen = send(m_nSocket, msg, len, 0);// MSG_DONTROUTE);
	if (tmpLen == SOCKET_ERROR)
	{
		WriteError("send Failed, msg = %s, len = %u, Err:%d", msg, len, WSAGetLastError());
		closesocket(m_nSocket);
		WSACleanup();
		return false;
	}
	int recvMsgLen = 0;
	tmpLen = recv(m_nSocket, recvBuffer, recvMsgLen, 0);
	if (SOCKET_ERROR == tmpLen)
	{
		WriteError("recv Failed, Err: %u", GetLastError());
		closesocket(m_nSocket);
		WSACleanup();
		recvMsgLen = 0;
		return false;
	}
	recvLen = recvMsgLen;
	return true;
}

void CKepServerSocket::resetConnect()
{
	m_nSocket = initSocket(m_nIp, m_nPort);
}

bool CKepServerSocket::GetSocketStatus()
{
	if (m_nSocket == -1)
	{
		return false;
	}
	return true;
}

SOCKET CKepServerSocket::initSocket(unsigned int ip, unsigned int port)
{
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		err = WSAGetLastError();
		WriteError("err = %u", err);
		return -1;
	}
	SOCKET socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFD == -1)
	{
		WSACleanup();
		return -1;
	}

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr));

	//接收时限
	int nNetTimeout = 1000;
	setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = htonl(ip);

	if (0 != connect(socketFD, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		err = WSAGetLastError();
		
		WriteError("connect failed, err = %d", err);
		closesocket(socketFD);
		WSACleanup();
		return -1;
	}
	return socketFD;
}