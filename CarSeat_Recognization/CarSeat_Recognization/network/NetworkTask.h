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
发送历史识别结果到服务器，以及当网络连接失败时，保存历史识别结果，等网络状态号的时候发送到服务器

相关配置文件在config.txt中的sendFailedRecog字段

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
	function: 获取网络条形码的接口,

	parameter:
		ip: 通信ip
		port: 通信端口

	return:
		获取到的条形码字符串，解析之前的字符串
	*/

	//std::wstring getBarcodeByNet(unsigned int ip, unsigned int port);

	/*
	function: 启动相机拍照程序

	parameter:
	lineID		产线编号

	return:  返回照片路径
	*/
	//std::string TakeImage(std::string lineID);

	/*
	function: 将图像发送到识别模块中

	parameter:
		图像路径

	*/
	//bool __ImageClassify(std::wstring &path);
	

	std::mutex m_MutexMsg;
	message *m_pMsgQueue;

	//缓存没有成功发送的历史识别记录
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

