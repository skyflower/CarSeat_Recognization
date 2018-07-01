// CarSeat_RecognizationDlg.cpp :
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CarSeat_RecognizationDlg.h"
#include "afxdialogex.h"
#include "InputDlg.h"
#include "./common/utils.h"
#include "LoginDlg.h"
#include "CameraParameterDlg.h"
// add start by xiexinpeng
#include <string>
#include "kepserver/OPC.h"
// add end by xiexinpeng
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

//
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCarSeat_RecognizationDlg

BEGIN_DHTML_EVENT_MAP(CCarSeat_RecognizationDlg)

END_DHTML_EVENT_MAP()


CCarSeat_RecognizationDlg::CCarSeat_RecognizationDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(IDD_CARSEAT_RECOGNIZATION_DIALOG, IDR_HTML_CARSEAT_RECOGNIZATION_DIALOG, pParent),
	m_bThreadStatus(true),
	m_nSuccessCount(0),
	m_nFailCount(0),
	m_nCameraIndex(-1),
	m_pParamManager(nullptr),
	m_pNetworkTask(nullptr),
	m_pClassify(nullptr),
	m_pLineCamera(nullptr)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	//m_pFont.CreatePointFont(24, L"楷体");

	m_pLabelManager = new CLabelManager();	
	m_pCameraManager = CCameraManager::GetInstance();
}

void CCarSeat_RecognizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_PATTERN, m_ImagePattern);
	DDX_Control(pDX, IDC_IMAGE_REC, m_ImageRec);
	DDX_Control(pDX, IDC_BARCODE, m_barCode);
	DDX_Control(pDX, IDC_REG_RATIO, m_RegRatio);
	DDX_Control(pDX, IDC_IMAGE_PATTERN, m_ImagePattern);
}


BEGIN_MESSAGE_MAP(CCarSeat_RecognizationDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_USRINPUT, &CCarSeat_RecognizationDlg::OnUsrinput)
	ON_COMMAND(ID_START_CAMERA, &CCarSeat_RecognizationDlg::OnStartCamera)
	ON_UPDATE_COMMAND_UI(ID_START_CAMERA, &CCarSeat_RecognizationDlg::OnUpdateStartCamera)
	ON_WM_CLOSE()
	ON_COMMAND(ID_TAKE_PHOTO, &CCarSeat_RecognizationDlg::OnTakePhoto)
	ON_UPDATE_COMMAND_UI(ID_TAKE_PHOTO, &CCarSeat_RecognizationDlg::OnUpdateTakePhoto)
	ON_COMMAND(ID_USR_LOGIN, &CCarSeat_RecognizationDlg::OnUsrLogin)
	ON_COMMAND(ID_CLOSE_CAMERA, &CCarSeat_RecognizationDlg::OnCloseCamera)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_CAMERA, &CCarSeat_RecognizationDlg::OnUpdateCloseCamera)
	ON_COMMAND(ID_EXPOSURE_TIME_TEST, &CCarSeat_RecognizationDlg::OnExposureTimeTest)
	ON_COMMAND(ID_SET_CAMERA_PARAMETER, &CCarSeat_RecognizationDlg::OnSetCameraParameter)
	ON_UPDATE_COMMAND_UI(ID_SET_CAMERA_PARAMETER, &CCarSeat_RecognizationDlg::OnUpdateSetCameraParameter)
END_MESSAGE_MAP()


