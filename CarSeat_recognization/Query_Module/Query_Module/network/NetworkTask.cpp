#include "../stdafx.h"
#include "NetworkTask.h"
#include "../common/ParamManager.h"
#include "../common/Log.h"
#include <chrono>
#include <ctime>
#include <Iphlpapi.h>
#include <IcmpAPI.h>
#include "../common/utils.h"
#include "../xml/tinyxml.h"
#include <afxinet.h>

//CNetworkTask *CNetworkTask::m_pInstance = nullptr;


CNetworkTask::CNetworkTask(unsigned int serverIp, unsigned short port):
	m_nPort(port),
	m_nIp(serverIp)
{
	int err = 0;
	m_nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_nSocket == -1)
	{
		TRACE("create socket failed\n");
	}

	sockaddr_in addr;
	//接收时限
	int nNetTimeout = 1000;
	setsockopt(m_nSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	_aux_connect();
}


CNetworkTask::~CNetworkTask()
{
	if ((m_nSocket != 0) && (m_nSocket != INVALID_SOCKET))
	{
		closesocket(m_nSocket);
		m_nSocket = INVALID_SOCKET;
	}
}

bool CNetworkTask::IsReachable(unsigned int clientIp, unsigned int serverIp)
{
	HANDLE IcmpHandle = IcmpCreateFile();
	char reply[200] = { 0 };
	PIP_OPTION_INFORMATION info = NULL;

	if (0 == IcmpSendEcho2Ex(IcmpHandle, NULL, NULL, NULL, \
		htonl(clientIp), htonl(serverIp), "icmp", strlen("icmp"), \
		info, reply, sizeof(reply), 100))
	{
		WriteError("icmpSendEcho failed clientIp = 0x%X, ServerIp = 0x%X", clientIp, serverIp);
		IcmpCloseHandle(IcmpHandle);
		//WSACleanup();
		return false;
	}
	
	IcmpCloseHandle(IcmpHandle);
	return true;
}

bool CNetworkTask::heartBlood(unsigned int serverIp, unsigned int port)
{
	char *str = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\
		\n<bloodheart status=\"%s\" pcName=\"%s\" time=\"%s\"></bloodheart>";

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

	std::string tmpPCName("NULL");
	{
		char buf[100] = { 0 };
		if (gethostname(buf, sizeof(buf)) < 0)
		{
			WriteError("get local name Failed");
			return 0;
		}
		tmpPCName = std::string(buf);// util
	}

	_snprintf_s(bloodheart, sizeof(bloodheart), str, \
		"success", tmpPCName.c_str(), timeStr);

	bool ret = false;
	char recvBlood[200];
	size_t recvMsgLen = sizeof(recvBlood);
	// 发送心跳包数据
	ret = __sendToServer(m_nSocket, bloodheart, strlen(bloodheart), recvBlood, recvMsgLen);
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
	lconfigXML.Clear();
	return true;
}

