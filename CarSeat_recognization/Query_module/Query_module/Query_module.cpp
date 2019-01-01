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
	TRACE1("ret = %d\n", ret);
	WriteInfo("date = [begin = %s],[end = %s]", filter.mDateTimeBeign, filter.mDateTimeEnd);
	//WriteInfo("time = [begin = %s],[end = %s]", filter.mTimeBegin, filter.mTimeEnd);
	WriteInfo("line = [begin = %s],[end = %s]", filter.mLineBegin, filter.mLineEnd);
	WriteInfo("barcode = [begin = %s],[end = %s]", filter.mBarcodeBegin, filter.mBarcodeEnd);
	WriteInfo("seatType = [%s], methodType = [%s]", filter.mSeatType, filter.mMethodType);
	

	char tmpTime[20] = { 0 };
	memset(tmpTime, 0, sizeof(tmpTime));

	time_t now_time = time(NULL);
	tm tmp_now_tm;
	localtime_s(&tmp_now_tm, &now_time);

	/*
	将查询条件以及用户名和密码发送到服务器端
	*/
	char queryXml[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?><search><version>0.1</version><info ip=\"%s\" usr=\"%s\" passwd=\"%s\" time=\"%s\"/>%s</search>";

	

	/*<timePolicy control = \"%d\" startTime=\"%s\" endTime=\"%s\"/>				\
		<linePolicy control=\"%d\" startLine=\"%s\" endLine=\"%s\"/>				\
		<barcodePolicy control=\"%d\" startBarcode=\"%s\" endBarcode=\"%s\"/>		\
		<typePolicy control=\"%d\" type=\"%s\"/>										\
		<methodPolicy control=\"%d\" method=\"%s\"/>		*/					\



	const size_t length = 1000;
	char *tmpXml = new char[length];
	if (tmpXml == nullptr)
	{
		WriteError("malloc %d memory failed", length);
		AfxMessageBox(L"malloc 1000 memory faield");
		return;
	}
	memset(tmpXml, 0, sizeof(char) * length);

	char *tmpChar = new char[length];
	if (tmpChar == nullptr)
	{
		WriteError("malloc %d memory failed", length);
		AfxMessageBox(L"malloc 1000 memory faield");
		delete tmpXml;
		tmpXml = nullptr;
		return;
	}
	memset(tmpChar, 0, sizeof(char) * length);




	time_t tmpTime_t = time(NULL);
	struct tm nowTime;
	localtime_s(&nowTime, &tmpTime_t);


	sprintf_s(tmpTime, "%04d%02d%02d.%02d%02d%02d", \
		nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, \
		nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);

	char ipStr[20] = { 0 };
	memset(ipStr, 0, sizeof(ipStr));
	unsigned int ip = m_pParamManager->GetLocalIP();

	in_addr in;
	in.S_un.S_addr = ip;
	
	inet_ntop(AF_INET, (void *)&in, ipStr, sizeof(ipStr));

	// 时间选择策略
	const int policyLength = 200;
	char *timePolicy = new char[policyLength];

	memset(timePolicy, 0, sizeof(timePolicy));
	int timePolicyControl = 1;
	if (strcmp(filter.mDateTimeBeign, filter.mDateTimeEnd) == 0)
	{
		timePolicyControl = 0;
	}

	sprintf_s(timePolicy, policyLength, "<timePolicy control=\"%d\" startTime=\"%s\" endTime=\"%s\"/>",
		timePolicyControl, filter.mDateTimeBeign, filter.mDateTimeEnd);


	//产线选择策略
	//char linePolicy[policyLength] = { 0 };
	char *linePolicy = new char[policyLength];
	memset(linePolicy, 0, sizeof(linePolicy));
	int linePolicyControl = 1;
	if ((strlen(filter.mLineBegin) == 0) && (strlen(filter.mLineEnd) == 0))
	{
		linePolicyControl = 0;
	}
	sprintf_s(linePolicy, policyLength, "<linePolicy control=\"%d\" startLine=\"%s\" endLine=\"%s\"/>",
		linePolicyControl, filter.mLineBegin, filter.mLineEnd);



	// 格式化条形码策略
	//char barcodePolicy[policyLength] = { 0 };
	char *barcodePolicy = new char[policyLength];
	memset(barcodePolicy, 0, sizeof(barcodePolicy));
	int barcodePolicyControl = 1;
	if ((strlen(filter.mBarcodeBegin) == 0) && (strlen(filter.mBarcodeEnd) == 0))
	{
		barcodePolicyControl = 0;
	}
	sprintf_s(barcodePolicy, policyLength, "<barcodePolicy control=\"%d\" startBarcode=\"%s\" endBarcode=\"%s\"/>",
		barcodePolicyControl, filter.mBarcodeBegin, filter.mBarcodeEnd);


	// 座椅类型策略
	//char typePolicy[policyLength] = { 0 };
	char *typePolicy = new char[policyLength];
	memset(typePolicy, 0, sizeof(typePolicy));
	int typePolicyControl = 1;
	if ((strlen(filter.mSeatType) == 0))
	{
		typePolicyControl = 0;
	}
	sprintf_s(typePolicy, policyLength, "<typePolicy control=\"%d\" type=\"%s\"/>",
		typePolicyControl, filter.mSeatType);


	// 识别类型策略，目前只支持自动识别
	//char methodPolicy[policyLength] = { 0 };
	char *methodPolicy = new char[policyLength];
	memset(methodPolicy, 0, sizeof(methodPolicy));
	int methodPolicyControl = 1;
	//if ((strlen(filter.mSeatType) == 0))
	//{
	//	methodPolicyControl = 0;
	//}
	sprintf_s(methodPolicy, policyLength, "<methodPolicy control=\"%d\" method=\"auto\"/>", methodPolicyControl);

	//timePolicy, barcodePolicy, linePolicy, typePolicy, methodPolicy
	WriteInfo("timePolicy = %s", timePolicy);
	WriteInfo("barcodePolicy = %s", barcodePolicy);
	WriteInfo("linePolicy = %s", linePolicy);
	WriteInfo("typePolicy = %s", typePolicy);
	WriteInfo("methodPolicy = %s", methodPolicy);

	//sprintf_s(tmpChar, sizeof(char) * length, "%s\n", linePolicy);


	sprintf_s(tmpChar, sizeof(char) * length, "%s\n%s\n%s\n%s\n%s\n", timePolicy, barcodePolicy, linePolicy, typePolicy, methodPolicy);

	if (timePolicy != nullptr)
	{
		delete[]timePolicy;
		timePolicy = nullptr;
	}
	if (typePolicy != nullptr)
	{
		delete[]typePolicy;
		typePolicy = nullptr;
	}
	if (barcodePolicy != nullptr)
	{
		delete[]barcodePolicy;
		barcodePolicy = nullptr;
	}
	if (linePolicy != nullptr)
	{
		delete[]linePolicy;
		linePolicy = nullptr;
	}
	if (methodPolicy != nullptr)
	{
		delete[]methodPolicy;
		methodPolicy = nullptr;
	}

	//sprintf_s(tmpChar, "%s\n%s\n%s\n%s\n%s\n", timePolicy, barcodePolicy, linePolicy, typePolicy, methodPolicy);

	//"%s%s%s%s%s";
	//TRACE1("strlen(tmpChar) = %d\n", strlen(tmpChar));
	//tmpTime, tmpChar
	//
	//std::unique_ptr<char> t(new char[10]);

	/*TRACE1("ipStr = %s\n", std::unique_ptr<wchar_t>(utils::CharToWChar(ipStr)));
	TRACE1("USname = %s\n", utils::StrToWStr(m_pParamManager->GetLoginUserName()).c_str());
	TRACE1("passwd = %s\n", utils::StrToWStr(m_pParamManager->GetLoginPasswd()).c_str());
	TRACE1("tmpTime = %s\n", std::unique_ptr<wchar_t>(utils::CharToWChar(tmpTime)));
	TRACE1("tmpChar = %s\n", std::unique_ptr<wchar_t>(utils::CharToWChar(tmpChar)));*/


	sprintf_s(tmpXml, sizeof(char) * length, queryXml, ipStr, m_pParamManager->GetLoginUserName().c_str(), m_pParamManager->GetLoginPasswd().c_str(), tmpTime, tmpChar);


	TRACE1("tmpXml = %s\n", std::unique_ptr<wchar_t>(utils::CharToWChar(tmpXml)));


	/*
	将过滤xml发送到服务器端
	*/
	delete[]tmpChar;
	
	const size_t resultLen = 1024 * 1024 * 3;
	tmpChar = new char[resultLen];
	memset(tmpChar, 0, sizeof(char) * resultLen);

	//utils::delBlankSpace<char>(tmpXml, strlen(tmpXml), ' ');
	//utils::delBlankSpace<char>(tmpXml, strlen(tmpXml), '\t');
	//utils::delBlankSpace<char>(tmpXml, strlen(tmpXml), '\n');

	std::vector<RecogResultW>* selectedRecog = QueryInfo(tmpXml, strlen(tmpXml), tmpChar, resultLen);
	if ((selectedRecog != nullptr) && ( selectedRecog->size() > 0))
	{
		/*if (true == ParseQueryResult(tmpChar, strlen(tmpChar)))
		{

		}*/
		//m_pMainWnd
		//m_pMainWnd->ChildWindowFromPoint()
		//AfxGetMainWnd()->FindWindowW();
		//AfxGetMainWnd()->FromHandle();
		//CWnd *pWnd = AfxGetMainWnd()->GetActiveWindow();
		//wchar_t tmpBuffer[200];
		//pWnd->GetWindowTextW(tmpBuffer, 200);
		//TRACE1("window text = %s\n", tmpBuffer);
		//CRuntimeClass *ptr = RUNTIME_CLASS(CQueryFrame);
		//pWnd = AfxGetMainWnd()->GetNextWindow();
		m_pQueryMDIFrame->InsertItem(selectedRecog);

		//pWnd->GetWindowTextW(tmpBuffer, 200);
		//TRACE1("window text = %s\n", tmpBuffer);
		//CWnd *pWnd = AfxGetMainWnd()->FindWindowW((LPCTSTR)ptr->m_lpszClassName, NULL);
		//GetWindow(GW_CHILD, );
	}

	

	delete []tmpChar;
	tmpChar = nullptr;

	delete[]tmpXml;
	tmpXml = nullptr;

	delete selectedRecog;
	selectedRecog = nullptr;
}


void CQuery_ModuleApp::OnButtonBarcode()
{
	// TODO: 在此添加命令处理程序代码
	/*
	条形码对照表
	*/



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