// CCarSeat_RecognizationDlg
BOOL CCarSeat_RecognizationDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// IDM_ABOUTBOX
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);
	//this->OnSetFont(&m_pFont);


    // kepserver add start by xiexinpeng	
    // kepserver
    COPC Opc;
	// server name
	CString Cstr = L"Kepware.KEPServerEX.V5";
	
	BSTR opcServer = nullptr;
	
	opcServer = Cstr.AllocSysString();
	
	Opc.AddServerName(opcServer);
	
	if(TRUE == Opc.ConnectServer())
	{
		Opc.bOPCConnect=true;
		CString flag = L"Channel1.Device1.Tag2";
		Opc.InitialOPC(opcServer, 1, &COleVariant(flag));
		Opc.PreWrite();	
		//wirte data demo
		float test = 500;
		Opc.WriteData(1, 0, &COleVariant(test));
	}
	if (opcServer != nullptr)
	{
		::SysFreeString(opcServer);
	}
	// add end by xiexinpeng

	CFont* curFont = this->GetFont();


	char tmpStr[] = "K215-黑色-菱形纹理";
	WCHAR result[200];

	m_barCode.SetWindowTextW(L"K215-黑色-菱形纹理");
		
	double ratio = 1.0;
	if (m_nFailCount + m_nSuccessCount != 0)
	{
		ratio = double(m_nSuccessCount) / double(m_nFailCount + m_nSuccessCount);
	}
	memset(result, 0, sizeof(WCHAR) * 200);
	wsprintfW(result, L"Success:%d\nFailed:%d\nSuccess Rate:%f%", m_nSuccessCount, m_nFailCount, ratio);

	m_RegRatio.SetWindowTextW(result);
	
	if (m_pCameraManager != nullptr)
	{
		if (m_pCameraManager->EnumCamera() == false)
		{
			TRACE0("init camera Failed\n");
		}
		TRACE1("camera count = %u\n", m_pCameraManager->GetCameraCount());
		if (m_pCameraManager->GetCameraCount() > 0)
		{
			m_nCameraIndex = 0;
		}
	}
	if (m_pLineCamera == nullptr)
	{
		if (m_nCameraIndex != -1)
		{

			MV_CC_DEVICE_INFO * pDevice = m_pCameraManager->GetCamera(m_nCameraIndex);
			m_pLineCamera = new CLineCamera(pDevice);
		}
	}
	
	//m_pUIThread = std::thread(run, this);

	return TRUE;  //
}

void CCarSeat_RecognizationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}


void CCarSeat_RecognizationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); //

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
	
}

HCURSOR CCarSeat_RecognizationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCarSeat_RecognizationDlg::run()
{
	std::wstring preImagePath;
	std::wstring barcode;
	m_pNetworkTask = CNetworkTask::GetInstance();
	m_pParamManager = CParamManager::GetInstance();
	if ((m_pNetworkTask == nullptr) || (m_pParamManager == nullptr))
	{
		return;
	}

	

	while (m_bThreadStatus)
	{
		
		std::wstring currentImage = m_pNetworkTask->GetCurrentImagePath();
		if (preImagePath != currentImage)
		{
			barcode = m_pNetworkTask->GetCurrentBarcode();
			std::string szCurImage = utils::WStrToStr(currentImage);
			std::wstring type = m_pClassify->GetImageType(szCurImage.c_str());
			if (type.size() != 0)
			{
				CheckAndUpdate(barcode, type);
			}
		}
		

		////// not implement send to server
		//

	}
}

void CCarSeat_RecognizationDlg::SetImageClassify(CImageClassify * pClassify)
{
	m_pClassify = pClassify;
}

void CCarSeat_RecognizationDlg::terminate()
{
	m_bThreadStatus = false;
}

void CCarSeat_RecognizationDlg::CheckAndUpdate(std::wstring barcode, std::wstring type)
{
	/*
	先将barcode转换成和type一致的类型，然后比较，然后刷新界面，如果错误，则弹出对话框，人工干预
	
	*/
	std::wstring barInternalType = m_pLabelManager->GetInternalTypeByBarcode(barcode);
	std::wstring typeInternalType = m_pLabelManager->GetInternalTypeByClassifyType(type);
	std::wstring reType;

	wchar_t result[MAX_CHAR_LENGTH] = { 0 };
	CNetworkTask::message msg;
	bool bUsrInput = true;
	if (barInternalType != typeInternalType)
	{
		/*
		添加报警系统，以及人工输入代码
		*/
		//m_nFailCount;
		CInputDlg dlg;
		dlg.SetManagePointer(m_pParamManager, m_pLabelManager);
		INT_PTR msg = dlg.DoModal();
		
		if (msg == IDOK)
		{
			reType = dlg.GetInputType();
		}
		////// send message to server
		////
		///  not implement
	}
	else
	{
		m_nSuccessCount++;
		////// send message to server
		////
		///  not implement
	}

	
	float ratio = 100 * m_nSuccessCount / (m_nSuccessCount + m_nFailCount + 0.0000001);

	wsprintfW(result, L"Success:%d\nFailed:%d\nSuccess Rate:%2f", m_nSuccessCount, m_nFailCount, ratio);
	m_RegRatio.SetWindowTextW(result);


	memset(result, 0, sizeof(result));
	wsprintfW(result, L"条形码：%s\n条形码结果：%s\n自动识别结果：%s", barcode, type, reType);
	m_barCode.SetWindowTextW(result);
}

