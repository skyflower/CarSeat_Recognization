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
	ON_CBN_SETFOCUS(IDC_LINE_END, &CConditonDlg::OnSetfocusLineEnd)
	ON_CBN_SETFOCUS(IDC_LINE_BEGIN, &CConditonDlg::OnSetfocusLineBegin)
	ON_CBN_SELCHANGE(IDC_LINE_BEGIN, &CConditonDlg::OnSelchangeLineBegin)
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
	int tmpLength = sizeof(mFilter.mDateBeign);
	utils::WCharToChar(text, mFilter.mDateBeign, &tmpLength);
	//memcpy(mFilter.mDateBeign, text, wcslen(text));

	//日期结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_DATE_END, text, sizeof(text));
	tmpLength = sizeof(mFilter.mDateEnd);
	utils::WCharToChar(text, mFilter.mDateEnd, &tmpLength);
	//memcpy(mFilter.mDateEnd, text, wcslen(text));


	//时间开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_TIME_BEGIN, text, sizeof(text));
	tmpLength = sizeof(mFilter.mTimeBegin);
	utils::WCharToChar(text, mFilter.mTimeBegin, &tmpLength);

	//memcpy(mFilter.mTimeBegin, text, wcslen(text));

	//时间结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_TIME_END, text, sizeof(text));
	tmpLength = sizeof(mFilter.mTimeEnd);
	utils::WCharToChar(text, mFilter.mTimeEnd, &tmpLength);

	//memcpy(mFilter.mTimeEnd, text, wcslen(text));


	//产线开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_LINE_BEGIN, text, sizeof(text));
	tmpLength = sizeof(mFilter.mLineBegin);
	utils::WCharToChar(text, mFilter.mLineBegin, &tmpLength);

	//memcpy(mFilter.mLineBegin, text, wcslen(text));

	//产线结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_LINE_END, text, sizeof(text));
	tmpLength = sizeof(mFilter.mLineEnd);
	utils::WCharToChar(text, mFilter.mLineEnd, &tmpLength);

	//memcpy(mFilter.mLineEnd, text, wcslen(text));

	//条形码开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_BARCODE_BEGIN, text, sizeof(text));
	tmpLength = sizeof(mFilter.mBarcodeBegin);
	utils::WCharToChar(text, mFilter.mBarcodeBegin, &tmpLength);

	//memcpy(mFilter.mBarcodeBegin, text, wcslen(text));

	//条形码结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_BARCODE_END, text, sizeof(text));
	tmpLength = sizeof(mFilter.mBarcodeEnd);
	utils::WCharToChar(text, mFilter.mBarcodeEnd, &tmpLength);

	//memcpy(mFilter.mBarcodeEnd, text, wcslen(text));

	//座椅类型
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_SEAT_TYPE, text, sizeof(text));
	tmpLength = sizeof(mFilter.mSeatType);
	utils::WCharToChar(text, mFilter.mSeatType, &tmpLength);

	//memcpy(mFilter.mSeatType, text, wcslen(text));

	//识别类型
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_METHOD_TYPE, text, sizeof(text));
	tmpLength = sizeof(mFilter.mMethodType);
	utils::WCharToChar(text, mFilter.mMethodType, &tmpLength);

	//memcpy(mFilter.mMethodType, text, wcslen(text));

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

	const std::vector<std::string> *p = pParamManager->GetMethodType();
	if (p != nullptr)
	{
		mMethodType.ResetContent();
		for (auto &k : *p)
		{
			std::wstring tmpStr = utils::StrToWStr(k);
			mMethodType.AddString(tmpStr.c_str());
		}
	}

	p = pParamManager->GetSeatType();
	if (p != nullptr)
	{
		mSeatType.ResetContent();
		for (auto &k : *p)
		{
			std::wstring tmpStr = utils::StrToWStr(k);
			mSeatType.AddString(tmpStr.c_str());
		}
	}

	p = pParamManager->GetLineNo();
	if (p != nullptr)
	{
		mLineBegin.ResetContent();
		mLineEnd.ResetContent();
		for (auto &k : *p)
		{
			std::wstring tmpStr = utils::StrToWStr(k);
			mLineBegin.AddString(tmpStr.c_str());
			mLineEnd.AddString(tmpStr.c_str());
		}
	}
	



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CConditonDlg::OnDatetimechangeDateBegin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	SYSTEMTIME begin;
	memset(&begin, 0, sizeof(begin));
	mDateBegin.GetTime(&begin);

	if (utils::SystemTimeCmp(curTime, begin) < 0)
	{
		mDateBegin.SetTime(&curTime);
		return;
	}

	SYSTEMTIME end;
	memset(&end, 0, sizeof(end));
	mDateEnd.GetTime(&end);
	if (utils::SystemTimeCmp(end, begin) < 0)
	{
		mDateEnd.SetTime(&begin);
	}

	*pResult = 0;
}


