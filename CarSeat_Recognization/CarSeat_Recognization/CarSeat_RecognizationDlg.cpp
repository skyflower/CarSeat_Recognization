// CarSeat_RecognizationDlg.cpp :
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CarSeat_RecognizationDlg.h"
#include "afxdialogex.h"
#include "InputDlg.h"
#include "./common/utils.h"
#include "LoginDlg.h"
#include "./xml/tinyxml.h"
#include "CameraParameterDlg.h"
#include "./common/Log.h"
#include <stack>
#include "QueryDlg.h"
#include "./common/RecogResultManager.h"
#include "./Camera/Camera/CameraEventListener.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_USER_DOWNLOAD_COMPLETE		WM_APP+1
#define WM_USER_PROGRESS_REPORT			WM_APP+2



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
	//m_pLineCamera(nullptr),
	m_nCxScreen(0),
	m_nCyScreen(0),
	m_pRFIDReader(nullptr),
	m_pKepServer(nullptr),
	m_bBeginJob(false)
	//m_pUIThread(nullptr)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	//m_pLabelManager = new CLabelManager();	
	m_pCameraManager = CCameraManager::GetInstance();
	m_pRecogManager = CRecogResultManager::GetInstance();
	m_pParamManager = CParamManager::GetInstance();
}

void CCarSeat_RecognizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_PATTERN, m_stImagePattern);
	DDX_Control(pDX, IDC_IMAGE_REC, m_stImageRec);
	DDX_Control(pDX, IDC_BARCODE, m_barCode);
	DDX_Control(pDX, IDC_REG_RATIO, m_RegRatio);
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
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_QUERY_BARCODE, &CCarSeat_RecognizationDlg::OnMenuQueryBarcode)
	ON_UPDATE_COMMAND_UI(ID_MENU_QUERY_BARCODE, &CCarSeat_RecognizationDlg::OnUpdateMenuQueryBarcode)
	ON_COMMAND(ID_CHOOSE_CAMERA, &CCarSeat_RecognizationDlg::OnChooseCamera)
	ON_UPDATE_COMMAND_UI(ID_CHOOSE_CAMERA, &CCarSeat_RecognizationDlg::OnUpdateChooseCamera)
	ON_BN_CLICKED(IDC_BUTTON_BEGIN_JOB, &CCarSeat_RecognizationDlg::OnBnClickedButtonBeginJob)
	ON_COMMAND(ID_MENU_START_GRAB, &CCarSeat_RecognizationDlg::OnMenuStartGrab)
	ON_COMMAND(ID_MENU_STOP_GRAB, &CCarSeat_RecognizationDlg::OnMenuStopGrab)
	ON_MESSAGE(WM_USER_DOWNLOAD_COMPLETE, OnDownloadComplete)
	ON_MESSAGE(WM_USER_PROGRESS_REPORT, OnProgressReport)
END_MESSAGE_MAP()


// CCarSeat_RecognizationDlg
BOOL CCarSeat_RecognizationDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	//AfxSocketInit();

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

	m_nCxScreen = GetSystemMetrics(SM_CXSCREEN);
	m_nCyScreen = GetSystemMetrics(SM_CYSCREEN);

	SendMessage(WM_SIZE, m_nCxScreen * 5 / 7, m_nCyScreen * 5 / 7);

	wchar_t tmpStr[] = L"条形码:\n条形码结果:\n自动识别结果:";
	CString tmpBarcodeStr;
	tmpBarcodeStr.Format(L"%s", tmpStr);
	m_barCode.SetWindowTextW(tmpBarcodeStr);
	
	float ratio = 0.0;
	if (m_nFailCount + m_nSuccessCount != 0)
	{
		ratio = float(m_nSuccessCount) / float(m_nFailCount + m_nSuccessCount);
	}
	
	tmpBarcodeStr.Format(L"Success:%d\nFailed:%d\nSuccess Rate:%f", m_nSuccessCount, m_nFailCount, ratio);
	m_RegRatio.SetWindowTextW(tmpBarcodeStr);

	if (m_pImagePattern == nullptr)
	{
		m_pImagePattern = new CImage();
	}

	//setupListener(_controller);
	//setupObserver(getCameraModel());

	//Execute controller
	_controller->run();

	/*if (m_pImageRec == nullptr)
	{
		m_pImageRec = new CImage();
	}*/
	
	//initCameraModule();

#ifdef  _DEBUG
	testXML();
