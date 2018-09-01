
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
    , m_nExposureTimeMax(1500000)
	, m_nExposureTimeMin(500000)
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
	, m_nWidthMax(0)
	, m_nHeightMax(0)
	, m_nROI_Height(0)
	, m_nROI_Width(0)
	, m_nROI_OffsetX(0)
	, m_nROI_OffsetY(0)
	, m_hwndDisplay(NULL)
{
	//GetCurrentDirectory()
	
	memset(m_szImageDir, 0, sizeof(m_szImageDir));
	strcpy_s(m_szImageDir, sizeof(m_szImageDir), ".\\");
}

CLineCamera::~CLineCamera()
{
	CloseDevice();
	if (m_pBufForSaveImage != NULL)
	{
		delete []m_pBufForSaveImage;
		m_pBufForSaveImage = nullptr;
	}
	if (m_pBufForDriver != NULL)
	{
		delete[]m_pBufForDriver;
		m_pBufForDriver = nullptr;
	}
}



// ch:打开设备 | en:Open Device
int CLineCamera::OpenDevice(void)
{

    // ch:由设备信息创建设备实例 | en:Device instance created by device information
    if (NULL == m_pDevice)
    {
        WriteError("Device does not exist");
        return STATUS_ERROR;
    }

    if (NULL != m_pcMyCamera)
    {
		WriteError("camera not open more chan twice;");
        return STATUS_ERROR;
    }

    m_pcMyCamera = new CCamera;
    if (NULL == m_pcMyCamera)
    {
		WriteError("new CCamera FAILED");
        return STATUS_ERROR;
    }
	if (m_pDevice == NULL)
	{
		WriteError("Camera device m_pDevice is null");
		delete m_pcMyCamera;
		m_pcMyCamera = NULL;
		return -1;
	}

    int nRet = m_pcMyCamera->Open(m_pDevice);
    if (MV_OK != nRet)
    {
		WriteError("my camera open device failed, CAMERA macADDRESS = 0x%X%X", m_pDevice->nMacAddrHigh, m_pDevice->nMacAddrLow);
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
        return nRet;
    }

	m_status = CCamera::CameraStatus::CAMERA_OPEN;
	m_pcMyCamera->RegisterExceptionCallBack(ReconnectDevice, this);
    return MV_OK;
}

// ch:关闭设备 | en:Close Device
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
        delete [] m_pBufForSaveImage;
        m_pBufForSaveImage = NULL;
    }
    m_nBufSizeForSaveImage  = 0;
    return MV_OK;
}

// ch:获取触发模式 | en:Get Trigger Mode
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

// ch:设置触发模式 | en:Set Trigger Mode
bool CLineCamera::SetTriggerMode(MV_CAM_TRIGGER_MODE mode)
{
	if (mode == MV_TRIGGER_MODE_UNKNOWN)
	{
		return false;
	}
	
    //int nRet = m_pcMyCamera->SetEnumValue("TriggerMode", m_nTriggerMode);
	unsigned int enMode = MV_TRIGGER_MODE_OFF; //打开
	int nRet = MV_CC_SetTriggerMode(m_pcMyCamera->GetHandle(), enMode);

    if (MV_OK != nRet)
    {
        return false;
    }
	m_nTriggerMode = mode;
    return true;
}

// ch:获取曝光时间 | en:Get Exposure Time
int CLineCamera::GetExposureTimeMax()
{
    return m_nExposureTimeMax;
}

int CLineCamera::GetExposureTimeMin()
{
	return m_nExposureTimeMin;
}

