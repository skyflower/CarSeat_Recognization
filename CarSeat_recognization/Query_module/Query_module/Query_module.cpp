// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// Query_Module.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Query_Module.h"
#include "MainFrm.h"
#include "LoginDlg.h"
#include "./common/utils.h"
#include "ChildFrm.h"
#include <time.h>
#include <WinSock2.h>
#include "./xml/tinystr.h"
#include "./xml/tinyxml.h"
#include "BarcodeCheck.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQuery_ModuleApp

BEGIN_MESSAGE_MAP(CQuery_ModuleApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CQuery_ModuleApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CQuery_ModuleApp::OnFileNew)
	ON_COMMAND(ID_BUTTON_CHOOSE, &CQuery_ModuleApp::OnButtonChoose)
	ON_COMMAND(ID_BUTTON_BARCODE, &CQuery_ModuleApp::OnButtonBarcode)
	ON_COMMAND(ID_BUTTON_LOGIN, &CQuery_ModuleApp::OnButtonLogin)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CHOOSE, &CQuery_ModuleApp::OnUpdateButtonChoose)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_BARCODE, &CQuery_ModuleApp::OnUpdateButtonBarcode)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LOGIN, &CQuery_ModuleApp::OnUpdateButtonLogin)
END_MESSAGE_MAP()


// CQuery_ModuleApp 构造

CQuery_ModuleApp::CQuery_ModuleApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Query_Module.AppID.NoVersion"));

	m_pLog = CLog::GetInstance();
	m_pParamManager = CParamManager::GetInstance();
	m_pQueryMDIFrame = nullptr;


	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	/*CMFCRibbonBar* pRibbon = ((CMDIFrameWndEx*)AfxGetMainWnd())->GetRibbonBar();
	if (pRibbon != nullptr)
	{
		CMFCRibbonBaseElement *pChooseButton = pRibbon->FindByID(ID_BUTTON_CHOOSE);
		if (pChooseButton != nullptr)
		{
			pChooseButton->OnShow(FALSE);
		}
		pChooseButton = pRibbon->FindByID(ID_BUTTON_BARCODE);
		if (pChooseButton != nullptr)
		{
			pChooseButton->OnShow(FALSE);
		}
	}*/
}

// 唯一的一个 CQuery_ModuleApp 对象

CQuery_ModuleApp theApp;


// CQuery_ModuleApp 初始化

BOOL CQuery_ModuleApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建主 MDI 框架窗口
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// 试图加载共享 MDI 菜单和快捷键表
	//TODO: 添加附加成员变量，并加载对
	//	应用程序可能需要的附加菜单类型的调用
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_Query_ModuleTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_Query_ModuleTYPE));

	// 主窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();


	pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	pFrame->LockWindowUpdate();
	// 创建新的 MDI 子窗口
	m_pQueryMDIFrame = (CQueryFrame*)pFrame->CreateNewChild(
		RUNTIME_CLASS(CQueryFrame), IDR_Query_ModuleTYPE, m_hMDIMenu, m_hMDIAccel);
	pFrame->UnlockWindowUpdate();

	

	return TRUE;
}

int CQuery_ModuleApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	if (m_pParamManager != nullptr)
	{
		delete m_pParamManager;
		m_pParamManager = nullptr;
	}

	if (m_pLog != nullptr)
	{
		delete m_pLog;
		m_pLog = nullptr;
	}


	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CQuery_ModuleApp 消息处理程序

void CQuery_ModuleApp::OnFileNew()
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	pFrame->LockWindowUpdate();
	// 创建新的 MDI 子窗口
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_Query_ModuleTYPE, m_hMDIMenu, m_hMDIAccel);
	pFrame->UnlockWindowUpdate();
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CQuery_ModuleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CQuery_ModuleApp 自定义加载/保存方法

void CQuery_ModuleApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CQuery_ModuleApp::LoadCustomState()
{
}

void CQuery_ModuleApp::SaveCustomState()
{

}


