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
#include <Python.h>

#include <afxinet.h>

CNetworkTask *CNetworkTask::m_pInstance = nullptr;


CNetworkTask::CNetworkTask():
	m_bThreadStatus(true),
	m_pClassify(nullptr)
{
	m_pParamManager = CParamManager::GetInstance();
	m_pMsgQueue = new message[CNetworkTask::msg::MAX_MSG_SIZE];
	m_nMsgSize = 0;
	m_nIn = m_nOut = 0;
}


CNetworkTask::~CNetworkTask()
{
	if (m_pMsgQueue != nullptr)
	{
		delete[]m_pMsgQueue;
		m_pMsgQueue = nullptr;
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

	std::string tmpPCName = utils::WStrToStr(m_pParamManager->GetLocalName());
	
	_snprintf_s(bloodheart, sizeof(bloodheart), str, ipLocalStr, \
		"success", tmpPCName.c_str(), timeStr);

	bool ret = false;
	char recvBlood[200];
	size_t recvMsgLen = sizeof(recvBlood);
	// ��������������
	ret = __sendToServer(serverIp, port, bloodheart, strlen(bloodheart), recvBlood, recvMsgLen);
	if ((ret == false) || (recvMsgLen == 0))
	{
		return false;
	}
	
	// ��������������xml
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



void CNetworkTask::SendMessageTo(message * msg)
{
	std::unique_lock<std::mutex> lock(m_MutexMsg, std::defer_lock);
	if (lock.try_lock())
	{
		if (m_nMsgSize == MAX_MSG_SIZE)
		{
			WriteInfo("networkTask message queue is full");
			return;
		}
		else
		{
			memcpy(&m_pMsgQueue[m_nIn], msg, sizeof(message));
			m_nIn = (m_nIn + 1) % MAX_MSG_SIZE;
			m_nMsgSize++;
		}
	}
}

bool CNetworkTask::GetThreadStatus()
{
	return m_bThreadStatus;
}

void CNetworkTask::SetThreadStatus(bool status)
{
	m_bThreadStatus = status;
}

void CNetworkTask::SetImageClassify(CImageClassify * pClassify)
{
	m_pClassify = pClassify;
}

void CNetworkTask::run()
{
	std::chrono::system_clock::time_point preBlood = std::chrono::system_clock::now();
	message tmpMsg;
	while (m_bThreadStatus)
	{
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - preBlood).count() >= 60 * 1000)
		{
			heartBlood(m_pParamManager->GetServerIP(), m_pParamManager->GetServerPort());
		}
		if (m_nMsgSize > 0)
		{
			std::unique_lock<std::mutex> lock(m_MutexMsg, std::defer_lock);
			if (lock.try_lock())
			{
				if ((m_pMsgQueue[m_nOut].serverIp == -1) && (m_pMsgQueue[m_nOut].serverPort == -1))
				{
					memcpy(&tmpMsg, &m_pMsgQueue[m_nOut], sizeof(message));
					m_nOut--;
					m_nMsgSize--;
					break;
				}
			}
		}
		///////////////////////////
		//  add code

		unsigned int barcodeIp = m_pParamManager->GetBarcodeIp();
		unsigned int barcodePort = m_pParamManager->GetBarcodePort();

		std::wstring tmpBarcode = getBarcodeByNet(barcodeIp, barcodePort);

		if (tmpBarcode.size() != 0)
		{
			std::wstring path = TakeImage(0);
			//std::wstring tmpPath(L"J:\\AutocarSeat_Recognition\\backupImage\\D2_black_pvc_hole_cloth\\1\\1009.jpg");
			__ImageClassify(path);

		}


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
		if (m_nMsgSize == 0)
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

	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
		err = WSAGetLastError();
		WriteError("err = %u", err);
		recvMsgLen = 0;
		return false;
	}
	SOCKET socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFD == -1)
	{
		TRACE("create socket failed\n");
		WSACleanup();
		recvMsgLen = 0;
		return false;
	}

	sockaddr_in addr;

	//����ʱ��
	int nNetTimeout = 1000;
	setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = htonl(serverIp);

	if (0 != connect(socketFD, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		err = WSAGetLastError();
		TRACE1("connect failed,err = %u\n", err);
		WriteError("connect failed, err = %u", err);
		closesocket(socketFD);
		WSACleanup();
		recvMsgLen = 0;
		return false;
	}

	size_t tmpLen = send(socketFD, sendMsg, sendMsgLen, 0);// MSG_DONTROUTE);
	if (tmpLen == SOCKET_ERROR)
	{
		WriteError("send Failed, msg = %s, len = %u, Err:", sendMsg, sendMsgLen, WSAGetLastError());
		closesocket(socketFD);
		WSACleanup();
		recvMsgLen = 0;
		return false;
	}
	tmpLen = recv(socketFD, recvMsg, recvMsgLen, 0);
	if (SOCKET_ERROR == tmpLen)
	{
		WriteError("recv Failed, Err: %u", GetLastError());
		closesocket(socketFD);
		WSACleanup();
		recvMsgLen = 0;
		return false;
	}
	recvMsgLen = tmpLen;
	closesocket(socketFD);
	socketFD = -1;

	WSACleanup();

	return true;
}

