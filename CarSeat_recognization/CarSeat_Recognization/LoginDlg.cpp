// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_USR_LOGIN, pParent)
	, m_bAutoSaveFlag(FALSE)
	, m_strUsrName(_T(""))
	, m_strPasswd(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SAVE_USRNAME_PASSWD, m_bAutoSaveFlag);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_strUsrName);
	DDX_Text(pDX, IDC_EDIT_USRNAME, m_strPasswd);
}

BOOL CLoginDlg::GetAutoSaveFlag()
{
	return m_bAutoSaveFlag;
}

CString CLoginDlg::GetLoginUsrName()
{

	return m_strUsrName;
}

CString CLoginDlg::GetLoginPasswd()
{
	return m_strPasswd;
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CDialogEx::OnOK();
}
