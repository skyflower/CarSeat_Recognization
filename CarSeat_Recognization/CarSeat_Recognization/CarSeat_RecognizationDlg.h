
// CarSeat_RecognizationDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "LabelManager.h"


// CCarSeat_RecognizationDlg �Ի���
class CCarSeat_RecognizationDlg : public CDHtmlDialog
{
// ����
public:
	CCarSeat_RecognizationDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CARSEAT_RECOGNIZATION_DIALOG, IDH = IDR_HTML_CARSEAT_RECOGNIZATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	//HRESULT OnButtonOK(IHTMLElement *pElement);
	//HRESULT OnButtonCancel(IHTMLElement *pElement);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
private:
	CFont m_pFont;
	CStatic m_ImagePattern;
	CStatic m_ImageRec;

	CStatic m_barCode;
	
	// ��ʾͳ�ƽ���������ɹ�������ʧ�ܴ������ɹ���
	CStatic m_RegRatio;

	size_t m_nSuccessCount;
	size_t m_nFailCount;

	bool m_bThreadStatus;

	/* CImage::Load(filePath)   */
	// CStatic.SetBitmap((HBITMAP)CImage))
	//CImage m_image;

	//CLog *m_pLog;
	CParamManager *m_pParamManager;
	CNetworkTask *m_pNetworkTask;
	//std::thread m_pUIThread;

	CImageClassify *m_pClassify;
	CLabelManager *m_pLabelManager;

	//CLabelManager *m_pLabelManager;


	//void displayRecImage(const wchar_t *path);

	void CheckAndUpdate(std::wstring barcode, std::wstring type);
	


public:
	afx_msg void OnUsrinput();

	/*
	ui����ˢ���߳�
	*/
	void run();

	void SetImageClassify(CImageClassify*pClassify);

	void terminate();
};
