// CarSeat_RecognizationDlg.cpp :
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CarSeat_RecognizationDlg.h"
#include "afxdialogex.h"
#include "InputDlg.h"
#include "./common/utils.h"
#include "LoginDlg.h"
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
	m_pParamManager(nullptr),
	m_pNetworkTask(nullptr),
	m_pClassify(nullptr)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_pFont.CreatePointFont(24, L"楷体");

	m_pLabelManager = new CLabelManager();	
	m_pCameraManager = new CCameraManager();
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
	this->OnSetFont(&m_pFont);


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
		m_pCameraManager = new CCameraManager();
	}
	CCameraManager::CameraStatus status = m_pCameraManager->GetStatus();
	TRACE1("Camera Status = %d\n", status);
	bool ret = true;
	switch(status)
	{
		case CCameraManager::CameraStatus::INIT:
			if ((ret = m_pCameraManager->EnumButton()) == false)
			{
				TRACE0("CameraManager Enum Cameras Failed\n");
			}

		case CCameraManager::CameraStatus::SEARCH:
			if (ret == true)
			{
				if ((ret = m_pCameraManager->OpenButton()) == false)
				{
					TRACE0("Open Camera Failed\n");
				}
			}

		case CCameraManager::CameraStatus::OPEN:
			if (ret == true)
			{
				HWND hwnd = GetDlgItem(IDC_IMAGE_REC)->GetSafeHwnd();
				m_pCameraManager->SetDisplayHwnd(hwnd);
			}

		case CCameraManager::CameraStatus::BIND:
			if (ret == true)
			{
				m_pCameraManager->StartGrabbingButton();
			}

		case CCameraManager::CameraStatus::GRAB:

		default:
			break;
	}
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
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
	return;
}


void CCarSeat_RecognizationDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
	if (m_pCameraManager != nullptr)
	{
		CCameraManager::CameraStatus status = m_pCameraManager->GetStatus();
		if (status == CCameraManager::CameraStatus::GRAB)
		{
			m_pCameraManager->SaveJpgButton();
		}
	}
}


void CCarSeat_RecognizationDlg::OnUpdateTakePhoto(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if ((m_pCameraManager == nullptr) || (m_pCameraManager->GetStatus() != CCameraManager::CameraStatus::GRAB))
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