#endif //  _DEBUG

	//m_pUIThread = new std::thread(&CCarSeat_RecognizationDlg::run, this);

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
	std::unique_lock<std::mutex> lineCameraLock(m_LineCameraMutex);

	m_pNetworkTask = CNetworkTask::GetInstance();
	m_pParamManager = CParamManager::GetInstance();
	if ((m_pNetworkTask == nullptr) || (m_pParamManager == nullptr))
	{
		return;
	}
	
	if (m_pParamManager != nullptr)
	{
		unsigned int tmpKepServerIp = m_pParamManager->GetKepServerIp();
		unsigned int tmpKepServerPort = m_pParamManager->GetKepServerPort();
		try
		{
			m_pKepServer = new CKepServerSocket(tmpKepServerIp, tmpKepServerPort);
			m_pKepServer->resetConnect();
		}
		catch (const std::exception&)
		{
			WriteError(" kepServer socket init failed");
		}
	}
	if (m_pRFIDReader == nullptr)
	{
		m_pRFIDReader = new CRFIDReader();
	}

	std::string imagepath;
	std::string reType;
	std::string tmpImageDir(m_pParamManager->GetImageDirectory());

	//clock_t pingBeginTime = clock();

	
	while (m_bThreadStatus)
	{
		heartBloodServer(m_pParamManager->GetServerIP(), m_pParamManager->GetServerPort());
		

		// 是否开始识别标志
		if (m_bBeginJob == false)
		{
			std::chrono::duration<int, std::milli> a = std::chrono::milliseconds(50);
			std::this_thread::sleep_for(a);
			continue;
		}

		// 和kepServer模块的心跳包
		if (m_pKepServer != nullptr)
		{
			m_pKepServer->HeartBlood();
		}

		// 检测rfid的连接状态
		if (m_pRFIDReader->isConnect() != CRFIDReader::ErrorType::ERROR_OK)
		{
			unsigned int rfidIP = m_pParamManager->GetBarcodeIp();
			unsigned int rfidPort = m_pParamManager->GetBarcodePort();
			m_pRFIDReader->initRFID(rfidIP, rfidPort);
			if (CRFIDReader::ErrorType::ERROR_SOCKET_INVALID == m_pRFIDReader->reset(m_pParamManager->GetBarcodeResetParam()))
			{
				m_pRFIDReader->initRFID(rfidIP, rfidPort);
				m_pRFIDReader->reset(m_pParamManager->GetBarcodeResetParam());
			}

			std::chrono::duration<int, std::milli> b = std::chrono::milliseconds(100);
			std::this_thread::sleep_for(b);
		}

		/*
		读取条形码
		*/
		imagepath = std::string();

		// 读取条形码后需要延迟在取照片
		std::string tmpBarcode = m_pRFIDReader->readBarcode();
		if (m_pParamManager->GetBarcodeTime() < 100)
		{
			std::chrono::duration<int, std::milli> a = std::chrono::milliseconds(100);
			std::this_thread::sleep_for(a);
		}
		else
		{
			std::chrono::duration<int, std::milli> a = std::chrono::milliseconds(m_pParamManager->GetBarcodeTime());
			std::this_thread::sleep_for(a);
		}


		if (tmpBarcode.size() != 0)
		{
			/*if (m_pLineCamera == nullptr)
			{
				initCameraModule();
			}
			if (m_pLineCamera != nullptr)
			{
				imagepath = m_pLineCamera->SaveJpg();
				if (imagepath.size() == 0)
				{
					initCameraModule();

					OnStartCamera();

					CCamera::CameraStatus status = m_pLineCamera->GetCameraStatus();
					if (status == CCamera::CameraStatus::CAMERA_GRAB)
					{
						imagepath = m_pLineCamera->SaveJpg();
					}
					
				}
			}*/
		}
		if ((imagepath.size() != 0) && (tmpBarcode.size() != 0))
		{
			//std::string tmpPath = utils::WStrToStr(imagepath);
			//m_pParamManager->GetImageDirectory();
			//std::string tmpImageAbsolutePath = tmpImageDir + "\\" + imagepath;
			reType = m_pClassify->compute(imagepath.c_str());
			CheckAndUpdate(tmpBarcode, reType, imagepath);
		}
		
		
		std::chrono::duration<int, std::milli> a = std::chrono::milliseconds(50);
		std::this_thread::sleep_for(a);

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

bool CCarSeat_RecognizationDlg::SetLabelManager(CLabelManager * pLabelManager)
{
	if (pLabelManager == nullptr)
	{
		return false;
	}
	m_pLabelManager = pLabelManager;
	return true;
}

void CCarSeat_RecognizationDlg::update(Observable * from, CameraEvent * e)
{
	std::string event = e->getEvent();

	//End of download of image
	if (event == "DownloadComplete")
	{
		//The update processing can be executed from another thread. 
		::PostMessage(this->m_hWnd, WM_USER_DOWNLOAD_COMPLETE, NULL, NULL);
	}

	//Progress of download of image
	if (event == "ProgressReport")
	{
		EdsInt32 percent = *static_cast<EdsInt32 *>(e->getArg());

		//The update processing can be executed from another thread. 
		::PostMessage(this->m_hWnd, WM_USER_PROGRESS_REPORT, percent, NULL);
	}

	//shutdown event
	if (event == "shutDown")
	{
		//::PostMessage(this->m_hWnd, WM_CLOSE, 0, NULL);
	}
}

void CCarSeat_RecognizationDlg::CheckAndUpdate(std::string barcode, std::string RecogType, std::string tmpPath)
{
	/*
	先将barcode转换成和type一致的类型，然后比较，然后刷新界面，如果错误，则弹出对话框，人工干预
	
	*/
	std::string barInternalType = m_pLabelManager->GetInternalTypeByBarcode(barcode);
	std::string typeInternalType = m_pLabelManager->GetInternalTypeByClassifyType(RecogType);
	std::string RecogExternalType = m_pLabelManager->GetExternalTypeByClassifyType(RecogType);
	std::string barExternalType = m_pLabelManager->GetExternalTypeByBarcode(barcode);

	std::string reType;

	wchar_t result[MAX_CHAR_LENGTH] = { 0 };
	//CNetworkTask::message msg;
	bool bUsrInput = true;
	RecogResultA tmpResult;
	memset(&tmpResult, 0, sizeof(RecogResultA));

	//std::string cBarcode = utils::WStrToStr(barcode);
	memcpy(tmpResult.m_szBarcode, barcode.c_str(), sizeof(char) * barcode.size());

	//std::string cRecogExternalType = utils::WStrToStr(RecogExternalType);
	memcpy(tmpResult.m_szTypeByRecog, RecogExternalType.c_str(), sizeof(char) * RecogExternalType.size());

	strcpy_s(tmpResult.m_szCameraName, m_pCameraManager->GetDesriptorByIndex(m_nCameraIndex));

	strcpy_s(tmpResult.m_szLineName, m_pParamManager->GetLineName());

	strcpy_s(tmpResult.m_szRecogMethod, "auto");
	
	//std::string cBarExternalType = utils::WStrToStr(barExternalType);
	memcpy(tmpResult.m_szTypeByBarcode, barExternalType.c_str(), sizeof(char) * barExternalType.size());

	time_t  time1 = time(NULL);//获取系统时间，单位为秒;

	struct tm tmpTime;
	localtime_s(&tmpTime, &time1);//转换成tm类型的结构体;

	sprintf_s(tmpResult.m_szTime, "%04d-%02d-%02d:%02d-%02d-%02d",	\
		tmpTime.tm_year + 1900, tmpTime.tm_mon + 1, tmpTime.tm_mday,	\
		tmpTime.tm_hour, tmpTime.tm_min, tmpTime.tm_sec);

	std::string tmpUsrName(m_pLabelManager->GetLoginUsrName());
	//std::string cUsrName = utils::WStrToStr(tmpUsrName);

	memcpy(tmpResult.m_szUsrName, tmpUsrName.c_str(), sizeof(char) * tmpUsrName.size());

	
	/*
	计算拍照图片的绝对路径
	*/
	//std::wstring tmpWPath = utils::StrToWStr(tmpPath);
	//std::string tmpImageDirectory(m_pParamManager->GetImageDirectory());
	//std::string tmpImagePath = tmpImageDirectory + "\\" + tmpPath;

	/*

	*/
	//sprintf_s(tmpResult.m_szImagePath, "%s\\%s", tmpImageDirectory.c_str(), tmpPath.c_str());
	memcpy(tmpResult.m_szImagePath, tmpPath.c_str(), sizeof(char) * tmpPath.size());

	memset(result, 0, sizeof(result));
	wsprintfW(result, L"条形码：%s\n条形码结果：%s\n自动识别结果：%s", \
		utils::StrToWStr(barcode).c_str(), utils::StrToWStr(RecogType).c_str(), utils::StrToWStr(reType).c_str());
	m_barCode.SetWindowTextW(result);

	if (barInternalType != typeInternalType)	// 识别类型不匹配
	{
		/*
		添加报警系统，以及人工输入代码
		*/
		m_nFailCount++;
		tmpResult.m_bIsCorrect = false;
		float ratio = 100 * m_nSuccessCount / (m_nSuccessCount + m_nFailCount + 0.0000001f);

		wsprintfW(result, L"Success:%d\nFailed:%d\nSuccess Rate:%d.%02d%%", m_nSuccessCount, m_nFailCount, ratio * 100, (ratio * 10000) / 100);
		m_RegRatio.SetWindowTextW(result);


		// 人工输入对话框

		CInputDlg dlg;
		dlg.SetManagePointer(m_pParamManager, m_pLabelManager);
		WriteInfo("inputDlg set imagepath = %s", tmpPath.c_str());
		
		dlg.SetTestImagePath(tmpPath);
		INT_PTR msg = dlg.DoModal();
		
		if (msg == IDOK)
		{
			reType = dlg.GetInputType();
			//std::string cReType = utils::WStrToStr(reType);
			memcpy(tmpResult.m_szTypeByUsrInput, reType.c_str(), sizeof(char) * reType.size());
		}

		////// send message to server
		////
		///  not implement
		if (m_pKepServer != nullptr)
		{
			//m_pKepServer->SetError();
		}
		
	}
	else
	{
		m_nSuccessCount++;
		tmpResult.m_bIsCorrect = true;

		float ratio = 100 * m_nSuccessCount / (m_nSuccessCount + m_nFailCount + 0.0000001f);

		wsprintfW(result, L"Success:%d\nFailed:%d\nSuccess Rate:%d.%02d%%", m_nSuccessCount, m_nFailCount, ratio * 100, (ratio * 10000) / 100);
		m_RegRatio.SetWindowTextW(result);

		if (m_pImagePattern != nullptr)
		{
			//LPCTSTR;

			std::wstring tmpWRecogType = utils::StrToWStr(RecogType);
			//RecogType;//RecogType
			std::string tmpPatternDir(m_pParamManager->GetPatternImagePath());

			std::wstring tmpWPatternDir = utils::StrToWStr(tmpPatternDir);

			tmpWPatternDir = tmpWPatternDir + L"\\" + tmpWRecogType + L".jpg";

			m_pImagePattern->Load(tmpWPatternDir.c_str());
			displayImage(m_pImagePattern, &m_stImagePattern);
			//m_stImagePattern.SetBitmap((HBITMAP)(*m_pImagePattern));
			//m_pImageRec->Load()
		}
		////// send message to server
		////
		///  not implement

		//m_pKepServer->SetCorrect();
	}

	m_pRecogManager->add(tmpResult);
}

void CCarSeat_RecognizationDlg::initCameraModule()
{
	if (m_pCameraManager == nullptr)
	{
		m_pCameraManager = CCameraManager::GetInstance();
	}
	if (m_pCameraManager != nullptr)
	{
		if (m_pCameraManager->GetCameraCount() == 0)
		{
			if (m_pCameraManager->EnumCamera() == false)
			{
				TRACE0("init camera Failed\n");
				WriteError("init camera Failed");
			}
		}
		
		TRACE1("camera count = %d\n", m_pCameraManager->GetCameraCount()); 
		WriteInfo("camera count = %d", m_pCameraManager->GetCameraCount());
		if (m_pCameraManager->GetCameraCount() > 0)
		{
			const char *tmpName = m_pParamManager->GetCameraName();
			m_nCameraIndex = m_pCameraManager->GetCameraIndexByName(tmpName);
		}
	}
	//if (m_pLineCamera == nullptr)
	{
		if (m_nCameraIndex != -1)
		{
			EdsCameraRef  pDevice = m_pCameraManager->GetCamera(m_nCameraIndex);
			if (pDevice != nullptr)
			{
				//m_pLineCamera = new CLineCamera(pDevice);
				const char* tmpImageDir = m_pParamManager->GetImageDirectory();

				WriteInfo("create line camera, set image directory = %s", tmpImageDir);

				/*if (m_pLineCamera != nullptr)
				{
					m_pLineCamera->OpenButton();
					m_pLineCamera->SetDisplayHwnd(GetDlgItem(IDC_IMAGE_REC)->m_hWnd);
					m_pLineCamera->SetImageSaveDirectory(tmpImageDir);

					m_pLineCamera->StartGrabbing();
				}*/

				int err = 0;
				if (err == EDS_ERR_OK)
				{
					err = EdsSetPropertyEventHandler(pDevice, kEdsPropertyEvent_All, CameraEventListener::handlePropertyEvent, (EdsVoid *)_controller);
				}

				//Set Object Event Handler
				if (err == EDS_ERR_OK)
				{
					err = EdsSetObjectEventHandler(pDevice, kEdsObjectEvent_All, CameraEventListener::handleObjectEvent, (EdsVoid *)_controller);
				}

				//Set State Event Handler
				if (err == EDS_ERR_OK)
				{
					err = EdsSetCameraStateEventHandler(pDevice, kEdsStateEvent_All, CameraEventListener::handleStateEvent, (EdsVoid *)_controller);
				}
			}
		}
	}
	WriteInfo("init CameraModule success");
}

SIZE CCarSeat_RecognizationDlg::adjustRecSize(SIZE imageSize, SIZE recSize)
{
	if ((imageSize.cx == 0) || (imageSize.cy == 0) || (recSize.cx == 0) || (recSize.cy == 0))
	{
		return recSize;
	}
	double ratio = static_cast<double>(imageSize.cx) / imageSize.cy;
	double tmpRatio = static_cast<double>(recSize.cx) / recSize.cy;
	SIZE result = recSize;
	if (ratio > tmpRatio)
	{
		result.cy = static_cast<long>(recSize.cx / ratio);
	}
	else
	{
		result.cx = static_cast<long>(recSize.cy * ratio);
	}
	return result;
}

void CCarSeat_RecognizationDlg::adjustControlLocate(int width, int height)
{
	RECT clientRecRect;
	this->GetClientRect(&clientRecRect);
	ScreenToClient(&clientRecRect);

	int yStep = (clientRecRect.bottom - clientRecRect.top) / 5;
	int xStep = (clientRecRect.right - clientRecRect.left) / 5;

	//条形码静态框
	CWnd * pWnd = GetDlgItem(IDC_BARCODE);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(0, 21, xStep, yStep, TRUE);
	}

	// 成功失败以及失败率等，静态框
	pWnd = GetDlgItem(IDC_REG_RATIO);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(0, 50 + yStep, xStep, yStep, TRUE);
	}

	//开始按钮
	pWnd = GetDlgItem(IDC_BUTTON_BEGIN_JOB);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(0, 4 * yStep, xStep, yStep, TRUE);
	}

	// 模板图像
	pWnd = GetDlgItem(IDC_IMAGE_PATTERN);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(xStep, 20, 2 * xStep, 4 * yStep, TRUE);
	}

	//相机视频
	pWnd = GetDlgItem(IDC_IMAGE_REC);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(3 * xStep, 20, 2 * xStep, 4 * yStep, TRUE);
	}

	pWnd = GetDlgItem(IDC_STATIC_LINK_STATUS);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(0, 3 * yStep, xStep, yStep, TRUE);
	}


	Invalidate();


	//CRect rect;

	//GetDlgItem(IDC_IMAGE_REC)->GetWindowRect(&rect);//获取控件的屏幕坐标
	//ScreenToClient(&rect);//转换为对话框上的客户坐标

	//SIZE recSize;
	//recSize.cx = tmpRecRect.right - tmpRecRect.left;
	//recSize.cy = tmpRecRect.bottom - tmpRecRect.top;
	//SIZE tmpAdjustSize = adjustRecSize(tmpSize, recSize);
	//GetDlgItem(IDC_IMAGE_REC)->MoveWindow(rect.left, rect.top, tmpRecRect.left + tmpAdjustSize.cx, tmpRecRect.top + tmpAdjustSize.cy, TRUE);



}

