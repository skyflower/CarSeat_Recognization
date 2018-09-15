
// CarSeat_Recognization.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once
 
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "./common/ParamManager.h"
#include "./common/Log.h"
#include "./network/NetworkTask.h"
#include <thread>
#include <chrono>
#include "./image/ImageClassify.h"
#include "./common/LabelManager.h"
#include "./Camera/Camera/CameraController.h"
#include "./Camera/Camera/CameraModel.h"
#include "./Camera/Camera/CameraModelLegacy.h"


// CCarSeat_RecognizationApp: 
// 有关此类的实现，请参阅 CarSeat_Recognization.cpp
//

class CCarSeat_RecognizationApp : public CWinApp
{
public:
	CCarSeat_RecognizationApp();

// 重写
public:
	virtual BOOL InitInstance();
	

private:


	bool LoginSystem();
	
	void initSystem();


	//销毁系统
	void DeInitSystem();

	CameraModel* cameraModelFactory(EdsCameraRef camera, EdsDeviceInfo deviceInfo);


// 实现
	CLog *m_pLog;
	CParamManager *m_pParamManager;
	CNetworkTask *m_pNetworkTask;
	std::thread m_NetworkThread;
	
	CImageClassify *m_pClassify;
	//std::thread m_pClassifyThread;
	CLabelManager *m_pLabelManager;

	CCameraManager *m_pCameraManager;


	CameraModel*		_model;
	CameraController*	_controller;

	std::thread m_UIThread;


	DECLARE_MESSAGE_MAP()
};

extern CCarSeat_RecognizationApp theApp;