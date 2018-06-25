// CameraParameter.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CameraParameter.h"
#include "afxdialogex.h"


// CCameraParameter 对话框

IMPLEMENT_DYNAMIC(CCameraParameter, CDialogEx)

CCameraParameter::CCameraParameter(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CCameraParameter::~CCameraParameter()
{
}

void CCameraParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraParameter, CDialogEx)
END_MESSAGE_MAP()


// CCameraParameter 消息处理程序
