
// KepServerComDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Communication.h"

// CKepServerComDlg �Ի���
class CKepServerComDlg : public CDialogEx
{
// ����
public:
	CKepServerComDlg(CWnd* pParent = NULL);	// ��׼���캯��
	COPC Opc;
	COleVariant *ReadItem;
	COleVariant *WriteItem;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEPSERVERCOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton B_ServerConnect;
	CString m_ServerName;
	CString m_ReadFlagName;
	CString m_ConncetResult;
	CString m_WriteFlagName;
	afx_msg void OnBnClickedConnect();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
