#include "../stdafx.h"
#include "Camera.h"
#include <string.h>

CCamera::CCamera()
{
    m_hDevHandle = NULL; 
	m_pBufForSaveImage = nullptr;
	m_nBufSizeForSaveImage = 0;

	m_pBufForDriver = nullptr;
	m_nBufSizeForDriver = 0;
}

CCamera::~CCamera()
{
    if (m_hDevHandle)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle    = NULL;
    }
}

int CCamera::EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList)
{
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, pstDevList);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    return MV_OK;
}

void * CCamera::GetHandle()
{
	return m_hDevHandle;
}

// ch:���豸 | en:Open Device
int     CCamera::Open(MV_CC_DEVICE_INFO* pstDeviceInfo)
{
    if (NULL == pstDeviceInfo)
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_OK;
    if(m_hDevHandle == NULL)
    {
        nRet  = MV_CC_CreateHandle(&m_hDevHandle, pstDeviceInfo);
        if (MV_OK != nRet)
        {
            return nRet;
        }
    }

    nRet = MV_CC_OpenDevice(m_hDevHandle);
    if (MV_OK != nRet)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = NULL;

        return nRet;
    }

    return MV_OK;
}


// ch:�ر��豸 | en:Close Device
int     CCamera::Close()
{
    int nRet = MV_OK;

    if (NULL == m_hDevHandle)
    {
        return MV_E_PARAMETER;
    }

    MV_CC_CloseDevice(m_hDevHandle);
    nRet = MV_CC_DestroyHandle(m_hDevHandle);
    m_hDevHandle = NULL;

    return nRet;
}


// ch:����ץͼ | en:Start Grabbing
int     CCamera::StartGrabbing()
{
    return MV_CC_StartGrabbing(m_hDevHandle);
}


// ch:ֹͣץͼ | en:Stop Grabbing
int     CCamera::StopGrabbing()
{
    return MV_CC_StopGrabbing(m_hDevHandle);
}

int     CCamera::GetOneFrameTimeout(unsigned char* pData, unsigned int* pnDataLen, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec)
{
    if (NULL == pnDataLen)
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_OK;

    *pnDataLen  = 0;

    nRet = MV_CC_GetOneFrameTimeout(m_hDevHandle, pData, nDataSize, pFrameInfo, nMsec);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    *pnDataLen = pFrameInfo->nFrameLen;

    return nRet;
}


// ch:������ʾ���ھ�� | en:Set Display Window Handle
int     CCamera::Display(void* hWnd)
{
    return MV_CC_Display(m_hDevHandle, hWnd);
}

bool CCamera::SetBalanceWhile(MV_CAM_BALANCEWHITE_AUTO balanceWhile)
{
	unsigned int nValue = balanceWhile; //һ�ΰ�ƽ��ģʽ
	int nRet = MV_CC_SetBalanceWhiteAuto(m_hDevHandle, nValue);
	//unsigned int nValue = MV_BALANCEWHITE_AUTO_ONCE; //һ�ΰ�ƽ��ģʽ
	//nRet = MV_CC_SetBalanceWhiteAuto(m_handle, nValue);

	if (MV_OK != nRet)
	{
		//printf("error: SetBalanceWhiteAuto fail [%x]\n", nRet);
		return false;
	}
	return true;
}

MV_CAM_BALANCEWHITE_AUTO CCamera::GetBalanceWhile()
{
	MVCC_ENUMVALUE struEnumValue = { 0 };
	int nRet = MV_CC_GetBalanceWhiteAuto(m_hDevHandle, &struEnumValue);
	if (MV_OK != nRet)
	{
		//printf("error: GetBalanceWhiteAuto fail [%x]\n", nRet);
		return MV_BALANCEWHITE_AUTO_UNKNOWN;
	}
	return (MV_CAM_BALANCEWHITE_AUTO)struEnumValue.nCurValue;
}


int CCamera::SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam)
{
    if (NULL == pstParam)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_SaveImageEx2(m_hDevHandle, pstParam);
}

// ch:ע��ͼ�����ݻص� | en:Register Image Data CallBack
int CCamera::RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, 
                                                                void* pUser),void* pUser)
{
    return MV_CC_RegisterImageCallBackEx(m_hDevHandle, cbOutput, pUser);
}


