// CameraParameter.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CameraParameterDlg.h"
#include "afxdialogex.h"


// CCameraParameterDlg 对话框

IMPLEMENT_DYNAMIC(CCameraParameterDlg, CDialogEx)

CCameraParameterDlg::CCameraParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CAMERA_PARAMETER, pParent)
{

}

CCameraParameterDlg::~CCameraParameterDlg()
{
}

void CCameraParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraParameterDlg, CDialogEx)
END_MESSAGE_MAP()


// CCameraParameterDlg 消息处理程序