// ch:设置曝光时间 | en:Set Exposure Time
bool CLineCamera::SetExposureTime(unsigned int timeMax, unsigned int timeMin)
{
    // ch:调节这两个曝光模式，才能让曝光时间生效
    // en:Adjust these two exposure mode to allow exposure time effective
	unsigned int value = timeMin;
	int nRet = MV_CC_SetAutoExposureTimeLower(m_pcMyCamera->GetHandle(), value);
	if (MV_OK != nRet)
	{
		TRACE0("MV_CC_SetAutoExposureTimeLower failed\n");
		//return false;
	}
	value = timeMax;
	//nRet = MV_CC_SetAutoExposureTimeLower(m_pcMyCamera->GetHandle(), timeMin);
	nRet = MV_CC_SetAutoExposureTimeUpper(m_pcMyCamera->GetHandle(), value);
	if (MV_OK != nRet)
	{
		TRACE0("MV_CC_SetAutoExposureTimeUpper failed\n");
		//return false;
	}
    /*int nRet = m_pcMyCamera->SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
    if (MV_OK != nRet)
    {
        return false;
    }

    nRet = m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_CONTINUOUS);
	if (nRet != MV_OK)
	{
		return false;
	}*/
	value = timeMin;
    nRet = m_pcMyCamera->SetIntValue("AutoExposureTimeLowerLimit", value);
    if (MV_OK != nRet)
    {
		TRACE0("set AutoExposureTimeLowerLimit failed\n");
        //return false;
    }
	
	value = timeMax;
	nRet = m_pcMyCamera->SetIntValue("AutoExposureTimeUpperLimit", value);
	if (MV_OK != nRet)
	{
		TRACE0("set AutoExposureTimeUpperLimit failed\n");
		return false;
	}
	
	m_nExposureTimeMax = timeMax;
	m_nExposureTimeMin = timeMin;
    return true;
}

// ch:获取增益 | en:Get Gain
double CLineCamera::GetGain()
{
    return m_dExposureGain;
}

