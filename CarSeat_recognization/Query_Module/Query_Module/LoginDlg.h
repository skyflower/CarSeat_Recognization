#pragma once

#include "./common/utils.h"

// CLoginDlg �Ի���

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

	bool GetLoginUserName(wchar_t *str, int &length);
	bool GetLoginPasswd(wchar_t *str, int &length);
	bool GetAutoSaveFlag();

	void SetLoginUserName(const wchar_t *str);
	void SetLoginPasswd(const wchar_t *str);
	void SetAutoSaveFlag(bool flag);

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	bool mAutoSaveFlag;
	wchar_t mUserName[MAX_CHAR_LENGTH];
	wchar_t mPasswd[MAX_CHAR_LENGTH];
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