// CQuery_ModuleApp 消息处理程序
void CQuery_ModuleApp::OnButtonChoose()
{
	// TODO: 在此添加命令处理程序代码
	int ret = mConditionDlg.DoModal();

	if (ret == IDCANCEL)
	{
		return;
	}
	CConditionFilterA filter = mConditionDlg.GetFilterCondition();

	CNetworkTask *queryTask = new CNetworkTask(m_pParamManager->GetServerIP(), m_pParamManager->GetServerPort());
	const std::string &usrName = m_pParamManager->GetLoginUserName();
	const std::string &passwd = m_pParamManager->GetLoginPasswd();

	std::vector<RecogResultW>* selectedRecog = queryTask->QueryRecogInfo(filter, m_pParamManager->GetLocalIP(), usrName, passwd);
	
	if ((selectedRecog != nullptr) && ( selectedRecog->size() > 0))
	{
		m_pQueryMDIFrame->InsertItem(selectedRecog);
	}
	else
	{
		AfxMessageBox(L"查询失败");
	}

	delete queryTask;
	queryTask = nullptr;

	delete selectedRecog;
	selectedRecog = nullptr;
}


void CQuery_ModuleApp::OnButtonBarcode()
{
	// TODO: 在此添加命令处理程序代码
	/*
	条形码对照表
	*/
	//TRACE0("edit barcode\n");
	CBarcodeCheck dlg;
	dlg.SetTypePointer(m_pParamManager->GetBarcodeType(), m_pParamManager->GetClassifyType());
	INT_PTR ret = dlg.DoModal();
	if (ret == IDOK)
	{
		//TRACE0("CBarcodeCheck return IDOK\n");
	}
}


void CQuery_ModuleApp::OnButtonLogin()
{
	// TODO: 在此添加命令处理程序代码
	CLoginDlg dlg;
	dlg.SetLoginUserName(utils::StrToWStr(m_pParamManager->GetLoginUserName()).c_str());
	dlg.SetLoginPasswd(utils::StrToWStr(m_pParamManager->GetLoginPasswd()).c_str());
	dlg.SetAutoSaveFlag(m_pParamManager->GetAutoSaveFlag());

	int ret = dlg.DoModal();
	if (ret == IDCANCEL)
	{
		return;
	}

	int length = 0;
	if (dlg.GetLoginUserName(mUsrName, length) == false)
	{
		AfxMessageBox(L"获取用户名失败");
		return;
	}
	if (dlg.GetLoginPasswd(mPasswd, length) == false)
	{
		AfxMessageBox(L"获取密码失败");
		return;
	}

	bool tmpSaveFlag = dlg.GetAutoSaveFlag();

	/*
	判断用户名是否正确，
	*/



	std::wstring tmp = std::wstring(mUsrName);

	m_pParamManager->SetLoginUserName(utils::WStrToStr(tmp));

	tmp = std::wstring(mPasswd);

	m_pParamManager->SetLoginPasswd(utils::WStrToStr(mPasswd));
}


void CQuery_ModuleApp::OnUpdateButtonChoose(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	std::string tmpUserName = m_pParamManager->GetLoginUserName();
	if ((tmpUserName.size() == 0) ||(wcslen(mUsrName) == 0))
	{
		pCmdUI->Enable(FALSE);
	}
}


void CQuery_ModuleApp::OnUpdateButtonBarcode(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	std::string tmpUserName = m_pParamManager->GetLoginUserName();
	if ((tmpUserName.size() == 0) || (wcslen(mUsrName) == 0))
	{
		pCmdUI->Enable(FALSE);
	}
}


void CQuery_ModuleApp::OnUpdateButtonLogin(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码

}

