#pragma once
#include "afxwin.h"


// CInputDlg 对话框

class CInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInputDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT_TYPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ColorCombo;

	CComboBox m_OutlineCombo;
	CComboBox m_TexCombo;
	CStatic m_StPatternImage;
private:
	CStatic m_StTestImage;
public:
	virtual BOOL OnInitDialog();
};
