
// CarSeat_RecognizationDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "LabelManager.h"
#include "./Camera/Camera.h"
#include "./Camera/CameraManager.h"
#include "./Camera/LineCamera.h"


// CCarSeat_RecognizationDlg 对话框
class CCarSeat_RecognizationDlg : public CDHtmlDialog
{
// 构造
public:
	CCarSeat_RecognizationDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CARSEAT_RECOGNIZATION_DIALOG, IDH = IDR_HTML_CARSEAT_RECOGNIZATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
private:

	int m_nCxScreen;
	int m_nCyScreen;
	
	CStatic m_ImagePattern;
	CStatic m_ImageRec;

	CStatic m_barCode;
	
	// 显示统计结果，包括成功次数，失败次数，成功率
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
	CLineCamera *m_pLineCamera;
	int m_nCameraIndex;
	CCameraManager *m_pCameraManager;
	

	void CheckAndUpdate(std::wstring barcode, std::wstring type);
	
	void initCameraModule();

	SIZE adjustRecSize(SIZE imageSize, SIZE recSize);

	void adjustControlLocate(int width, int height);

public:
	afx_msg void OnUsrinput();

	/*
	ui界面刷新线程
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
};