std::vector<RecogResultW>* CQuery_ModuleApp::QueryInfo(char *queryXml, int queryLen, char *result, int resultLen)
{
	std::function<bool(char *xml, char* packageName)> replayFunc = [](char *xml, char* packageName)	\
		->bool
	{
		if ((xml == nullptr) || (packageName == nullptr)	\
			|| (strlen(xml) == 0) || (strlen(packageName) == 0))
		{
			return false;
		}
		TiXmlDocument lconfigXML;
		lconfigXML.Parse(xml);
		TiXmlElement *rootElement = nullptr;
		bool flag = false;
		do
		{
			if (lconfigXML.Error())
			{
				break;
			}
			TiXmlElement *rootElement = lconfigXML.RootElement();
			if ((rootElement == nullptr) || (strncmp(rootElement->Value(), "reply", strlen("reply")) != 0))
			{
				break;
			}
			if (strcmp(rootElement->Attribute("package"), packageName) != 0)
			{
				WriteInfo("package = %s", rootElement->Attribute("package"));
				break;
			}
			if (strcmp(rootElement->Attribute("status"), "success") != 0)
			{
				break;
			}
			flag = true;;
		} while (0);
		if (rootElement != nullptr)
		{
			rootElement->Clear();
		}
		lconfigXML.Clear();
		return flag;
	};

	if ((queryLen == 0) || (queryXml == nullptr) || (result == nullptr) || (resultLen == 0))
	{
		return false;
	}
	memset(result, 0, sizeof(char) * resultLen);
	unsigned int localIp = m_pParamManager->GetLocalIP();
	if ((localIp == 0) || (localIp == -1))
	{
		return nullptr;
	}

	int err = 0;
	SOCKET socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketFD == -1)
	{
		TRACE("create socket failed\n");
		return nullptr;
	}

	sockaddr_in addr;

	//接收时限
	int nNetTimeout = 1000;
	setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_pParamManager->GetServerPort());
	addr.sin_addr.S_un.S_addr = htonl(m_pParamManager->GetServerIP());

	if (0 != connect(socketFD, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		err = WSAGetLastError();
		TRACE1("connect failed,err = %u\n", err);
		WriteError("connect failed, err = %u", err);
		closesocket(socketFD);
		socketFD = INVALID_SOCKET;
		return nullptr;
	}

	size_t tmpLen = send(socketFD, queryXml, queryLen, 0);// MSG_DONTROUTE);
	if (tmpLen == SOCKET_ERROR)
	{
		WriteError("send Failed, msg = %s, len = %u, Err:", queryXml, queryLen, WSAGetLastError());
		closesocket(socketFD);
		socketFD = INVALID_SOCKET;
		return nullptr;
	}
	tmpLen = recv(socketFD, result, resultLen, MSG_WAITALL);
	if (SOCKET_ERROR == tmpLen)
	{
		WriteError("recv Failed, Err: %u", GetLastError());
		closesocket(socketFD);
		socketFD = INVALID_SOCKET;
		return nullptr;
	}
	WriteInfo("query Result = [%s]", result);
	std::vector<RecogResultW> *value = ParseQueryResult(result, strlen(result));

	replyMessage(socketFD, "reply", "reply", "success");

	const size_t bufferSize = 10 * 1024 * 1024;
	char *buffer = new char[bufferSize];
	memset(buffer, 0, sizeof(char) * bufferSize);
	if ((value != nullptr) && (value->size() > 0))
	{
		imageHeader tmpHeader;
		char tmpImagePath[MAX_CHAR_LENGTH];
		for (size_t i = 0; i < value->size(); ++i)
		{
			tmpLen = recv(socketFD, (char*)&tmpHeader, sizeof(imageHeader), MSG_WAITALL);
			if (SOCKET_ERROR == tmpLen)
			{
				WriteError("recv ImageHeader, Err: %u", GetLastError());
				closesocket(socketFD);
				socketFD = INVALID_SOCKET;
				return value;
			}
			replyMessage(socketFD, "reply", "imageHeader", "success");

			recv(socketFD, buffer, bufferSize, MSG_WAITALL);
			
			replyMessage(socketFD, "reply", "imageData", "success");

			memset(tmpImagePath, 0, sizeof(tmpImagePath));
			sprintf_s(tmpImagePath, sizeof(tmpImagePath), "%s\\%s", m_pParamManager->GetCacheDirectory(), tmpHeader.path);

			if (true == saveCacheImage(buffer, tmpHeader.size, tmpImagePath))
			{
				wchar_t *tmpWImagePath = utils::CharToWChar(tmpImagePath);
				if (tmpWImagePath != nullptr)
				{
					memcpy(value->at(i).m_szImagePath, tmpWImagePath, sizeof(wchar_t) * wcslen(tmpWImagePath));
					delete[] tmpWImagePath;
					tmpWImagePath = nullptr;
				}
			}
		}
	}
	delete[]buffer;
	buffer = nullptr;

	closesocket(socketFD);
	socketFD = INVALID_SOCKET;

	return value;
	
}

