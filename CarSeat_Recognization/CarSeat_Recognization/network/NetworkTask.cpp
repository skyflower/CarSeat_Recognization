#include "../stdafx.h"
#include "NetworkTask.h"
#include "../common/ParamManager.h"
#include "../common/Log.h"
#include <chrono>
#include <ctime>
#include <Iphlpapi.h>
#include <IcmpAPI.h>
#include "../common/utils.h"

//#include <time.h>
#include <afxinet.h>

CNetworkTask *CNetworkTask::m_pInstance = nullptr;


CNetworkTask::CNetworkTask():
	m_bThreadStatus(true)
{
	m_pParamManager = CParamManager::GetInstance();
	m_pMsgQueue = new message[CNetworkTask::msg::MAX_MSG_SIZE];
	m_nMsgSize = 0;
	m_nIn = m_nOut = 0;
	//m_pTask = std::thread(this, &CNetworkTask::run);
}


CNetworkTask::~CNetworkTask()
{

}

bool CNetworkTask::IsReachable(unsigned int clientIp, unsigned int serverIp)
{
	HANDLE IcmpHandle = IcmpCreateFile();
	char reply[200] = { 0 };
	PIP_OPTION_INFORMATION info = NULL;
	//WriteInfo("icmp echo clientIp = 0x%X, serverIp = 0x%X", clientIp, serverIp);
	
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
		time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,		\
		time.tm_hour, time.tm_min, time.tm_sec);

	std::string tmpPCName = utils::WStrToStr(m_pParamManager->GetLocalName());
	
	snprintf(bloodheart, sizeof(bloodheart), str, ipLocalStr, \
		"success", tmpPCName.c_str(), timeStr);


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
	
	//unsigned long ul = 1;
	//ioctlsocket(socketFD, FIONBIO, &ul);
	int nNetTimeout = 1000;
	setsockopt(socketFD, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	//接收时限
	setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

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

	send(socketFD, bloodheart, strlen(bloodheart), 0);// MSG_DONTROUTE);
	char tmpRecv[1024] = { 0 };
	recv(socketFD, tmpRecv, sizeof(tmpRecv), 0);// MSG_PEEK);
	TRACE1("link test recv = %s\n", tmpRecv);
	closesocket(socketFD);
	socketFD = -1;

	WSACleanup();

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

void CNetworkTask::run()
{
	std::chrono::system_clock::time_point preBlood = std::chrono::system_clock::now();
	message tmpMsg;
	while (m_bThreadStatus)
	{
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::microseconds>(now - preBlood).count() >= 60 * 1000)
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

		//const char *ftpName = "siyuanxu";
		//const char *ftpPasswd = "yuan";

#if _DEBUG
		const wchar_t *filename = L"config.txt";
		std::vector<std::wstring> *ftpParam = m_pParamManager->GetFtpParameter();

		if ((ftpParam != nullptr) && (ftpParam->size() >= 3))
		{
			if (false == ftpUpload(m_pParamManager->GetServerIP(),
				ftpParam->at(0).c_str(), ftpParam->at(1).c_str(), ftpParam->at(2).c_str(), filename))
			{
				WriteError("user = %s, passwd = %s,upload File = %s to serverIp = %u Failed", ftpParam->at(0).c_str(), ftpParam->at(1).c_str(), filename, m_pParamManager->GetServerIP());
			}
		}

		const wchar_t *downloadFile = L"config_copy.txt";

		if ((ftpParam != nullptr) && (ftpParam->size() >= 3))
		{
			if (false == ftpDownload(m_pParamManager->GetServerIP(),
				ftpParam->at(0).c_str(), ftpParam->at(1).c_str(), ftpParam->at(2).c_str(), downloadFile))
			{
				WriteError("user = %s, passwd = %s,download File = %s to serverIp = %u Failed", ftpParam->at(0).c_str(), ftpParam->at(1).c_str(), downloadFile, m_pParamManager->GetServerIP());
			}
		}
#endif // _DEBUG
		


		//CLOCKS_PER_SEC

		//////////////////////////////
		std::this_thread::sleep_for(std::chrono::seconds(2));
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

bool CNetworkTask::__sendToServer()
{

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
	//建立连接  
	pInternetSession = new CInternetSession(AfxGetAppName());
	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);      // 5秒的连接超时
	pInternetSession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);           // 1秒的发送超时
	pInternetSession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 7000);        // 7秒的接收超时
	pInternetSession->SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1000);     // 1秒的发送超时
	pInternetSession->SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);       // 7秒的接收超时
	pInternetSession->SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);          // 1次重试
	//服务器的ip地址
	CString strADddress;
	strADddress.Format(L"%u.%u.%u.%u", ((serverIp & 0xFF000000) >> 24), \
		((serverIp & 0xFF0000) >> 16), ((serverIp & 0xFF00) >> 8), \
		(serverIp & 0xFF));

	//用户名与密码  
	//CString strUserName(name);
	//CString strPwd(passwd);
	//服务器的目录  
	//CString strDir = L"/home/ftpImage/files"; //若要设置为服务器的根目录，则使用"\\"就可  这个目录是你ftp里面的文件夹目录  
						   //创建了一个CFtpConnection对象，之后就可以通过这个对象进行上传文件，下载文件了  
	pFtpConnection = pInternetSession->GetFtpConnection(strADddress, name, passwd);
	if (pFtpConnection == NULL)
	{
		WriteError("usr = %s, passwd = %s connect Failed", name, passwd);
		return false;
	}
	//设置服务器的目录  
	bool bRetVal = pFtpConnection->SetCurrentDirectory(ftpDir);
	if (bRetVal == false)
	{
		AfxMessageBox(L"目录设置失败");
		return false;
	}
	else
	{
		//把本地文件上传到服务器上  
		//CString strLocalFile(fileName);
		//CString strRemoteFile(fileName);
		pFtpConnection->PutFile(fileName, fileName);
	}
	//释放资源  
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
	//建立连接
	pInternetSession = new CInternetSession(AfxGetAppName());
	//服务器的ip地址
	//若要设置为服务器的根目录，则使用"\\"就可以了  
	//创建了一个CFtpConnection对象，之后就可以通过这个对象进行上传文件，下载文件了  
	CString strADddress;
	strADddress.Format(L"%u.%u.%u.%u", ((serverIp & 0xFF000000) >> 24), \
		((serverIp & 0xFF0000) >> 16), ((serverIp & 0xFF00) >> 8), \
		(serverIp & 0xFF));

	//用户名与密码  
	//CString strUserName(name);
	//CString strPwd(passwd);

	
	pFtpConnection = pInternetSession->GetFtpConnection(strADddress, name, passwd);
	//设置服务器的目录
	bool bRetVal = pFtpConnection->SetCurrentDirectory(ftpDir);
	if (bRetVal == false)
	{
		AfxMessageBox(L"目录设置失败");
		return false;
	}
	else
	{
		//CString strLocalFile(fileName);
		//CString strRemoteFile(fileName);
		pFtpConnection->GetFile(fileName, fileName);
	}
	//释放源  
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