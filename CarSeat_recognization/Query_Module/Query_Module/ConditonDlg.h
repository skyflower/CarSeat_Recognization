#pragma once

#include <iostream>
#include <vector>
#include "afxwin.h"
#include "afxdtctl.h"

template<typename type>
struct CConditionFilter
{
	type mDateTimeBeign[32];
	type mDateTimeEnd[32];
	//type mTimeBegin[20];
	//type mTimeEnd[20];
	type mLineBegin[20];
	type mLineEnd[20];
	type mBarcodeBegin[50];
	type mBarcodeEnd[50];
	type mSeatType[50];
	type mMethodType[20];
};


typedef CConditionFilter<char> CConditionFilterA;
typedef CConditionFilter<wchar_t> CConditionFilterW;

// CConditonDlg 对话框

class CConditonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConditonDlg)

public:
	CConditonDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConditonDlg();
private:
	CConditionFilterA mFilter;
	//void SetLine(std::vector<std::string> &line);
	//CParamManager *m_pParamManager;

public:
	CConditionFilterA GetFilterCondition();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONDITION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
private:
	// //产线开始选择
	CComboBox mLineBegin;
	// //产线结束
	CComboBox mLineEnd;
	// //条形码开始
	CComboBox mBarcodeBegin;
	// //条形码结束
	CComboBox mBarcodeEnd;
	// //座椅类型
	CComboBox mSeatType;
	// //识别类型,auto or manual
	CComboBox mMethodType;
	// //日期起点
	CDateTimeCtrl mDateTimeBegin;
	// //时间起点
	//CDateTimeCtrl mTimeBegin;
	// //日期结束
	CDateTimeCtrl mDateTimeEnd;
	// 时间结束
	//CDateTimeCtrl mTimeEnd;
public:
	afx_msg void OnDatetimechangeDateBegin(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnDatetimechangeTimeBegin(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnDatetimechangeTimeEnd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDateEnd(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnSetfocusLineEnd();
	//afx_msg void OnSetfocusLineBegin();
	afx_msg void OnSelchangeLineEnd();
	afx_msg void OnSelchangeLineBegin();
};
