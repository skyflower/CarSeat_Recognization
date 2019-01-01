#pragma once
#include "afxcmn.h"
#include "RecogResult.h"
#include "DetailInfoDlg.h"
#include <iostream>
#include <vector>
class CRecogInfoCtrl :
	public CListCtrl
{

	DECLARE_DYNCREATE(CRecogInfoCtrl);

public:
	CRecogInfoCtrl();
	virtual ~CRecogInfoCtrl();

	void initColumn();

	bool InsertItem(std::vector<RecogResultW> *pRecog);

	bool ClearAllItem();

private:
	std::vector<RecogResultW> *m_pData;
	//CDetailInfoDlg m_displayDlg;

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

