#pragma once
#include <Windows.h>
#include <iostream>
#include "./Camera/LineCamera.h"

// CCameraParameterDlg �Ի���

class CCameraParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraParameterDlg)

public:
	CCameraParameterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCameraParameterDlg();

	void SetLineCamera(CLineCamera *pCamera);

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAMERA_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// //����db
	double m_dGainDB;
	// //�����ع�ʱ��
	UINT m_uExposureTime;
	// //֡��
	float m_fCameraFPS;

	CLineCamera *m_pLineCamera;

public:
	afx_msg void OnBnClickedButtonGetParameter();
	afx_msg void OnBnClickedButtonSetParameter();
	virtual BOOL OnInitDialog();
};
