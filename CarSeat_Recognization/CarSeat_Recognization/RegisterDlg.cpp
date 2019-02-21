// RegisterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "common/utils.h"


// CRegisterDlg �Ի���

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


// CRegisterDlg ��Ϣ�������


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	wchar_t info[] = L"�밴�����²���ע�������"
		"1.�뽫%s���Լ����ע�����ͣ���̨�豸ע�ᡢ10̨�豸��������ע�ᣩͨ���ʼ����͸���������š�"
		"2.����Ҫע����豸���ӵ�������"
		"3.����������ŷ��͵����ע�������뵽�ұߵı༭���У�Ȼ����ȷ������ȷ����������豸һֱ������������״̬��";

	GetDlgItem(IDC_STATIC_REGISTER_INFO)->SetWindowTextW(info);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
