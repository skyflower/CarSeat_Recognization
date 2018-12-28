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

// CConditonDlg �Ի���

class CConditonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConditonDlg)

public:
	CConditonDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConditonDlg();
private:
	CConditionFilterA mFilter;
	//void SetLine(std::vector<std::string> &line);
	//CParamManager *m_pParamManager;

public:
	CConditionFilterA GetFilterCondition();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONDITION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
private:
	// //���߿�ʼѡ��
	CComboBox mLineBegin;
	// //���߽���
	CComboBox mLineEnd;
	// //�����뿪ʼ
	CComboBox mBarcodeBegin;
	// //���������
	CComboBox mBarcodeEnd;
	// //��������
	CComboBox mSeatType;
	// //ʶ������,auto or manual
	CComboBox mMethodType;
	// //�������
	CDateTimeCtrl mDateTimeBegin;
	// //ʱ�����
	//CDateTimeCtrl mTimeBegin;
	// //���ڽ���
	CDateTimeCtrl mDateTimeEnd;
	// ʱ�����
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
