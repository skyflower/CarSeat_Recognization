
// BasicDemoDlg.cpp : implementation file
// 

#include "../stdafx.h"
#include "LineCamera.h"
#include "../common/Log.h"
#include "../common/utils.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CLineCamera::CLineCamera(MV_CC_DEVICE_INFO *pDevice):m_pcMyCamera(NULL)
    , m_bOpenDevice(FALSE)
	, m_pDevice(pDevice)
    , m_bStartGrabbing(false)
    , m_nTriggerMode(MV_TRIGGER_MODE_OFF)
    , m_dExposureTime(0)
    , m_dExposureGain(0)
    , m_dFrameRate(0)
    , m_bSoftWareTriggerCheck(FALSE)
    , m_nSaveImageType(MV_Image_Undefined)
    , m_nTriggerSource(MV_TRIGGER_SOURCE_SOFTWARE)
    , m_pBufForSaveImage(NULL)
    , m_nBufSizeForSaveImage(0)
    , m_pBufForDriver(NULL)
    , m_nBufSizeForDriver(0)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLineCamera::~CLineCamera()
{
	CloseDevice();
}


// ch:��ʾ������Ϣ | en:Show error message
void CLineCamera::ShowErrorMsg(CString csMessage, int nErrorNum)
{
    CString errorMsg;
    if (nErrorNum == 0)
    {
        errorMsg.Format(_T("%s"), csMessage);
    }
    else
    {
        errorMsg.Format(_T("%s: Error = %x: "), csMessage, nErrorNum);
    }

    switch(nErrorNum)
    {
    case MV_E_HANDLE:           errorMsg += "Error or invalid handle ";                                         break;
    case MV_E_SUPPORT:          errorMsg += "Not supported function ";                                          break;
    case MV_E_BUFOVER:          errorMsg += "Cache is full ";                                                   break;
    case MV_E_CALLORDER:        errorMsg += "Function calling order error ";                                    break;
    case MV_E_PARAMETER:        errorMsg += "Incorrect parameter ";                                             break;
    case MV_E_RESOURCE:         errorMsg += "Applying resource failed ";                                        break;
    case MV_E_NODATA:           errorMsg += "No data ";                                                         break;
    case MV_E_PRECONDITION:     errorMsg += "Precondition error, or running environment changed ";              break;
    case MV_E_VERSION:          errorMsg += "Version mismatches ";                                              break;
    case MV_E_NOENOUGH_BUF:     errorMsg += "Insufficient memory ";                                             break;
    case MV_E_ABNORMAL_IMAGE:   errorMsg += "Abnormal image, maybe incomplete image because of lost packet ";   break;
    case MV_E_UNKNOW:           errorMsg += "Unknown error ";                                                   break;
    case MV_E_GC_GENERIC:       errorMsg += "General error ";                                                   break;
    case MV_E_GC_ACCESS:        errorMsg += "Node accessing condition error ";                                  break;
    case MV_E_ACCESS_DENIED:	errorMsg += "No permission ";                                                   break;
    case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected ";                         break;
    case MV_E_NETER:            errorMsg += "Network error ";                                                   break;
    }
	
    //AfxMessageBox(errorMsg, TEXT("PROMPT"), MB_OK | MB_ICONWARNING);
}

// ch:���豸 | en:Open Device
int CLineCamera::OpenDevice(void)
{
    if (TRUE == m_bOpenDevice)
    {
        return STATUS_ERROR;
    }

    // ch:���豸��Ϣ�����豸ʵ�� | en:Device instance created by device information
    if (NULL == m_pDevice)
    {
        ShowErrorMsg(TEXT("Device does not exist"), 0);
        return STATUS_ERROR;
    }

    if (NULL != m_pcMyCamera)
    {
        return STATUS_ERROR;
    }

    m_pcMyCamera = new CCamera;
    if (NULL == m_pcMyCamera)
    {
        return STATUS_ERROR;
    }

    int nRet = m_pcMyCamera->Open(m_pDevice);
    if (MV_OK != nRet)
    {
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
        return nRet;
    }

    m_bOpenDevice = TRUE;
    return MV_OK;
}

// ch:�ر��豸 | en:Close Device
int CLineCamera::CloseDevice(void)
{   
    if (m_pcMyCamera)
    {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
    }

    m_bOpenDevice = FALSE;
    m_bStartGrabbing = FALSE;

    if (m_pBufForDriver)
    {
        free(m_pBufForDriver);
        m_pBufForDriver = NULL;
    }
    m_nBufSizeForDriver = 0;

    if (m_pBufForSaveImage)
    {
        free(m_pBufForSaveImage);
        m_pBufForSaveImage = NULL;
    }
    m_nBufSizeForSaveImage  = 0;
    return MV_OK;
}

