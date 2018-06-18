#pragma once

#include <iostream>
#include <vector>

struct CConditionFilter
{
	char mDateBeign[20];
	char mDateEnd[20];
	char mTimeBegin[20];
	char mTimeEnd[20];
	char mLineBegin[20];
	char mLineEnd[20];
	char mBarcodeBegin[50];
	char mBarcodeEnd[50];
	char mSeatType[50];
	char mMethodType[20];
};


// CConditonDlg 对话框

class CConditonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConditonDlg)

public:
	CConditonDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConditonDlg();
private:
	CConditionFilter mFilter;
	//void SetLine(std::vector<std::string> &line);
	
public:
	CConditionFilter GetFilterCondition();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONDITION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
};