std::vector<RecogResultW>* CNetworkTask::QueryRecogInfo(CConditionFilterA & filter, unsigned int ip, const std::string & userName, const std::string & passwd)
{
	if ((m_nSocket == 0)|| (m_nSocket == SOCKET_ERROR) || (m_nSocket == INVALID_SOCKET))
	{
		return nullptr;
	}
	char *queryXml = generateSearchXml(filter, ip, userName, passwd);
	if (queryXml == nullptr)
	{
		return nullptr;
	}
	const int bufferSize = 3 * 1024 * 1024;

	char *resultInfo = new char[bufferSize];
	memset(resultInfo, 0, sizeof(char) * bufferSize);
	size_t infoSize = bufferSize;
	std::vector<RecogResultW> *value = nullptr;
	do
	{
		if (__sendToServer(m_nSocket, queryXml, strlen(queryXml), resultInfo, infoSize) == false)
		{
			WriteInfo("send query xml failed");
			break;
		}
		value = ParseQueryResult(resultInfo, strlen(resultInfo));

		replyMessage(m_nSocket, "reply", "reply", "success");

		const size_t bufferSize = 10 * 1024 * 1024;
		char *buffer = new char[bufferSize];
		memset(buffer, 0, sizeof(char) * bufferSize);
		if ((value != nullptr) && (value->size() == 1))
		{
			imageHeader tmpHeader;
			char tmpImagePath[MAX_CHAR_LENGTH];
			for (size_t i = 0; i < value->size(); ++i)
			{
				int tmpLen = recv(m_nSocket, (char*)&tmpHeader, sizeof(imageHeader), MSG_WAITALL);
				if (SOCKET_ERROR == tmpLen)
				{
					WriteError("recv ImageHeader, Err: %u", GetLastError());
					closesocket(m_nSocket);
					m_nSocket = INVALID_SOCKET;
					break;
				}
				replyMessage(m_nSocket, "reply", "imageHeader", "success");

				recv(m_nSocket, buffer, bufferSize, MSG_WAITALL);

				replyMessage(m_nSocket, "reply", "imageData", "success");

				CParamManager *pParamManager = CParamManager::GetInstance();

				memset(tmpImagePath, 0, sizeof(tmpImagePath));
				sprintf_s(tmpImagePath, sizeof(tmpImagePath), "%s\\%s", pParamManager->GetCacheDirectory(), tmpHeader.path);

				if (true == utils::saveFile(buffer, tmpHeader.size, tmpImagePath))
				{
					wchar_t *tmpWImagePath = utils::CharToWChar(tmpImagePath);
					if (tmpWImagePath != nullptr)
					{
						memcpy(value->at(i).m_szImagePath, tmpWImagePath, sizeof(wchar_t) * wcslen(tmpWImagePath));
						delete[] tmpWImagePath;
						tmpWImagePath = nullptr;
					}
				}
			}
		}
		delete[]buffer;
		buffer = nullptr;

	} while (0);

	delete[]resultInfo;
	resultInfo = nullptr;


	return value;
}

bool CNetworkTask::UpdateBarcodeCheckList(std::unordered_map<std::string, std::string>* pBarcode, std::unordered_map<std::string, std::string>* pClassify)
{

	return false;
}

bool CNetworkTask::_aux_connect()
{
	sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPort);
	addr.sin_addr.S_un.S_addr = htonl(m_nIp);

	if (0 != connect(m_nSocket, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		int err = WSAGetLastError();
		TRACE1("connect failed,err = %u\n", err);
		WriteError("connect failed, err = %u", err);
		closesocket(m_nSocket);
		m_nSocket = INVALID_SOCKET;
		return false;
	}

	return true;
}

//
//
//void CNetworkTask::SendMessageTo(message * msg)
//{
//	std::unique_lock<std::mutex> lock(m_MutexMsg, std::defer_lock);
//	if (lock.try_lock())
//	{
//		if (m_nMsgSize == MAX_MSG_SIZE)
//		{
//			WriteInfo("networkTask message queue is full");
//			return;
//		}
//		else
//		{
//			memcpy(&m_pMsgQueue[m_nIn], msg, sizeof(message));
//			m_nIn = (m_nIn + 1) % MAX_MSG_SIZE;
//			m_nMsgSize++;
//		}
//	}
//}
//
//bool CNetworkTask::GetThreadStatus()
//{
//	return m_bThreadStatus;
//}
//
//void CNetworkTask::SetThreadStatus(bool status)
//{
//	m_bThreadStatus = status;
//}
//
//
//std::wstring CNetworkTask::GetCurrentImagePath()
//{
//	return m_szImagePath;
//}
//
//
//void CNetworkTask::run()
//{
//	std::chrono::system_clock::time_point preBlood = std::chrono::system_clock::now();
//	message tmpMsg;
//	message recvMsg;
//	while (m_bThreadStatus)
//	{
//		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
//		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - preBlood).count() >= 60 * 1000)
//		{
//			heartBlood(m_pParamManager->GetServerIP(), m_pParamManager->GetServerPort());
//		}
//		if (m_nMsgSize > 0)
//		{
//			std::unique_lock<std::mutex> lock(m_MutexMsg, std::defer_lock);
//			if (lock.try_lock())
//			{
//				memcpy(&tmpMsg, &m_pMsgQueue[m_nOut], sizeof(message));
//				m_nOut--;
//				m_nMsgSize--;
//				if ((tmpMsg.serverIp == -1) && (tmpMsg.serverPort == -1))
//				{
//					break;
//				}
//				size_t recvLength = 0;
//				__sendToServer(tmpMsg.serverIp, tmpMsg.serverPort, tmpMsg.data, strlen(tmpMsg.data), recvMsg.data, recvLength);
//			}
//		}
		///////////////////////////
		//  add code



