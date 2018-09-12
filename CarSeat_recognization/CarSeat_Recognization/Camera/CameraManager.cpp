
// BasicDemoDlg.cpp : implementation file
// 

#include "../stdafx.h"
#include "CameraManager.h"
#include "../common/Log.h"
#include "../common/utils.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CCameraManager *CCameraManager::m_pInstance = nullptr;
std::mutex CCameraManager::m_Mutex;

CCameraManager::CCameraManager()
{
	memset(&m_stDevList, 0, sizeof(m_stDevList));
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

}


// ch:按下查找设备按钮:枚举 | en:Click Find Device button:Enumeration 
bool  CCameraManager::EnumCamera()
{
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return false;
	}
    CString strMsg;
    // ch:初始化设备信息列表 | en:Device Information List Initialization
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
    int nRet = CCamera::EnumDevices(&m_stDevList);
    if (MV_OK != nRet)
    {
        return false;
    }

#ifdef _DEBUG
	testPrint();
#endif // _DEBUG


	

    // ch:将值加入到信息列表框中并显示出来 | en:Add value to the information list box and display
    unsigned int i = 0;
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

    if (0 == m_stDevList.nDeviceNum)
    {
        return false;
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
	return m_stDevList.nDeviceNum;
}

MV_CC_DEVICE_INFO * CCameraManager::GetCamera(int index)
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
	
	if (tmpIndex >= m_stDevList.nDeviceNum)
	{
		return nullptr;
	}
	return m_stDevList.pDeviceInfo[tmpIndex];
}

int CCameraManager::GetCameraIndexByName(const char * name)
{
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return -1;
	}
	if (m_stDevList.nDeviceNum <= 0)
	{
		return -1;
	}
	char tmpMac[20] = { 0 };
	char tmpName[20] = { 0 };
	memset(tmpName, 0, sizeof(tmpName));

	for (int i = 0; (i < strlen(name)) && (i < sizeof(tmpName)); ++i)
	{
		tmpName[i] = tolower(name[i]);
	}
	tmpName[sizeof(tmpName) - 1] = '\0';
	
	for (unsigned int i = 0; i < m_stDevList.nDeviceNum; ++i)
	{
		memset(tmpMac, 0, sizeof(tmpMac));
		sprintf_s(tmpMac, sizeof(tmpMac), "%x%x", \
			m_stDevList.pDeviceInfo[i]->nMacAddrHigh, \
			m_stDevList.pDeviceInfo[i]->nMacAddrLow);
		tmpMac[sizeof(tmpMac) - 1] = '\0';
		size_t length = strlen(tmpMac);
		for (size_t j = 0; j < length; ++j)
		{
			tmpMac[j] = tolower(tmpMac[j]);
		}
		if (strncmp(tmpName, tmpMac, length) == 0)
		{
			return i;
		}
	}
	return -1;
}

MV_CC_DEVICE_INFO * CCameraManager::GetCamera(const char * name)
{
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return false;
	}
	int index = GetCameraIndexByName(name);
	if (index == -1)
	{
		return nullptr;
	}
	return GetCamera(index);
}

void CCameraManager::testPrint()
{
	std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	if (lock.try_lock() == false)
	{
		return;
	}
	//m_stDevList;
	unsigned int i = 0;
	while(i < m_stDevList.nDeviceNum)
	{
		WriteInfo("index = %d, Ver = 0x%X.%x", i, m_stDevList.pDeviceInfo[i]->nMajorVer, m_stDevList.pDeviceInfo[i]->nMinorVer);
		WriteInfo("index = %d, MAC = 0x%X.%x", i, m_stDevList.pDeviceInfo[i]->nMacAddrHigh, m_stDevList.pDeviceInfo[i]->nMacAddrLow);
		WriteInfo("index = %d, DeviceVer = %s", i, m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chDeviceVersion);
		WriteInfo("index = %d, ManufacturerName = %s", i, m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chManufacturerName);
		WriteInfo("index = %d, ManufacturerSpecificInfo = %s", i, m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chManufacturerSpecificInfo);
		WriteInfo("index = %d, ModelName = %s", i, m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chModelName);
		WriteInfo("index = %d, SerialNumber = %s", i, m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber);
		WriteInfo("index = %d, UserDefinedName = %s", i, m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chUserDefinedName);
		WriteInfo("index = %d, CurrentIp = %u", i, m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp);
		++i;
	}
}