void CCarSeat_RecognizationDlg::testXML()
{
	char greedyXML[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>	\
		<frame>													\
		<cmd>											\
		<id>100000129</id>								\
		<hostGreetings>									\
		<readerType>SIMATIC_RF640R</readerType>			\
		<readerMode>Run</readerMode>					\
		<supportedVersions>								\
		<version>V1.0</version>							\
		</supportedVersions>							\
		</hostGreetings>								\
		</cmd>											\
		</frame>";

	char reXml[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>	\
		<frame>											\
		<reply>											\
		<id>100000129</id>								\
		<resultCode>0</resultCode>						\
		<hostGreetings>									\
		<returnValue>									\
		<version>V1.0</version>							\
		<configType>value_configType</configType>		\
		<configID>value_configID</configID>				\
		</returnValue>									\
		</hostGreetings>								\
		</reply>										\
		</frame>";


	TiXmlDocument lconfigXML;
	//TiXmlParsingData data;
	lconfigXML.Parse(greedyXML);
	if (lconfigXML.Error())
	{
		TRACE0("xml Format is invalid\n");
		WriteError("recvBlood = [%s]", greedyXML);
		return ;
	}
	TiXmlElement *rootElement = lconfigXML.RootElement();
	if ((rootElement == nullptr) || (strncmp(rootElement->Value(), "frame", strlen("frame")) != 0))
	{
		WriteError("recvBlood get root element Failed, %s", greedyXML);
		return ;
	}

	if (rootElement->FirstChildElement() != nullptr)
	{
		std::stack<TiXmlElement*> tmpVec;
		tmpVec.push(rootElement->FirstChildElement());
		while (tmpVec.size() != 0)
		{
			TiXmlElement* tmpNode = tmpVec.top();
			tmpVec.pop();
			for (TiXmlElement *child = tmpNode->FirstChildElement();	\
				child != NULL; child = child->NextSiblingElement())
			{
				tmpVec.push(child);
			}

			WriteInfo("value = %s, text = %s", tmpNode->Value(), tmpNode->GetText());
			//tmpNode->GetText();

			for (TiXmlAttribute *attribute = tmpNode->FirstAttribute(); attribute != NULL;
				attribute->Next())
			{
				WriteInfo("attribute name = %s, value = %s", attribute->Name(),	\
				attribute->Value());
			}
		}
	}
	

	rootElement->Clear();
	lconfigXML.Clear();

}

std::wstring CCarSeat_RecognizationDlg::testGenerateBarcode()
{
	wchar_t tmp[20];
	memset(tmp, 0, sizeof(tmp));
	static double x = 0.000012345678;

	int tmpInt = rand() % 1000;

	x = x + (tmpInt + 2031) / 7777;
	x = x - (int)x;

	for (int i = 0; i < 10; ++i)
	{
		x = 3.9999 * x * (1 - x);
	}
	double tmpValue = x * pow(10, 8);
	int hiValue = static_cast<int>(tmpValue);
	int  lowValue = static_cast<int>((tmpValue - int(tmpValue)) * pow(10, 8));
	swprintf_s(tmp, sizeof(tmp) / sizeof(wchar_t), L"%08d%08d", hiValue, lowValue);

	return std::wstring(tmp);
}

bool CCarSeat_RecognizationDlg::heartBloodServer(unsigned int ip, unsigned int port)
{
	static clock_t pingBeginTime = clock();
	clock_t pingEndTime = clock();
	bool flag = false;
	if (pingEndTime - pingBeginTime > 10 * CLOCKS_PER_SEC)
	{
		// 界面显示与服务器的连接状态
		CStatic *pLinkStatusControl = (CStatic*)GetDlgItem(IDC_STATIC_LINK_STATUS);
		unsigned int tmpServerIp = m_pParamManager->GetServerIP();
		unsigned int tmpLocalIp = m_pParamManager->GetLocalIP();
		WriteInfo("tmpServerIp = 0x%X, tmpLocalIp = 0x%X", tmpServerIp, tmpLocalIp);
		if ((tmpServerIp == 0))
		{
			WriteError("serverIp is 0");
			return false;
		}
		if (CNetworkTask::IsReachable(tmpLocalIp, tmpServerIp) == true)
		{
			if (pLinkStatusControl != nullptr)
			{
				pLinkStatusControl->SetWindowTextW(L"网络连接成功");
			}
			flag = true;
		}
		else
		{
			if (pLinkStatusControl != nullptr)
			{
				pLinkStatusControl->SetWindowTextW(L"与服务器连接失败");
			}
		}
		pingBeginTime = pingEndTime;
		flag = false;
	}
	return flag;
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
		if (m_pCameraManager->GetCameraCount() == 0)
		{
			if (m_pCameraManager->EnumCamera() == false)
			{
				AfxMessageBox(L"enum Camera Failed");
				return;
			}
		}
		
		if (m_pCameraManager->GetCameraCount() == 0)
		{
			AfxMessageBox(L"no Camera connect software");
			return;
		}
		//m_nCameraIndex = 0;
		const char *tmpName = m_pParamManager->GetCameraName();
		//TRACE1("CameraName = %s", tmpName);
		m_nCameraIndex = m_pCameraManager->GetCameraIndexByName(tmpName);
	}
	//if ((m_pLineCamera == nullptr) && (m_nCameraIndex != -1))
	{
		EdsCameraRef pDevice = m_pCameraManager->GetCamera(m_nCameraIndex);
		if (pDevice == nullptr)
		{
			AfxMessageBox(L"get Camera device Failed");
			return;
		}
		//m_pLineCamera = new CLineCamera(pDevice);
		const char* tmpImageDir = m_pParamManager->GetImageDirectory();
		//wchar_t *tmpWPath = utils::CharToWchar(const_cast<char*>(tmpImageDir));

		
		//if (m_pLineCamera != nullptr)
		{
			//m_pLineCamera->SetImageSaveDirectory(tmpImageDir);
			//m_pLineCamera->StartGrabbing();
		}
			
	}
	/*if (m_pLineCamera == nullptr)
	{
		AfxMessageBox(L"line camera init Failed");
		return;
	}
	CCamera::CameraStatus tmpStatus = m_pLineCamera->GetCameraStatus();
	if (tmpStatus == CCamera::CameraStatus::CAMERA_GRAB)
	{
		return;
	}*/

	//do
	//{
	//	if (tmpStatus != CCamera::CameraStatus::CAMERA_OPEN)
	//	{
	//		if (m_pLineCamera->OpenButton() == false)
	//		{
	//			TRACE0("open button Failed\n");
	//			WriteError("open button Failed");
	//			break;
	//		}
	//	}


	//	if (m_pLineCamera->SetExposureTimeAutoMode(MV_EXPOSURE_AUTO_MODE_CONTINUOUS) != true)
	//	{
	//		TRACE0("Set exposure mode timed Failed\n");
	//		WriteError("Set exposure mode time Failed");
	//	}

	//	/*if (m_pLineCamera->SetExposureTimeAutoMode(MV_EXPOSURE_AUTO_MODE_CONTINUOUS) == true)
	//	{
	//		TRACE0("Set auto exposure time mode Failed\n");
	//		WriteError("Set auto exposure time mode Failed");
	//	}*/

	//	int tmpMax = m_pLabelManager->GetExposureTimeMax();
	//	int tmpMin = m_pLabelManager->GetExposureTimeMin();
	//	if (m_pLineCamera->SetExposureTime(tmpMax, tmpMin) != true)
	//	{
	//		TRACE2("Set exposureTime Max = %d,Min = %d Failed\n", tmpMax, tmpMin);
	//		WriteError("Set exposureTime Max = %d,Min = %d Failed", tmpMax, tmpMin);
	//	}

	//	if (m_pLineCamera->SetPixelFormat(PixelType_Gvsp_RGB16_Packed) != true)
	//	{
	//		TRACE0("Set PixelFormat Failed\n");
	//		WriteError("Set PixelFormat Failed");
	//	}
	//	
	//	if (m_pLineCamera->SetBalanceWhile(MV_BALANCEWHITE_AUTO_CONTINUOUS) == false)
	//	{
	//		TRACE0("set BALANCE WHITE AUTO CONTINUOUS Failed\n");
	//		WriteError("set BALANCE WHITE AUTO CONTINUOUS Failed");
	//	}

	//	if (m_pLineCamera->SetGain(0) == false)
	//	{
	//		TRACE0("SetGain Failed\n");
	//		WriteError("SetGain Failed");
	//	}
	//	if (m_pLineCamera->SetTriggerMode(MV_TRIGGER_MODE_OFF) == false)
	//	{
	//		TRACE0("SetTriggerMode MV_TRIGGER_MODE_ON Failed\n");
	//		WriteError("SetTriggerMode MV_TRIGGER_MODE_ON Failed");
	//	}
	//	if (m_pLineCamera->SetTriggerSource(MV_TRIGGER_SOURCE_LINE0) == false)
	//	{
	//		TRACE0("SetTriggerSource MV_TRIGGER_SOURCE_LINE0 Failed\n");
	//		WriteError("SetTriggerSource MV_TRIGGER_SOURCE_LINE0 Failed");
	//	}
	//	HWND imageHwnd = GetDlgItem(IDC_IMAGE_REC)->GetSafeHwnd();
	//	m_pLineCamera->SetDisplayHwnd(imageHwnd);
	//	
	//	SIZE tmpSize = m_pLineCamera->GetImageSize();
	//	if ((tmpSize.cx != 0) && (tmpSize.cy != 0))
	//	{
	//		RECT tmpRecRect;
	//		GetDlgItem(IDC_IMAGE_REC)->GetClientRect(&tmpRecRect);

	//		CRect rect;

	//		GetDlgItem(IDC_IMAGE_REC)->GetWindowRect(&rect);//获取控件的屏幕坐标
	//		ScreenToClient(&rect);//转换为对话框上的客户坐标

	//		SIZE recSize;
	//		recSize.cx = tmpRecRect.right - tmpRecRect.left;
	//		recSize.cy = tmpRecRect.bottom - tmpRecRect.top;
	//		SIZE tmpAdjustSize = adjustRecSize(tmpSize, recSize);
	//		GetDlgItem(IDC_IMAGE_REC)->MoveWindow(rect.left, rect.top, tmpRecRect.left + tmpAdjustSize.cx, tmpRecRect.top + tmpAdjustSize.cy, TRUE);

	//	}

	//	if (m_pLineCamera->StartGrabbing() == false)
	//	{
	//		TRACE0("StartGrabbing Failed\n");
	//		WriteError("StartGrabbing Failed");
	//		break;
	//	}
	//	return;
	//} while (0);
	//
	//m_pLineCamera->CloseDevice();
}


void CCarSeat_RecognizationDlg::OnUpdateStartCamera(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码

	/*if (m_pCameraManager == nullptr)
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
	}*/

	pCmdUI->Enable(TRUE);
	return;
}


void CCarSeat_RecognizationDlg::OnClose()
{
	fireEvent("closing");
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bThreadStatus == true)
	{
		m_bThreadStatus = false;
		std::unique_lock<std::mutex> lineCameraLock(m_LineCameraMutex, std::defer_lock);
		while (!lineCameraLock.try_lock())
		{
			std::chrono::duration<int, std::milli> a = std::chrono::milliseconds(50);
			std::this_thread::sleep_for(a);
		}
		/*if (m_pUIThread->joinable())
		{
			m_pUIThread->join();
			delete m_pUIThread;
			m_pUIThread == nullptr;
		}*/
	}
	
	/*if (m_pLineCamera)
	{
		delete m_pLineCamera;
		m_pLineCamera = nullptr;
	}*/
	if (m_pRecogManager != nullptr)
	{
		delete m_pRecogManager;
		m_pRecogManager = nullptr;
	}
	if (m_pKepServer != nullptr)
	{
		delete m_pKepServer;
		m_pKepServer = nullptr;
	}
	
	/*if (m_pImageRec != nullptr)
	{
		delete m_pImageRec;
		m_pImageRec = nullptr;
	}*/
	if (m_pImagePattern != nullptr)
	{
		delete m_pImagePattern;
		m_pImagePattern = nullptr;
	}
	
	CDHtmlDialog::OnClose();
}