std::wstring CNetworkTask::getBarcodeByNet(unsigned int ip, unsigned int port)
{
	// ��ӻ�ȡ������Ĵ���
	// ��ʱΪ��
	
	return std::wstring();
}

//  

std::wstring CNetworkTask::TakeImage(std::wstring lineID)
{
	std::wstring CameraID = m_pParamManager->FindCameraByLineID(lineID);
	std::wstring path = m_Camera.takePhoto(CameraID);
	return path;
}

bool CNetworkTask::__ImageClassify(std::wstring & path)
{
	if (m_pClassify != nullptr)
	{
		std::string tmpPath = utils::WStrToStr(path);

		m_pClassify->pushImage(tmpPath.c_str());
		return true;
	}
	return false;
}

bool CNetworkTask::ftpUpload(unsigned int serverIp, const wchar_t *name, const wchar_t *passwd, const wchar_t *ftpDir, 
	const wchar_t *fileName)
{
	unsigned int localIP = m_pParamManager->GetLocalIP();
	if ((localIP == 0) || (IsReachable(localIP, serverIp) == false))
	{
		WriteError("get LocalIp Failed");
		return false;
	}
	CInternetSession * pInternetSession = NULL;
	CFtpConnection* pFtpConnection = NULL;
	//��������  
	pInternetSession = new CInternetSession(AfxGetAppName());
	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);      // 5������ӳ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);           // 1��ķ��ͳ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 7000);        // 7��Ľ��ճ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1000);     // 1��ķ��ͳ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);       // 7��Ľ��ճ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);          // 1������
	//��������ip��ַ
	CString strADddress;
	strADddress.Format(L"%u.%u.%u.%u", ((serverIp & 0xFF000000) >> 24), \
		((serverIp & 0xFF0000) >> 16), ((serverIp & 0xFF00) >> 8), \
		(serverIp & 0xFF));

	pFtpConnection = pInternetSession->GetFtpConnection(strADddress, name, passwd);
	if (pFtpConnection == NULL)
	{
		WriteError("usr = %s, passwd = %s connect Failed", name, passwd);
		return false;
	}
	//���÷�������Ŀ¼  
	BOOL bRetVal = pFtpConnection->SetCurrentDirectory(ftpDir);
	if (bRetVal == FALSE)
	{
		AfxMessageBox(L"Ŀ¼����ʧ��");
		return false;
	}
	else
	{
		//�ѱ����ļ��ϴ�����������  
		pFtpConnection->PutFile(fileName, fileName);
	}
	//�ͷ���Դ  
	if (NULL != pFtpConnection)
	{
		pFtpConnection->Close();
		delete pFtpConnection;
		pFtpConnection = NULL;
	}
	if (NULL != pInternetSession)
	{
		delete pInternetSession;
		pInternetSession = NULL;
	}
	return true;
}

bool CNetworkTask::ftpDownload(unsigned int serverIp, const wchar_t *name, 
	const wchar_t *passwd, const wchar_t *ftpDir, const wchar_t *fileName)
{
	CInternetSession* pInternetSession = NULL;
	
	CFtpConnection* pFtpConnection = NULL;
	//��������
	pInternetSession = new CInternetSession(AfxGetAppName());

	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);      // 5������ӳ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);           // 1��ķ��ͳ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 7000);        // 7��Ľ��ճ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1000);     // 1��ķ��ͳ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);       // 7��Ľ��ճ�ʱ
	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);          // 1������


	//��������ip��ַ
	//��Ҫ����Ϊ�������ĸ�Ŀ¼����ʹ��"\\"�Ϳ�����  
	//������һ��CFtpConnection����֮��Ϳ���ͨ�������������ϴ��ļ��������ļ���  
	CString strADddress;
	strADddress.Format(L"%u.%u.%u.%u", ((serverIp & 0xFF000000) >> 24), \
		((serverIp & 0xFF0000) >> 16), ((serverIp & 0xFF00) >> 8), \
		(serverIp & 0xFF));

	pFtpConnection = pInternetSession->GetFtpConnection(strADddress, name, passwd);
	//���÷�������Ŀ¼
	BOOL bRetVal = pFtpConnection->SetCurrentDirectory(ftpDir);
	if (bRetVal == FALSE)
	{
		AfxMessageBox(L"Ŀ¼����ʧ��");
		return false;
	}
	else
	{
		pFtpConnection->GetFile(fileName, fileName);
	}
	//�ͷ�Դ  
	if (NULL != pFtpConnection)
	{
		pFtpConnection->Close();
		delete pFtpConnection;
		pFtpConnection = NULL;
	}
	if (NULL != pInternetSession)
	{
		delete pInternetSession;
		pInternetSession = NULL;
	}
	return false;
}