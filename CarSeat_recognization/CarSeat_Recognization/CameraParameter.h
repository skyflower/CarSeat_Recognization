#pragma once
#include <Windows.h>
#include <iostream>

// CCameraParameter 对话框

class CCameraParameter : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraParameter)

public:
	CCameraParameter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCameraParameter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAMERA_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
