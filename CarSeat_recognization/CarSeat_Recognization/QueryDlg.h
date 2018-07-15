#pragma once
#include "afxwin.h"


// CQueryDlg �Ի���

class CQueryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CQueryDlg)

public:
	CQueryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CQueryDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_QUERY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:

	CButton m_QueryButton;
	CString m_szQueryBarcode;
	CStatic m_ImageDisplay;
	CString m_szQueryResult;
};
