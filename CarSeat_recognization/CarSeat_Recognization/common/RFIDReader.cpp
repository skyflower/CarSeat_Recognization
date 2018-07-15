#include "../stdafx.h"
#include "RFIDReader.h"
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include "utils.h"
#include "Log.h"
#include "../xml/tinyxml.h"
#include <stack>


CRFIDReader::CRFIDReader():
	m_nDefaultPort(10001),
	m_bDefaultDHCP(false),
	m_nSocket(-1),
	m_nSessionID(1000)
{
	init();
	
}


CRFIDReader::~CRFIDReader()
{
	if (m_nSocket == -1)
	{
		closesocket(m_nSocket);
		WSACleanup();
	}
}

std::wstring CRFIDReader::readBarcode()
{
	return std::wstring();
}

bool CRFIDReader::init()
{
	memset(m_szDefaultIp, 0, sizeof(m_szDefaultIp));
	strcpy_s(m_szDefaultIp, "192.168.0.254");
	
	memset(m_szDefaultNetmask, 0, sizeof(m_szDefaultNetmask));
	strcpy_s(m_szDefaultNetmask, "255.255.255.0");

	memset(m_szDefaultGatewap, 0, sizeof(m_szDefaultGatewap));
	strcpy_s(m_szDefaultGatewap, "192.168.0.1");

	memset(m_szVersion, 0, sizeof(m_szVersion));
	strcpy_s(m_szVersion, "V1.0");

	memset(m_szReadType, 0, sizeof(m_szReadType));
	strcpy_s(m_szReadType, "SIMATIC_RF640R");

	memset(m_szReadMode, 0, sizeof(m_szReadMode));
	strcpy_s(m_szReadMode, "Run");

	return true;
}

CRFIDReader::ErrorType CRFIDReader::initRFID(unsigned int serverIp, int port)
{
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		err = WSAGetLastError();
		WriteError("err = %u", err);
		return ErrorType::ERROR_SOCKET_INIT;
	}
	m_nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_nSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return ErrorType::ERROR_SOCKET_CREATE;
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
		return ErrorType::ERROR_SOCKET_CONNECT;
	}

	return ErrorType::ERROR_OK;
}

CRFIDReader::ErrorType CRFIDReader::isConnect(SOCKET fd)
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
	return ErrorType::ERROR_OK;
}

CRFIDReader::ErrorType CRFIDReader::hostGreetings(SOCKET fd)
{
	char greedyXML[] = "<frame>	\
		<cmd>							\
		<id>%d</id>		\
		<hostGreetings>			\
		<readerType>%s</readerType>	\
		<readerMode>%s</readerMode>	\
		<supportedVersions>								\
		<version>%s</version>			\
		</supportedVersions>							\
		</hostGreetings>								\
		</cmd>				\
		</frame>";


	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);

	sprintf_s(buffer, length, greedyXML, m_nSessionID, m_szReadType, m_szReadMode, m_szVersion);

	ErrorType ret = __communicate(fd, buffer, length);
	if (ret != ErrorType::ERROR_OK)
	{
		delete[]buffer;
		buffer = nullptr;
		WriteError("communicate err = %d", ret);
		return ret;
	}

	int reID = 0;
	int reCode = -1;
	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);

	delete[]buffer;
	buffer = nullptr;

	if ((reID == m_nSessionID) && (reCode == 0))
	{
		return ErrorType::ERROR_OK;
	}
	return ret;
}

CRFIDReader::ErrorType CRFIDReader::hostGoodbye(SOCKET fd)
{
	char goodbye[] = "<frame>							\
		<cmd>											\
		<id>%d</id>										\
		<hostGoodbye>									\
		<readerMode>%s</readerMode>						\
		</hostGoodbye>									\
		</cmd>											\
		</frame>";

	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);
	sprintf_s(buffer, length, goodbye, m_nSessionID, m_szReadMode);

	ErrorType ret = __communicate(fd, buffer, length);
	if (ret != ErrorType::ERROR_OK)
	{
		delete[]buffer;
		buffer = nullptr;
		WriteError("communicate err = %d", ret);
		return ret;
	}

	int reID = 0;
	int reCode = -1;
	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);

	delete[]buffer;
	buffer = nullptr;

	if ((reID == m_nSessionID) && (reCode == 0))
	{
		return ErrorType::ERROR_OK;
	}
	return ret;
}

