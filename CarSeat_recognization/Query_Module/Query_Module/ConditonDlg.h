#pragma once

#include <iostream>
#include <vector>
#include "afxwin.h"
#include "afxdtctl.h"

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


// CConditonDlg �Ի���

class CConditonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConditonDlg)

public:
	CConditonDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConditonDlg();
private:
	CConditionFilter mFilter;
	//void SetLine(std::vector<std::string> &line);
	//CParamManager *m_pParamManager;

public:
	CConditionFilter GetFilterCondition();
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
	CDateTimeCtrl mDateBegin;
	// //ʱ�����
	CDateTimeCtrl mTimeBegin;
	// //���ڽ���
	CDateTimeCtrl mDateEnd;
	// ʱ�����
	CDateTimeCtrl mTimeEnd;
public:
	afx_msg void OnDatetimechangeDateBegin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeTimeBegin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeTimeEnd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDateEnd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeLineBegin();
	afx_msg void OnSelchangeLineEnd();
};
