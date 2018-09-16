
// BasicDemoDlg.cpp : implementation file
// 

#include "../stdafx.h"
#include "CameraManager.h"
#include "../common/Log.h"
#include "../common/utils.h"
#include "./Header/EDSDK.h"
 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CCameraManager *CCameraManager::m_pInstance = nullptr;
std::mutex CCameraManager::m_Mutex;

CCameraManager::CCameraManager()
{
	isSDKLoaded = false;
	m_stDevList = nullptr;

	//memset(&m_stDevList, 0, sizeof(m_stDevList));

	EdsError	 err = EDS_ERR_OK;
	// Initialization of SDK
	err = EdsInitializeSDK();

	if (err == EDS_ERR_OK)
	{
		isSDKLoaded = true;
	}
	//m_Mutex = 
}

CCameraManager * CCameraManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
		if (lock.try_lock())
		{
			m_pInstance = new CCameraManager();
		}
	}
	return m_pInstance;
}

CCameraManager::~CCameraManager()
{
	if (m_stDevList != nullptr)
	{
		EdsRelease(m_stDevList);
	}
	if (isSDKLoaded)
	{
		EdsTerminateSDK();
	}
}


// ch:按下查找设备按钮:枚举 | en:Click Find Device button:Enumeration 
bool  CCameraManager::EnumCamera()
{
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return false;
	}
    //CString strMsg;
    // ch:初始化设备信息列表 | en:Device Information List Initialization
    //memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
    //int nRet = CCamera::EnumDevices(&m_stDevList);
	int err = EdsGetCameraList(&m_stDevList);
    if (EDS_ERR_OK != err)
    {
        return false;
    }
	lock.unlock();

#ifdef _DEBUG
	testPrint();
#endif // _DEBUG


	

    // ch:将值加入到信息列表框中并显示出来 | en:Add value to the information list box and display
    /*unsigned int i = 0;
    int nIp1 = 0, nIp2 = 0, nIp3 = 0, nIp4 = 0;
    for (i = 0; i < m_stDevList.nDeviceNum; i++)
    {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (NULL == pDeviceInfo)
        {
            continue;
        }
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
        {
            nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
            nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
            nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
            nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

            wchar_t* pUserName = NULL;
            if (strcmp("", (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0)
            {
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
            }
            else
            {
                char strUserName[256] = {0};
                sprintf_s(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]GigE:    %s  (%d.%d.%d.%d)"), i, 
                pUserName, nIp1, nIp2, nIp3, nIp4);
            if (NULL != pUserName)
            {
                delete(pUserName);
                pUserName = NULL;
            }

        }
        else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
        {
            wchar_t* pUserName = NULL;

            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
            {
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
            }
            else
            {
                char strUserName[256];
                sprintf_s(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]UsbV3:  %s"), i, pUserName);
            if (NULL != pUserName)
            {
                delete(pUserName);
                pUserName = NULL;
            }
        }
    }
*/
	if (err == EDS_ERR_OK)
	{
		EdsUInt32 count = 0;
		err = EdsGetChildCount(m_stDevList, &count);
		if (count == 0)
		{
			err = EDS_ERR_DEVICE_NOT_FOUND;
		}
	}
    return true;
}



int CCameraManager::GetCameraCount()
{
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return 0;
	}
	if (m_stDevList != nullptr)
	{
		EdsUInt32 count = 0;
		int err = EdsGetChildCount(m_stDevList, &count);
		if (count == 0)
		{
			err = EDS_ERR_DEVICE_NOT_FOUND;
		}
		return count;
	}
	return 0;
}

EdsCameraRef CCameraManager::GetCamera(int index)
{
	if (index < 0)
	{
		return nullptr;
	}
	unsigned int tmpIndex = index;
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return nullptr;
	}
	if (m_stDevList == nullptr)
	{
		return nullptr;
	}
	EdsUInt32 count = 0;
	int err = EdsGetChildCount(m_stDevList, &count);
	
	if (count <= tmpIndex)
	{
		return nullptr;
	}
	EdsCameraRef camera = NULL;
	err = EdsGetChildAtIndex(m_stDevList, index, &camera);

	return camera;
}

