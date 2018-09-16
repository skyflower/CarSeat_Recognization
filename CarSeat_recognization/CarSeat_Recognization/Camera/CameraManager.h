
// BasicDemoDlg.h : header file
// 
 
#pragma once 

#include <iostream>
//#include "Camera.h"
//#include <Windows.h>
#include <mutex>
#include "./Header/EDSDK.h"


class CCameraManager
{
// Construction
public:
	static CCameraManager*GetInstance();
	~CCameraManager();

	int GetCameraCount();

	EdsCameraRef GetCamera(int index);

	int GetCameraIndexByName(const char *name);

	const char* GetDesriptorByIndex(int index);

	EdsDeviceInfo GetDeviceInfoByIndex(int index);

	void testPrint();

	/*ch:初始化 | en:Initialization*/
	bool EnumCamera();               // ch:查找设备 | en:Find Devices

private:
	EdsCameraListRef m_stDevList;         // ch:设备信息列表结构体变量，用来存储设备列表
                                               
	CCameraManager();	// Standard constructor

	static CCameraManager *m_pInstance;

	static std::mutex m_Mutex;

	bool isSDKLoaded;
    
};