void CCarSeat_RecognizationDlg::OnTakePhoto()
{
	// TODO: 在此添加命令处理程序代码
	/*if (m_pLineCamera != nullptr)
	{
		CCamera::CameraStatus status = m_pLineCamera->GetCameraStatus();
		if (status == CCamera::CameraStatus::CAMERA_GRAB)
		{
			std::string path = m_pLineCamera->SaveJpg();
		}
	}*/
}


void CCarSeat_RecognizationDlg::OnUpdateTakePhoto(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	/*if ((m_pLineCamera == nullptr) ||(m_pLineCamera->GetCameraStatus() != CCamera::CameraStatus::CAMERA_GRAB))
	{
		pCmdUI->Enable(FALSE);
		return;
	}*/
	pCmdUI->Enable(TRUE);
}


void CCarSeat_RecognizationDlg::OnUsrLogin()
{
	// TODO: 在此添加命令处理程序代码
	CLoginDlg dlg;
	INT_PTR ret = dlg.DoModal();
	if (ret == IDOK)
	{
		// 登录部分代码，更新用户名和密码

	}
}


void CCarSeat_RecognizationDlg::OnCloseCamera()
{
	// TODO: 在此添加命令处理程序代码
	/*if (m_pLineCamera != nullptr)
	{
		m_pLineCamera->CloseDevice();
	}*/
}


