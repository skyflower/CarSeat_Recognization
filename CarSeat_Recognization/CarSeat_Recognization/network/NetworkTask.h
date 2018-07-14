#pragma once

#include <iostream>
#include <Windows.h>
#include <winsock.h>
#include "../common/ParamManager.h"
#include <mutex>
#include <thread>
#include "../image/ImageClassify.h"
#include "../Camera/CameraManager.h"
#include "../Camera/LineCamera.h"
#include "../Camera/Camera.h"


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

	void SetImageClassify(CImageClassify *pClassify);

	std::wstring GetCurrentImagePath();
	std::wstring GetCurrentBarcode();

	void run();
	static CNetworkTask* GetInstance();
	bool ftpUpload(unsigned int serverIp, const wchar_t *name, const wchar_t *passwd, const wchar_t *ftpDir, const wchar_t *fileName);
	bool ftpDownload(unsigned int serverIp, const wchar_t *name, const wchar_t *passwd, const wchar_t *ftpDir, const wchar_t *fileName);

	bool __sendToServer(unsigned int serverIp, int port, const char *sendMsg, \
		size_t sendMsgLen, char *recvMsg, size_t &recvMsgLen);

private:
	enum msg
	{
		MAX_MSG_SIZE = 20
	};

	

	/*
	function: ��ȡ����������Ľӿ�,

	parameter:
		ip: ͨ��ip
		port: ͨ�Ŷ˿�

	return:
		��ȡ�����������ַ���������֮ǰ���ַ���
	*/
	std::wstring getBarcodeByNet(unsigned int ip, unsigned int port);

	/*
	function: ����������ճ���

	parameter:
	lineID		���߱��

	return:  ������Ƭ·��
	*/
	std::wstring TakeImage(std::wstring lineID);

	/*
	function: ��ͼ���͵�ʶ��ģ����

	parameter:
		ͼ��·��

	*/
	bool __ImageClassify(std::wstring &path);
	

	std::mutex m_MutexMsg;
	message *m_pMsgQueue;

	size_t m_nMsgSize;
	int m_nIn;
	int m_nOut;

	std::wstring m_szBarCode;
	std::wstring m_szImagePath;


	static CNetworkTask *m_pInstance;
	bool m_bThreadStatus;
	//CCamera m_Camera;

	CParamManager *m_pParamManager;
	CImageClassify *m_pClassify;
};