void CConditonDlg::OnDatetimechangeTimeBegin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	SYSTEMTIME begin;
	memset(&begin, 0, sizeof(begin));
	mTimeBegin.GetTime(&begin);

	if (utils::SystemTimeCmp(curTime, begin) < 0)
	{
		mTimeBegin.SetTime(&curTime);
		return;
	}

	SYSTEMTIME end;
	memset(&end, 0, sizeof(end));
	mTimeEnd.GetTime(&end);
	if (utils::SystemTimeCmp(end, begin) < 0)
	{
		mTimeEnd.SetTime(&begin);
	}
}


void CConditonDlg::OnDatetimechangeTimeEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	SYSTEMTIME end;
	memset(&end, 0, sizeof(end));
	mTimeEnd.GetTime(&end);

	if (utils::SystemTimeCmp(curTime, end) < 0)
	{
		mTimeBegin.SetTime(&curTime);
		return;
	}

	SYSTEMTIME begin;
	memset(&begin, 0, sizeof(begin));
	mTimeBegin.GetTime(&begin);
	if (utils::SystemTimeCmp(end, begin) < 0)
	{
		mTimeEnd.SetTime(&begin);
	}
}


void CConditonDlg::OnDatetimechangeDateEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	SYSTEMTIME end;
	memset(&end, 0, sizeof(end));
	mDateEnd.GetTime(&end);

	if (utils::SystemTimeCmp(curTime, end) < 0)
	{
		mDateBegin.SetTime(&curTime);
		return;
	}

	SYSTEMTIME begin;
	memset(&begin, 0, sizeof(begin));
	mDateBegin.GetTime(&begin);
	if (utils::SystemTimeCmp(end, begin) < 0)
	{
		mDateEnd.SetTime(&begin);
	}
}

void CConditonDlg::OnSetfocusLineEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	const size_t length = 20;
	wchar_t begin[20] = { 0 };
	int sel = mBarcodeBegin.GetCurSel();
	mBarcodeBegin.GetLBText(sel, begin);

	wchar_t end[length] = { 0 };
	sel = mBarcodeEnd.GetCurSel();
	mBarcodeEnd.GetLBText(sel, end);

	if (wcscmp(begin, end) > 0)
	{
		int sel = mBarcodeBegin.GetCurSel();
		mBarcodeEnd.SetCurSel(sel);
	}
}


void CConditonDlg::OnSetfocusLineBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	const size_t length = 20;
	wchar_t begin[20] = { 0 };
	mBarcodeBegin.GetWindowTextW(begin, length);

	wchar_t end[length] = { 0 };
	mBarcodeEnd.GetWindowTextW(end, length);

	if (wcscmp(begin, end) > 0)
	{
		int sel = mBarcodeEnd.GetCurSel();
		mBarcodeBegin.SetCurSel(sel);
	}
}


void CConditonDlg::OnSelchangeLineBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	const size_t length = 20;
	wchar_t begin[20] = { 0 };
	int sel = mBarcodeBegin.GetCurSel();
	mBarcodeBegin.GetLBText(sel, begin);
	
	wchar_t end[length] = { 0 };
	sel = mBarcodeEnd.GetCurSel();
	mBarcodeEnd.GetLBText(sel, end);

	if (wcscmp(begin, end) > 0)
	{
		int sel = mBarcodeEnd.GetCurSel();
		mBarcodeBegin.SetCurSel(sel);
	}
}
