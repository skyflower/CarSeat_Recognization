// ConditonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Query_Module.h"
#include "ConditonDlg.h"
#include "afxdialogex.h"


// CConditonDlg 对话框

IMPLEMENT_DYNAMIC(CConditonDlg, CDialogEx)

CConditonDlg::CConditonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONDITION, pParent)
{

}

CConditonDlg::~CConditonDlg()
{
}

CConditionFilter CConditonDlg::GetFilterCondition()
{
	return mFilter;
}

void CConditonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConditonDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConditonDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConditonDlg 消息处理程序



void CConditonDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	memset(&mFilter, 0, sizeof(CConditionFilter));

	wchar_t text[256];

	//日期开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_DATE_BEGIN, text, sizeof(text));
	memcpy(mFilter.mDateBeign, text, wcslen(text));

	//日期结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_DATE_END, text, sizeof(text));
	memcpy(mFilter.mDateEnd, text, wcslen(text));


	//时间开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_TIME_BEGIN, text, sizeof(text));
	memcpy(mFilter.mTimeBegin, text, wcslen(text));

	//时间结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_TIME_END, text, sizeof(text));
	memcpy(mFilter.mTimeEnd, text, wcslen(text));


	//产线开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_LINE_BEGIN, text, sizeof(text));
	memcpy(mFilter.mLineBegin, text, wcslen(text));

	//产线结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_LINE_END, text, sizeof(text));
	memcpy(mFilter.mLineEnd, text, wcslen(text));

	//条形码开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_BARCODE_BEGIN, text, sizeof(text));
	memcpy(mFilter.mBarcodeBegin, text, wcslen(text));

	//条形码结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_BARCODE_END, text, sizeof(text));
	memcpy(mFilter.mBarcodeEnd, text, wcslen(text));

	//座椅类型
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_SEAT_TYPE, text, sizeof(text));
	memcpy(mFilter.mSeatType, text, wcslen(text));

	//识别类型
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_METHOD_TYPE, text, sizeof(text));
	memcpy(mFilter.mMethodType, text, wcslen(text));

	CDialogEx::OnOK();
}
