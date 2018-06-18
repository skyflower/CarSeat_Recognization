// ConditonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Query_Module.h"
#include "ConditonDlg.h"
#include "afxdialogex.h"
#include "./common/ParamManager.h"
#include "./common/Log.h"
#include "./common/utils.h"


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
	DDX_Control(pDX, IDC_LINE_BEGIN, mLineBegin);
	DDX_Control(pDX, IDC_LINE_END, mLineEnd);
	DDX_Control(pDX, IDC_BARCODE_BEGIN, mBarcodeBegin);
	DDX_Control(pDX, IDC_BARCODE_END, mBarcodeEnd);
	DDX_Control(pDX, IDC_SEAT_TYPE, mSeatType);
	DDX_Control(pDX, IDC_METHOD_TYPE, mMethodType);
	DDX_Control(pDX, IDC_DATE_BEGIN, mDateBegin);
	DDX_Control(pDX, IDC_TIME_BEGIN, mTimeBegin);
	DDX_Control(pDX, IDC_DATE_END, mDateEnd);
	DDX_Control(pDX, IDC_TIME_END, mTimeEnd);
}


BEGIN_MESSAGE_MAP(CConditonDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConditonDlg::OnBnClickedOk)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_BEGIN, &CConditonDlg::OnDatetimechangeDateBegin)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIME_BEGIN, &CConditonDlg::OnDatetimechangeTimeBegin)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIME_END, &CConditonDlg::OnDatetimechangeTimeEnd)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_END, &CConditonDlg::OnDatetimechangeDateEnd)
	ON_CBN_SELCHANGE(IDC_LINE_BEGIN, &CConditonDlg::OnSelchangeLineBegin)
	ON_CBN_SELCHANGE(IDC_LINE_END, &CConditonDlg::OnSelchangeLineEnd)
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


BOOL CConditonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CParamManager *pParamManager = CParamManager::GetInstance();
	if (pParamManager == nullptr)
	{
		WriteInfo("get CParamManager::GetInstance failed");
		return TRUE;
	}

	const std::vector<std::wstring> *p = pParamManager->GetMethodType();
	if (p != nullptr)
	{
		mMethodType.ResetContent();
		for (auto &k : *p)
		{
			mMethodType.AddString(k.c_str());
		}
	}

	p = pParamManager->GetSeatType();
	if (p != nullptr)
	{
		mSeatType.ResetContent();
		for (auto &k : *p)
		{
			mSeatType.AddString(k.c_str());
		}
	}

	p = pParamManager->GetLineNo();
	if (p != nullptr)
	{
		mLineBegin.ResetContent();
		mLineEnd.ResetContent();
		for (auto &k : *p)
		{
			mLineBegin.AddString(k.c_str());
			mLineEnd.AddString(k.c_str());
		}
	}
	



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CConditonDlg::OnDatetimechangeDateBegin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	wchar_t text[200];
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_DATE_BEGIN, text, sizeof(text));
	TRACE1("date time begin = %s\n", text);

	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_DATE_END, text, sizeof(text));
	TRACE1("date time end = %s\n", text);




	*pResult = 0;
}


void CConditonDlg::OnDatetimechangeTimeBegin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CConditonDlg::OnDatetimechangeTimeEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CConditonDlg::OnDatetimechangeDateEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CConditonDlg::OnSelchangeLineBegin()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CConditonDlg::OnSelchangeLineEnd()
{
	// TODO: 在此添加控件通知处理程序代码
}