void CCarSeat_RecognizationDlg::OnUsrinput()
{
	// TODO: 在此添加命令处理程序代码
	CInputDlg dlg;
	dlg.SetManagePointer(m_pParamManager, m_pLabelManager);
	INT_PTR msg = dlg.DoModal();

	if (msg == IDOK)
	{

	}

}


void CCarSeat_RecognizationDlg::OnStartCamera()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pCameraManager == nullptr)
	{
		m_pCameraManager = CCameraManager::GetInstance();
	}
	if (m_nCameraIndex == -1)
	{
		if (m_pCameraManager->EnumCamera() == false)
		{
			AfxMessageBox(L"enum Camera Failed");
			return;
		}
		if (m_pCameraManager->GetCameraCount() == 0)
		{
			AfxMessageBox(L"no Camera connect software");
			return;
		}
		m_nCameraIndex = 0;
	}
	if ((m_pLineCamera == nullptr) && (m_nCameraIndex != -1))
	{
		MV_CC_DEVICE_INFO *pDevice = m_pCameraManager->GetCamera(m_nCameraIndex);
		if (pDevice == nullptr)
		{
			AfxMessageBox(L"get Camera device Failed");
			return;
		}
		m_pLineCamera = new CLineCamera(pDevice);
	}
	if (m_pLineCamera == nullptr)
	{
		AfxMessageBox(L"line camera init Failed");
		return;
	}
	CCamera::CameraStatus tmpStatus = m_pLineCamera->GetCameraStatus();
	if (tmpStatus == CCamera::CameraStatus::CAMERA_GRAB)
	{
		return;
	}

	do
	{
		if (tmpStatus != CCamera::CameraStatus::CAMERA_OPEN)
		{
			if (m_pLineCamera->OpenButton() == false)
			{
				TRACE0("open button Failed\n");
				WriteError("open button Failed");
				break;
			}
		}
		
		if (m_pLineCamera->SetFrameRate(20.0) == false)
		{
			TRACE0("SetFrameRate Failed\n");
			WriteError("SetFrameRate Failed");
		}
		if (m_pLineCamera->SetGain(0) == false)
		{
			TRACE0("SetGain Failed\n");
			WriteError("SetGain Failed");
		}
		if (m_pLineCamera->SetTriggerMode(MV_TRIGGER_MODE_OFF) == false)
		{
			TRACE0("SetTriggerMode MV_TRIGGER_MODE_ON Failed\n");
			WriteError("SetTriggerMode MV_TRIGGER_MODE_ON Failed");
		}
		if (m_pLineCamera->SetTriggerSource(MV_TRIGGER_SOURCE_LINE0) == false)
		{
			TRACE0("SetTriggerSource MV_TRIGGER_SOURCE_LINE0 Failed\n");
			WriteError("SetTriggerSource MV_TRIGGER_SOURCE_LINE0 Failed");
		}
		HWND imageHwnd = GetDlgItem(IDC_IMAGE_REC)->GetSafeHwnd();
		m_pLineCamera->SetDisplayHwnd(imageHwnd);
		if (m_pLineCamera->StartGrabbing() == false)
		{
			TRACE0("StartGrabbing Failed\n");
			WriteError("StartGrabbing Failed");
			break;
		}
		return;
	} while (0);
	
	m_pLineCamera->CloseDevice();
}


