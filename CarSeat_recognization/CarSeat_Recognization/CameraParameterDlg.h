#pragma once
#include <Windows.h>
#include <iostream>

// CCameraParameterDlg �Ի���

class CCameraParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraParameterDlg)

public:
	CCameraParameterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCameraParameterDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAMERA_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
