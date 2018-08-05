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
	m_pLineCamera(nullptr),
	m_nCxScreen(0),
	m_nCyScreen(0),
	m_pRFIDReader(nullptr),
	m_pKepServer(nullptr)
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

	m_nCxScreen = GetSystemMetrics(SM_CXSCREEN);
	m_nCyScreen = GetSystemMetrics(SM_CYSCREEN);

	SendMessage(WM_SIZE, m_nCxScreen * 5 / 7, m_nCyScreen * 5 / 7);

	wchar_t tmpStr[] = L"K215-黑色-菱形纹理";
	CString tmpBarcodeStr;
	tmpBarcodeStr.Format(L"%s", tmpStr);
	m_barCode.SetWindowTextW(tmpBarcodeStr);
	
	float ratio = 0.0;
	if (m_nFailCount + m_nSuccessCount != 0)
	{
		ratio = double(m_nSuccessCount) / double(m_nFailCount + m_nSuccessCount);
	}
	
	tmpBarcodeStr.Format(L"Success:%d\nFailed:%d\nSuccess Rate:%f", m_nSuccessCount, m_nFailCount, ratio);
	m_RegRatio.SetWindowTextW(tmpBarcodeStr);

	if (m_pImagePattern == nullptr)
	{
		m_pImagePattern = new CImage();
	}
	if (m_pImageRec == nullptr)
	{
		m_pImageRec = new CImage();
	}
	
	initCameraModule();

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
	//std::wstring preImagePath;
	//std::wstring barcode;
	m_pNetworkTask = CNetworkTask::GetInstance();
	m_pParamManager = CParamManager::GetInstance();
	if ((m_pNetworkTask == nullptr) || (m_pParamManager == nullptr))
	{
		return;
	}
	const char* tmpImageDir = m_pParamManager->GetImageDirectory();
	//char *tmpPointer = const_cast<char*>(tmpImageDir);
	wchar_t *tmpWPath = utils::CharToWchar(const_cast<char*>(tmpImageDir));

	if (tmpWPath != nullptr)
	{
		if (m_pLineCamera != nullptr)
		{
			m_pLineCamera->SetImageSaveDirectory(tmpWPath);
			m_pLineCamera->StartGrabbing();
		}
		delete[]tmpWPath;
		tmpWPath = nullptr;
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

	std::wstring imagepath;
	std::wstring reType;
	while (m_bThreadStatus)
	{

		/*
		
		
		*/


		// 和kepServer模块的心跳包
		if (m_pKepServer != nullptr)
		{
			m_pKepServer->HeartBlood();
		}
		

		imagepath = std::wstring();
		std::wstring tmpBarcode = L".....";// m_pRFIDReader->readBarcode();
		if (tmpBarcode.size() != 0)
		{
			if (m_pLineCamera == nullptr)
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
			}
		}
		if ((imagepath.size() != 0) && (tmpBarcode.size() != 0))
		{
			std::string tmpPath = utils::WStrToStr(imagepath);
			reType = m_pClassify->compute(tmpPath.c_str());
			CheckAndUpdate(tmpBarcode, reType, tmpPath);
		}
		
		/*std::wstring currentImage = m_pNetworkTask->GetCurrentImagePath();
		if (preImagePath != currentImage)
		{
			barcode = m_pNetworkTask->GetCurrentBarcode();
			std::string szCurImage = utils::WStrToStr(currentImage);
			std::wstring type = m_pClassify->GetImageType(szCurImage.c_str());
			if (type.size() != 0)
			{
				CheckAndUpdate(barcode, type);
			}
		}*/
		std::chrono::duration<int, std::milli> a = std::chrono::milliseconds(200);
		std::this_thread::sleep_for(a);

		

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

bool CCarSeat_RecognizationDlg::SetLabelManager(CLabelManager * pLabelManager)
{
	if (pLabelManager == nullptr)
	{
		return false;
	}
	m_pLabelManager = pLabelManager;
	return true;
}

void CCarSeat_RecognizationDlg::CheckAndUpdate(std::wstring barcode, std::wstring RecogType, std::string tmpPath)
{
	/*
	先将barcode转换成和type一致的类型，然后比较，然后刷新界面，如果错误，则弹出对话框，人工干预
	
	*/
	std::wstring barInternalType = m_pLabelManager->GetInternalTypeByBarcode(barcode);
	std::wstring typeInternalType = m_pLabelManager->GetInternalTypeByClassifyType(RecogType);
	std::wstring RecogExternalType = m_pLabelManager->GetExternalTypeByClassifyType(RecogType);
	std::wstring barExternalType = m_pLabelManager->GetExternalTypeByBarcode(barcode);

	std::wstring reType;

	wchar_t result[MAX_CHAR_LENGTH] = { 0 };
	//CNetworkTask::message msg;
	bool bUsrInput = true;
	struct RecogResult tmpResult;
	memset(&tmpResult, 0, sizeof(RecogResult));

	std::string cBarcode = utils::WStrToStr(barcode);
	memcpy(tmpResult.m_szBarcode, cBarcode.c_str(), sizeof(char) * cBarcode.size());

	std::string cRecogExternalType = utils::WStrToStr(RecogExternalType);
	memcpy(tmpResult.m_szTypeByRecog, cRecogExternalType.c_str(), sizeof(char) * cRecogExternalType.size());

	strcpy_s(tmpResult.m_szCameraName, (char*)m_pCameraManager->GetCamera(m_nCameraIndex)->SpecialInfo.stGigEInfo.chModelName);

	strcpy_s(tmpResult.m_szLineName, m_pParamManager->GetLineName());

	strcpy_s(tmpResult.m_szRecogMethod, "auto");
	
	std::string cBarExternalType = utils::WStrToStr(barExternalType);
	memcpy(tmpResult.m_szTypeByBarcode, cBarExternalType.c_str(), sizeof(char) * cBarExternalType.size());

	time_t  time1 = time(NULL);//获取系统时间，单位为秒;

	struct tm tmpTime;
	localtime_s(&tmpTime, &time1);//转换成tm类型的结构体;

	sprintf_s(tmpResult.m_szTime, "%04d-%02d-%02d:%02d-%02d-%02d",	\
		tmpTime.tm_year + 1900, tmpTime.tm_mon + 1, tmpTime.tm_mday,	\
		tmpTime.tm_hour, tmpTime.tm_min, tmpTime.tm_sec);

	std::wstring tmpUsrName(m_pLabelManager->GetLoginUsrName());
	std::string cUsrName = utils::WStrToStr(tmpUsrName);

	memcpy(tmpResult.m_szUsrName, cUsrName.c_str(), sizeof(char) * cUsrName.size());	

	memcpy(tmpResult.m_szImagePath, tmpPath.c_str(), sizeof(char) * tmpPath.size());

	std::wstring tmpWPath = utils::StrToWStr(tmpPath);

	if (m_pImageRec != nullptr)
	{
		//LPCTSTR;
		if (_waccess_s(tmpWPath.c_str(), 0x04) == 0)
		{
			m_pImageRec->Destroy();
			HRESULT ret = m_pImageRec->Load(tmpWPath.c_str());
			if (ret != S_OK)
			{
				return;
			}
			if (m_stImageRec.GetSafeHwnd() != NULL)
			{
				m_stImageRec.SetBitmap((HBITMAP)(*m_pImageRec));
			}

		}
		//m_pImageRec->Load(tmpWPath.c_str());
		//m_stImageRec.SetBitmap((HBITMAP)(*m_pImageRec));
		//m_pImageRec->Load()
	}
	/* CImage::Load(filePath)   */
	// CStatic.SetBitmap((HBITMAP)CImage))
	//CImage m_image;


	if (barInternalType != typeInternalType)
	{
		/*
		添加报警系统，以及人工输入代码
		*/
		//m_nFailCount;
		tmpResult.m_bIsCorrect = false;
		CInputDlg dlg;
		dlg.SetManagePointer(m_pParamManager, m_pLabelManager);
		dlg.SetTestImagePath(tmpWPath);
		INT_PTR msg = dlg.DoModal();
		
		if (msg == IDOK)
		{
			reType = dlg.GetInputType();
			std::string cReType = utils::WStrToStr(reType);
			memcpy(tmpResult.m_szTypeByUsrInput, cReType.c_str(), sizeof(char) * cReType.size());
		}
		////// send message to server
		////
		///  not implement
		if (m_pKepServer != nullptr)
		{
			m_pKepServer->SetError();
		}
		
	}
	else
	{
		m_nSuccessCount++;
		tmpResult.m_bIsCorrect = true;
		if (m_pImagePattern != nullptr)
		{
			//LPCTSTR;

			//std::wstring tmpWPath = utils::StrToWStr(tmpPath);
			//RecogType;//RecogType
			std::string tmpPatternDir(m_pParamManager->GetPatternImagePath());

			std::wstring tmpWPatternDir = utils::StrToWStr(tmpPatternDir);

			tmpWPatternDir = tmpWPatternDir + L"\\" + RecogType + L".jpg";


			m_pImagePattern->Load(tmpWPatternDir.c_str());
			m_stImagePattern.SetBitmap((HBITMAP)(*m_pImagePattern));
			//m_pImageRec->Load()
		}
		////// send message to server
		////
		///  not implement
		m_pKepServer->SetCorrect();
	}

	
	float ratio = 100 * m_nSuccessCount / (m_nSuccessCount + m_nFailCount + 0.0000001);

	wsprintfW(result, L"Success:%d\nFailed:%d\nSuccess Rate:%2f", m_nSuccessCount, m_nFailCount, ratio);
	m_RegRatio.SetWindowTextW(result);

	memset(result, 0, sizeof(result));
	wsprintfW(result, L"条形码：%s\n条形码结果：%s\n自动识别结果：%s", barcode, RecogType, reType);
	m_barCode.SetWindowTextW(result);
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
			}
		}
		
		TRACE1("camera count = %u\n", m_pCameraManager->GetCameraCount());
		if (m_pCameraManager->GetCameraCount() > 0)
		{
			const char *tmpName = m_pParamManager->GetCameraName();
			//TRACE1("CameraName = %s", tmpName);
			m_nCameraIndex = m_pCameraManager->GetCameraIndexByName(tmpName);
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
		result.cy = recSize.cx / ratio;
	}
	else
	{
		result.cx = recSize.cy * ratio;
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

	CWnd * pWnd = GetDlgItem(IDC_BARCODE);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(0, 20, xStep, yStep, TRUE);
	}
	pWnd = GetDlgItem(IDC_REG_RATIO);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(0, 50 + yStep, xStep, yStep, TRUE);
	}

	pWnd = GetDlgItem(IDC_IMAGE_PATTERN);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(xStep, 20, 2 * xStep, 4 * yStep, TRUE);
	}

	pWnd = GetDlgItem(IDC_IMAGE_REC);
	if (pWnd != nullptr)
	{
		pWnd->MoveWindow(3 * xStep, 20, 2 * xStep, 4 * yStep, TRUE);
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
		
		SIZE tmpSize = m_pLineCamera->GetImageSize();
		if ((tmpSize.cx != 0) && (tmpSize.cy != 0))
		{
			RECT tmpRecRect;
			GetDlgItem(IDC_IMAGE_REC)->GetClientRect(&tmpRecRect);

			CRect rect;

			GetDlgItem(IDC_IMAGE_REC)->GetWindowRect(&rect);//获取控件的屏幕坐标
			ScreenToClient(&rect);//转换为对话框上的客户坐标

			SIZE recSize;
			recSize.cx = tmpRecRect.right - tmpRecRect.left;
			recSize.cy = tmpRecRect.bottom - tmpRecRect.top;
			SIZE tmpAdjustSize = adjustRecSize(tmpSize, recSize);
			GetDlgItem(IDC_IMAGE_REC)->MoveWindow(rect.left, rect.top, tmpRecRect.left + tmpAdjustSize.cx, tmpRecRect.top + tmpAdjustSize.cy, TRUE);

		}

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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bThreadStatus == true)
	{
		m_bThreadStatus = false;
		/*if (m_pUIThread->joinable())
		{
			m_pUIThread->join();
			delete m_pUIThread;
			m_pUIThread == nullptr;
		}*/
	}
	
	if (m_pLineCamera)
	{
		delete m_pLineCamera;
		m_pLineCamera = nullptr;
	}
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
	
	if (m_pImageRec != nullptr)
	{
		delete m_pImageRec;
		m_pImageRec = nullptr;
	}
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
