// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "./network/NetworkTask.h"
#include "./common/ParamManager.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_USR_LOGIN, pParent)
	, m_bAutoSaveFlag(FALSE)
	, m_strUsrName(_T(""))
	, m_strPasswd(_T(""))
	, m_pLabelManager(nullptr)
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

bool CLoginDlg::SetLabelManager(CLabelManager * pLabelManager)
{
	if (pLabelManager == nullptr)
	{
		return false;
	}
	m_pLabelManager = pLabelManager;

	return true;
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


bool CLoginDlg::CheckUsrNameAndPasswd()
{
	return true;
}

void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (CheckUsrNameAndPasswd() == false)
	{
		return;
	}
	
	CDialogEx::OnOK();
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_pLabelManager != nullptr)
	{
		
		m_strUsrName = m_pLabelManager->GetLoginUsrName();
		m_strPasswd = m_pLabelManager->GetLoginPasswd();
		m_bAutoSaveFlag = m_pLabelManager->GetLoginAutoSave();
		UpdateData(FALSE); // false 将数值从变量传给控件
	}
	


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
