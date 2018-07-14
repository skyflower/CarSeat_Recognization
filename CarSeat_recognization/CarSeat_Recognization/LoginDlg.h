#pragma once

#include "./common/LabelManager.h"

// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_USR_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	BOOL GetAutoSaveFlag();
	CString GetLoginUsrName();
	CString GetLoginPasswd();

	bool SetLabelManager(CLabelManager *pLabelManager);
	
private:
	// //自动保存用户名和密码
	BOOL m_bAutoSaveFlag;
	// //用户名
	CString m_strUsrName;
	// //密码
	CString m_strPasswd;

	CLabelManager *m_pLabelManager;

	/*
	通过网络模块验证用户名和密码，还未全部实现，默认返回验证通过
	*/
	bool CheckUsrNameAndPasswd();

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