// ch:��ȡ����ģʽ | en:Get Trigger Mode
MV_CAM_TRIGGER_MODE CLineCamera::GetTriggerMode()
{
	if (m_nTriggerMode != MV_TRIGGER_MODE_UNKNOWN)
	{
		return m_nTriggerMode;
	}
    unsigned int nEnumValue = 0;

    int nRet = m_pcMyCamera->GetEnumValue("TriggerMode", &nEnumValue);
    if (MV_OK != nRet)
    {
        return MV_TRIGGER_MODE_UNKNOWN;
    }

    
    /*if (MV_TRIGGER_MODE_ON ==  m_nTriggerMode)
    {
        TriggerMode();
    }
    else if (MV_TRIGGER_MODE_OFF == m_nTriggerMode)
    {
        ContinusMode();
    }
    else
    {
		m_nTriggerMode = MV_TRIGGER_MODE_UNKNOWN;
        return MV_TRIGGER_MODE_UNKNOWN;
    }*/
	m_nTriggerMode = (MV_CAM_TRIGGER_MODE)nEnumValue;
    return m_nTriggerMode;
}

// ch:���ô���ģʽ | en:Set Trigger Mode
bool CLineCamera::SetTriggerMode(MV_CAM_TRIGGER_MODE mode)
{
	if (mode == MV_TRIGGER_MODE_UNKNOWN)
	{
		return false;
	}
	
    int nRet = m_pcMyCamera->SetEnumValue("TriggerMode", m_nTriggerMode);
    if (MV_OK != nRet)
    {
        return false;
    }
	m_nTriggerMode = mode;
    return true;
}

// ch:��ȡ�ع�ʱ�� | en:Get Exposure Time
double CLineCamera::GetExposureTime()
{
    return m_dExposureTime;
}

// ch:�����ع�ʱ�� | en:Set Exposure Time
bool CLineCamera::SetExposureTime(double time)
{
    // ch:�����������ع�ģʽ���������ع�ʱ����Ч
    // en:Adjust these two exposure mode to allow exposure time effective
    int nRet = m_pcMyCamera->SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
    if (MV_OK != nRet)
    {
        return false;
    }

    nRet = m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
	if (nRet != MV_OK)
	{
		return false;
	}
    nRet = m_pcMyCamera->SetFloatValue("ExposureTime", time);
    if (MV_OK != nRet)
    {
        return false;
    }
	m_dExposureTime = time;
    return true;
}

// ch:��ȡ���� | en:Get Gain
double CLineCamera::GetGain()
{
    return m_dExposureGain;
}

// ch:�������� | en:Set Gain
bool CLineCamera::SetGain(int gain)
{
    // ch:��������ǰ�Ȱ��Զ�����رգ�ʧ�����践��
    //en:Set Gain after Auto Gain is turned off, this failure does not need to return
    int nRet = m_pcMyCamera->SetEnumValue("GainAuto", 0);
	if (nRet != MV_OK)
	{
		return false;
	}
	nRet = m_pcMyCamera->SetFloatValue("Gain", gain);
	if (nRet != MV_OK)
	{
		return false;
	}

	m_dExposureGain = gain;
	return true;
}

// ch:��ȡ֡�� | en:Get Frame Rate
double CLineCamera::GetFrameRate()
{
    return m_dFrameRate;
}

// ch:����֡�� | en:Set Frame Rate
bool CLineCamera::SetFrameRate(double rate)
{
    int nRet = m_pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
    if (MV_OK != nRet)
    {
        return false;
    }

	nRet = m_pcMyCamera->SetFloatValue("AcquisitionFrameRate", rate);
	if (nRet != MV_OK)
	{
		return false;
	}
	m_dFrameRate = rate;
	return true;
}

// ch:��ȡ����Դ | en:Get Trigger Source
MV_CAM_TRIGGER_SOURCE CLineCamera::GetTriggerSource()
{
    return m_nTriggerSource;
}

// ch:���ô���Դ | en:Set Trigger Source
bool CLineCamera::SetTriggerSource(MV_CAM_TRIGGER_SOURCE source)
{
    
    //m_nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
    int nRet = m_pcMyCamera->SetEnumValue("TriggerSource", source);
    if (MV_OK != nRet)
    {
        WriteError("Set Software Trigger Fail");
        return false;
    }
	m_nTriggerSource = source;
    return true;
}