CRFIDReader::ErrorType CRFIDReader::startRead(SOCKET fd)
{
	char startXML[] = "<frame>		\
		<cmd>						\
		<id>%d</id>					\
		<startReader/>				\
		</cmd>						\
		</frame>";


	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);
	sprintf_s(buffer, length, startXML, m_nSessionID);


	ErrorType ret = __communicate(m_nSocket, buffer, length);
	if (ret != ErrorType::ERROR_OK)
	{
		WriteError("communite error %d", ret);
		delete[]buffer;
		buffer = nullptr;
		return ret;
	}

	int reID = 0;
	int reCode = -1;
	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);

	delete[]buffer;
	buffer = nullptr;

	if ((reID == m_nSessionID) && (reCode == 0))
	{
		return ErrorType::ERROR_OK;
	}
	return ret;
}

CRFIDReader::ErrorType CRFIDReader::stopRead(SOCKET fd)
{
	char stopXML[] = "<frame>			\
		<cmd>							\
		<id>%d</id>			\
		<stopReader/>					\
		</cmd>						\
		</frame>";

	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);
	sprintf_s(buffer, length, stopXML, m_nSessionID);

	ErrorType ret = __communicate(fd, buffer, length);
	if (ret != ErrorType::ERROR_OK)
	{
		delete[]buffer;
		buffer = nullptr;
		WriteError("communicate err = %d", ret);
		return ret;
	}

	int reID = 0;
	int reCode = -1;
	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);

	delete[]buffer;
	buffer = nullptr;

	if ((reID == m_nSessionID) && (reCode == 0))
	{
		return ErrorType::ERROR_OK;
	}
	return ret;
}

CRFIDReader::ErrorType CRFIDReader::heartBeat(SOCKET fd)
{
	char heartBeatXML[] = "<frame>			\
		<cmd>							\
		<id>%d</id>			\
		<heartBeat/>					\
		</cmd>						\
		</frame>";

	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);
	sprintf_s(buffer, length, heartBeatXML, m_nSessionID);

	ErrorType ret = __communicate(fd, buffer, length);
	if (ret != ErrorType::ERROR_OK)
	{
		delete[]buffer;
		buffer = nullptr;
		WriteError("communicate err = %d", ret);
		return ret;
	}

	int reID = 0;
	int reCode = -1;
	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);

	delete[]buffer;
	buffer = nullptr;

	if ((reID == m_nSessionID) && (reCode == 0))
	{
		return ErrorType::ERROR_OK;
	}
	return ret;
}

CRFIDReader::ErrorType CRFIDReader::setIPConfig(SOCKET fd, char * Ip, char * Netmask, char * Gateway, bool enableDHCP)
{
	char ipConfigXml[] = "<frame>						\
		<cmd>											\
		<id>%d</id>							\
		<setIPConfig>									\
		<iPAddress>%s</iPAddress>		\
		<subNetMask>%s</subNetMask>	\
		<dHCPEnable>%d</dHCPEnable>	\
		<gateway>%s</gateway>			\
		</setIPConfig>								\
		</cmd>										\
		</frame>";

	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);

	int dhcpValue = ((enableDHCP == true) ? 1 : 0);

	sprintf_s(buffer, length, ipConfigXml, m_nSessionID, Ip, Netmask, dhcpValue, Gateway);


	ErrorType ret = __communicate(fd, buffer, length);
	if (ret != ErrorType::ERROR_OK)
	{
		delete[]buffer;
		buffer = nullptr;
		WriteError("communicate err = %d", ret);
		return ret;
	}

	int reID = 0;
	int reCode = -1;
	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);

	delete[]buffer;
	buffer = nullptr;

	if ((reID == m_nSessionID) && (reCode == 0))
	{
		return ErrorType::ERROR_OK;
	}
	return ret;
}

