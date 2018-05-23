#pragma once

#include <iostream>
#include <Windows.h>
#include <winsock.h>
#include "../common/ParamManager.h"
#include <mutex>
#include <thread>


class CNetworkTask
{
public:
	enum class MessageType
	{
		NETWORK,
		THREAD
	};
	struct message
	{
		unsigned int serverIp;
		unsigned int serverPort;
		unsigned int mLine;
		char data[2000];
	};
	CNetworkTask();
	~CNetworkTask();
	static bool IsReachable(unsigned int clientIp, unsigned int serverIp);
	bool heartBlood(unsigned int serverIp, unsigned int port);


	void SendMessageTo(message* msg);
	bool GetThreadStatus();
	void SetThreadStatus(bool status);

	void run();
	static CNetworkTask* GetInstance();
	bool ftpUpload(unsigned int serverIp, const wchar_t *name, const wchar_t *passwd, const wchar_t *ftpDir, const wchar_t *fileName);
	bool ftpDownload(unsigned int serverIp, const wchar_t *name, const wchar_t *passwd, const wchar_t *ftpDir, const wchar_t *fileName);

private:
	enum msg
	{
		MAX_MSG_SIZE = 20
	};

	bool __sendToServer(unsigned int serverIp, int port, const char *sendMsg, char *recvMsg);

	std::wstring getBarcodeByNet(unsigned int ip);
	

	std::mutex m_MutexMsg;
	message *m_pMsgQueue;
	size_t m_nMsgSize;
	int m_nIn;
	int m_nOut;

	static CNetworkTask *m_pInstance;
	bool m_bThreadStatus;

	CParamManager *m_pParamManager;
};