std::vector<RecogResultW>* CQuery_ModuleApp::ParseQueryResult(char *info, int length)
{
	TiXmlDocument lconfigXML;
	lconfigXML.Parse(info);
	TiXmlElement *rootElement = nullptr;
	bool flag = false;
	std::vector<RecogResultW> *result = new std::vector<RecogResultW>();
	do
	{
		if (lconfigXML.Error())
		{
			break;
		}
		TiXmlElement *rootElement = lconfigXML.RootElement();
		if ((rootElement == nullptr) || (strncmp(rootElement->Value(), "reply", strlen("reply")) != 0))
		{
			break;
		}
		if (strcmp(rootElement->Attribute("package"), "search") != 0)
		{
			WriteInfo("package = %s", rootElement->Attribute("package"));
			break;
		}
		if (strcmp(rootElement->Attribute("status"), "success") != 0)
		{
			break;
		}
		if (strcmp(rootElement->Attribute("version"), "0.1") != 0)
		{
			break;
		}
		size_t seatCount = 0;
		int i = 0;
		RecogResultA tmpResult;
		for (TiXmlElement*child = rootElement->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
		{
			if (strcmp(child->Value(), "seatCount") == 0)
			{
				seatCount = atoi(child->GetText());
			}
			else
			{
				char label[20];
				sprintf_s(label, sizeof(label), "Seat%d", i + 1);
				if (strcmp(child->Value(), label) == 0)
				{
					memset(&tmpResult, 0, sizeof(RecogResultA));
					ParseRecogResult(child, tmpResult);
				}
				RecogResultW tmpResultW;
				utils::RecogResultCToW(tmpResult, tmpResultW);
				result->emplace_back(tmpResultW);
				i = i + 1;
			}
		}


		flag = true;;
	} while (0);
	if (rootElement != nullptr)
	{
		rootElement->Clear();
	}
	lconfigXML.Clear();
	return result;
}

bool CQuery_ModuleApp::ParseRecogResult(TiXmlElement * elment, RecogResultA &recog)
{
	if (elment == nullptr)
	{
		return false;
	}
	TiXmlElement *child = elment->FirstChildElement();
	for (; child != nullptr; child = child->NextSiblingElement())
	{
		if (strcmp(child->Value(), "lineID") == 0)
		{
			memcpy(recog.m_szLineName, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "ip") == 0)
		{
			TRACE1("ip = %s\n", child->GetText());
			//memcpy(recog.m_szLineName, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "time") == 0)
		{
			memcpy(recog.m_szTime, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "barcode") == 0)
		{
			memcpy(recog.m_szBarcode, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "barcodeResult") == 0)
		{
			memcpy(recog.m_szInternalType, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "imageName") == 0)
		{
			memcpy(recog.m_szImagePath, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "method") == 0)
		{
			memcpy(recog.m_szRecogMethod, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "usrName") == 0)
		{
			memcpy(recog.m_szUsrName, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "typeByRecog") == 0)
		{
			memcpy(recog.m_szTypeByRecog, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "typeByBarcode") == 0)
		{
			memcpy(recog.m_szTypeByBarcode, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "typeByInput") == 0)
		{
			memcpy(recog.m_szTypeByUsrInput, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "cameraName") == 0)
		{
			memcpy(recog.m_szCameraName, child->GetText(), strlen(child->GetText()));
		}
		else if (strcmp(child->Value(), "correct") == 0)
		{
			int flag = atoi(child->GetText());
			recog.m_bIsCorrect = (flag == 0 ? false : true);
		}

	}
	return true;
}

bool CQuery_ModuleApp::saveCacheImage(char *buffer, size_t bufferLength, char *imagePath)
{
	FILE *fp = nullptr;
	fopen_s(&fp, imagePath, "wb+");
	if (fp == nullptr)
	{
		return false;
	}
	fwrite(buffer, 1, bufferLength, fp);
	fclose(fp);
	fp = nullptr;

	return true;
}

bool CQuery_ModuleApp::replyMessage(SOCKET & socketFD, char *rootName, char *packageName, char* status)
{
	TiXmlDocument *xdoc = new TiXmlDocument();

	TiXmlDeclaration* xdec = new TiXmlDeclaration("1.0", "utf-8", "");
	xdoc->LinkEndChild(xdec);

	TiXmlElement *root = new TiXmlElement(rootName);
	xdoc->LinkEndChild(root);

	char tmpBuf[100];
	memset(tmpBuf, 0, sizeof(tmpBuf));
	sprintf_s(tmpBuf, "%d.%d", 0, 1);

	root->SetAttribute("version", tmpBuf);
	root->SetAttribute("package", packageName);
	root->SetAttribute("status", status);

	TiXmlPrinter printer;
	std::string xmlstr;
	xdoc->Accept(&printer);
	xmlstr = printer.CStr();

	size_t nSize = 2000;
	char *xml = new char[nSize];
	memset(xml, 0, sizeof(char) * nSize);
	memcpy(xml, xmlstr.c_str(), xmlstr.size() * sizeof(char));
	xml[xmlstr.size()] = '\0';

	send(socketFD, xml, strlen(xml), 0);

	delete[] xml;
	xml = nullptr;

	delete xdoc;
	xdoc = nullptr;

	return true;
}