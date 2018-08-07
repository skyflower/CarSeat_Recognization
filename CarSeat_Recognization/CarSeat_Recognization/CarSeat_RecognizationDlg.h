
// CarSeat_RecognizationDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "./common/LabelManager.h"
#include "./Camera/Camera.h"
#include "./Camera/CameraManager.h"
#include "./Camera/LineCamera.h"
#include "./common/RFIDReader.h"
#include "./common/RecogResultManager.h"
#include "./network/KepServerSocket.h"


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

	int m_nCxScreen;
	int m_nCyScreen;
	
	CStatic m_stImagePattern;
	CStatic m_stImageRec;

	//CImage *m_pImageRec;
	CImage *m_pImagePattern;

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
	
	CImageClassify *m_pClassify;
	CLabelManager *m_pLabelManager;

	std::mutex m_LineCameraMutex;
	CLineCamera *m_pLineCamera;

	int m_nCameraIndex;
	CCameraManager *m_pCameraManager;

	CRFIDReader *m_pRFIDReader;
	CRecogResultManager *m_pRecogManager;
	CKepServerSocket *m_pKepServer;

	bool m_bBeginJob;


	void CheckAndUpdate(std::wstring barcode, std::wstring type, std::string tmppath);
	
	/*
	��ʼ�����ģ��
	*/
	void initCameraModule();

	SIZE adjustRecSize(SIZE imageSize, SIZE recSize);

	/*
	�����Ի���ؼ�λ��,������Ƭ��ʾ�Լ���̬�ı����λ�õ�����
	*/
	void adjustControlLocate(int width, int height);

	void displayImage(CImage * pImage, CStatic * pStatic);



	// ���ڲ���xml�Ľ���,
	void testXML();

	std::wstring testGenerateBarcode();


public:
	afx_msg void OnUsrinput();

	/*
	ui����ˢ���߳�
	*/
	void run();

	void SetImageClassify(CImageClassify*pClassify);

	void terminate();
	bool SetLabelManager(CLabelManager *pLabelManager);




	afx_msg void OnStartCamera();
	afx_msg void OnUpdateStartCamera(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnTakePhoto();
	afx_msg void OnUpdateTakePhoto(CCmdUI *pCmdUI);
	afx_msg void OnUsrLogin();
	afx_msg void OnCloseCamera();
	afx_msg void OnUpdateCloseCamera(CCmdUI *pCmdUI);
	afx_msg void OnExposureTimeTest();
	afx_msg void OnSetCameraParameter();
	afx_msg void OnUpdateSetCameraParameter(CCmdUI *pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuQueryBarcode();
	afx_msg void OnUpdateMenuQueryBarcode(CCmdUI *pCmdUI);
	afx_msg void OnChooseCamera();
	afx_msg void OnUpdateChooseCamera(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedButtonBeginJob();
	afx_msg void OnMenuStartGrab();
	afx_msg void OnMenuStopGrab();
};
