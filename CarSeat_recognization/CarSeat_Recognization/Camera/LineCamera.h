
// BasicDemoDlg.h : header file
// 

#pragma once

#include <iostream>
#include "Camera.h"
#include <Windows.h>
#include <Windows.h>

class CLineCamera
{
// Construction
public:
	CLineCamera(MV_CC_DEVICE_INFO *pDevice);	// Standard constructor

	~CLineCamera();

/*ch:控件对应变量 | en:Control corresponding variable*/
private:
    /*ch:初始化 | en:Initialization*/
    
	//CameraStatus m_Status;
    /*ch:图像采集 | en:Image Acquisition*/
      // ch:软触发 | en:Software trigger
    BOOL    m_bSoftWareTriggerCheck;
            // ch:保存为jpg | en:Save as jpg

    /*ch:参数设置获取 | en:Get Parameter Setting*/
          // ch:曝光时间 | en:Exposure Time
    double  m_dExposureTime;	//曝光时间
    double  m_dExposureGain; // 增益
    double  m_dFrameRate;  // fps 
    
    /*ch:设备显示下拉框 | en:Device display drop-down box*/
    //int      m_nDeviceCombo;

/*ch:内部函数 | en:Built-in function*/
private:
    
    void ShowErrorMsg(CString csMessage, int nErrorNum);

    
   
    /*ch:设置、获取参数操作 | en:Set and get parameters operation*/
    bool SetTriggerMode(MV_CAM_TRIGGER_MODE mode);                // ch:设置触发模式 | en:Set Trigger Mode
	MV_CAM_TRIGGER_MODE GetTriggerMode();
    double GetExposureTime();               // ch:设置曝光时间 | en:Set Exposure Time
    bool SetExposureTime(double time); 
    double GetGain();                       // ch:设置增益 | en:Set Gain
    bool SetGain(int gain);
    double GetFrameRate();                  // ch:设置帧率 | en:Set Frame Rate
    bool SetFrameRate(double rate);
	MV_CAM_TRIGGER_SOURCE GetTriggerSource(void);              // ch:设置触发源 | en:Set Trigger Source
    bool SetTriggerSource(MV_CAM_TRIGGER_SOURCE source);


    /*ch:图片保存 | en:Save Image*/
    std::string SaveImage(void);                     // ch:保存图片 | en:Save Image

   
/*ch:自定义变量 | en:User Defined Variable*/
private:
	MV_CAM_TRIGGER_MODE GetTriggerModeByCamera();
	double GetGainByCamera();
	double GetFrameRateByCamera();
	double GetExposureTimeByCamera();
	MV_CAM_TRIGGER_SOURCE GetTriggerSourceByCamera(void);
	




    /*ch:状态 | en:Status*/
    BOOL  m_bOpenDevice;                        // ch:是否打开设备 | en:Whether to open device
    BOOL  m_bStartGrabbing;                     // ch:是否开始抓图 | en:Whether to start grabbing
	MV_CAM_TRIGGER_MODE   m_nTriggerMode;                       // ch:触发模式 | en:Trigger Mode
	MV_CAM_TRIGGER_SOURCE   m_nTriggerSource;                     // ch:触发源 | en:Trigger Source
    MV_SAVE_IAMGE_TYPE   m_nSaveImageType;      // ch:保存图像格式 | en:Save Image Type

    /*ch:设备相关 | en:Device Related*/
    CCamera*      m_pcMyCamera;               // ch:CMyCamera封装了常用接口 | en:CMyCamera packed commonly used interface
    HWND  m_hwndDisplay;                        // ch:显示句柄 | en:Display Handle
	MV_CC_DEVICE_INFO *m_pDevice;
	//MV_CC_DEVICE_INFO_LIST m_stDevList;         // ch:设备信息列表结构体变量，用来存储设备列表
                                                //en:Device information list structure variable used to store device list

    unsigned char*  m_pBufForSaveImage;         // ch:用于保存图像的缓存 | en:Buffer to save image
    unsigned int    m_nBufSizeForSaveImage;

    unsigned char*  m_pBufForDriver;            // ch:用于从驱动获取图像的缓存 | en:Buffer to get image from driver
    unsigned int    m_nBufSizeForDriver;

/*ch:按下控件操作 | en:Control operation*/
public:





    /*ch:初始化 | en:Initialization*/
    //bool EnumButton();               // ch:查找设备 | en:Find Devices
    bool OpenButton();               // ch:打开设备 | en:Open Devices
   
	/*ch:关闭销毁操作 | en:Close and deatroy operation*/
	int CloseDevice(void);   // ch:关闭设备 | en:Close Device


	/*ch:初始化相机操作 | en:Initialization*/
	int OpenDevice();   // ch:打开相机 | en:Open Device

   
    /*ch:图像采集 | en:Image Acquisition*/
    //void ContinusMode();        // ch:连续模式 | en:Continus Mode
    //void TriggerMode();         // ch:触发模式 | en:Trigger Mode
    

	bool StartGrabbing();      // ch:开始采集 | en:Start Grabbing
    void StopGrabbing();       // ch:结束采集 | en:Stop Grabbing
    void SoftwareOnce();       // ch:软触发一次 | en:Software Trigger Execute Once
  
    /*ch:图像保存 | en:Image Save*/
    std::string SaveBmp();            // ch:保存bmp | en:Save bmp
    std::string SaveJpg();            // ch:保存jpg | en:Save jpg
  
    /*ch:参数设置获取 | en:Parameters Get and Set*/
    void GetParameter();       // ch:获取参数 | en:Get Parameter
    
	void SetDisplayHwnd(HWND hwnd);

};
