// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Query_Module.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent),
	mAutoSaveFlag(false)
{
	memset(mUserName, 0, sizeof(mUserName));
	memset(mPasswd, 0, sizeof(mPasswd));
}

CLoginDlg::~CLoginDlg()
{
}

bool CLoginDlg::GetLoginUserName(wchar_t * str, int & length)
{
	if (wcslen(mUserName) > 0)
	{
		length = wcslen(mUserName);
		memcpy(str, mUserName, sizeof(wchar_t) * length);
		return true;
	}
	return false;
}

bool CLoginDlg::GetLoginPasswd(wchar_t * str, int & length)
{
	if (wcslen(mPasswd) > 0)
	{
		length = wcslen(mPasswd);
		memcpy(str, mPasswd, sizeof(wchar_t) * length);
		return true;
	}
	return false;
}

bool CLoginDlg::GetAutoSaveFlag()
{
	return mAutoSaveFlag;
}

void CLoginDlg::SetLoginUserName(const wchar_t * str)
{
	if (str != nullptr)
	{
		memcpy(mUserName, str, sizeof(wchar_t) * wcslen(str));
	}
}

void CLoginDlg::SetLoginPasswd(const wchar_t * str)
{
	if (str != nullptr)
	{
		memcpy(mPasswd, str, sizeof(wchar_t) * wcslen(str));
	}
}

void CLoginDlg::SetAutoSaveFlag(bool flag)
{
	mAutoSaveFlag = flag;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������


void CLoginDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	memset(mUserName, 0, sizeof(mUserName));
	GetDlgItemText(IDC_EDIT_USERNAME, mUserName, MAX_CHAR_LENGTH);

	
	memset(mPasswd, 0, sizeof(mPasswd));
	GetDlgItemText(IDC_EDIT_PASSWD, mPasswd, MAX_CHAR_LENGTH);

	int ret = ((CButton*)GetDlgItem(IDC_CHECK_SAVE_USERNAME_PASSWD))->GetCheck();
	
	mAutoSaveFlag = (ret == 1);

	CDialogEx::OnOK();
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if (wcslen(mUserName) > 0)
	{
		GetDlgItem(IDC_EDIT_USERNAME)->SetWindowTextW(mUserName);
	}

	if (wcslen(mPasswd) > 0)
	{
		GetDlgItem(IDC_EDIT_PASSWD)->SetWindowTextW(mPasswd);
	}
	((CButton*)GetDlgItem(IDC_CHECK_SAVE_USERNAME_PASSWD))->SetCheck(mAutoSaveFlag);

	//int ret = ((CButton*)GetDlgItem(IDC_CHECK_SAVE_USERNAME_PASSWD))->GetCheck();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
