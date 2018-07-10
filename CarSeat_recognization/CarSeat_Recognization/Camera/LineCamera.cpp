
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
    //, m_bOpenDevice(FALSE)
	, m_pDevice(pDevice)
    //, m_bStartGrabbing(false)
    , m_nTriggerMode(MV_TRIGGER_MODE_OFF)
    , m_dExposureTime(0)
    , m_dExposureGain(0)
    , m_dFrameRate(0)
    //, m_bSoftWareTriggerCheck(FALSE)
    , m_nSaveImageType(MV_Image_Undefined)
    , m_nTriggerSource(MV_TRIGGER_SOURCE_SOFTWARE)
    , m_pBufForSaveImage(NULL)
    , m_nBufSizeForSaveImage(0)
    , m_pBufForDriver(NULL)
    , m_nBufSizeForDriver(0)
	, m_status(CCamera::CameraStatus::CAMERA_INIT)
	, m_nBalanceWhile(MV_BALANCEWHITE_AUTO_CONTINUOUS)
{
	//GetCurrentDirectory()
	memset(m_szImageDir, 0, sizeof(m_szImageDir));
	StrCpy(m_szImageDir, L".\\");
}

CLineCamera::~CLineCamera()
{
	CloseDevice();
}



// ch:���豸 | en:Open Device
int CLineCamera::OpenDevice(void)
{

    // ch:���豸��Ϣ�����豸ʵ�� | en:Device instance created by device information
    if (NULL == m_pDevice)
    {
        WriteError("Device does not exist");
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

	m_status = CCamera::CameraStatus::CAMERA_OPEN;
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

    //m_bOpenDevice = FALSE;
    //m_bStartGrabbing = FALSE;
	m_status = CCamera::CameraStatus::CAMERA_INIT;

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

CCamera::CameraStatus CLineCamera::GetCameraStatus()
{
	return m_status;
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

bool CLineCamera::GetExposureTimeRange(double * timeMax, double * timeMin)
{
	*timeMax = m_dExposureTimeMax;
	*timeMin = m_dExposureTimeMin;
	return true;
}

bool CLineCamera::SetExposureTimeAutoMode(MV_CAM_EXPOSURE_AUTO_MODE mode)
{

	int nRet = m_pcMyCamera->SetEnumValue("ExposureAuto", mode);
	if (MV_OK != nRet)
	{
		WriteError("Set Software Trigger Fail");
		return false;
	}
	m_nExposureAutoMode = mode;
	return true;
}

bool CLineCamera::SetImageSaveDirectory(const wchar_t * fileDir)
{
	if (fileDir == nullptr)
	{
		return false;
	}

	//memcpy(m_szImageDir, fileDir, strlen(fileDir));
	size_t length = wcslen(fileDir);
	if (std::find(fileDir, fileDir + length, L'\\') != (fileDir + length))
	{
		if (fileDir[length - 1] == L'\\')
		{
			wmemcpy(m_szImageDir, fileDir, wcslen(fileDir));
			return true;
		}
		swprintf_s(m_szImageDir, sizeof(m_szImageDir), L"%s\\", fileDir);
		return true;
	}
	if (std::find(fileDir, fileDir + length, L'/') != (fileDir + length))
	{
		if (fileDir[length - 1] == L'/')
		{
			wmemcpy(m_szImageDir, fileDir, wcslen(fileDir));
			return true;
		}
		swprintf_s(m_szImageDir, sizeof(m_szImageDir), L"%s/", fileDir);
		return true;
	}
	swprintf_s(m_szImageDir, sizeof(m_szImageDir), L"%s\\", fileDir);
	return true;
}

const wchar_t * CLineCamera::GetImageSaveDirectory()
{
	return m_szImageDir;
}

bool CLineCamera::SetBalanceWhile(MV_CAM_BALANCEWHITE_AUTO balanceWhile)
{
	//�����Զ���ƽ��ģʽ
	if (m_pcMyCamera != nullptr)
	{
		bool flag = m_pcMyCamera->SetBalanceWhile(balanceWhile);
		return flag;
	}
	return false;
}

MV_CAM_BALANCEWHITE_AUTO CLineCamera::GetBalanceWhile()
{
	if (m_pcMyCamera != nullptr)
	{
		return m_pcMyCamera->GetBalanceWhile();
	}
	return MV_BALANCEWHITE_AUTO_UNKNOWN;
}

void CLineCamera::GetROIParameter(int * width, int * height, int * offsetX, int * offsetY)
{
	if (width != nullptr)
	{
		*width = m_nROI_Width;
		if (m_nROI_Width == -1)
		{
			int tmpWidth = -1;
			int tmpHeight = -1;
			int tmpOffsetX = -1;
			int tmpOffsetY = -1;
			bool ret = GetROIParameterByCamera(&tmpWidth, &tmpHeight, &tmpOffsetX, &tmpOffsetY);
			if (ret == true)
			{
				m_nROI_Height = tmpHeight;
				m_nROI_Width = tmpWidth;
				m_nROI_OffsetX = tmpOffsetX;
				m_nROI_OffsetY = tmpOffsetY;
			}
			*width = tmpWidth;
		}
	}
	if (height != nullptr)
	{
		*height = m_nROI_Height;
		if (m_nROI_Height == -1)
		{
			int tmpWidth = -1;
			int tmpHeight = -1;
			int tmpOffsetX = -1;
			int tmpOffsetY = -1;
			bool ret = GetROIParameterByCamera(&tmpWidth, &tmpHeight, &tmpOffsetX, &tmpOffsetY);
			if (ret == true)
			{
				m_nROI_Height = tmpHeight;
				m_nROI_Width = tmpWidth;
				m_nROI_OffsetX = tmpOffsetX;
				m_nROI_OffsetY = tmpOffsetY;
			}
			*height = tmpHeight;
		}
	}
	if (offsetX != nullptr)
	{
		*offsetX = m_nROI_OffsetX;
		if (m_nROI_OffsetX == -1)
		{
			int tmpWidth = -1;
			int tmpHeight = -1;
			int tmpOffsetX = -1;
			int tmpOffsetY = -1;
			bool ret = GetROIParameterByCamera(&tmpWidth, &tmpHeight, &tmpOffsetX, &tmpOffsetY);
			if (ret == true)
			{
				m_nROI_Height = tmpHeight;
				m_nROI_Width = tmpWidth;
				m_nROI_OffsetX = tmpOffsetX;
				m_nROI_OffsetY = tmpOffsetY;
			}
			*offsetX = tmpOffsetX;
		}
	}
	if (offsetY != nullptr)
	{
		*offsetY = m_nROI_OffsetY;
		if (m_nROI_OffsetY == -1)
		{
			int tmpWidth = -1;
			int tmpHeight = -1;
			int tmpOffsetX = -1;
			int tmpOffsetY = -1;
			bool ret = GetROIParameterByCamera(&tmpWidth, &tmpHeight, &tmpOffsetX, &tmpOffsetY);
			if (ret == true)
			{
				m_nROI_Height = tmpHeight;
				m_nROI_Width = tmpWidth;
				m_nROI_OffsetX = tmpOffsetX;
				m_nROI_OffsetY = tmpOffsetY;
			}
			*offsetY = tmpOffsetY;
		}
	}
}

SIZE CLineCamera::GetImageSize()
{
	SIZE tmpSize;
	tmpSize.cx = m_nWidthMax;
	tmpSize.cy = m_nHeightMax;
	return tmpSize;
}

// ch:����ͼƬ | en:Save Image
std::wstring CLineCamera::SaveImage()
{
    if (CCamera::CameraStatus::CAMERA_GRAB != m_status)
    {
        return std::wstring();
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
            WriteError("failed in get PayloadSize, nRet = %d", nRet);
            return std::wstring();
        }
        // ch:һ֡���ݴ�С
        // en:One frame size
        m_nBufSizeForDriver = nRecvBufSize;  
        m_pBufForDriver = (unsigned char *)malloc(m_nBufSizeForDriver);
        if (NULL == m_pBufForDriver)
        {
			WriteError("malloc m_pBufForDriver failed, run out of memory");
            return std::wstring();
        }
    }

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

    unsigned int nDataSize = nRecvBufSize;
    unsigned int nImageNum = 1;
    unsigned int nDataLen = 0;

	std::wstring imagePath;

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

            wchar_t chImageName[IMAGE_NAME_LEN] = {0};
			memset(chImageName, 0, sizeof(wchar_t) * IMAGE_NAME_LEN);
            if (MV_Image_Bmp == stParam.enImageType)
            {
                swprintf_s(chImageName, IMAGE_NAME_LEN, L"Image_w%d_h%d_fn%03d.bmp", stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
            }
            else if (MV_Image_Jpeg == stParam.enImageType)
            {
                swprintf_s(chImageName, IMAGE_NAME_LEN, L"Image_w%d_h%d_fn%03d.jpg", stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
            }
			wchar_t absoluteName[MAX_CHAR_LENGTH] = { 0 };
			memset(absoluteName, 0, sizeof(wchar_t) * MAX_CHAR_LENGTH);

			WriteInfo("m_szImageDir = %s", m_szImageDir);
			WriteInfo("chImageName = %s", chImageName);

			TRACE1("m_szImageDir = %s\n", m_szImageDir);
			TRACE1("chImageName = %s\n", chImageName);
			
			swprintf_s(absoluteName, sizeof(absoluteName), L"%s%s", m_szImageDir, chImageName);
			
			char *tmpFileName = utils::WcharToChar(absoluteName);
			if (tmpFileName == nullptr)
			{
				return std::wstring();
			}
			FILE* fp = nullptr;
			fopen_s(&fp, tmpFileName, "wb");

			delete []tmpFileName;
			tmpFileName = nullptr;

            if (NULL == fp)
            {
                WriteError("write image failed, maybe you have no privilege");
                return std::wstring();
            }
            fwrite(m_pBufForSaveImage, 1, stParam.nImageLen, fp);
            fclose(fp);
			imagePath = std::wstring(chImageName);
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

double CLineCamera::GetExposureTimeMaxByCamera()
{
	float tmpValue = 0;
	int nRet = m_pcMyCamera->GetFloatValue("AutoExposureTimeLowerLimit", &tmpValue);
	if (nRet != MV_OK)
	{
		return -1;
	}
	return tmpValue;
}

double CLineCamera::GetExposureTimeMinByCamera()
{
	float tmpValue = 0;
	int nRet = m_pcMyCamera->GetFloatValue("AutoExposureTimeUpperLimit", &tmpValue);
	if (nRet != MV_OK)
	{
		return -1;
	}
	return tmpValue;
}

MV_CAM_EXPOSURE_AUTO_MODE CLineCamera::GetExposureAutoModeByCamera()
{
	MV_CAM_EXPOSURE_AUTO_MODE tmpValue = MV_EXPOSURE_AUTO_MODE_UNKNOWN;
	int nRet = m_pcMyCamera->GetEnumValue("ExposureAuto", (unsigned int*)&tmpValue);
	if (nRet != MV_OK)
	{
		return MV_EXPOSURE_AUTO_MODE_UNKNOWN;
	}
	return tmpValue;
}

int CLineCamera::GetWidthMaxByCamera()
{
	//
	unsigned int tmpValue = -1;
	int nRet = m_pcMyCamera->GetIntValue("WidthMax", &tmpValue);
	if (nRet != MV_OK)
	{
		return -1;
	}
	return tmpValue;
}

int CLineCamera::GetHeightMaxByCamera()
{
	unsigned int tmpValue = -1;
	int nRet = m_pcMyCamera->GetIntValue("HeightMax", &tmpValue);
	if (nRet != MV_OK)
	{
		return -1;
	}
	return tmpValue;
}

bool CLineCamera::GetROIParameterByCamera(int * width, int * height, int * offsetX, int * offsetY)
{
	*width = *height = *offsetX = *offsetY = -1;

	unsigned int tmpValue = -1;
	bool flag = true;
	int nRet = m_pcMyCamera->GetIntValue("Width", &tmpValue);
	if (nRet != MV_OK)
	{
		TRACE1("get roi width failed, nret = %d\n", nRet);
		WriteError("get roi width failed,nret = %d", nRet);
		*width = -1;
		flag = false;
	}
	else
	{
		*width = tmpValue;
	}
	

	tmpValue = -1;
	nRet = m_pcMyCamera->GetIntValue("Height", &tmpValue);
	if (nRet != MV_OK)
	{
		TRACE1("get roi Height failed, nret = %d\n", nRet);
		WriteError("get roi Height failed,nret = %d", nRet);
		*height = -1;
		flag = false;
	}
	else
	{
		*height = tmpValue;
	}
	

	tmpValue = -1;
	nRet = m_pcMyCamera->GetIntValue("OffsetX", &tmpValue);
	if (nRet != MV_OK)
	{
		TRACE1("get roi OffsetX failed, nret = %d\n", nRet);
		WriteError("get roi OffsetX failed,nret = %d", nRet);
		*offsetX = -1;
		flag = false;
	}
	else
	{
		*offsetX = tmpValue;
	}
	

	tmpValue = -1;
	nRet = m_pcMyCamera->GetIntValue("OffsetY", &tmpValue);
	if (nRet != MV_OK)
	{
		TRACE1("get roi OffsetY failed, nret = %d\n", nRet);
		WriteError("get roi OffsetY failed,nret = %d", nRet);
		*offsetY = -1;
		flag = false;
	}
	else
	{
		*offsetY = tmpValue;
	}
	
	return flag;
}



// ch:���´��豸��ť�����豸 | en:Click Open button: Open Device
bool CLineCamera::OpenButton()
{
    int nRet = OpenDevice();
    if (MV_OK != nRet)
    {
        WriteError("Open Fail, nRet = %u", nRet);
        return false;
    }

    GetParameter(); // ch:��ȡ���� | en:Get Parameter
    
    return true;
}


// ch:���¿�ʼ�ɼ���ť | en:Click Start button
bool CLineCamera::StartGrabbing()
{
	if ((m_status == CCamera::CameraStatus::CAMERA_GRAB) || (m_status != CCamera::CameraStatus::CAMERA_OPEN))
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
	m_status = CCamera::CameraStatus::CAMERA_GRAB;
    
    return true;
}

// ch:���½����ɼ���ť | en:Click Stop button
void CLineCamera::StopGrabbing()
{
	if ((m_status != CCamera::CameraStatus::CAMERA_GRAB) && (m_status != CCamera::CameraStatus::CAMERA_OPEN))
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
    //m_bStartGrabbing = FALSE;
	m_status = CCamera::CameraStatus::CAMERA_OPEN;
    
    return;
}

// ch:���»�ȡ������ť | en:Click Get Parameter button
void CLineCamera::GetParameter()
{
	m_nTriggerMode = GetTriggerModeByCamera();
    if (m_nTriggerMode == MV_TRIGGER_MODE_UNKNOWN)
    {
        WriteError("Get Trigger Mode Fail");
    }
	
	m_dExposureTime = GetExposureTimeByCamera();
    if (m_dExposureTime == -1)
    {
		WriteError("Get Exposure Time Fail");
    }

	m_dExposureGain = GetGainByCamera();
    if (m_dExposureGain == -1)
    {
		WriteError("Get Gain Fail");
    }

	m_dFrameRate = GetFrameRateByCamera();
    if (m_dFrameRate == -1)
    {
		WriteError("Get Frame Rate Fail");
    }

	m_nTriggerSource = GetTriggerSourceByCamera();
    if (m_nTriggerSource == MV_TRIGGER_SOURCE_UNKNOWN)
    {
		WriteError("Get Trigger Source Fail");
    }
	m_dExposureTimeMax = GetExposureTimeMaxByCamera();
	if (m_dExposureTimeMax == -1)
	{
		WriteError("Get Exposure Time Max Fail");
	}
	m_dExposureTimeMin = GetExposureTimeMinByCamera();
	if (m_dExposureTimeMin == -1)
	{
		WriteError("Get Exposure Time Min Fail");
	}

	if (GetROIParameterByCamera(&m_nROI_Width, &m_nROI_Height, &m_nROI_OffsetX, &m_nROI_OffsetY) == false)
	{
		TRACE2("get roi width =%d, height = %d\n", m_nROI_Width, m_nROI_Height);
		WriteError("get roi width =%d, height = %d", m_nROI_Width, m_nROI_Height);
		TRACE2("get roi offsetX =%d, offsetY = %d\n", m_nROI_OffsetX, m_nROI_OffsetY);
		WriteError("get roi offsetX =%d, offsetY = %d", m_nROI_OffsetX, m_nROI_OffsetY);
	}


	//m_pcMyCamera->
    return;
}

void CLineCamera::SetDisplayHwnd(HWND hwnd)
{
	m_hwndDisplay = hwnd;
}


// ch:��������һ�ΰ�ť | en:Click Execute button
void CLineCamera::SoftwareOnce()
{
    if (CCamera::CameraStatus::CAMERA_GRAB != m_status)
    {
        return;
    }

    int nRet = m_pcMyCamera->CommandExecute("TriggerSoftware");

    return;
}

// ch:���±���bmpͼƬ��ť | en:Click Save BMP button
std::wstring CLineCamera::SaveBmp()
{
    m_nSaveImageType = MV_Image_Bmp;
    std::wstring path = SaveImage();
    if (path.size() == 0)
    {
        WriteError("Save bmp fail");
        return path;
    }
    //ShowErrorMsg(TEXT("Save bmp succeed"), nRet);

    return path;
}

// ch:���±���jpgͼƬ��ť | en:Click Save JPG button
std::wstring CLineCamera::SaveJpg()
{
    m_nSaveImageType = MV_Image_Jpeg;
	std::wstring path = SaveImage();
    if (path.size() == 0)
    {
        WriteError("Save jpg fail");
        return path;
    }
    return path;
}