// ch:����ͼƬ | en:Save Image
std::string CLineCamera::SaveImage()
{
    if (FALSE == m_bStartGrabbing)
    {
        return std::string();
    }
    // ch:��ȡ1��ͼ | en:get one image
    unsigned int nRecvBufSize = 0;
    int nRet = MV_OK;

    // ch:���ڵ�һ�α���ͼ��ʱ���뻺�棬�� CloseDevice ʱ�ͷ�
    // en:Request buffer only when save image for first time, release after CloseDevice
    if (NULL == m_pBufForDriver)
    {
        // ch:������л�ȡһ֡ͼ���С | en:Get size of one frame from camera
        nRet = m_pcMyCamera->GetIntValue("PayloadSize", &nRecvBufSize);
        if (nRet != MV_OK)
        {
            ShowErrorMsg(TEXT("failed in get PayloadSize"), nRet);
            return std::string();
        }
        // ch:һ֡���ݴ�С
        // en:One frame size
        m_nBufSizeForDriver = nRecvBufSize;  
        m_pBufForDriver = (unsigned char *)malloc(m_nBufSizeForDriver);
        if (NULL == m_pBufForDriver)
        {
            ShowErrorMsg(TEXT("malloc m_pBufForDriver failed, run out of memory"), 0);
            return std::string();
        }
    }

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

    unsigned int nDataSize = nRecvBufSize;
    unsigned int nImageNum = 1;
    unsigned int nDataLen = 0;

	std::string imagePath;

    while(nImageNum)
    {
        nRet = m_pcMyCamera->GetOneFrameTimeout(m_pBufForDriver, &nDataLen, m_nBufSizeForDriver, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            nImageNum--;

            // ch:���ڵ�һ�α���ͼ��ʱ���뻺�棬�� CloseDevice ʱ�ͷ�
            // en:Request buffer only when save image for first time, release after CloseDevice
            if (NULL == m_pBufForSaveImage)
            {
                // ch:BMPͼƬ��С��width * height * 3 + 2048(Ԥ��BMPͷ��С)
                // en:// BMP image size: width * height * 3 + 2048 (Reserved BMP header size)
                m_nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight * 3 + 2048;

                m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage);
                if (NULL == m_pBufForSaveImage)
                {
                    break;
                }
            }
            // ch:���ö�Ӧ��������� | en:Set camera parameter
            MV_SAVE_IMAGE_PARAM_EX stParam = {0};
            stParam.enImageType = m_nSaveImageType; // ch:��Ҫ�����ͼ������ | en:Image format to save
            stParam.enPixelType = stImageInfo.enPixelType;  // ch:�����Ӧ�����ظ�ʽ | en:Camera pixel type
            stParam.nWidth      = stImageInfo.nWidth;         // ch:�����Ӧ�Ŀ� | en:Width
            stParam.nHeight     = stImageInfo.nHeight;          // ch:�����Ӧ�ĸ� | en:Height
            stParam.nDataLen    = stImageInfo.nFrameLen;
            stParam.pData       = m_pBufForDriver;
            stParam.pImageBuffer = m_pBufForSaveImage;
            stParam.nBufferSize = m_nBufSizeForSaveImage;  // ch:�洢�ڵ�Ĵ�С | en:Buffer node size
            stParam.nJpgQuality     = 80;       // ch:jpg���룬���ڱ���Jpgͼ��ʱ��Ч������BMPʱSDK�ں��Ըò���
                                                // en:jpg encoding, only valid when saving as Jpg. SDK ignore this parameter when saving as BMP

            nRet = m_pcMyCamera->SaveImage(&stParam);
            if(MV_OK != nRet)
            {
                break;
            }

            char chImageName[IMAGE_NAME_LEN] = {0};
            if (MV_Image_Bmp == stParam.enImageType)
            {
                sprintf_s(chImageName, IMAGE_NAME_LEN, "Image_w%d_h%d_fn%03d.bmp", stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
            }
            else if (MV_Image_Jpeg == stParam.enImageType)
            {
                sprintf_s(chImageName, IMAGE_NAME_LEN, "Image_w%d_h%d_fn%03d.jpg", stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
            }
            
			FILE* fp = nullptr;
			fopen_s(&fp, chImageName, "wb");
            if (NULL == fp)
            {
                ShowErrorMsg(TEXT("write image failed, maybe you have no privilege"), 0);
                return std::string();
            }
            fwrite(m_pBufForSaveImage, 1, stParam.nImageLen, fp);
            fclose(fp);
			imagePath = std::string(chImageName);
        }
        else
        {
            break;
        }
    }

    return imagePath;
}

MV_CAM_TRIGGER_MODE CLineCamera::GetTriggerModeByCamera()
{
	MV_CAM_TRIGGER_MODE nEnumValue = MV_TRIGGER_MODE_UNKNOWN;

	int nRet = m_pcMyCamera->GetEnumValue("TriggerMode", (unsigned int*)&nEnumValue);
	if (MV_OK != nRet)
	{
		return MV_TRIGGER_MODE_UNKNOWN;
	}
	return nEnumValue;
}

double CLineCamera::GetGainByCamera()
{
	float  fFloatValue = 0.0;
	int nRet = m_pcMyCamera->GetFloatValue("ExposureTime", &fFloatValue);
	if (MV_OK != nRet)
	{
		return -1;
	}
	return fFloatValue;
}

double CLineCamera::GetFrameRateByCamera()
{
	float  fFloatValue = 0.0;

	int nRet = m_pcMyCamera->GetFloatValue("ResultingFrameRate", &fFloatValue);
	if (MV_OK != nRet)
	{
		return -1;
	}
	return fFloatValue;
}

double CLineCamera::GetExposureTimeByCamera()
{
	float  fFloatValue = 0.0;
	int nRet = m_pcMyCamera->GetFloatValue("ExposureTime", &fFloatValue);
	if (MV_OK != nRet)
	{
		return -1;
	}
	return fFloatValue;
}

MV_CAM_TRIGGER_SOURCE CLineCamera::GetTriggerSourceByCamera(void)
{
	MV_CAM_TRIGGER_SOURCE nEnumValue = MV_TRIGGER_SOURCE_UNKNOWN;

	int nRet = m_pcMyCamera->GetEnumValue("TriggerSource", (unsigned int*)&nEnumValue);
	if (MV_OK != nRet)
	{
		return MV_TRIGGER_SOURCE_UNKNOWN;
	}
	return nEnumValue;
}



// ch:���´��豸��ť�����豸 | en:Click Open button: Open Device
bool CLineCamera::OpenButton()
{
    int nRet = OpenDevice();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Open Fail"), nRet);
        return false;
    }

    GetParameter(); // ch:��ȡ���� | en:Get Parameter
    
    return true;
}


