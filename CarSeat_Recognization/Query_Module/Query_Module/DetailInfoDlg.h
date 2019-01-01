#pragma once
#include "afxwin.h"
#include <atlimage.h>
#include <afxcontrolbars.h>


// CDetailInfoDlg 对话框

class CDetailInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDetailInfoDlg)

public:
	CDetailInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetailInfoDlg();

	void SetDisplayInfo(wchar_t *info, wchar_t *imagePath);

private:
	void displayImage(CImage * pImage, CStatic * pStatic);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RECOG_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_szDetailInfo;
	wchar_t m_szImagePath[256];
	CStatic m_RecogImage;
	CImage m_Image;

public:
	virtual BOOL OnInitDialog();
};