// ch:设置增益 | en:Set Gain
bool CLineCamera::SetGain(float gain)
{
    // ch:设置增益前先把自动增益关闭，失败无需返回
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

// ch:获取帧率 | en:Get Frame Rate
double CLineCamera::GetFrameRate()
{
    return m_dFrameRate;
}

// ch:设置帧率 | en:Set Frame Rate
bool CLineCamera::SetFrameRate(double rate)
{
    int nRet = m_pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
    if (MV_OK != nRet)
    {
        return false;
    }
	float tmpRate = rate;

	nRet = m_pcMyCamera->SetFloatValue("AcquisitionFrameRate", tmpRate);
	if (nRet != MV_OK)
	{
		return false;
	}
	m_dFrameRate = rate;
	return true;
}

// ch:获取触发源 | en:Get Trigger Source
MV_CAM_TRIGGER_SOURCE CLineCamera::GetTriggerSource()
{
    return m_nTriggerSource;
}

// ch:设置触发源 | en:Set Trigger Source
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

//bool CLineCamera::SetExposureMode(MV_CAM_EXPOSURE_AUTO_MODE mode)
//{
//	//m_pcMyCamera->MV_CC_SetExposureAutoMode()
//	int nRet = MV_CC_SetExposureAutoMode(m_pcMyCamera->GetHandle(), mode);
//	//int nRet = m_pcMyCamera->SetEnumValue("ExposureMode", mode);
//	if (MV_OK != nRet)
//	{
//		WriteError("Set ExposureMode Fail");
//		return false;
//	}
//	m_nExposureMode = mode;
//	return true;
//}

bool CLineCamera::GetExposureTimeRange(double * timeMax, double * timeMin)
{
	*timeMax = m_nExposureTimeMax;
	*timeMin = m_nExposureTimeMin;
	return true;
}

bool CLineCamera::SetExposureTimeAutoMode(MV_CAM_EXPOSURE_AUTO_MODE mode)
{

	/*int nRet = m_pcMyCamera->SetEnumValue("ExposureAuto", mode);
	if (MV_OK != nRet)
	{
		WriteError("Set Software Trigger Fail");
		return false;
	}*/
	int nRet = m_pcMyCamera->SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
	if (MV_OK != nRet)
	{
		return false;
	}

	nRet = MV_CC_SetExposureAutoMode(m_pcMyCamera->GetHandle(), mode);
	//int nRet = m_pcMyCamera->SetEnumValue("ExposureMode", mode);
	if (MV_OK != nRet)
	{
		WriteError("Set ExposureMode Fail");
		return false;
	}
	//m_nExposureMode = mode;
	m_nExposureAutoMode = mode;
	return true;
}

bool CLineCamera::SetImageSaveDirectory(const char * fileDir)
{
	if (fileDir == nullptr)
	{
		return false;
	}

	//memcpy(m_szImageDir, fileDir, strlen(fileDir));
	memset(m_szImageDir, 0, sizeof(m_szImageDir));
	size_t length = strlen(fileDir);
	if (std::find(fileDir, fileDir + length, '\\') != (fileDir + length))
	{
		if (fileDir[length - 1] == '\\')
		{
			memcpy(m_szImageDir, fileDir, strlen(fileDir));
			return true;
		}
		sprintf_s(m_szImageDir, sizeof(m_szImageDir)/sizeof(char), "%s\\", fileDir);
		return true;
	}
	if (std::find(fileDir, fileDir + length, '/') != (fileDir + length))
	{
		if (fileDir[length - 1] == '/')
		{
			memcpy(m_szImageDir, fileDir, strlen(fileDir));
			return true;
		}
		sprintf_s(m_szImageDir, sizeof(m_szImageDir) / sizeof(char), "%s/", fileDir);
		return true;
	}
	sprintf_s(m_szImageDir, sizeof(m_szImageDir) / sizeof(char), "%s\\", fileDir);
	return true;
}

const char * CLineCamera::GetImageSaveDirectory()
{
	return m_szImageDir;
}

bool CLineCamera::SetBalanceWhile(MV_CAM_BALANCEWHITE_AUTO balanceWhile)
{
	//设置自动白平衡模式
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

bool CLineCamera::SetPixelFormat(int value)
{
	//int nRet = m_pcMyCamera->SetEnumValue("PixelFormat", value);
	//unsigned int enValue = PixelType_Gvsp_RGB16_Packed;
	int nRet = MV_CC_SetPixelFormat(m_pcMyCamera->GetHandle(), value);

	if (MV_OK != nRet)
	{
		return false;
	}
	return true;
}

bool CLineCamera::GetConnectStatus()
{
	
	return false;
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

// ch:保存图片 | en:Save Image
std::string CLineCamera::SaveImage()
{
	if (m_pcMyCamera == NULL)
	{
		OpenButton();
		if (m_status != CCamera::CameraStatus::CAMERA_OPEN)
		{
			WriteError("camera status not open");
			return std::string();
		}
	}

    if (CCamera::CameraStatus::CAMERA_GRAB != m_status)
    {
		WriteError("camera status not grab, m_status = %d", m_status);
		StartGrabbing();
        //return std::string();
    }
    // ch:获取1张图 | en:get one image
    unsigned int nRecvBufSize = 0;
    int nRet = MV_OK;

    // ch:仅在第一次保存图像时申请缓存，在 CloseDevice 时释放
    // en:Request buffer only when save image for first time, release after CloseDevice
    if ((NULL == m_pBufForDriver) || (m_pcMyCamera != NULL))
    {
        // ch:从相机中获取一帧图像大小 | en:Get size of one frame from camera
        nRet = m_pcMyCamera->GetIntValue("PayloadSize", &nRecvBufSize);
        if (nRet != MV_OK)
        {
            WriteError("failed in get PayloadSize, nRet = %d", nRet);
            return std::string();
        }
        // ch:一帧数据大小
        // en:One frame size
        m_nBufSizeForDriver = nRecvBufSize;  
        m_pBufForDriver = new unsigned char[m_nBufSizeForDriver];
        if (NULL == m_pBufForDriver)
        {
			WriteError("malloc m_pBufForDriver failed, run out of memory");
            return std::string();
        }
		memset(m_pBufForDriver, 0, sizeof(unsigned char) * m_nBufSizeForDriver);
    }

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

    unsigned int nDataSize = nRecvBufSize;
    unsigned int nImageNum = 1;
    unsigned int nDataLen = 0;

	std::string imagePath;

    while(nImageNum)
    {
		memset(m_pBufForDriver, 0, sizeof(unsigned char) * m_nBufSizeForDriver);
        nRet = m_pcMyCamera->GetOneFrameTimeout(m_pBufForDriver, &nDataLen, m_nBufSizeForDriver, &stImageInfo, 2000);
        if (nRet == MV_OK)
        {
            nImageNum--;

            // ch:仅在第一次保存图像时申请缓存，在 CloseDevice 时释放
            // en:Request buffer only when save image for first time, release after CloseDevice
            if (NULL == m_pBufForSaveImage)
            {
                // ch:BMP图片大小：width * height * 3 + 2048(预留BMP头大小)
                // en:// BMP image size: width * height * 3 + 2048 (Reserved BMP header size)
                m_nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight * 3 + 2048;

                m_pBufForSaveImage = new unsigned char[m_nBufSizeForSaveImage];
                if (NULL == m_pBufForSaveImage)
                {
                    break;
                }
				memset(m_pBufForSaveImage, 0, sizeof(unsigned char) * m_nBufSizeForSaveImage);
            }
            // ch:设置对应的相机参数 | en:Set camera parameter
            MV_SAVE_IMAGE_PARAM_EX stParam = {0};
            stParam.enImageType = m_nSaveImageType; // ch:需要保存的图像类型 | en:Image format to save
            stParam.enPixelType = stImageInfo.enPixelType;  // ch:相机对应的像素格式 | en:Camera pixel type
            stParam.nWidth      = stImageInfo.nWidth;         // ch:相机对应的宽 | en:Width
            stParam.nHeight     = stImageInfo.nHeight;          // ch:相机对应的高 | en:Height
            stParam.nDataLen    = stImageInfo.nFrameLen;
            stParam.pData       = m_pBufForDriver;
            stParam.pImageBuffer = m_pBufForSaveImage;
            stParam.nBufferSize = m_nBufSizeForSaveImage;  // ch:存储节点的大小 | en:Buffer node size
            stParam.nJpgQuality     = 99;       // ch:jpg编码，仅在保存Jpg图像时有效。保存BMP时SDK内忽略该参数
                                                // en:jpg encoding, only valid when saving as Jpg. SDK ignore this parameter when saving as BMP
			memset(m_pBufForSaveImage, 0, sizeof(unsigned char) * m_nBufSizeForSaveImage);
            nRet = m_pcMyCamera->SaveImage(&stParam);
            if(MV_OK != nRet)
            {
                break;
            }


			time_t  time1 = time(NULL);//获取系统时间，单位为秒;
			struct tm tmpTime;
			localtime_s(&tmpTime, &time1);//转换成tm类型的结构体;

			/*swprintf_s(tmpResult.m_szTime, "%04d-%02d-%02d:%02d-%02d-%02d", \
				tmpTime.tm_year + 1900, tmpTime.tm_mon + 1, tmpTime.tm_mday, \
				tmpTime.tm_hour, tmpTime.tm_min, tmpTime.tm_sec);*/


            char chImageName[IMAGE_NAME_LEN] = {0};
			memset(chImageName, 0, sizeof(char) * IMAGE_NAME_LEN);
            if (MV_Image_Bmp == stParam.enImageType)
            {
                //swprintf_s(chImageName, IMAGE_NAME_LEN, L"Image_w%d_h%d_fn%03d.bmp", stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
				sprintf_s(chImageName, IMAGE_NAME_LEN, "%04d%02d%02d_%02d%02d%02d.bmp", \
					tmpTime.tm_year + 1900, tmpTime.tm_mon + 1, tmpTime.tm_mday, \
					tmpTime.tm_hour, tmpTime.tm_min, tmpTime.tm_sec);
			}
            else if (MV_Image_Jpeg == stParam.enImageType)
            {
                //swprintf_s(chImageName, IMAGE_NAME_LEN, L"Image_w%d_h%d_fn%03d.jpg", stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
				sprintf_s(chImageName, IMAGE_NAME_LEN, "%04d%02d%02d_%02d%02d%02d.jpg", \
					tmpTime.tm_year + 1900, tmpTime.tm_mon + 1, tmpTime.tm_mday, \
					tmpTime.tm_hour, tmpTime.tm_min, tmpTime.tm_sec);
			}
			char absoluteName[MAX_CHAR_LENGTH] = { 0 };
			memset(absoluteName, 0, sizeof(char) * MAX_CHAR_LENGTH);
			
			sprintf_s(absoluteName, MAX_CHAR_LENGTH, "%s%s", m_szImageDir, chImageName);
			
			wchar_t *tmpFileName = utils::CharToWchar(absoluteName);
			if (tmpFileName == nullptr)
			{
				return std::string();
			}
			WriteInfo("image file Name = [%s]", absoluteName);

			
			CFile file;
			if (TRUE == file.Open(tmpFileName, CFile::modeCreate | CFile::modeWrite))
			{
				file.Write(m_pBufForSaveImage, stParam.nImageLen);

				file.Flush();

				file.Close();
			}
			else
			{
				delete[]tmpFileName;
				tmpFileName = nullptr;
				WriteError("create image file failed");
				return std::string();
			}

			

			delete[]tmpFileName;
			tmpFileName = nullptr;


			//FILE* fp = nullptr;
			//errno_t errRet;
			//errRet = fopen_s(&fp, absoluteName, "wb");

			//WriteInfo("errRet = %d", errRet);
   //         if (NULL == fp)
   //         {

   //             WriteError("write image failed, maybe you have no privilege");
			//	//delete[]tmpFileName;
			//	//tmpFileName = nullptr;
			//	return std::string();
   //         }
   //         fwrite(m_pBufForSaveImage, 1, stParam.nImageLen, fp);
   //         fclose(fp);
			//fp = nullptr;

			imagePath = std::string(absoluteName);
			//delete[]tmpFileName;
			//tmpFileName = nullptr;
        }
        else
        {
			WriteError("GetOneFrameTimeout failed");
            break;
        }
    }

    return imagePath;
}

MV_CAM_TRIGGER_MODE CLineCamera::GetTriggerModeByCamera()
{
	if (m_pcMyCamera == NULL)
	{
		return MV_TRIGGER_MODE_UNKNOWN;
	}
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
	if (m_pcMyCamera == NULL)
	{
		return -1;
	}
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
	if (m_pcMyCamera == NULL)
	{
		return -1;
	}
	float  fFloatValue = 0.0;

	int nRet = m_pcMyCamera->GetFloatValue("ResultingFrameRate", &fFloatValue);
	if (MV_OK != nRet)
	{
		return -1;
	}
	return fFloatValue;
}

//double CLineCamera::GetExposureTimeByCamera()
//{
//	float  fFloatValue = 0.0;
//	int nRet = m_pcMyCamera->GetFloatValue("ExposureTime", &fFloatValue);
//	if (MV_OK != nRet)
//	{
//		return -1;
//	}
//	return fFloatValue;
//}

MV_CAM_TRIGGER_SOURCE CLineCamera::GetTriggerSourceByCamera(void)
{
	if (m_pcMyCamera == NULL)
	{
		return MV_TRIGGER_SOURCE_UNKNOWN;
	}
	MV_CAM_TRIGGER_SOURCE nEnumValue = MV_TRIGGER_SOURCE_UNKNOWN;

	int nRet = m_pcMyCamera->GetEnumValue("TriggerSource", (unsigned int*)&nEnumValue);
	if (MV_OK != nRet)
	{
		return MV_TRIGGER_SOURCE_UNKNOWN;
	}
	return nEnumValue;
}

int CLineCamera::GetExposureTimeMaxByCamera()
{
	if (m_pcMyCamera == NULL)
	{
		return -1;
	}
	unsigned int tmpValue = 0;
	int nRet = m_pcMyCamera->GetIntValue("AutoExposureTimeUpperLimit", &tmpValue);
	if (nRet != MV_OK)
	{
		return -1;
	}
	return tmpValue;
}

int CLineCamera::GetExposureTimeMinByCamera()
{
	if (m_pcMyCamera == NULL)
	{
		return -1;
	}
	unsigned int tmpValue = 0;
	int nRet = m_pcMyCamera->GetIntValue("AutoExposureTimeLowerLimit", &tmpValue);
	if (nRet != MV_OK)
	{
		return -1;
	}
	return tmpValue;
}

MV_CAM_EXPOSURE_AUTO_MODE CLineCamera::GetExposureAutoModeByCamera()
{
	if (m_pcMyCamera == NULL)
	{
		return MV_EXPOSURE_AUTO_MODE_UNKNOWN;
	}
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
	if (m_pcMyCamera == NULL)
	{
		return -1;
	}
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
	if (m_pcMyCamera == NULL)
	{
		return -1;
	}
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
	if (m_pcMyCamera == NULL)
	{
		return false;
	}
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



// ch:按下打开设备按钮：打开设备 | en:Click Open button: Open Device
bool CLineCamera::OpenButton()
{
    int nRet = OpenDevice();
    if (MV_OK != nRet)
    {
        WriteError("Open Fail, nRet = %u", nRet);
        return false;
    }

    GetParameter(); // ch:获取参数 | en:Get Parameter
    
    return true;
}


// ch:按下开始采集按钮 | en:Click Start button
bool CLineCamera::StartGrabbing()
{
	if ((m_status == CCamera::CameraStatus::CAMERA_GRAB) || (m_status != CCamera::CameraStatus::CAMERA_OPEN))
	{
		WriteError("camera status is grab or not open, status = %d", m_status);
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

// ch:按下结束采集按钮 | en:Click Stop button
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

// ch:按下获取参数按钮 | en:Click Get Parameter button
void CLineCamera::GetParameter()
{
	m_nTriggerMode = GetTriggerModeByCamera();
    if (m_nTriggerMode == MV_TRIGGER_MODE_UNKNOWN)
    {
        WriteError("Get Trigger Mode Fail");
    }
	
	/*m_dExposureTime = GetExposureTimeByCamera();
    if (m_dExposureTime == -1)
    {
		WriteError("Get Exposure Time Fail");
    }*/

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
	m_nExposureTimeMax = GetExposureTimeMaxByCamera();
	if (m_nExposureTimeMax == -1)
	{
		WriteError("Get Exposure Time Max Fail");
	}
	m_nExposureTimeMin = GetExposureTimeMinByCamera();
	if (m_nExposureTimeMin == -1)
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

	m_nWidthMax = GetWidthMaxByCamera();
	if(m_nWidthMax == -1)
	{
		TRACE0("get m_nWidthMax failed\n");
		WriteError("get m_nWidthMax failed\n");
		m_nWidthMax = 0;
	}
	m_nHeightMax = GetHeightMaxByCamera();
	if (m_nHeightMax == -1)
	{
		TRACE0("get m_nHeightMax failed\n");
		WriteError("get m_nHeightMax failed\n");
		m_nHeightMax = 0;
	}
    return;
}

void CLineCamera::SetDisplayHwnd(HWND hwnd)
{
	m_hwndDisplay = hwnd;
}


// ch:按下软触发一次按钮 | en:Click Execute button
void CLineCamera::SoftwareOnce()
{
	if (m_pcMyCamera == NULL)
	{
		return;
	}
    if (CCamera::CameraStatus::CAMERA_GRAB != m_status)
    {
        return;
    }

    int nRet = m_pcMyCamera->CommandExecute("TriggerSoftware");

    return;
}

// ch:按下保存bmp图片按钮 | en:Click Save BMP button
std::string CLineCamera::SaveBmp()
{
	if (m_pcMyCamera == NULL)
	{
		WriteError("camera not open");
		return std::string();
	}
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

// ch:按下保存jpg图片按钮 | en:Click Save JPG button
std::string CLineCamera::SaveJpg()
{
    m_nSaveImageType = MV_Image_Jpeg;
	std::string path = SaveImage();
    if (path.size() == 0)
    {
        WriteError("Save jpg fail");
        return path;
    }
    return path;
}



void __stdcall CLineCamera::ReconnectDevice(unsigned int nMsgType, void* pUser)
{
	if (nMsgType == MV_EXCEPTION_DEV_DISCONNECT)
	{
		WriteInfo("camera reconnect");
		CLineCamera* pThis = (CLineCamera*)pUser;

		//pThis->EnableWindowWhenClose();
		//pThis->m_ctrlOpenButton.EnableWindow(TRUE);
		if ((pThis->m_status == CCamera::CameraStatus::CAMERA_OPEN)	\
			|| (pThis->m_status == CCamera::CameraStatus::CAMERA_GRAB))
		{
			pThis->CloseDevice();

			BOOL bConnected = FALSE;
			while (1)
			{
				int nRet = MV_OK;
				nRet = pThis->OpenDevice();
				//nRet = pThis->m_pcMyCamera->Open(pThis->m_pDevice);
				if (MV_OK == nRet)
				{
					//pThis->m_pcMyCamera->RegisterExceptionCallBack(ReconnectDevice, pUser);
					bConnected = TRUE;
					//pThis->EnableWindowWhenOpenNotStart();
					break;
				}
				else
				{
					Sleep(100);
				}
			}

			if (bConnected && (pThis->m_status == CCamera::CameraStatus::CAMERA_OPEN))
			{
				pThis->m_pcMyCamera->Display(pThis->m_hwndDisplay);
				pThis->m_pcMyCamera->StartGrabbing();
				//pThis->EnableWindowWhenStart();
			}
		}
	}
}