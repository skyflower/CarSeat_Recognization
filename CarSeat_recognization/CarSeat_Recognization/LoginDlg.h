#pragma once

#include "./common/LabelManager.h"

// CLoginDlg �Ի���

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_USR_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	BOOL GetAutoSaveFlag();
	CString GetLoginUsrName();
	CString GetLoginPasswd();

	bool SetLabelManager(CLabelManager *pLabelManager);
	
private:
	// //�Զ������û���������
	BOOL m_bAutoSaveFlag;
	// //�û���
	CString m_strUsrName;
	// //����
	CString m_strPasswd;

	CLabelManager *m_pLabelManager;

	/*
	ͨ������ģ����֤�û��������룬��δȫ��ʵ�֣�Ĭ�Ϸ�����֤ͨ��
	*/
	bool CheckUsrNameAndPasswd();

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