// ch:���¿�ʼ�ɼ���ť | en:Click Start button
bool CLineCamera::StartGrabbing()
{
    if (FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing)
    {
        return false;
    }

    int nRet = MV_OK;
    if (NULL != m_pcMyCamera)
    {
        nRet = m_pcMyCamera->StartGrabbing();
        if (nRet == MV_OK)
        {
            nRet = m_pcMyCamera->Display(m_hwndDisplay);
        }
    }
    else
    {
        return false;
    }

    if (MV_OK != nRet)
    {
        return false;
    }
    m_bStartGrabbing = TRUE;
    
    return true;
}

// ch:���½����ɼ���ť | en:Click Stop button
void CLineCamera::StopGrabbing()
{
    if (FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing)
    {
        return;
    }

    int nRet = MV_OK;
    if (NULL != m_pcMyCamera)
    {
        nRet = m_pcMyCamera->StopGrabbing();
    }
    else
    {
        return ;
    }

    if (MV_OK != nRet)
    {
        return ;
    }
    m_bStartGrabbing = FALSE;
    
    return;
}

// ch:���»�ȡ������ť | en:Click Get Parameter button
void CLineCamera::GetParameter()
{
	m_nTriggerMode = GetTriggerModeByCamera();
    if (m_nTriggerMode != MV_TRIGGER_MODE_UNKNOWN)
    {
        WriteError("Get Trigger Mode Fail");
    }
	
	m_dExposureTime = GetExposureTimeByCamera();
    if (m_dExposureTime != -1)
    {
		WriteError("Get Exposure Time Fail");
    }

	m_dExposureGain = GetGainByCamera();
    if (m_dExposureGain != -1)
    {
		WriteError("Get Gain Fail");
    }

	m_dFrameRate = GetFrameRateByCamera();
    if (m_dFrameRate != -1)
    {
		WriteError("Get Frame Rate Fail");
    }

	m_nTriggerSource = GetTriggerSourceByCamera();
    if (m_nTriggerSource != MV_TRIGGER_SOURCE_UNKNOWN)
    {
		WriteError("Get Trigger Source Fail");
    }
    return;
}

void CLineCamera::SetDisplayHwnd(HWND hwnd)
{
	m_hwndDisplay = hwnd;
}


// ch:��������һ�ΰ�ť | en:Click Execute button
void CLineCamera::SoftwareOnce()
{
    if (TRUE != m_bStartGrabbing)
    {
        return;
    }

    int nRet = m_pcMyCamera->CommandExecute("TriggerSoftware");

    return;
}

// ch:���±���bmpͼƬ��ť | en:Click Save BMP button
std::string CLineCamera::SaveBmp()
{
    m_nSaveImageType = MV_Image_Bmp;
    std::string path = SaveImage();
    if (path.size() == 0)
    {
        WriteError("Save bmp fail");
        return path;
    }
    //ShowErrorMsg(TEXT("Save bmp succeed"), nRet);

    return path;
}

// ch:���±���jpgͼƬ��ť | en:Click Save JPG button
std::string CLineCamera::SaveJpg()
{
    m_nSaveImageType = MV_Image_Jpeg;
	std::string path = SaveImage();
    if (path.size() != 0)
    {
        WriteError("Save jpg fail");
        return path;
    }
    //ShowErrorMsg(TEXT("Save jpg succeed"), nRet);
    return path;
}