//#if (defined _DEBUG) && (defined FTP_TEST)
//		const wchar_t *filename = L"config.txt";
//		std::vector<std::wstring> *ftpParam = m_pParamManager->GetFtpParameter();
//
//		if ((ftpParam != nullptr) && (ftpParam->size() >= 3))
//		{
//			if (false == ftpUpload(m_pParamManager->GetServerIP(),
//				ftpParam->at(0).c_str(), ftpParam->at(1).c_str(), ftpParam->at(2).c_str(), filename))
//			{
//				WriteError("user = %s, passwd = %s,upload File = %s to serverIp = %u Failed", ftpParam->at(0).c_str(), ftpParam->at(1).c_str(), filename, m_pParamManager->GetServerIP());
//			}
//		}
//
//		const wchar_t *downloadFile = L"config_copy.txt";
//
//		if ((ftpParam != nullptr) && (ftpParam->size() >= 3))
//		{
//			if (false == ftpDownload(m_pParamManager->GetServerIP(),
//				ftpParam->at(0).c_str(), ftpParam->at(1).c_str(), ftpParam->at(2).c_str(), downloadFile))
//			{
//				WriteError("user = %s, passwd = %s,download File = %s to serverIp = %u Failed", ftpParam->at(0).c_str(), ftpParam->at(1).c_str(), downloadFile, m_pParamManager->GetServerIP());
//			}
//		}
//#endif // _DEBUG
//

		
		//CLOCKS_PER_SEC

		//////////////////////////////
//		if (m_nMsgSize == 0)
//		{
//			std::this_thread::sleep_for(std::chrono::milliseconds(200));
//		}
//		
//	}
//}
//
//CNetworkTask * CNetworkTask::GetInstance()
//{
//	if (m_pInstance == nullptr)
//	{
//		m_pInstance = new CNetworkTask();
//	}
//	return m_pInstance;
//}
//

bool CNetworkTask::__sendToServer(SOCKET & socketFD, const char *sendMsg, \
	size_t sendMsgLen, char *recvMsg, size_t &recvMsgLen)
{
	int err = 0;
	size_t tmpLen = send(socketFD, sendMsg, sendMsgLen, 0);// MSG_DONTROUTE);
	if (tmpLen == SOCKET_ERROR)
	{
		WriteError("send Failed, msg = %s, len = %u, Err:", sendMsg, sendMsgLen, WSAGetLastError());
		closesocket(socketFD);
		socketFD = INVALID_SOCKET;
		recvMsgLen = 0;
		return false;
	}
	tmpLen = recv(socketFD, recvMsg, recvMsgLen, MSG_WAITALL);
	if (SOCKET_ERROR == tmpLen)
	{
		WriteError("recv Failed, Err: %u", GetLastError());
		closesocket(socketFD);
		socketFD = INVALID_SOCKET;
		recvMsgLen = 0;
		return false;
	}
	recvMsgLen = tmpLen;
	return true;
}


