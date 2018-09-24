﻿#include "../stdafx.h"
#include "NetworkTask.h"
#include "../common/ParamManager.h"
#include "../common/Log.h"
#include <chrono>
#include <ctime>
#include <Iphlpapi.h>
#include <IcmpAPI.h>
#include "../common/utils.h"
#include "../xml/tinyxml.h"
#include "../common/md5.h"

#ifdef PYTHON_TENSORFLOW
#include <Python.h>
#endif // PYTHON_TENSORFLOW


#include <afxinet.h>

CNetworkTask *CNetworkTask::m_pInstance = nullptr;


CNetworkTask::CNetworkTask():
	m_bThreadStatus(true)
{
	m_pParamManager = CParamManager::GetInstance();
	
	m_pMsgList = new std::list<message>;

	//设置临时保存文件
	memset(m_szCacheFile, 0, sizeof(m_szCacheFile));
	const char *tmpCacheFile = m_pParamManager->GetSendFailedCache();
	memcpy(m_szCacheFile, tmpCacheFile, strlen(tmpCacheFile));

	wchar_t *tmpWCacheFile = utils::CharToWchar((char*)tmpCacheFile);

	if (tmpWCacheFile != nullptr)
	{
		if (PathFileExists(tmpWCacheFile) == TRUE)
		{
			m_pLog.open(m_szCacheFile, std::ios::ate | std::ios::out | std::ios::in);
		}
		else
		{
			m_pLog.open(m_szCacheFile, std::ios::app | std::ios::out | std::ios::in);
		}
		delete[]tmpWCacheFile;
		tmpWCacheFile = nullptr;
		
	}
}


CNetworkTask::~CNetworkTask()
{
	if (m_pMsgList != nullptr)
	{
		serialize();

		delete m_pMsgList;
		m_pMsgList = nullptr;
	}
}

bool CNetworkTask::IsReachable(unsigned int clientIp, unsigned int serverIp)
{
	HANDLE IcmpHandle = IcmpCreateFile();
	char reply[200] = { 0 };
	PIP_OPTION_INFORMATION info = NULL;

	if (0 == IcmpSendEcho2Ex(IcmpHandle, NULL, NULL, NULL, \
		htonl(clientIp), htonl(serverIp), "icmp", strlen("icmp"), \
		info, reply, sizeof(reply), (DWORD)100))
	{
		WriteError("icmpSendEcho failed clientIp = 0x%X, ServerIp = 0x%X", clientIp, serverIp);
		IcmpCloseHandle(IcmpHandle);
		//WSACleanup();
		IcmpHandle = INVALID_HANDLE_VALUE;
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

	char *str = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\
		\n<bloodheart ip=\"%s\"  status=\"%s\" pcName=\"%s\" time=\"%s\"></bloodheart>";

	char ipLocalStr[20] = { 0 };
	_snprintf_s(ipLocalStr, sizeof(ipLocalStr), "%d.%d.%d.%d", \
		((localIp & 0xFF000000) >> 24), ((localIp & 0xFF0000) >> 16), \
		((localIp & 0xFF00) >> 8), (localIp & 0xFF));

	char bloodheart[200];
	memset(bloodheart, 0, sizeof(bloodheart));

	std::chrono::system_clock::time_point curPoint = std::chrono::system_clock::now();
	
	std::time_t t = std::time(nullptr);
	std::tm time;
	localtime_s(&time, &t);
	char timeStr[20] = { 0 };
	_snprintf_s(timeStr, sizeof(timeStr), "%04d%02d%02d-%02d%02d%02d", \
		time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,		\
		time.tm_hour, time.tm_min, time.tm_sec);

	std::string tmpPCName = m_pParamManager->GetLocalName();
	
	_snprintf_s(bloodheart, sizeof(bloodheart), str, ipLocalStr, \
		"success", tmpPCName.c_str(), timeStr);

	bool ret = false;
	char recvBlood[200];
	size_t recvMsgLen = sizeof(recvBlood);
	// 发送心跳包数据
	ret = __sendToServer(serverIp, port, bloodheart, strlen(bloodheart), recvBlood, recvMsgLen);
	if ((ret == false) || (recvMsgLen == 0))
	{
		return false;
	}
	
	// 解析服务器返回xml
	TiXmlDocument lconfigXML;
	//TiXmlParsingData data;
	lconfigXML.Parse(recvBlood);
	if (lconfigXML.Error())
	{
		TRACE0("xml Format is invalid\n");
		WriteError("recvBlood = [%s]", recvBlood);
		return false;
	}
	TiXmlElement *rootElement = lconfigXML.RootElement();

	if ((rootElement = nullptr) || (strncmp(rootElement->Value(), "reply", strlen("reply")) != 0))
	{
		WriteError("recvBlood get root element Failed, %s", recvBlood);
		return false;
	}
	if (strncmp(rootElement->Attribute("package"), "bloodheart", strlen("bloodheart") != 0))
	{
		WriteError("replay package error");
		return false;
	}
	rootElement->Clear();
	lconfigXML.Clear();
	return true;
}



void CNetworkTask::SendMessageTo(message * msg)
{
	std::lock_guard<std::mutex> lock(m_MutexMsg);
	m_pMsgList->push_back(*msg);
}

bool CNetworkTask::GetThreadStatus()
{
	return m_bThreadStatus;
}

void CNetworkTask::SetThreadStatus(bool status)
{
	m_bThreadStatus = status;
}


void CNetworkTask::run()
{
	std::chrono::system_clock::time_point preBlood = std::chrono::system_clock::now();
	message tmpMsg;
	message recvMsg;
	while (m_bThreadStatus)
	{
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - preBlood).count() >= 10 * 1000)
		{
			heartBlood(m_pParamManager->GetServerIP(), m_pParamManager->GetServerPort());
			preBlood = now;
		}
		if (m_pMsgList->size() > 0)
		{
			std::unique_lock<std::mutex> lock(m_MutexMsg, std::defer_lock);
			if (lock.try_lock())
			{
				tmpMsg = m_pMsgList->front();
				m_pMsgList->pop_front();

				//memcpy(&tmpMsg, &m_pMsgQueue[m_nOut], sizeof(message));
				//m_nOut = (m_nOut + 1) % MAX_MSG_SIZE;
				//m_nMsgSize--;
				if ((tmpMsg.serverIp == -1) && (tmpMsg.serverPort == -1))
				{
					break;
				}
				size_t recvLength = 0;
				lock.release();
				WriteInfo("get recog result");
				if (__sendRecogToServer(tmpMsg.serverIp, tmpMsg.serverPort, tmpMsg.imagePort, &tmpMsg.mRecogResult) == false)
				{
					SendMessageTo(&tmpMsg);
				}
				
			}
		}

		//////////////////////////////
		if (m_pMsgList->size() == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		
	}
}

