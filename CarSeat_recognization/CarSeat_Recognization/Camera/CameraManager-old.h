
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



/*ch:�ؼ���Ӧ���� | en:Control corresponding variable*/
private:
    /*ch:��ʼ�� | en:Initialization*/
    
	CameraStatus m_Status;
    /*ch:ͼ��ɼ� | en:Image Acquisition*/
      // ch:���� | en:Software trigger
    BOOL    m_bSoftWareTriggerCheck;
            // ch:����Ϊjpg | en:Save as jpg

    /*ch:�������û�ȡ | en:Get Parameter Setting*/
          // ch:�ع�ʱ�� | en:Exposure Time
    double  m_dExposureEdit;
    double  m_dGainEdit;
    double  m_dFrameRateEdit;   
    
    /*ch:�豸��ʾ������ | en:Device display drop-down box*/
    int      m_nDeviceCombo;

/*ch:�ڲ����� | en:Built-in function*/
private:
    
    void ShowErrorMsg(CString csMessage, int nErrorNum);

    
    /*ch:��ʼ��������� | en:Initialization*/
    int OpenDevice(void);                    // ch:����� | en:Open Device

    /*ch:���á���ȡ�������� | en:Set and get parameters operation*/
    int SetTriggerMode(void);                // ch:���ô���ģʽ | en:Set Trigger Mode
    int GetTriggerMode(void);
    int GetExposureTime(void);               // ch:�����ع�ʱ�� | en:Set Exposure Time
    int SetExposureTime(void); 
    int GetGain(void);                       // ch:�������� | en:Set Gain
    int SetGain(void);
    int GetFrameRate(void);                  // ch:����֡�� | en:Set Frame Rate
    int SetFrameRate(void);
    int GetTriggerSource(void);              // ch:���ô���Դ | en:Set Trigger Source
    int SetTriggerSource(void);


    /*ch:ͼƬ���� | en:Save Image*/
    int SaveImage(void);                     // ch:����ͼƬ | en:Save Image

    /*ch:�ر����ٲ��� | en:Close and deatroy operation*/
    int CloseDevice(void);                   // ch:�ر��豸 | en:Close Device

/*ch:�Զ������ | en:User Defined Variable*/
private:
    /*ch:״̬ | en:Status*/
    BOOL  m_bOpenDevice;                        // ch:�Ƿ���豸 | en:Whether to open device
    BOOL  m_bStartGrabbing;                     // ch:�Ƿ�ʼץͼ | en:Whether to start grabbing
    int   m_nTriggerMode;                       // ch:����ģʽ | en:Trigger Mode
    int   m_nTriggerSource;                     // ch:����Դ | en:Trigger Source
    MV_SAVE_IAMGE_TYPE   m_nSaveImageType;      // ch:����ͼ���ʽ | en:Save Image Type

    /*ch:�豸��� | en:Device Related*/
    CCamera*      m_pcMyCamera;               // ch:CMyCamera��װ�˳��ýӿ� | en:CMyCamera packed commonly used interface
    HWND  m_hwndDisplay;                        // ch:��ʾ��� | en:Display Handle
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // ch:�豸��Ϣ�б�ṹ������������洢�豸�б�
                                                //en:Device information list structure variable used to store device list

    unsigned char*  m_pBufForSaveImage;         // ch:���ڱ���ͼ��Ļ��� | en:Buffer to save image
    unsigned int    m_nBufSizeForSaveImage;

    unsigned char*  m_pBufForDriver;            // ch:���ڴ�������ȡͼ��Ļ��� | en:Buffer to get image from driver
    unsigned int    m_nBufSizeForDriver;

/*ch:���¿ؼ����� | en:Control operation*/
public:
    /*ch:��ʼ�� | en:Initialization*/
    bool EnumButton();               // ch:�����豸 | en:Find Devices
    bool OpenButton();               // ch:���豸 | en:Open Devices
    void CloseButton();              // ch:�ر��豸 | en:Close Devices
   
    /*ch:ͼ��ɼ� | en:Image Acquisition*/
    void ContinusModeRadio();        // ch:����ģʽ | en:Continus Mode
    void TriggerModeRadio();         // ch:����ģʽ | en:Trigger Mode
    bool StartGrabbingButton();      // ch:��ʼ�ɼ� | en:Start Grabbing
    void StopGrabbingButton();       // ch:�����ɼ� | en:Stop Grabbing
    void SoftwareTriggerCheck();     // ch:���� | en:Software Trigger
    void SoftwareOnceButton();       // ch:����һ�� | en:Software Trigger Execute Once
  
    /*ch:ͼ�񱣴� | en:Image Save*/
    void SaveBmpButton();            // ch:����bmp | en:Save bmp
    void SaveJpgButton();            // ch:����jpg | en:Save jpg
  
    /*ch:�������û�ȡ | en:Parameters Get and Set*/
    void GetParameterButton();       // ch:��ȡ���� | en:Get Parameter
    void SetParameterButton();       // ch:���ò��� | en:Exit from upper right corner

	int GetCameraCount();
	void SetDisplayHwnd(HWND hwnd);
	CameraStatus GetStatus();

};
