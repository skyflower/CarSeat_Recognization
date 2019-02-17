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

CConditionFilterA CConditonDlg::GetFilterCondition()
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
	DDX_Control(pDX, IDC_DATE_BEGIN, mDateTimeBegin);
	//DDX_Control(pDX, IDC_TIME_BEGIN, mTimeBegin);
	DDX_Control(pDX, IDC_DATE_END, mDateTimeEnd);
	//DDX_Control(pDX, IDC_TIME_END, mTimeEnd);
}


BEGIN_MESSAGE_MAP(CConditonDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConditonDlg::OnBnClickedOk)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_BEGIN, &CConditonDlg::OnDatetimechangeDateBegin)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_END, &CConditonDlg::OnDatetimechangeDateEnd)
	ON_CBN_SELCHANGE(IDC_LINE_BEGIN, &CConditonDlg::OnSelchangeLineBegin)
	ON_CBN_SELCHANGE(IDC_LINE_END, &CConditonDlg::OnSelchangeLineEnd)
END_MESSAGE_MAP()


// CConditonDlg 消息处理程序



void CConditonDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	memset(&mFilter, 0, sizeof(CConditionFilterA));

	wchar_t text[256];
	int tmpLength = 0;

	//日期开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_DATE_BEGIN, text, sizeof(text));
	tmpLength = sizeof(mFilter.mDateTimeBeign);
	utils::WCharToChar(text, mFilter.mDateTimeBeign, &tmpLength);
	//utils::delBlankSpace<char>(mFilter.mDateTimeBeign, strlen(mFilter.mDateTimeBeign), '/');
	
	//日期结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_DATE_END, text, sizeof(text));
	tmpLength = sizeof(mFilter.mDateTimeEnd);
	utils::WCharToChar(text, mFilter.mDateTimeEnd, &tmpLength);
	//utils::delBlankSpace<char>(mFilter.mDateTimeEnd, strlen(mFilter.mDateTimeEnd), '/');


	//时间开始
	/*memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_TIME_BEGIN, text, sizeof(text));
	tmpLength = sizeof(mFilter.mTimeBegin);
	utils::WCharToChar(text, mFilter.mTimeBegin, &tmpLength);
	utils::delBlankSpace<char>(mFilter.mTimeBegin, strlen(mFilter.mTimeBegin), ':');*/


	//时间结束
	/*memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_TIME_END, text, sizeof(text));
	tmpLength = sizeof(mFilter.mTimeEnd);
	utils::WCharToChar(text, mFilter.mTimeEnd, &tmpLength);
	utils::delBlankSpace<char>(mFilter.mTimeEnd, strlen(mFilter.mTimeEnd), ':');*/


	//产线开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_LINE_BEGIN, text, sizeof(text));
	tmpLength = sizeof(mFilter.mLineBegin);
	utils::WCharToChar(text, mFilter.mLineBegin, &tmpLength);


	//产线结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_LINE_END, text, sizeof(text));
	tmpLength = sizeof(mFilter.mLineEnd);
	utils::WCharToChar(text, mFilter.mLineEnd, &tmpLength);

	
	//条形码开始
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_BARCODE_BEGIN, text, sizeof(text));
	tmpLength = sizeof(mFilter.mBarcodeBegin);
	utils::WCharToChar(text, mFilter.mBarcodeBegin, &tmpLength);


	//条形码结束
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_BARCODE_END, text, sizeof(text));
	tmpLength = sizeof(mFilter.mBarcodeEnd);
	utils::WCharToChar(text, mFilter.mBarcodeEnd, &tmpLength);

	
	////座椅类型
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_SEAT_TYPE, text, sizeof(text));
	tmpLength = sizeof(mFilter.mSeatType);
	utils::WCharToChar(text, mFilter.mSeatType, &tmpLength);

	
	//识别类型
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_METHOD_TYPE, text, sizeof(text));
	tmpLength = sizeof(mFilter.mMethodType);
	utils::WCharToChar(text, mFilter.mMethodType, &tmpLength);

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

	mDateTimeBegin.SetFormat(L"yyyy-MM-dd HH:mm:ss");
	mDateTimeEnd.SetFormat(L"yyyy-MM-dd HH:mm:ss");

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
	mDateTimeBegin.GetTime(&begin);

	if (utils::SystemTimeCmp(curTime, begin) < 0)
	{
		mDateTimeBegin.SetTime(&curTime);
		return;
	}

	SYSTEMTIME end;
	memset(&end, 0, sizeof(end));
	mDateTimeEnd.GetTime(&end);
	if (utils::SystemTimeCmp(end, begin) < 0)
	{
		mDateTimeEnd.SetTime(&begin);
	}

	*pResult = 0;
}