CNetworkTask * CNetworkTask::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CNetworkTask();
	}
	return m_pInstance;
}

bool CNetworkTask::__sendToServer(unsigned int serverIp, int port, const char *sendMsg, \
	size_t sendMsgLen, char *recvMsg, size_t &recvMsgLen)
{
	unsigned int localIp = m_pParamManager->GetLocalIP();
	if ((localIp == 0) || (localIp == -1))
	{
		recvMsgLen = 0;
		return false;
	}

	int err = 0;
	SOCKET socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFD == -1)
	{
		TRACE("create socket failed\n");
		recvMsgLen = 0;
		return false;
	}

	sockaddr_in addr;

	//接收时限
	int nNetTimeout = 1000;
	setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//unsigned long tmpBlock = 0;
	//ioctlsocket(socketFD, FIONBIO, &tmpBlock);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = htonl(serverIp);

	if (0 != connect(socketFD, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		err = WSAGetLastError();
		TRACE1("connect failed,err = %u\n", err);
		WriteError("connect failed, err = %u", err);
		closesocket(socketFD);
		//WSACleanup();
		recvMsgLen = 0;
		return false;
	}

	int tmpLen = send(socketFD, sendMsg, sendMsgLen, 0);// MSG_DONTROUTE);
	if (tmpLen == SOCKET_ERROR)
	{
		WriteError("send Failed, msg = %s, len = %u, Err:", sendMsg, sendMsgLen, WSAGetLastError());
		closesocket(socketFD);
		//WSACleanup();
		recvMsgLen = 0;
		return false;
	}
	tmpLen = recv(socketFD, recvMsg, recvMsgLen, 0);
	if (SOCKET_ERROR == tmpLen)
	{
		WriteError("recv Failed, Err: %u", GetLastError());
		closesocket(socketFD);
		//WSACleanup();
		recvMsgLen = 0;
		return false;
	}
	recvMsgLen = tmpLen;
	closesocket(socketFD);
	socketFD = -1;

	//WSACleanup();

	return true;
}
bool CNetworkTask::__sendRecogToServer(unsigned int serverIp, int textPort, int imagePort, RecogResultA * recog)
{
	////////根据通信标准格式化成xml
	// MD5为图像数据的md5值
	char textXml[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>	\
		<identification lineID = \"%s\" ip = \"%s\" time=\"%s\">	\
		<data barcode = \"%s\" barcodeResult = \"%s\" md5 = \"%s\"	\
		  method = \"auto\" usrName=\"%s\" imagePath=\"%s\"	\
		typeByRecog = \"%s\" typeByBarcode=\"%s\" typeByInput=\"%s\"	\
		cameraName=\"%s\"	/>	\
		</identification>";

	char *content = nullptr;
	size_t imageSize = 0;
	utils::readFile(recog->m_szImagePath, content, imageSize);
	if (imageSize == 0)
	{
		if (connect != nullptr)
		{
			delete[]content;
			content = nullptr;
		}
		WriteError("read image file %s failed", recog->m_szImagePath);
		return false;
	}
	std::string tmpStr(content, content + imageSize);

	//delete[]content;
	//content = nullptr;
	
	MD5 md5(tmpStr);
	const char *digest = (const char*)md5.getDigest();

	const size_t length = 1000;
	char sendXml[length];
	memset(sendXml, 0, sizeof(char) * length);

	sprintf_s(sendXml, length, textXml, recog->m_szLineName, m_pParamManager->GetLocalIP(), recog->m_szTime,	\
		recog->m_szBarcode, recog->m_szInternalType, digest, recog->m_szUsrName, recog->m_szImagePath, recog->m_szTypeByRecog,	\
	recog->m_szTypeByBarcode,recog->m_szTypeByUsrInput, recog->m_szCameraName);

	char recvText[length];
	memset(recvText, 0, sizeof(char) * imageSize);
	size_t recvMsgLen = 0;

	bool flag = false;
	do
	{
		if (false == __sendToServer(serverIp, textPort, sendXml, strlen(sendXml), recvText, recvMsgLen))
		{
			WriteError("send recog text to server failed");
			break;
		}
		if (recvMsgLen == 0)
		{
			WriteError("get return text failed");
			break;
		}

		TiXmlDocument lconfigXML;
		lconfigXML.Parse(recvText);
		if (lconfigXML.Error())
		{
			WriteError("parse return text recvMsg = [%s]", recvText);
			break;
		}
		TiXmlElement *rootElement = lconfigXML.RootElement();
		if ((rootElement == nullptr) || (strncmp(rootElement->Value(), "reply", strlen("reply")) != 0))
		{
			WriteError("recvBlood get root element Failed, %s", recvText);
			break;
		}
		if (strcmp(rootElement->Attribute("statu"), "ok") != 0)
		{
			WriteError("return recvMsg error， %s", recvText);
			break;
		}
		rootElement->Clear();
		lconfigXML.Clear();

		__sendToServer(serverIp, imagePort, content, imageSize, recvText, recvMsgLen);
		
		lconfigXML;
		lconfigXML.Parse(recvText);
		if (lconfigXML.Error())
		{
			WriteError("parse return text recvMsg = [%s]", recvText);
			break;
		}
		rootElement = lconfigXML.RootElement();
		if ((rootElement == nullptr) || (strncmp(rootElement->Value(), "reply", strlen("reply")) != 0))
		{
			WriteError("recvBlood get root element Failed, %s", recvText);
			break;
		}
		if (strcmp(rootElement->Attribute("statu"), "ok") != 0)
		{
			WriteError("return recvMsg error， %s", recvText);
			break;
		}
		rootElement->Clear();
		lconfigXML.Clear();
		flag = true;

	} while (0);

	if (content != nullptr)
	{
		delete[]content;
		content = nullptr;
	}

	return flag;
}
bool CNetworkTask::initCacheFile()
{

	m_pLog.seekg(0, std::ios::end);

	size_t fileLength = m_pLog.tellg();
	char *content = new char[fileLength + 1];
	if (content == nullptr)
	{
		return false;
	}
	memset(content, 0, sizeof(char) * (fileLength + 1));

	m_pLog.read(content, fileLength);

	RecogResultA tmpResult;
	

	const size_t lineLength = 1000;
	char *line = new char[lineLength];
	if (line == nullptr)
	{
		delete[]content;
		content = nullptr;
		return false;
	}

	int64_t begin = 0;
	while (1)
	{
		memset(line, 0, sizeof(char) * lineLength);
		char *lineEnd = std::find(content + begin, content + fileLength, '\n');
		if (lineEnd == content + fileLength)
		{
			break;
		}
		memcpy(line, content + begin, lineEnd - content - begin);
		memset(&tmpResult, 0, sizeof(RecogResultA));
		RecogResultA::TextToRecog(tmpResult, line);
	}

	delete[]line;
	line = nullptr;

	delete[]content;
	content = nullptr;

	return true;
}

void CNetworkTask::serialize()
{
	if (m_pMsgList->size() > 0)
	{
		size_t nSize = m_pMsgList->size();
		m_pLog.clear();

		char tmpLine[1000];

		for (std::list<message>::iterator iter = m_pMsgList->begin(); iter != m_pMsgList->end(); ++iter)
		{
			memset(tmpLine, 0, sizeof(tmpLine));
			message::serialize(*iter, tmpLine);
			m_pLog << tmpLine << "\n";
		}
	}
}


bool CNetworkTask::message::serialize(message & a, char * line)
{
	std::stringstream ss;
	ss << a.serverIp << "," << a.serverPort << "," << a.imagePort << ",";

	constexpr const size_t tmpLength = sizeof(RecogResultA);
	
	char tmp[tmpLength];
	
	memset(tmp, 0, sizeof(tmp));

	RecogResultA::RecogToText(a.mRecogResult, tmp);

	ss << tmp;

	std::string str = ss.str();

	memcpy(line, str.c_str(), str.size());

	return true;
}

bool CNetworkTask::message::deserialize(message & a, char * line)
{
	std::stringstream ss(line);
	ss >> a.serverIp >> "," >> a.serverPort >> "," >> a.imagePort >> ",";

	constexpr const size_t tmpLength = sizeof(RecogResultA);

	char tmp[tmpLength];

	memset(tmp, 0, sizeof(tmp));

	ss >> tmp;

	RecogResultA::TextToRecog(a.mRecogResult, tmp);

	return true;
}