//
//bool CNetworkTask::ftpUpload(unsigned int serverIp, const wchar_t *name, const wchar_t *passwd, const wchar_t *ftpDir, 
//	const wchar_t *fileName)
//{
//	unsigned int localIP = m_pParamManager->GetLocalIP();
//	if ((localIP == 0) || (IsReachable(localIP, serverIp) == false))
//	{
//		WriteError("get LocalIp Failed");
//		return false;
//	}
//	CInternetSession * pInternetSession = NULL;
//	CFtpConnection* pFtpConnection = NULL;
//	//建立连接  
//	pInternetSession = new CInternetSession(AfxGetAppName());
//	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);      // 5秒的连接超时
//	pInternetSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);           // 1秒的发送超时
//	pInternetSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 7000);        // 7秒的接收超时
//	pInternetSession->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1000);     // 1秒的发送超时
//	pInternetSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);       // 7秒的接收超时
//	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);          // 1次重试
//	//服务器的ip地址
//	CString strADddress;
//	strADddress.Format(L"%u.%u.%u.%u", ((serverIp & 0xFF000000) >> 24), \
//		((serverIp & 0xFF0000) >> 16), ((serverIp & 0xFF00) >> 8), \
//		(serverIp & 0xFF));
//
//	pFtpConnection = pInternetSession->GetFtpConnection(strADddress, name, passwd);
//	if (pFtpConnection == NULL)
//	{
//		WriteError("usr = %s, passwd = %s connect Failed", name, passwd);
//		return false;
//	}
//	//设置服务器的目录  
//	BOOL bRetVal = pFtpConnection->SetCurrentDirectory(ftpDir);
//	if (bRetVal == FALSE)
//	{
//		AfxMessageBox(L"目录设置失败");
//		return false;
//	}
//	else
//	{
//		//把本地文件上传到服务器上  
//		pFtpConnection->PutFile(fileName, fileName);
//	}
//	//释放资源  
//	if (NULL != pFtpConnection)
//	{
//		pFtpConnection->Close();
//		delete pFtpConnection;
//		pFtpConnection = NULL;
//	}
//	if (NULL != pInternetSession)
//	{
//		delete pInternetSession;
//		pInternetSession = NULL;
//	}
//	return true;
//}
//
//bool CNetworkTask::ftpDownload(unsigned int serverIp, const wchar_t *name, 
//	const wchar_t *passwd, const wchar_t *ftpDir, const wchar_t *fileName)
//{
//	CInternetSession* pInternetSession = NULL;
//	
//	CFtpConnection* pFtpConnection = NULL;
//	//建立连接
//	pInternetSession = new CInternetSession(AfxGetAppName());
//
//	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);      // 5秒的连接超时
//	pInternetSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);           // 1秒的发送超时
//	pInternetSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 7000);        // 7秒的接收超时
//	pInternetSession->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1000);     // 1秒的发送超时
//	pInternetSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);       // 7秒的接收超时
//	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);          // 1次重试
//
//
//	//服务器的ip地址
//	//若要设置为服务器的根目录，则使用"\\"就可以了  
//	//创建了一个CFtpConnection对象，之后就可以通过这个对象进行上传文件，下载文件了  
//	CString strADddress;
//	strADddress.Format(L"%u.%u.%u.%u", ((serverIp & 0xFF000000) >> 24), \
//		((serverIp & 0xFF0000) >> 16), ((serverIp & 0xFF00) >> 8), \
//		(serverIp & 0xFF));
//
//	pFtpConnection = pInternetSession->GetFtpConnection(strADddress, name, passwd);
//	//设置服务器的目录
//	BOOL bRetVal = pFtpConnection->SetCurrentDirectory(ftpDir);
//	if (bRetVal == FALSE)
//	{
//		AfxMessageBox(L"目录设置失败");
//		return false;
//	}
//	else
//	{
//		pFtpConnection->GetFile(fileName, fileName);
//	}
//	//释放源  
//	if (NULL != pFtpConnection)
//	{
//		pFtpConnection->Close();
//		delete pFtpConnection;
//		pFtpConnection = NULL;
//	}
//	if (NULL != pInternetSession)
//	{
//		delete pInternetSession;
//		pInternetSession = NULL;
//	}
//	return false;
//}



