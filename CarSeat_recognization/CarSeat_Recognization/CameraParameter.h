#pragma once
#include <Windows.h>
#include <iostream>

// CCameraParameter �Ի���

class CCameraParameter : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraParameter)

public:
	CCameraParameter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCameraParameter();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAMERA_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