int CCameraManager::GetCameraIndexByName(const char * name)
{
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return -1;
	}
	if (m_stDevList == nullptr)
	{
		return -1;
	}
	EdsUInt32 count = 0;
	int err = EdsGetChildCount(m_stDevList, &count);

	if (count == 0)
	{
		return -1;
	}
	for (int i = 0; i < count; ++i)
	{
		EdsCameraRef camera = NULL;
		EdsDeviceInfo deviceInfo;
		err = EdsGetChildAtIndex(m_stDevList, i, &camera);
		if ((err != EDS_ERR_OK) || (camera == NULL))
		{
			continue;
		}

		err = EdsGetDeviceInfo(camera, &deviceInfo);

		if (strncmp(deviceInfo.szDeviceDescription, name, strlen(name)) == 0)
		{
			return i;
		}
	}
	return -1;
}

const char * CCameraManager::GetDesriptorByIndex(int index)
{
	static char desc[MAX_CHAR_LENGTH];
	memset(desc, 0, sizeof(desc));

	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return desc;
	}
	EdsUInt32 count = 0;
	int err = EdsGetChildCount(m_stDevList, &count);
	if ((count == 0) || (index >= count))
	{
		err = EDS_ERR_DEVICE_NOT_FOUND;
	}
	
	EdsCameraRef camera = NULL;
	err = EdsGetChildAtIndex(m_stDevList, index, &camera);

	if (camera == nullptr)
	{
		return desc;
	}
	EdsDeviceInfo deviceInfo;
	memset(&deviceInfo, 0, sizeof(EdsDeviceInfo));
	EdsGetDeviceInfo(camera, &deviceInfo);

	sprintf_s(desc, sizeof(desc), "%s_%u", deviceInfo.szDeviceDescription, deviceInfo.deviceSubType);
		
	return desc;
}

EdsDeviceInfo CCameraManager::GetDeviceInfoByIndex(int index)
{
	EdsDeviceInfo deviceInfo;
	memset(&deviceInfo, 0, sizeof(EdsDeviceInfo));
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return deviceInfo;
	}
	EdsUInt32 count = 0;
	int err = EdsGetChildCount(m_stDevList, &count);
	if ((count == 0) || (index >= count))
	{
		err = EDS_ERR_DEVICE_NOT_FOUND;
	}
	
	EdsCameraRef camera = NULL;
	err = EdsGetChildAtIndex(m_stDevList, index, &camera);
	if (camera == nullptr)
	{
		return deviceInfo;
	}
	
	EdsGetDeviceInfo(camera, &deviceInfo);
	return deviceInfo;
}

void CCameraManager::testPrint()
{
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return;
	}
	EdsUInt32 count = 0;
	int err = EdsGetChildCount(m_stDevList, &count);
	if (count == 0)
	{
		err = EDS_ERR_DEVICE_NOT_FOUND;
	}
	//m_stDevList;
	unsigned int i = 0;
	
	while(i < count)
	{
		EdsCameraRef camera = NULL;
		int err = EdsGetChildAtIndex(m_stDevList, i, &camera);
		
		if (camera == nullptr)
		{
			continue;
		}
		EdsDeviceInfo deviceInfo;
		memset(&deviceInfo, 0, sizeof(EdsDeviceInfo));
		EdsGetDeviceInfo(camera, &deviceInfo);

		WriteInfo("index = %u, deviceSubType = %u", i, deviceInfo.deviceSubType);
		WriteInfo("index = %u, deviceDesc = %s", i, deviceInfo.szDeviceDescription);
		WriteInfo("index = %u, devicePortName = %s", i, deviceInfo.szPortName);
		++i;
	}
}
