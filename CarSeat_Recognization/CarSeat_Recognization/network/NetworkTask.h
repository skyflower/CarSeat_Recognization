#pragma once

#include <iostream>
#include <Windows.h>
#include <winsock.h>
#include "../common/ParamManager.h"
#include <mutex>
#include <thread>
#include "../image/Camera.h"


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

	bool __sendToServer(unsigned int serverIp, int port, const char *sendMsg, \
		size_t sendMsgLen, char *recvMsg, size_t &recvMsgLen);

	/*
	function: 获取网络条形码的接口,

	parameter:
		ip: 通信ip
		port: 通信端口

	return:
		获取到的条形码字符串，解析之前的字符串
	*/
	std::wstring getBarcodeByNet(unsigned int ip, unsigned int port);

	/*
	function: 启动相机拍照程序

	parameter:
	lineID		产线编号

	return:  返回照片路径
	*/
	std::wstring TakeImage(std::wstring lineID);

	/*
	function: 图像识别过程

	parameter:
		图像路径

	return:  返回识别结果
	*/
	std::wstring __ImageClassify(std::wstring &path);
	

	std::mutex m_MutexMsg;
	message *m_pMsgQueue;


	size_t m_nMsgSize;
	int m_nIn;
	int m_nOut;

	static CNetworkTask *m_pInstance;
	bool m_bThreadStatus;
	CCamera m_Camera;

	CParamManager *m_pParamManager;
};

