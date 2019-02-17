#pragma once

struct checkListNode
{
	wchar_t m_szImagePath[256];
	wchar_t m_szBarcode[256];
	wchar_t m_szInternalType[256];
	wchar_t m_szExternalType[256];
};

// CCheckListDlg 对话框

class CCheckListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckListDlg)

public:
	CCheckListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckListDlg();

	void SetCheckListData(const checkListNode &tmp);

	checkListNode GetCheckListData();

private:
	checkListNode data;

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REVISE_TYPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonChooseImage();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