std::vector<RecogResultW>* CNetworkTask::ParseQueryResult(char *info, int length)
{
	TiXmlDocument lconfigXML;
	lconfigXML.Parse(info);
	TiXmlElement *rootElement = nullptr;
	bool flag = false;
	std::vector<RecogResultW> *result = new std::vector<RecogResultW>();
	do
	{
		if (lconfigXML.Error())
		{
			break;
		}
		TiXmlElement *rootElement = lconfigXML.RootElement();
		if ((rootElement == nullptr) || (strncmp(rootElement->Value(), "reply", strlen("reply")) != 0))
		{
			break;
		}
		if (strcmp(rootElement->Attribute("package"), "search") != 0)
		{
			WriteInfo("package = %s", rootElement->Attribute("package"));
			break;
		}
		if (strcmp(rootElement->Attribute("status"), "success") != 0)
		{
			break;
		}
		if (strcmp(rootElement->Attribute("version"), "0.1") != 0)
		{
			break;
		}
		size_t seatCount = 0;
		int i = 0;
		RecogResultA tmpResult;
		for (TiXmlElement*child = rootElement->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
		{
			if (strcmp(child->Value(), "seatCount") == 0)
			{
				seatCount = atoi(child->GetText());
			}
			else
			{
				char label[20];
				sprintf_s(label, sizeof(label), "Seat%d", i + 1);
				if (strcmp(child->Value(), label) == 0)
				{
					memset(&tmpResult, 0, sizeof(RecogResultA));
					ParseRecogResult(child, tmpResult);
				}
				RecogResultW tmpResultW;
				utils::RecogResultCToW(tmpResult, tmpResultW);
				result->emplace_back(tmpResultW);
				i = i + 1;
			}
		}


		flag = true;;
	} while (0);
	if (rootElement != nullptr)
	{
		rootElement->Clear();
	}
	lconfigXML.Clear();
	return result;
}


bool CNetworkTask::ParseRecogResult(TiXmlElement * elment, RecogResultA &recog)
{
	if (elment == nullptr)
	{
		return false;
	}
	TiXmlElement *child = elment->FirstChildElement();
	for (; child != nullptr; child = child->NextSiblingElement())
	{
		if (strcmp(child->Value(), "lineID") == 0)
		{
			memcpy(recog.m_szLineName, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "ip") == 0)
		{
			TRACE1("ip = %s\n", child->GetText());
			//memcpy(recog.m_szLineName, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "time") == 0)
		{
			memcpy(recog.m_szTime, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "barcode") == 0)
		{
			memcpy(recog.m_szBarcode, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "barcodeResult") == 0)
		{
			memcpy(recog.m_szInternalType, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "imageName") == 0)
		{
			memcpy(recog.m_szImagePath, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "method") == 0)
		{
			memcpy(recog.m_szRecogMethod, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "usrName") == 0)
		{
			memcpy(recog.m_szUsrName, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "typeByRecog") == 0)
		{
			memcpy(recog.m_szTypeByRecog, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "typeByBarcode") == 0)
		{
			memcpy(recog.m_szTypeByBarcode, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "typeByInput") == 0)
		{
			memcpy(recog.m_szTypeByUsrInput, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "cameraName") == 0)
		{
			memcpy(recog.m_szCameraName, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "correct") == 0)
		{
			int flag = atoi(child->GetText());
			recog.m_bIsCorrect = (flag == 0 ? false : true);
		}

	}
	return true;
}


bool CNetworkTask::replyMessage(SOCKET & socketFD, char *rootName, char *packageName, char* status)
{
	TiXmlDocument *xdoc = new TiXmlDocument();

	TiXmlDeclaration* xdec = new TiXmlDeclaration("1.0", "utf-8", "");
	xdoc->LinkEndChild(xdec);

	TiXmlElement *root = new TiXmlElement(rootName);
	xdoc->LinkEndChild(root);

	char tmpBuf[100];
	memset(tmpBuf, 0, sizeof(tmpBuf));
	sprintf_s(tmpBuf, "%d.%d", 0, 1);

	root->SetAttribute("version", tmpBuf);
	root->SetAttribute("package", packageName);
	root->SetAttribute("status", status);

	TiXmlPrinter printer;
	std::string xmlstr;
	xdoc->Accept(&printer);
	xmlstr = printer.CStr();

	size_t nSize = 2000;
	char *xml = new char[nSize];
	memset(xml, 0, sizeof(char) * nSize);
	memcpy(xml, xmlstr.c_str(), xmlstr.size() * sizeof(char));
	xml[xmlstr.size()] = '\0';

	send(socketFD, xml, strlen(xml), 0);

	delete[] xml;
	xml = nullptr;

	delete xdoc;
	xdoc = nullptr;

	return true;
}

bool CNetworkTask::replayFunc(char *xml, char* packageName)
{
	if ((xml == nullptr) || (packageName == nullptr)	\
		|| (strlen(xml) == 0) || (strlen(packageName) == 0))
	{
		return false;
	}
	TiXmlDocument lconfigXML;
	lconfigXML.Parse(xml);
	TiXmlElement *rootElement = nullptr;
	bool flag = false;
	do
	{
		if (lconfigXML.Error())
		{
			break;
		}
		TiXmlElement *rootElement = lconfigXML.RootElement();
		if ((rootElement == nullptr) || (strncmp(rootElement->Value(), "reply", strlen("reply")) != 0))
		{
			break;
		}
		if (strcmp(rootElement->Attribute("package"), packageName) != 0)
		{
			WriteInfo("package = %s", rootElement->Attribute("package"));
			break;
		}
		if (strcmp(rootElement->Attribute("status"), "success") != 0)
		{
			break;
		}
		flag = true;;
	} while (0);
	if (rootElement != nullptr)
	{
		rootElement->Clear();
	}
	lconfigXML.Clear();
	return flag;
}