// ch:ע����Ϣ�쳣�ص� | en:Register Message Exception CallBack
int     CCamera::RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType, void* pUser),void* pUser)
{
    return MV_CC_RegisterExceptionCallBack(m_hDevHandle, cbException, pUser);
}


// ch:��ȡInt�Ͳ������� Width��Height����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::GetIntValue(IN const char* strKey, OUT unsigned int *pnValue)
{
    if (NULL == strKey || NULL == pnValue)
    {
        return MV_E_PARAMETER;
    }

    MVCC_INTVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    int nRet = MV_CC_GetIntValue(m_hDevHandle, strKey, &stParam);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    *pnValue = stParam.nCurValue;

    return MV_OK;
}


// ch:����Int�Ͳ������� Width��Height����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Set Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::SetIntValue(IN const char* strKey, IN unsigned int nValue)
{
    if (NULL == strKey)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_SetIntValue(m_hDevHandle, strKey, nValue);
}


// ch:��ȡFloat�Ͳ������� ExposureTime��Gain����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::GetFloatValue(IN const char* strKey, OUT float *pfValue)
{
    if (NULL == strKey || NULL == pfValue)
    {
        return MV_E_PARAMETER;
    }

    MVCC_FLOATVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_FLOATVALUE));
    int nRet = MV_CC_GetFloatValue(m_hDevHandle, strKey, &stParam);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    *pfValue = stParam.fCurValue;

    return MV_OK;
}


// ch:����Float�Ͳ������� ExposureTime��Gain����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Set Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::SetFloatValue(IN const char* strKey, IN float fValue)
{
    if (NULL == strKey)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}


// ch:��ȡEnum�Ͳ������� PixelFormat����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Get Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::GetEnumValue(IN const char* strKey, OUT unsigned int *pnValue)
{
    if (NULL == strKey || NULL == pnValue)
    {
        return MV_E_PARAMETER;
    }

    MVCC_ENUMVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_ENUMVALUE));
    int nRet = MV_CC_GetEnumValue(m_hDevHandle, strKey, &stParam);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    *pnValue = stParam.nCurValue;

    return MV_OK;
}


// ch:����Enum�Ͳ������� PixelFormat����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Set Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::SetEnumValue(IN const char* strKey, IN unsigned int nValue)
{
    if (NULL == strKey)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_SetEnumValue(m_hDevHandle, strKey, nValue);
}


// ch:��ȡBool�Ͳ������� ReverseX����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::GetBoolValue(IN const char* strKey, OUT bool *pbValue)
{
    if (NULL == strKey || NULL == pbValue)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
}


// ch:����Bool�Ͳ������� ReverseX����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Set Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::SetBoolValue(IN const char* strKey, IN bool bValue)
{
    if (NULL == strKey)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
}


// ch:��ȡString�Ͳ������� DeviceUserID����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�UserSetSave
// en:Get String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::GetStringValue(IN const char* strKey, IN OUT char* strValue, IN unsigned int nSize)
{
    if (NULL == strKey || NULL == strValue)
    {
        return MV_E_PARAMETER;
    }

    MVCC_STRINGVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_STRINGVALUE));
    int nRet = MV_CC_GetStringValue(m_hDevHandle, strKey, &stParam);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    strcpy_s(strValue, nSize, stParam.chCurValue);

    return MV_OK;
}


// ch:����String�Ͳ������� DeviceUserID����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�UserSetSave
// en:Set String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::SetStringValue(IN const char* strKey, IN const char* strValue)
{
    if (NULL == strKey)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_SetStringValue(m_hDevHandle, strKey, strValue);
}


// ch:ִ��һ��Command������� UserSetSave����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Execute Command once, such as UserSetSave, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CCamera::CommandExecute(IN const char* strKey)
{
    if (NULL == strKey)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_SetCommandValue(m_hDevHandle, strKey);
}

int     CCamera::GetAllMatchInfo(MV_ALL_MATCH_INFO* pstInfo)
{
    if (NULL == pstInfo)
    {
        return MV_E_PARAMETER;
    }

    return MV_CC_GetAllMatchInfo(m_hDevHandle,pstInfo);;
}
