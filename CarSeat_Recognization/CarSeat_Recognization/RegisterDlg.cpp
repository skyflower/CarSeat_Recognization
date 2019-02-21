// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "common/utils.h"


// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REGISTER_SOFTWARE_DLG, pParent)
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::SetMachineCode(char * code, int len)
{
	memset(m_szMachineCode, 0, sizeof(m_szMachineCode));
	int tmpLen = (len >= 200 ? sizeof(m_szMachineCode) / sizeof(m_szMachineCode[0]) - 1 : len);
	memcpy(m_szMachineCode, code, sizeof(char) * tmpLen);
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
END_MESSAGE_MAP()


// CRegisterDlg 消息处理程序


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	wchar_t info[] = L"请按照以下步骤注册软件："
		"1.请将%s，以及软件注册类型（单台设备注册、10台设备以上批量注册）通过邮件发送给软件服务部门。"
		"2.将需要注册的设备连接到互联网"
		"3.将软件服务部门发送的软件注册码输入到右边的编辑框中，然后点击确定。（确保这个过程设备一直处于网络连接状态）";

	GetDlgItem(IDC_STATIC_REGISTER_INFO)->SetWindowTextW(info);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
