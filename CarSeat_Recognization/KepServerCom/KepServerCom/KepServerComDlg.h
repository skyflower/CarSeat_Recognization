
// KepServerComDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Communication.h"
#include <thread>

// CKepServerComDlg 对话框
class CKepServerComDlg : public CDialogEx
{
// 构造
public:
	CKepServerComDlg(CWnd* pParent = NULL);	// 标准构造函数
	void DoEvent();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEPSERVERCOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	std::thread m_pThread;
	bool pServerConnected;
	CDataCtrl CD;
	static void MyThreadProc(CKepServerComDlg*pThis, LPVOID pParam);

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
	CButton B_ServerConnect;
	CString m_ServerName;
	CString m_ReadFlagName;
	CString m_ConncetResult;
	CString m_WriteFlagName;
	afx_msg void OnBnClickedConnect();
	afx_msg void OnClose();
};
