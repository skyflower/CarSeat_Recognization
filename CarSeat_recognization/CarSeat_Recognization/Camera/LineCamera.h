
// BasicDemoDlg.h : header file
// 

#pragma once

#include <iostream>
#include "Camera.h"
#include <Windows.h>
#include <Windows.h>
#include "../common/utils.h"

class CLineCamera
{
// Construction
public:
	CLineCamera(MV_CC_DEVICE_INFO *pDevice);	// Standard constructor
	CLineCamera() = delete;



	~CLineCamera();

/*ch:�ؼ���Ӧ���� | en:Control corresponding variable*/
private:
   
    /*ch:ͼ��ɼ� | en:Image Acquisition*/
      // ch:���� | en:Software trigger
    //BOOL    m_bSoftWareTriggerCheck;
            // ch:����Ϊjpg | en:Save as jpg

    /*ch:�������û�ȡ | en:Get Parameter Setting*/
          // ch:�ع�ʱ�� | en:Exposure Time
    double  m_dExposureTime;	//�ع�ʱ��
    double  m_dExposureGain; // ����
    double  m_dFrameRate;  // fps 

	CCamera::CameraStatus m_status;
	double m_dExposureTimeMax;
	double m_dExposureTimeMin;

	int m_nROI_Width;
	int m_nROI_Height;
	int m_nROI_OffsetX;
	int m_nROI_OffsetY;

	MV_CAM_EXPOSURE_AUTO_MODE m_nExposureAutoMode;
	int m_nWidthMax;
	int m_nHeightMax;
	wchar_t m_szImageDir[MAX_CHAR_LENGTH];

	//MV_CAM_BALANCEWHITE_AUTO m_nBalanceWhile;




    
   
/*ch:�ڲ����� | en:Built-in function*/
public:
    
	CCamera::CameraStatus GetCameraStatus();
   
    /*ch:���á���ȡ�������� | en:Set and get parameters operation*/
    bool SetTriggerMode(MV_CAM_TRIGGER_MODE mode);                // ch:���ô���ģʽ | en:Set Trigger Mode
	MV_CAM_TRIGGER_MODE GetTriggerMode();
    double GetExposureTime();               // ch:�����ع�ʱ�� | en:Set Exposure Time
    bool SetExposureTime(double time); 
    double GetGain();                       // ch:�������� | en:Set Gain
    bool SetGain(int gain);
    double GetFrameRate();                  // ch:����֡�� | en:Set Frame Rate
    bool SetFrameRate(double rate);
	MV_CAM_TRIGGER_SOURCE GetTriggerSource(void);              // ch:���ô���Դ | en:Set Trigger Source
    bool SetTriggerSource(MV_CAM_TRIGGER_SOURCE source);
	bool GetExposureTimeRange(double *timeMax, double *timeMin);
	bool SetExposureTimeAutoMode(MV_CAM_EXPOSURE_AUTO_MODE mode);
	bool SetImageSaveDirectory(const wchar_t* fileDir);
	const wchar_t *GetImageSaveDirectory();

	bool SetBalanceWhile(MV_CAM_BALANCEWHITE_AUTO balanceWhile);
	MV_CAM_BALANCEWHITE_AUTO GetBalanceWhile();



	void GetROIParameter(int *width, int *height, int *offsetX, int *offsetY);

	SIZE GetImageSize();

    /*ch:ͼƬ���� | en:Save Image*/
    std::wstring SaveImage(void);                     // ch:����ͼƬ | en:Save Image

   
/*ch:�Զ������ | en:User Defined Variable*/
private:
	MV_CAM_TRIGGER_MODE GetTriggerModeByCamera();
	double GetGainByCamera();
	double GetFrameRateByCamera();
	double GetExposureTimeByCamera();
	MV_CAM_TRIGGER_SOURCE GetTriggerSourceByCamera(void);
	double GetExposureTimeMaxByCamera();
	double GetExposureTimeMinByCamera();
	MV_CAM_EXPOSURE_AUTO_MODE GetExposureAutoModeByCamera();
	int GetWidthMaxByCamera();
	int GetHeightMaxByCamera();
	bool GetROIParameterByCamera(int *width, int *height, int *offsetX, int *offsetY);


	/*ch:�������û�ȡ | en:Parameters Get and Set*/
	void GetParameter();       // ch:��ȡ���� | en:Get Parameter
	

    /*ch:״̬ | en:Status*/
    //BOOL  m_bOpenDevice;                        // ch:�Ƿ���豸 | en:Whether to open device
    //BOOL  m_bStartGrabbing;                     // ch:�Ƿ�ʼץͼ | en:Whether to start grabbing
	MV_CAM_TRIGGER_MODE   m_nTriggerMode;                       // ch:����ģʽ | en:Trigger Mode
	MV_CAM_TRIGGER_SOURCE   m_nTriggerSource;                     // ch:����Դ | en:Trigger Source
    MV_SAVE_IAMGE_TYPE   m_nSaveImageType;      // ch:����ͼ���ʽ | en:Save Image Type

    /*ch:�豸��� | en:Device Related*/
    CCamera*      m_pcMyCamera;               // ch:CMyCamera��װ�˳��ýӿ� | en:CMyCamera packed commonly used interface
    HWND  m_hwndDisplay;                        // ch:��ʾ��� | en:Display Handle
	MV_CC_DEVICE_INFO *m_pDevice;
	
    unsigned char*  m_pBufForSaveImage;         // ch:���ڱ���ͼ��Ļ��� | en:Buffer to save image
    unsigned int    m_nBufSizeForSaveImage;

    unsigned char*  m_pBufForDriver;            // ch:���ڴ�������ȡͼ��Ļ��� | en:Buffer to get image from driver
    unsigned int    m_nBufSizeForDriver;

/*ch:���¿ؼ����� | en:Control operation*/
public:

    /*ch:��ʼ�� | en:Initialization*/
    //bool EnumButton();               // ch:�����豸 | en:Find Devices
    bool OpenButton();               // ch:���豸 | en:Open Devices
   
	/*ch:�ر����ٲ��� | en:Close and deatroy operation*/
	int CloseDevice(void);   // ch:�ر��豸 | en:Close Device


	/*ch:��ʼ��������� | en:Initialization*/
	int OpenDevice();   // ch:����� | en:Open Device

	bool StartGrabbing();      // ch:��ʼ�ɼ� | en:Start Grabbing
    void StopGrabbing();       // ch:�����ɼ� | en:Stop Grabbing
    void SoftwareOnce();       // ch:����һ�� | en:Software Trigger Execute Once
  
    /*ch:ͼ�񱣴� | en:Image Save*/
    std::wstring SaveBmp();            // ch:����bmp | en:Save bmp
    std::wstring SaveJpg();            // ch:����jpg | en:Save jpg
  
   
    
	void SetDisplayHwnd(HWND hwnd);
};