void CCarSeat_RecognizationDlg::OnUpdateCloseCamera(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	/*if (m_pLineCamera == nullptr)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CCamera::CameraStatus tmpStatus = m_pLineCamera->GetCameraStatus();
	if (tmpStatus == CCamera::CameraStatus::CAMERA_INIT)
	{
		pCmdUI->Enable(FALSE);
		return;
	}*/
	pCmdUI->Enable(TRUE);
}


void CCarSeat_RecognizationDlg::OnExposureTimeTest()
{
	// TODO: 在此添加命令处理程序代码
	//SendMessage()

	/*if (m_pLineCamera != nullptr)
	{
		CCamera::CameraStatus status = m_pLineCamera->GetCameraStatus();
		if (status == CCamera::CameraStatus::CAMERA_GRAB)
		{*/
			
			/*m_pLineCamera->SetExposureTimeAutoMode(MV_EXPOSURE_AUTO_MODE_OFF);
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
			}*/
		/*}
	}*/
}


void CCarSeat_RecognizationDlg::OnSetCameraParameter()
{
	// TODO: 在此添加命令处理程序代码
	/*CCameraParameterDlg dlg;
	dlg.SetLineCamera(m_pLineCamera);
	INT_PTR ret = dlg.DoModal();
	if (ret == IDOK)
	{

	}*/
}


