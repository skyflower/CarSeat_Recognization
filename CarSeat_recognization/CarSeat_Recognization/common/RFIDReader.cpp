#include "../stdafx.h"
#include "RFIDReader.h"
#include <cstdlib>
#include <cstdio>
//#include <Windows.h>
#include "utils.h"
#include "Log.h"
#include "../xml/tinyxml.h"
#include <IcmpAPI.h>
#include <stack>


CRFIDReader::CRFIDReader():
	m_nIp(10001),
	m_nPort(10001),
	m_nSocket(INVALID_SOCKET)
{
	memset(m_szCurrentValue, 0, sizeof(m_szCurrentValue));
	init();
	
}


CRFIDReader::~CRFIDReader()
{

	//stopRead(m_nSocket);
	//hostGoodbye(m_nSocket);
	
	if (m_nSocket != INVALID_SOCKET)
	{
		
		closesocket(m_nSocket);
		//WSACleanup();
		m_nSocket = INVALID_SOCKET;
	}
}

int CRFIDReader::readBarcode(bool flag, std::string &reCode)
{
	//N160310118880001   6-8位有效
	if (flag == false)
	{
		reCode = getByRandomGenerate();
		return 1;
	}
	else
	{
		std::string tmpStr = getBySocket();
		if (tmpStr.size() == 0)
		{
			//// reset()
			return -1;
		}
		else
		{
			if (strncmp(tmpStr.c_str(), m_szCurrentValue, tmpStr.size()) == 0)
			{
				return 0;
			}
			memset(m_szCurrentValue, 0, sizeof(m_szCurrentValue));
			memcpy(m_szCurrentValue, tmpStr.c_str(), tmpStr.size());

			reCode = std::string(m_szCurrentValue);

			return 1;
		}
	}
	return -1;
}

bool CRFIDReader::init()
{
	/*memset(m_szDefaultIp, 0, sizeof(m_szDefaultIp));
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
	strcpy_s(m_szReadMode, "Run");*/

	return true;
}

CRFIDReader::ErrorType CRFIDReader::initRFID(unsigned int serverIp, int port)
{
	//WSADATA wsaData;
	/*int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		err = WSAGetLastError();
		WriteError("err = %u", err);
		return ErrorType::ERROR_SOCKET_INIT;
	}*/
	int err = 0;
	m_nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_nSocket == INVALID_SOCKET)
	{
		closesocket(m_nSocket);
		m_nSocket = INVALID_SOCKET;
		//WSACleanup();
		return ErrorType::ERROR_SOCKET_CREATE;
	}

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));

	//接收时限
	int value = 2000;
	setsockopt(m_nSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&value, sizeof(int));
	setsockopt(m_nSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&value, sizeof(int));

	value = 1;
	setsockopt(m_nSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(int));

	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = htonl(serverIp);

	if (0 != connect(m_nSocket, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		err = WSAGetLastError();
		TRACE1("connect failed,err = %u\n", err);
		WriteError("connect failed, ip = 0x%X, port = %u, err = %u", serverIp, port, err);
		closesocket(m_nSocket);
		m_nSocket = INVALID_SOCKET;
		//WSACleanup();
		return ErrorType::ERROR_SOCKET_CONNECT;
	}
	//WriteInfo("rfid socket connect success");
	m_nIp = serverIp;
	m_nPort = port;
	return ErrorType::ERROR_OK;
}

CRFIDReader::ErrorType CRFIDReader::reset(const char * param)
{
	if (m_nSocket == INVALID_SOCKET)
	{
		WriteError("rfid socket not connect");
		return ErrorType::ERROR_SOCKET_INVALID;
	}
	char resetXML[] = "<command><reset><param>%s</param></reset></command>";

	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);
	
	sprintf_s(buffer, length, resetXML, param);
	int sendLen = strlen(buffer);

	if (SOCKET_ERROR == send(m_nSocket, buffer, sendLen, 0))
	{
		int ret = WSAGetLastError();
		if (ret == WSAENOTCONN)
		{
			WriteError("rfid server has closed");
			closesocket(m_nSocket);
			m_nSocket = INVALID_SOCKET;
			return ErrorType::ERROR_SOCKET_CLOSED;
		}
		WriteError("socket = %d Error, %d", m_nSocket, WSAGetLastError());
		return ErrorType::ERROR_SOCKET_SEND;
	}
	
	delete[]buffer;
	buffer = nullptr;
	return ErrorType::ERROR_OK;
}

