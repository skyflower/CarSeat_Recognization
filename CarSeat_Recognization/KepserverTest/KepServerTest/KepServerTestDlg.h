
// KepServerTestDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "OPC.h"


// CKepServerTestDlg 对话框
class CKepServerTestDlg : public CDialogEx
{
// 构造
public:
	CKepServerTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEPSERVERTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton onButtonConnectServer;
//	CEdit onButtonWriteData;
private:
	CString m_ServerName;
public:
	COPC Opc;
	CString m_PlcFlag;
	CButton onButtonWriteData;
	float m_value;
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonWriteData();
};