CRFIDReader::ErrorType CRFIDReader::resetReader(SOCKET fd, char *resetType)
{
	char resetXml[] = "<frame>						\
		<cmd>										\
		<id>%d</id>						\
		<resetReader>								\
		<resetType>%s</resetType>	\
		</resetReader>							\
		</cmd>									\
		</frame>";
	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);

	sprintf_s(buffer, length, resetXml, m_nSessionID, resetType);

	ErrorType ret = __communicate(fd, buffer, length);
	if (ret != ErrorType::ERROR_OK)
	{
		delete[]buffer;
		buffer = nullptr;
		WriteError("communicate err = %d", ret);
		return ret;
	}

	int reID = 0;
	int reCode = -1;
	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);

	delete[]buffer;
	buffer = nullptr;

	if ((reID == m_nSessionID) && (reCode == 0))
	{
		return ErrorType::ERROR_OK;
	}
	return ret;
}

CRFIDReader::ErrorType CRFIDReader::parseReplyPackage(char * buffer, int length, int & id, int & resultCode)
{
	id = 0;
	resultCode = -1;
	if (buffer == nullptr)
	{
		return ErrorType::ERROR_BUFFER_NULL;
	}
	TiXmlDocument lconfigXML;
	lconfigXML.Parse(buffer);
	if (lconfigXML.Error())
	{
		TRACE0("xml Format is invalid\n");
		WriteError("recvBlood = [%s]", buffer);
		return ErrorType::ERROR_XML_PARSE;
	}
	TiXmlElement *rootElement = lconfigXML.RootElement();

	if ((rootElement == nullptr) || (strncmp(rootElement->Value(), "frame", strlen("frame")) != 0))
	{
		WriteError("recvBlood get root element Failed, %s", buffer);
		return ErrorType::ERROR_XML_LABEL;
	}

	char tmpResult[20] = { 0 };
	memset(tmpResult, 0, sizeof(tmpResult));

	char tmpID[20] = { 0 };
	memset(tmpID, 0, sizeof(tmpID));

	if (rootElement->FirstChildElement() != nullptr)
	{
		std::stack<TiXmlElement*> tmpVec;
		tmpVec.push(rootElement->FirstChildElement());
		while (tmpVec.size() != 0)
		{
			TiXmlElement* tmpNode = tmpVec.top();
			tmpVec.pop();
			for (TiXmlElement *child = tmpNode->FirstChildElement();	\
				child != NULL; child = child->NextSiblingElement())
			{
				tmpVec.push(child);
			}

			if (strncmp("resultCode", tmpNode->Value(), strlen("resultCode")) == 0)
			{
				const char *tmpText = tmpNode->GetText();
				memcpy(tmpResult, tmpText, strlen(tmpText));
			}
			if (strncmp("id", tmpNode->Value(), strlen("id")) == 0)
			{
				const char *tmpText = tmpNode->GetText();
				memcpy(tmpID, tmpText, strlen(tmpText));
			}
		}
	}

	rootElement->Clear();
	lconfigXML.Clear();

	if ((strlen(tmpID) != 0) && (strlen(tmpResult) != 0))
	{
		id = atoi(tmpID);
		resultCode = atoi(tmpResult);
		return ErrorType::ERROR_OK;
	}
	return ErrorType::ERROR_XML_LABEL;
}

CRFIDReader::ErrorType CRFIDReader::__communicate(SOCKET fd, char * buffer, int length)
{

	if (SOCKET_ERROR == send(fd, buffer, strlen(buffer), MSG_WAITALL))
	{
		int ret = WSAGetLastError();
		if (ret == WSAENOTCONN)
		{
			WriteError("rfid server has closed");
			closesocket(fd);
			fd = -1;
			return ErrorType::ERROR_SOCKET_CLOSED;
		}
		WriteError("socket Error, %d", WSAGetLastError());
		return ErrorType::ERROR_SOCKET_SEND;
	}

	memset(buffer, 0, sizeof(char) * length);

	int recvLen = length;
	int nret = recv(fd, buffer, recvLen, MSG_WAITALL);
	if (nret == 0)
	{
		WriteError("rfid server has closed");
		closesocket(fd);
		fd = -1;
		return ErrorType::ERROR_SOCKET_CLOSED;
	}
	else if (nret == SOCKET_ERROR)
	{
		WriteError("rfid server SOCKET ERROR");
		return ErrorType::ERROR_SOCKET_RECV;
	}
	return ErrorType::ERROR_OK;
}
