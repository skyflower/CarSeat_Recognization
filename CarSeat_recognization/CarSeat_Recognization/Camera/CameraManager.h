
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
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // ch:�豸��Ϣ�б�ṹ������������洢�豸�б�
                                               
	CCameraManager();	// Standard constructor

	static CCameraManager *m_pInstance;
public:
    /*ch:��ʼ�� | en:Initialization*/
    bool EnumCamera();               // ch:�����豸 | en:Find Devices
    
};