void CCarSeat_RecognizationDlg::OnUpdateStartCamera(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码

	if (m_pCameraManager == nullptr)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if (m_pCameraManager->GetCameraCount() == 0)
	{
		m_pCameraManager->EnumCamera();
		if (m_pCameraManager->GetCameraCount() == 0)
		{
			pCmdUI->Enable(FALSE);
			return;
		}
	}

	pCmdUI->Enable(TRUE);
	return;
}


void CCarSeat_RecognizationDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pLineCamera)
	{
		delete m_pLineCamera;
		m_pLineCamera = nullptr;
	}
	if (m_pCameraManager)
	{
		delete m_pCameraManager;
		m_pCameraManager = nullptr;
	}
	if (m_pLabelManager != nullptr)
	{
		delete m_pLabelManager;
		m_pLabelManager = nullptr;
	}

	CDHtmlDialog::OnClose();
}


void CCarSeat_RecognizationDlg::OnTakePhoto()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pLineCamera != nullptr)
	{
		CCamera::CameraStatus status = m_pLineCamera->GetCameraStatus();
		if (status == CCamera::CameraStatus::CAMERA_GRAB)
		{
			std::wstring path = m_pLineCamera->SaveJpg();
		}
	}
}


void CCarSeat_RecognizationDlg::OnUpdateTakePhoto(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if ((m_pLineCamera == nullptr) ||(m_pLineCamera->GetCameraStatus() != CCamera::CameraStatus::CAMERA_GRAB))
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}


void CCarSeat_RecognizationDlg::OnUsrLogin()
{
	// TODO: 在此添加命令处理程序代码
	CLoginDlg dlg;
	int ret = dlg.DoModal();
	if (ret == IDOK)
	{
		// 登录部分代码，更新用户名和密码

	}
}


void CCarSeat_RecognizationDlg::OnCloseCamera()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pLineCamera != nullptr)
	{
		m_pLineCamera->CloseDevice();
	}
}


void CCarSeat_RecognizationDlg::OnUpdateCloseCamera(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_pLineCamera == nullptr)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CCamera::CameraStatus tmpStatus = m_pLineCamera->GetCameraStatus();
	if (tmpStatus == CCamera::CameraStatus::CAMERA_INIT)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pCmdUI->Enable(TRUE);
}


void CCarSeat_RecognizationDlg::OnExposureTimeTest()
{
	// TODO: 在此添加命令处理程序代码
	//SendMessage()

	if (m_pLineCamera != nullptr)
	{
		CCamera::CameraStatus status = m_pLineCamera->GetCameraStatus();
		if (status == CCamera::CameraStatus::CAMERA_GRAB)
		{
			
			m_pLineCamera->SetExposureTimeAutoMode(MV_EXPOSURE_AUTO_MODE_OFF);
			double timeMax, timeMin;
			m_pLineCamera->GetExposureTimeRange(&timeMax, &timeMin);
			m_pLineCamera->SetImageSaveDirectory(L"C:\\Users\\Administrator\\Desktop\\CodeStudy\\hikvision\\");
			
			TRACE2("exposure time range = %f, %f\n", timeMax, timeMin);

			const size_t M = 20;
			double timeStep = (timeMax - timeMin) / M;
			for (int i = 0; i < M; ++i)
			{
				m_pLineCamera->SetExposureTime(timeMin + i * timeStep);
				std::wstring path = m_pLineCamera->SaveJpg();
			}
		}
	}
}


void CCarSeat_RecognizationDlg::OnSetCameraParameter()
{
	// TODO: 在此添加命令处理程序代码
	CCameraParameterDlg dlg;
	dlg.SetLineCamera(m_pLineCamera);
	INT_PTR ret = dlg.DoModal();
	if (ret == IDOK)
	{

	}
}


void CCarSeat_RecognizationDlg::OnUpdateSetCameraParameter(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_pLineCamera == nullptr)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CCamera::CameraStatus tmpStatus = m_pLineCamera->GetCameraStatus();
	if ((tmpStatus == CCamera::CameraStatus::CAMERA_OPEN) || \
		(tmpStatus == CCamera::CameraStatus::CAMERA_GRAB))
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	pCmdUI->Enable(FALSE);
	return;
}
