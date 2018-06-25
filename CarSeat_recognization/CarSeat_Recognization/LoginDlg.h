#pragma once


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
	
private:
	// //自动保存用户名和密码
	BOOL m_bAutoSaveFlag;
	// //用户名
	CString m_strUsrName;
	// //密码
	CString m_strPasswd;
public:
	afx_msg void OnBnClickedOk();
};