CRFIDReader::ErrorType CRFIDReader::isConnect()
{
	if ((m_nSocket == 0) || (m_nSocket == INVALID_SOCKET) || (m_nSocket == -1))
	{
		return ErrorType::ERROR_SOCKET_CLOSED;
	}
	if (IsReachable(m_nIp) == false)
	{
		WriteError("rfid server no rearchable = 0x%X", m_nIp);
		return ErrorType::ERROR_SOCKET_CONNECT;
	}
	
	return ErrorType::ERROR_OK;
}

bool CRFIDReader::IsReachable(unsigned int serverIp)
{
	HANDLE IcmpHandle = IcmpCreateFile();
	char reply[200] = { 0 };
	PIP_OPTION_INFORMATION info = NULL;

	if (0 == IcmpSendEcho(IcmpHandle, htonl(serverIp), "icmp", strlen("icmp"), \
		info, reply, sizeof(reply), (DWORD)1000))
	{
		WriteError("icmpSendEcho failed ServerIp = 0x%X", serverIp);
		IcmpCloseHandle(IcmpHandle);
		IcmpHandle = INVALID_HANDLE_VALUE;
		return false;
	}

	IcmpCloseHandle(IcmpHandle);
	return true;
}

std::string CRFIDReader::getByRandomGenerate()
{
	char tmp[20];
	memset(tmp, 0, sizeof(tmp));
	static double x = 0.000012345678;

	clock_t tmpInt = clock();

	x = x + (tmpInt + 2031) / 7777;
	x = x - (int)x;

	for (int i = 0; i < 10; ++i)
	{
		x = 3.9999 * x * (1 - x);
	}
	double tmpValue = x * pow(10, 8);
	int hiValue = tmpValue;
	int  lowValue = (tmpValue - int(tmpValue)) * pow(10, 8);
	sprintf_s(tmp, sizeof(tmp) / sizeof(char), "N%07d%08d", hiValue, lowValue);

	return std::string(tmp);
}

