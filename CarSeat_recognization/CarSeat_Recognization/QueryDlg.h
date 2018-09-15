﻿#pragma once
#include "afxwin.h"
#include "./common/RecogResultManager.h"
#include <atlimage.h>


// CQueryDlg 对话框

class CQueryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQueryDlg)

public:
	CQueryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQueryDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_QUERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CRecogResultManager *m_pRecogManager;

	CButton m_QueryButton;
	CString m_szQueryBarcode;
	CStatic m_ImageDisplay;
	CString m_szQueryResult;
	CImage m_Image;


	void displayImage(CImage * pImage, CStatic * pStatic);
public:
	afx_msg void OnBnClickedButtonQuery();
	virtual BOOL OnInitDialog();
};
