
// CarSeat_RecognizationDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "./common/LabelManager.h"
//#include "./Camera/Camera.h"
#include "./Camera/CameraManager.h"
#include "./Camera/LineCamera.h"
#include "./common/RFIDReader.h"
#include "./common/RecogResultManager.h"
#include "./network/KepServerSocket.h"
#include "./Camera/Class/ActionSource.h"
#include "./Camera/Class/Observer.h"
#include "./Camera/Camera/CameraEvent.h"
#include "./Action/EVFPictureBox.h"

// C EvfZoomAction




// CCarSeat_RecognizationDlg 对话框
class CCarSeat_RecognizationDlg : public CDHtmlDialog, public ActionSource, public Observer
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


protected:
	CameraController* _controller;
	CameraModel *_model;

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
	
	CStatic m_stImagePattern;

	//CStatic m_stImageRec;

	CImage *m_pImagePattern;

	CStatic m_barCode;
	
	// 显示统计结果，包括成功次数，失败次数，成功率
	CStatic m_RegRatio;

	size_t m_nSuccessCount;
	size_t m_nFailCount;

	bool m_bThreadStatus;

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

	
	CEVFPictureBox	_pictureBox;


	void CheckAndUpdate(std::string barcode, std::string type, std::string tmppath);
	
	/*
	初始化相机模块
	*/
	void initCameraModule();

	SIZE adjustRecSize(SIZE imageSize, SIZE recSize);

	/*
	调整对话框控件位置,包括照片显示以及静态文本框的位置调整等
	*/
	void adjustControlLocate(int width, int height);

	void displayImage(CImage * pImage, CStatic * pStatic);



	// 用于测试xml的解析,
	void testXML();

	std::wstring testGenerateBarcode();

	bool heartBloodServer(unsigned int ip, unsigned int port);

	CameraModel* cameraModelFactory(EdsCameraRef camera, EdsDeviceInfo deviceInfo);


	void setupListener(ActionListener* listener);

	void setupObserver(Observable* ob);


public:
	afx_msg void OnUsrinput();

	/*
	ui界面刷新线程
	*/
	void run();

	void SetImageClassify(CImageClassify*pClassify);

	void terminate();
	bool SetLabelManager(CLabelManager *pLabelManager);

	virtual void update(Observable* from, CameraEvent *e);
	void setCameraController(CameraController* controller) { _controller = controller; }


	afx_msg LRESULT OnDownloadComplete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressReport(WPARAM wParam, LPARAM lParam);

	CameraModel *getAppCameraModel();


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