std::string CRFIDReader::getBySocket()
{
	char readXML[] = "<command><readTagData><startAddress>0000</startAddress><dataLength>001E</dataLength></readTagData></command>";

	const size_t length = 1000;
	char *buffer = new char[length];
	memset(buffer, 0, sizeof(char) * length);

	sprintf_s(buffer, length, "%s", readXML);

	ErrorType ret = __communicate(m_nSocket, buffer, length);
	if (ret != ErrorType::ERROR_OK)
	{
		delete[]buffer;
		buffer = nullptr;
		WriteError("communicate err = %d", ret);
		return std::string();
	}
	
	int id = 0;
	char resultCode[100] = { 0 };
	memset(resultCode, 0, sizeof(resultCode));

	char barcodeChar[100] = { 0 };
	memset(barcodeChar, 0, sizeof(barcodeChar));

	do
	{
		if ((buffer == nullptr) || (strlen(buffer) == 0))
		{
			break;
		}
		char *tmpBegin = strstr(buffer, "<resultCode>");
		if (tmpBegin == NULL)
		{
			break;
		}
		char *tmpEnd = strstr(tmpBegin, "</resultCode>");
		if (tmpEnd == NULL)
		{
			break;
		}
		memcpy(resultCode, tmpBegin + strlen("<resultCode>"), tmpEnd - tmpBegin - strlen("<resultCode>"));

		tmpBegin = strstr(buffer, "<data>");
		if (tmpBegin == NULL)
		{
			break;
		}
		tmpEnd = strstr(tmpBegin, "</data>");
		if (tmpEnd == NULL)
		{
			break;
		}
		memcpy(barcodeChar, tmpBegin + strlen("<data>"), tmpEnd - tmpBegin - strlen("<data>"));

		/*TiXmlDocument lconfigXML;
		lconfigXML.Parse(buffer);
		if (lconfigXML.Error())
		{
			TRACE0("xml Format is invalid\n");
			WriteError("recvBlood = [%s]", buffer);
			break;
		}
		TiXmlElement *rootElement = lconfigXML.RootElement();
		if (rootElement == nullptr)
		{
			lconfigXML.Clear();
			break;
		}

		if (rootElement->FirstChildElement() != nullptr)
		{
			std::stack<TiXmlElement*> tmpVec;
			tmpVec.push(rootElement->FirstChildElement());
			while (tmpVec.size() != 0)
			{
				TiXmlElement* tmpNode = tmpVec.top();
				tmpVec.pop();

				TiXmlElement *tmpSibElement = tmpNode->NextSiblingElement();
				while (tmpSibElement != nullptr)
				{
					tmpVec.push(tmpSibElement);
					tmpSibElement = tmpSibElement->NextSiblingElement();
				}

				for (TiXmlElement *child = tmpNode->FirstChildElement();	\
					child != NULL; child = child->NextSiblingElement())
				{
					tmpVec.push(child);
				}

				if (strncmp("resultCode", tmpNode->Value(), strlen("resultCode")) == 0)
				{
					const char *tmpText = tmpNode->GetText();
					memcpy(resultCode, tmpText, strlen(tmpText));
				}
				if (strncmp("data", tmpNode->Value(), strlen("data")) == 0)
				{
					const char *tmpText = tmpNode->GetText();
					memcpy(barcodeChar, tmpText, strlen(tmpText));
				}
			}
		}

		rootElement->Clear();
		lconfigXML.Clear();*/

	} while (0);

	//WriteInfo("readData return = [%s]", buffer);

	if (buffer != nullptr)
	{
		delete[]buffer;
		buffer = nullptr;
	}

	if (strncmp(resultCode, "0000", strlen("0000")) != 0)
	{
		return std::string();
	}

	if (strlen(barcodeChar) == 0)
	{
		return std::string();
	}


	char tmpValue[256] = { 0 };
	memset(tmpValue, 0, sizeof(tmpValue));
	/*
	从返回消息中解析得到条形码字段
	*/
	if (parseBarcode(barcodeChar, tmpValue) == true)
	{
		std::string barcodeStr(tmpValue);
		return barcodeStr;
	}

	return std::string();
}
//
//CRFIDReader::ErrorType CRFIDReader::hostGreetings(SOCKET fd)
//{
//	char greedyXML[] = "<frame>	\
//		<cmd>							\
//		<id>%d</id>		\
//		<hostGreetings>			\
//		<readerType>%s</readerType>	\
//		<readerMode>%s</readerMode>	\
//		<supportedVersions>								\
//		<version>%s</version>			\
//		</supportedVersions>							\
//		</hostGreetings>								\
//		</cmd>				\
//		</frame>";
//
//
//	const size_t length = 1000;
//	char *buffer = new char[length];
//	memset(buffer, 0, sizeof(char) * length);
//
//	sprintf_s(buffer, length, greedyXML, m_nSessionID, m_szReadType, m_szReadMode, m_szVersion);
//
//	ErrorType ret = __communicate(fd, buffer, length);
//	if (ret != ErrorType::ERROR_OK)
//	{
//		delete[]buffer;
//		buffer = nullptr;
//		WriteError("communicate err = %d", ret);
//		return ret;
//	}
//
//	int reID = 0;
//	int reCode = -1;
//	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);
//
//	delete[]buffer;
//	buffer = nullptr;
//
//	if ((reID == m_nSessionID) && (reCode == 0))
//	{
//		return ErrorType::ERROR_OK;
//	}
//	return ret;
//}
//
//CRFIDReader::ErrorType CRFIDReader::hostGoodbye(SOCKET fd)
//{
//	char goodbye[] = "<frame>							\
//		<cmd>											\
//		<id>%d</id>										\
//		<hostGoodbye>									\
//		<readerMode>%s</readerMode>						\
//		</hostGoodbye>									\
//		</cmd>											\
//		</frame>";
//
//	const size_t length = 1000;
//	char *buffer = new char[length];
//	memset(buffer, 0, sizeof(char) * length);
//	sprintf_s(buffer, length, goodbye, m_nSessionID, m_szReadMode);
//
//	ErrorType ret = __communicate(fd, buffer, length);
//	if (ret != ErrorType::ERROR_OK)
//	{
//		delete[]buffer;
//		buffer = nullptr;
//		WriteError("communicate err = %d", ret);
//		return ret;
//	}
//
//	int reID = 0;
//	int reCode = -1;
//	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);
//
//	delete[]buffer;
//	buffer = nullptr;
//
//	if ((reID == m_nSessionID) && (reCode == 0))
//	{
//		return ErrorType::ERROR_OK;
//	}
//	return ret;
//}
//
//CRFIDReader::ErrorType CRFIDReader::startRead(SOCKET fd)
//{
//	char startXML[] = "<frame>		\
//		<cmd>						\
//		<id>%d</id>					\
//		<startReader/>				\
//		</cmd>						\
//		</frame>";
//
//
//	const size_t length = 1000;
//	char *buffer = new char[length];
//	memset(buffer, 0, sizeof(char) * length);
//	sprintf_s(buffer, length, startXML, m_nSessionID);
//
//
//	ErrorType ret = __communicate(m_nSocket, buffer, length);
//	if (ret != ErrorType::ERROR_OK)
//	{
//		WriteError("communite error %d", ret);
//		delete[]buffer;
//		buffer = nullptr;
//		return ret;
//	}
//
//	int reID = 0;
//	int reCode = -1;
//	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);
//
//	delete[]buffer;
//	buffer = nullptr;
//
//	if ((reID == m_nSessionID) && (reCode == 0))
//	{
//		return ErrorType::ERROR_OK;
//	}
//	return ret;
//}
//
//CRFIDReader::ErrorType CRFIDReader::stopRead(SOCKET fd)
//{
//	char stopXML[] = "<frame>			\
//		<cmd>							\
//		<id>%d</id>			\
//		<stopReader/>					\
//		</cmd>						\
//		</frame>";
//
//	const size_t length = 1000;
//	char *buffer = new char[length];
//	memset(buffer, 0, sizeof(char) * length);
//	sprintf_s(buffer, length, stopXML, m_nSessionID);
//
//	ErrorType ret = __communicate(fd, buffer, length);
//	if (ret != ErrorType::ERROR_OK)
//	{
//		delete[]buffer;
//		buffer = nullptr;
//		WriteError("communicate err = %d", ret);
//		return ret;
//	}
//
//	int reID = 0;
//	int reCode = -1;
//	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);
//
//	delete[]buffer;
//	buffer = nullptr;
//
//	if ((reID == m_nSessionID) && (reCode == 0))
//	{
//		return ErrorType::ERROR_OK;
//	}
//	return ret;
//}
//
//CRFIDReader::ErrorType CRFIDReader::heartBeat(SOCKET fd)
//{
//	char heartBeatXML[] = "<frame>			\
//		<cmd>							\
//		<id>%d</id>			\
//		<heartBeat/>					\
//		</cmd>						\
//		</frame>";
//
//	const size_t length = 1000;
//	char *buffer = new char[length];
//	memset(buffer, 0, sizeof(char) * length);
//	sprintf_s(buffer, length, heartBeatXML, m_nSessionID);
//
//	ErrorType ret = __communicate(fd, buffer, length);
//	if (ret != ErrorType::ERROR_OK)
//	{
//		delete[]buffer;
//		buffer = nullptr;
//		WriteError("communicate err = %d", ret);
//		return ret;
//	}
//
//	int reID = 0;
//	int reCode = -1;
//	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);
//
//	delete[]buffer;
//	buffer = nullptr;
//
//	if ((reID == m_nSessionID) && (reCode == 0))
//	{
//		return ErrorType::ERROR_OK;
//	}
//	return ret;
//}
//
//CRFIDReader::ErrorType CRFIDReader::setIPConfig(SOCKET fd, char * Ip, char * Netmask, char * Gateway, bool enableDHCP)
//{
//	char ipConfigXml[] = "<frame>						\
//		<cmd>											\
//		<id>%d</id>							\
//		<setIPConfig>									\
//		<iPAddress>%s</iPAddress>		\
//		<subNetMask>%s</subNetMask>	\
//		<dHCPEnable>%d</dHCPEnable>	\
//		<gateway>%s</gateway>			\
//		</setIPConfig>								\
//		</cmd>										\
//		</frame>";
//
//	const size_t length = 1000;
//	char *buffer = new char[length];
//	memset(buffer, 0, sizeof(char) * length);
//
//	int dhcpValue = ((enableDHCP == true) ? 1 : 0);
//
//	sprintf_s(buffer, length, ipConfigXml, m_nSessionID, Ip, Netmask, dhcpValue, Gateway);
//
//
//	ErrorType ret = __communicate(fd, buffer, length);
//	if (ret != ErrorType::ERROR_OK)
//	{
//		delete[]buffer;
//		buffer = nullptr;
//		WriteError("communicate err = %d", ret);
//		return ret;
//	}
//
//	int reID = 0;
//	int reCode = -1;
//	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);
//
//	delete[]buffer;
//	buffer = nullptr;
//
//	if ((reID == m_nSessionID) && (reCode == 0))
//	{
//		return ErrorType::ERROR_OK;
//	}
//	return ret;
//}
//
//CRFIDReader::ErrorType CRFIDReader::resetReader(SOCKET fd, char *resetType)
//{
//	char resetXml[] = "<frame>						\
//		<cmd>										\
//		<id>%d</id>						\
//		<resetReader>								\
//		<resetType>%s</resetType>	\
//		</resetReader>							\
//		</cmd>									\
//		</frame>";
//	const size_t length = 1000;
//	char *buffer = new char[length];
//	memset(buffer, 0, sizeof(char) * length);
//
//	sprintf_s(buffer, length, resetXml, m_nSessionID, resetType);
//
//	ErrorType ret = __communicate(fd, buffer, length);
//	if (ret != ErrorType::ERROR_OK)
//	{
//		delete[]buffer;
//		buffer = nullptr;
//		WriteError("communicate err = %d", ret);
//		return ret;
//	}
//
//	int reID = 0;
//	int reCode = -1;
//	ret = parseReplyPackage(buffer, strlen(buffer), reID, reCode);
//
//	delete[]buffer;
//	buffer = nullptr;
//
//	if ((reID == m_nSessionID) && (reCode == 0))
//	{
//		return ErrorType::ERROR_OK;
//	}
//	return ret;
//}

