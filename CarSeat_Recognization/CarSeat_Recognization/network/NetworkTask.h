#pragma once

#include <iostream>
#include <Windows.h>
#include <winsock.h>
#include "../common/ParamManager.h"
#include <mutex>
#include <thread>
#include <unordered_set>
#include "../image/ImageClassify.h"
#include "../Camera/CameraManager.h"
#include "../Camera/LineCamera.h"
#include "../Camera/Camera.h"
#include "../common/RecogResultManager.h"



/*
������ʷʶ���������������Լ�����������ʧ��ʱ��������ʷʶ������������״̬�ŵ�ʱ���͵�������

��������ļ���config.txt�е�sendFailedRecog�ֶ�

*/

class CNetworkTask
{
public:
	
	struct message
	{
		//std::mutex mMutex;
		unsigned int serverIp;
		unsigned int serverPort;
		unsigned int mLine;
		RecogResult mRecogResult;
	};
	CNetworkTask();
	~CNetworkTask();
	static bool IsReachable(unsigned int clientIp, unsigned int serverIp);
	bool heartBlood(unsigned int serverIp, unsigned int port);

	void SendMessageTo(message* msg);
	

	bool GetThreadStatus();
	void SetThreadStatus(bool status);

	//bool GetLinkStatus();

	//void SetImageClassify(CImageClassify *pClassify);

	//std::wstring GetCurrentImagePath();
	//std::wstring GetCurrentBarcode();

	void run();
	static CNetworkTask* GetInstance();
	//bool ftpUpload(unsigned int serverIp, const wchar_t *name, const wchar_t *passwd, const wchar_t *ftpDir, const wchar_t *fileName);
	//bool ftpDownload(unsigned int serverIp, const wchar_t *name, const wchar_t *passwd, const wchar_t *ftpDir, const wchar_t *fileName);

	

private:
	enum msg
	{
		MAX_MSG_SIZE = 20
	};

	bool __sendToServer(unsigned int serverIp, int port, const char *sendMsg, \
		size_t sendMsgLen, char *recvMsg, size_t &recvMsgLen);

	

	/*
	function: ��ȡ����������Ľӿ�,

	parameter:
		ip: ͨ��ip
		port: ͨ�Ŷ˿�

	return:
		��ȡ�����������ַ���������֮ǰ���ַ���
	*/

	//std::wstring getBarcodeByNet(unsigned int ip, unsigned int port);

	/*
	function: ����������ճ���

	parameter:
	lineID		���߱��

	return:  ������Ƭ·��
	*/
	//std::string TakeImage(std::string lineID);

	/*
	function: ��ͼ���͵�ʶ��ģ����

	parameter:
		ͼ��·��

	*/
	//bool __ImageClassify(std::wstring &path);
	

	std::mutex m_MutexMsg;
	message *m_pMsgQueue;

	//����û�гɹ����͵���ʷʶ���¼
	std::unordered_set<message> *m_pMsgList;

	size_t m_nMsgSize;
	int m_nIn;
	int m_nOut;

	//std::wstring m_szBarCode;
	//std::wstring m_szImagePath;


	static CNetworkTask *m_pInstance;
	bool m_bThreadStatus;
	//CCamera m_Camera;

	CParamManager *m_pParamManager;

	
	//CImageClassify *m_pClassify;
};