//
//void CConditonDlg::OnDatetimechangeTimeBegin(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//
//	SYSTEMTIME curTime;
//	GetLocalTime(&curTime);
//
//	SYSTEMTIME begin;
//	memset(&begin, 0, sizeof(begin));
//	mTimeBegin.GetTime(&begin);
//
//	if (utils::SystemTimeCmp(curTime, begin) < 0)
//	{
//		mTimeBegin.SetTime(&curTime);
//		return;
//	}
//
//	SYSTEMTIME end;
//	memset(&end, 0, sizeof(end));
//	mTimeEnd.GetTime(&end);
//	if (utils::SystemTimeCmp(end, begin) < 0)
//	{
//		mTimeEnd.SetTime(&begin);
//	}
//}

//
//void CConditonDlg::OnDatetimechangeTimeEnd(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//
//	SYSTEMTIME curTime;
//	GetLocalTime(&curTime);
//
//	SYSTEMTIME end;
//	memset(&end, 0, sizeof(end));
//	mTimeEnd.GetTime(&end);
//
//	if (utils::SystemTimeCmp(curTime, end) < 0)
//	{
//		mTimeBegin.SetTime(&curTime);
//		return;
//	}
//
//	SYSTEMTIME begin;
//	memset(&begin, 0, sizeof(begin));
//	mTimeBegin.GetTime(&begin);
//	if (utils::SystemTimeCmp(end, begin) < 0)
//	{
//		mTimeEnd.SetTime(&begin);
//	}
//}


void CConditonDlg::OnDatetimechangeDateEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	SYSTEMTIME end;
	memset(&end, 0, sizeof(end));
	mDateTimeEnd.GetTime(&end);

	if (utils::SystemTimeCmp(curTime, end) < 0)
	{
		mDateTimeBegin.SetTime(&curTime);
		return;
	}

	SYSTEMTIME begin;
	memset(&begin, 0, sizeof(begin));
	mDateTimeBegin.GetTime(&begin);
	if (utils::SystemTimeCmp(end, begin) < 0)
	{
		mDateTimeEnd.SetTime(&begin);
	}
}



void CConditonDlg::OnSelchangeLineBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	const size_t length = 20;
	wchar_t begin[20] = { 0 };
	int sel = mLineBegin.GetCurSel();
	mLineBegin.GetLBText(sel, begin);
	
	wchar_t end[length] = { 0 };
	sel = mLineEnd.GetCurSel();
	mLineEnd.GetLBText(sel, end);

	if (wcscmp(begin, end) > 0)
	{
		int sel = mLineBegin.GetCurSel();
		mLineEnd.SetCurSel(sel);
	}
}

void CConditonDlg::OnSelchangeLineEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	const size_t length = 20;
	wchar_t begin[20] = { 0 };
	int sel = mLineBegin.GetCurSel();
	mLineBegin.GetLBText(sel, begin);

	wchar_t end[length] = { 0 };
	sel = mLineEnd.GetCurSel();
	mLineEnd.GetLBText(sel, end);

	if (wcscmp(begin, end) > 0)
	{
		int sel = mLineEnd.GetCurSel();
		mLineBegin.SetCurSel(sel);
	}
}