void CCarSeat_RecognizationDlg::OnUpdateSetCameraParameter(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	/*if (m_pLineCamera == nullptr)
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
	}*/
	pCmdUI->Enable(FALSE);
	return;
}


void CCarSeat_RecognizationDlg::OnSize(UINT nType, int cx, int cy)
{
	CDHtmlDialog::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	adjustControlLocate(cx, cy);

}


void CCarSeat_RecognizationDlg::OnMenuQueryBarcode()
{
	// TODO: 在此添加命令处理程序代码
	CQueryDlg dlg;

	INT_PTR ret = dlg.DoModal();
}


void CCarSeat_RecognizationDlg::OnUpdateMenuQueryBarcode(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (pCmdUI != NULL)
	{
		pCmdUI->Enable(TRUE);
	}
}


void CCarSeat_RecognizationDlg::OnChooseCamera()
{
	// TODO: 在此添加命令处理程序代码

}


void CCarSeat_RecognizationDlg::OnUpdateChooseCamera(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码

}

void CCarSeat_RecognizationDlg::displayImage(CImage * pImage, CStatic * pStatic)
{
	if ((pImage == nullptr) || (pStatic == nullptr))
	{
		return;
	}
	if (pStatic->GetSafeHwnd() != NULL)
	{
		//CDC *pDC = pStatic->GetDC();
		RECT rect;
		pStatic->GetWindowRect(&rect);
		ScreenToClient(&rect);
		//pImage->Draw(pDC->GetSafeHdc(), rect);
		//pImage->StretchBlt(pDC->GetSafeHdc(), rect, 0);
		//pStatic->ReleaseDC(pDC);

		pStatic->SetBitmap((HBITMAP)(*pImage));
		pStatic->MoveWindow(rect.left, rect.top, rect.right - rect.left, \
			rect.bottom - rect.top, TRUE);
		//pStatic->ShowWindow(TRUE);
		Invalidate();
	}
}


