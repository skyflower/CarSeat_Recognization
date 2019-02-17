#pragma once
#include "afxcmn.h"

// CBarcodeCheck 对话框

class CBarcodeCheck : public CDialogEx
{
	DECLARE_DYNAMIC(CBarcodeCheck)

public:
	CBarcodeCheck(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBarcodeCheck();

	void SetTypePointer(std::unordered_map<std::string, std::string> *barcode, 
		std::unordered_map<std::string, std::string> * classifyType);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BARCODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_BarcodeList;

	std::unordered_map<std::string, std::string> *m_pBarcode;
	std::unordered_map<std::string, std::string> *m_pClassify;

	bool m_bRevise;
	
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMDblclkBarcodeChecklist(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAddOneLine();
	afx_msg void OnBnClickedButtonDeleteSelected();
};
