#pragma once

#include "./common/utils.h"

// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

	bool GetLoginUserName(wchar_t *str, int &length);
	bool GetLoginPasswd(wchar_t *str, int &length);
	bool GetAutoSaveFlag();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	bool mAutoSaveFlag;
	wchar_t mUserName[MAX_CHAR_LENGTH];
	wchar_t mPasswd[MAX_CHAR_LENGTH];
	virtual void OnOK();
};
