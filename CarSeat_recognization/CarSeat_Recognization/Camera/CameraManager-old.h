
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
	CCameraManager();	// Standard constructor

	~CCameraManager();

	enum class CameraStatus
	{
		INIT,
		SEARCH,
		OPEN,
		BIND,
		GRAB,
		CLOSE
	};



/*ch:控件对应变量 | en:Control corresponding variable*/
private:
    /*ch:初始化 | en:Initialization*/
    
	CameraStatus m_Status;
    /*ch:图像采集 | en:Image Acquisition*/
      // ch:软触发 | en:Software trigger
    BOOL    m_bSoftWareTriggerCheck;
            // ch:保存为jpg | en:Save as jpg

    /*ch:参数设置获取 | en:Get Parameter Setting*/
          // ch:曝光时间 | en:Exposure Time
    double  m_dExposureEdit;
    double  m_dGainEdit;
    double  m_dFrameRateEdit;   
    
    /*ch:设备显示下拉框 | en:Device display drop-down box*/
    int      m_nDeviceCombo;

/*ch:内部函数 | en:Built-in function*/
private:
    
    void ShowErrorMsg(CString csMessage, int nErrorNum);

    
    /*ch:初始化相机操作 | en:Initialization*/
    int OpenDevice(void);                    // ch:打开相机 | en:Open Device

    /*ch:设置、获取参数操作 | en:Set and get parameters operation*/
    int SetTriggerMode(void);                // ch:设置触发模式 | en:Set Trigger Mode
    int GetTriggerMode(void);
    int GetExposureTime(void);               // ch:设置曝光时间 | en:Set Exposure Time
    int SetExposureTime(void); 
    int GetGain(void);                       // ch:设置增益 | en:Set Gain
    int SetGain(void);
    int GetFrameRate(void);                  // ch:设置帧率 | en:Set Frame Rate
    int SetFrameRate(void);
    int GetTriggerSource(void);              // ch:设置触发源 | en:Set Trigger Source
    int SetTriggerSource(void);


    /*ch:图片保存 | en:Save Image*/
    int SaveImage(void);                     // ch:保存图片 | en:Save Image

    /*ch:关闭销毁操作 | en:Close and deatroy operation*/
    int CloseDevice(void);                   // ch:关闭设备 | en:Close Device

/*ch:自定义变量 | en:User Defined Variable*/
private:
    /*ch:状态 | en:Status*/
    BOOL  m_bOpenDevice;                        // ch:是否打开设备 | en:Whether to open device
    BOOL  m_bStartGrabbing;                     // ch:是否开始抓图 | en:Whether to start grabbing
    int   m_nTriggerMode;                       // ch:触发模式 | en:Trigger Mode
    int   m_nTriggerSource;                     // ch:触发源 | en:Trigger Source
    MV_SAVE_IAMGE_TYPE   m_nSaveImageType;      // ch:保存图像格式 | en:Save Image Type

    /*ch:设备相关 | en:Device Related*/
    CCamera*      m_pcMyCamera;               // ch:CMyCamera封装了常用接口 | en:CMyCamera packed commonly used interface
    HWND  m_hwndDisplay;                        // ch:显示句柄 | en:Display Handle
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // ch:设备信息列表结构体变量，用来存储设备列表
                                                //en:Device information list structure variable used to store device list

    unsigned char*  m_pBufForSaveImage;         // ch:用于保存图像的缓存 | en:Buffer to save image
    unsigned int    m_nBufSizeForSaveImage;

    unsigned char*  m_pBufForDriver;            // ch:用于从驱动获取图像的缓存 | en:Buffer to get image from driver
    unsigned int    m_nBufSizeForDriver;

/*ch:按下控件操作 | en:Control operation*/
public:
    /*ch:初始化 | en:Initialization*/
    bool EnumButton();               // ch:查找设备 | en:Find Devices
    bool OpenButton();               // ch:打开设备 | en:Open Devices
    void CloseButton();              // ch:关闭设备 | en:Close Devices
   
    /*ch:图像采集 | en:Image Acquisition*/
    void ContinusModeRadio();        // ch:连续模式 | en:Continus Mode
    void TriggerModeRadio();         // ch:触发模式 | en:Trigger Mode
    bool StartGrabbingButton();      // ch:开始采集 | en:Start Grabbing
    void StopGrabbingButton();       // ch:结束采集 | en:Stop Grabbing
    void SoftwareTriggerCheck();     // ch:软触发 | en:Software Trigger
    void SoftwareOnceButton();       // ch:软触发一次 | en:Software Trigger Execute Once
  
    /*ch:图像保存 | en:Image Save*/
    void SaveBmpButton();            // ch:保存bmp | en:Save bmp
    void SaveJpgButton();            // ch:保存jpg | en:Save jpg
  
    /*ch:参数设置获取 | en:Parameters Get and Set*/
    void GetParameterButton();       // ch:获取参数 | en:Get Parameter
    void SetParameterButton();       // ch:设置参数 | en:Exit from upper right corner

	int GetCameraCount();
	void SetDisplayHwnd(HWND hwnd);
	CameraStatus GetStatus();

};