CRFIDReader::ErrorType CRFIDReader::parseReplyPackage(char * buffer, size_t length, int & id, int & resultCode)
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

CRFIDReader::ErrorType CRFIDReader::__communicate(SOCKET &fd, char * buffer, int length)
{
	int sendLen = strlen(buffer);
	if (SOCKET_ERROR == send(fd, buffer, sendLen, 0))
	{
		int ret = WSAGetLastError();
		if ((ret == WSAENOTCONN) || (ret == WSAECONNABORTED))
		{
			WriteError("rfid server has closed");
			closesocket(fd);
			fd = INVALID_SOCKET;
			return ErrorType::ERROR_SOCKET_CLOSED;
		}

		WriteError("socket = %d Error, %d", fd, ret);
		return ErrorType::ERROR_SOCKET_SEND;
	}

	memset(buffer, 0, sizeof(char) * length);
	Sleep(50);

	int recvLen = length;
	int nret = recv(fd, buffer, recvLen, 0);
	if (nret == 0)
	{
		WriteError("rfid server has closed");
		closesocket(fd);
		fd = INVALID_SOCKET;
		return ErrorType::ERROR_SOCKET_CLOSED;
	}
	else if (nret == SOCKET_ERROR)
	{
		int ret = WSAGetLastError();
		if (ret == WSAECONNABORTED)
		{
			closesocket(fd);
			fd = INVALID_SOCKET;
		}
		WriteError("rfid server SOCKET ERROR, socket = %u, error = %d", fd, ret);
		return ErrorType::ERROR_SOCKET_RECV;
	}
	return ErrorType::ERROR_OK;
}

bool CRFIDReader::parseBarcode(const char * text, char * barcode)
{
	int i = 0;
	int k = 0;
	size_t tmpSize = strlen(text);
	char tmpText[10] = { 0 };
	while (i + 2 < tmpSize)
	{
		memset(tmpText, 0, sizeof(tmpText));
		memcpy(tmpText, text + i, sizeof(char) * 2);

		int j = atoi(tmpText);
		barcode[k++] = j;
		if (k >= 16)
		{
			break;
		}

		i = i + 2;
	}

	return true;
}
