
// generateRegisterCodeDlg.h : ͷ�ļ�
//

#pragma once


#include "../SoftwareRegisterDll/BaseRegister.h"
#include "../SoftwareRegisterDll/TimeRegister.h"


// CgenerateRegisterCodeDlg �Ի���
class CgenerateRegisterCodeDlg : public CDialogEx
{
// ����
public:
	CgenerateRegisterCodeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GENERATEREGISTERCODE_DIALOG };
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

private:
	registerNode getRegisterInfo();

public:
	afx_msg void OnBnClickedButtonDecipher();
};
