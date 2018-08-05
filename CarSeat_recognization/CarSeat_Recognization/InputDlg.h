#pragma once
#include "afxwin.h"
#include "./common/ParamManager.h"
#include "./common/LabelManager.h"


// CInputDlg �Ի���

class CInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT_TYPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	
	CStatic m_StPatternImage;
	CStatic m_StTestImage;
	CImage *m_pPatternImage;
	CImage *m_pTestImage;

	wchar_t m_szReType[40];

	CParamManager *m_pParamManager;
	CLabelManager *m_pLabelManager;

public:
	virtual BOOL OnInitDialog();

	std::wstring GetInputType();

	void SetManagePointer(CParamManager *pParamManager, CLabelManager *pLabelManager);
	void SetTestImagePath(std::wstring path);

private:
	CComboBox m_TypeCombo;
public:
	afx_msg void OnSelchangeComboType();
	afx_msg void OnStnClickedPatternImage();
};