void CCarSeat_RecognizationDlg::OnBnClickedButtonBeginJob()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bBeginJob == true)
	{
		m_bBeginJob = false;
		GetDlgItem(IDC_BUTTON_BEGIN_JOB)->SetWindowTextW(L"开始作业");
	}
	else
	{
		m_bBeginJob = true;
		GetDlgItem(IDC_BUTTON_BEGIN_JOB)->SetWindowTextW(L"终止作业");
	}
}


void CCarSeat_RecognizationDlg::OnMenuStartGrab()
{
	// TODO: 在此添加命令处理程序代码
	/*if (m_pLineCamera->GetCameraStatus() == CCamera::CameraStatus::CAMERA_OPEN)
	{
		m_pLineCamera->StartGrabbing();
	}*/
}


void CCarSeat_RecognizationDlg::OnMenuStopGrab()
{
	// TODO: 在此添加命令处理程序代码
	/*if (m_pLineCamera->GetCameraStatus() == CCamera::CameraStatus::CAMERA_GRAB)
	{
		m_pLineCamera->StopGrabbing();
	}*/
}


LRESULT CCarSeat_RecognizationDlg::OnDownloadComplete(WPARAM wParam, LPARAM lParam)
{
	//End of download of image
	//_progress.SetPos(0);
	return 0;
}

LRESULT CCarSeat_RecognizationDlg::OnProgressReport(WPARAM wParam, LPARAM lParam)
{
	//_progress.SetPos((int)wParam);
	return 0;
}

