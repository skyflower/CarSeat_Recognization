// QueryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "QueryDlg.h"
#include "afxdialogex.h"


// CQueryDlg 对话框

IMPLEMENT_DYNAMIC(CQueryDlg, CDialogEx)

CQueryDlg::CQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_QUERY, pParent)
	, m_szQueryBarcode(_T(""))
	, m_szQueryResult(_T(""))
{

}

CQueryDlg::~CQueryDlg()
{
}

void CQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_ST_QUERY_BARCODE, m_szStatic);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_QueryButton);
	DDX_Text(pDX, IDC_ED_QUERY_BARCODE, m_szQueryBarcode);
	DDX_Control(pDX, IDC_QUERY_IMAGE, m_ImageDisplay);
	DDX_Text(pDX, IDC_IMAGE_RESULT, m_szQueryResult);
}


BEGIN_MESSAGE_MAP(CQueryDlg, CDialogEx)
END_MESSAGE_MAP()


// CQueryDlg 消息处理程序
