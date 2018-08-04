
// BasicDemoDlg.h : header file
// 

#pragma once

#include <iostream>
#include "Camera.h"
#include <Windows.h>


class CCameraManager
{
// Construction
public:
	static CCameraManager*GetInstance();
	~CCameraManager();

	int GetCameraCount();

	MV_CC_DEVICE_INFO* GetCamera(int index);

	int GetCameraIndexByName(const char *name);

	MV_CC_DEVICE_INFO* GetCamera(const char *name);

	void testPrint();

private:
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // ch:设备信息列表结构体变量，用来存储设备列表
                                               
	CCameraManager();	// Standard constructor

	static CCameraManager *m_pInstance;
public:
    /*ch:初始化 | en:Initialization*/
    bool EnumCamera();               // ch:查找设备 | en:Find Devices
    
};
