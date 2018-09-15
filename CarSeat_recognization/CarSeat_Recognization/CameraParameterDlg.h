#pragma once
#include <Windows.h>
#include <iostream>
//#include "./Camera/LineCamera.h"

// CCameraParameterDlg 对话框

class CCameraParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraParameterDlg)

public:
	CCameraParameterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCameraParameterDlg();

	//void SetLineCamera(CLineCamera *pCamera);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAMERA_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// //增益db
	double m_dGainDB;
	// //设置曝光时间
	UINT m_uExposureTimeLower;
	// //帧率
	float m_fCameraFPS;

	//CLineCamera *m_pLineCamera;

public:
	afx_msg void OnBnClickedButtonGetParameter();
	afx_msg void OnBnClickedButtonSetParameter();
	virtual BOOL OnInitDialog();
private:
	// //自动曝光上限
	UINT m_uExposureTimeUpper;
};
