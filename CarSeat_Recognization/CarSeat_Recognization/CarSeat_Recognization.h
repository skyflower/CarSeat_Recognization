
// CarSeat_Recognization.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "./common/ParamManager.h"
#include "./common/Log.h"
#include "./network/NetworkTask.h"
#include <thread>
#include <chrono>
#include "./image/ImageClassify.h"


// CCarSeat_RecognizationApp: 
// �йش����ʵ�֣������ CarSeat_Recognization.cpp
//

class CCarSeat_RecognizationApp : public CWinApp
{
public:
	CCarSeat_RecognizationApp();

// ��д
public:
	virtual BOOL InitInstance();

private:

// ʵ��
	CLog *m_pLog;
	CParamManager *m_pParamManager;
	CNetworkTask *m_pNetworkTask;
	std::thread m_NetworkThread;
	
	CImageClassify *m_pClassify;
	std::thread m_pClassifyThread;

	std::thread m_UIThread;


	DECLARE_MESSAGE_MAP()
};

extern CCarSeat_RecognizationApp theApp;