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
	CConditionFilter filter = mConditionDlg.GetFilterCondition();
	TRACE1("ret = %d\n", ret);
	WriteInfo("date = [begin = %s],[end = %s]", filter.mDateBeign, filter.mDateEnd);
	WriteInfo("time = [begin = %s],[end = %s]", filter.mTimeBegin, filter.mTimeEnd);
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
	char queryXml[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>	\
		<search>													\
		<ip=\"%s\" usr=\"%s\" passwd=\"%s\" time=\"%s\"/>			\
		%s													\
		</search>";


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

	//sprintf_s(tmpTime, "%04d%02d%02d-%02d%02d%02d", )

	sprintf_s(tmpTime, "%04d%02d%02d-%02d%02d%02d", \
		nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, \
		nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);

	char ipStr[20] = { 0 };
	memset(ipStr, 0, sizeof(ipStr));
	unsigned int ip = m_pParamManager->GetLocalIP();

	sprintf_s(ipStr, "%d.%d.%d.%d", (ip >> 24), ((ip >> 16) & 0xFF), ((ip >> 8) & 0xFF), (ip & 0xFF));


	// 时间选择策略
	const int policyLength = 200;
	char timePolicy[policyLength] = { 0 };
	memset(timePolicy, 0, sizeof(timePolicy));
	int timePolicyControl = 1;
	if ((strlen(filter.mTimeBegin) == 0) && (strlen(filter.mTimeEnd) == 0))
	{
		timePolicyControl = 0;
	}

	sprintf_s(timePolicy, "<timePolicy control = \"%d\" startTime=\"%s\" endTime=\"%s\"/>",
		timePolicyControl, filter.mTimeBegin, filter.mTimeEnd);


	//产线选择策略
	char linePolicy[policyLength] = { 0 };
	memset(linePolicy, 0, sizeof(linePolicy));
	int linePolicyControl = 1;
	if ((strlen(filter.mLineBegin) == 0) && (strlen(filter.mLineEnd) == 0))
	{
		linePolicyControl = 0;
	}
	sprintf_s(linePolicy, "<linePolicy control=\"%d\" startLine=\"%s\" endLine=\"%s\"/>",
		linePolicyControl, filter.mLineBegin, filter.mLineEnd);



	// 格式化条形码策略
	char barcodePolicy[policyLength] = { 0 };
	memset(barcodePolicy, 0, sizeof(barcodePolicy));
	int barcodePolicyControl = 1;
	if ((strlen(filter.mBarcodeBegin) == 0) && (strlen(filter.mBarcodeEnd) == 0))
	{
		barcodePolicyControl = 0;
	}
	sprintf_s(barcodePolicy, "<barcodePolicy control=\"%d\" startBarcode=\"%s\" endBarcode=\"%s\"/>",
		barcodePolicyControl, filter.mBarcodeBegin, filter.mBarcodeEnd);


	// 座椅类型策略
	char typePolicy[policyLength] = { 0 };
	memset(typePolicy, 0, sizeof(typePolicy));
	int typePolicyControl = 1;
	if ((strlen(filter.mSeatType) == 0))
	{
		typePolicyControl = 0;
	}
	sprintf_s(typePolicy, "<typePolicy control=\"%d\" type=\"%s\"/>",
		typePolicyControl, filter.mSeatType);


	// 识别类型策略，目前只支持自动识别
	char methodPolicy[policyLength] = { 0 };
	memset(methodPolicy, 0, sizeof(methodPolicy));
	int methodPolicyControl = 1;
	//if ((strlen(filter.mSeatType) == 0))
	//{
	//	methodPolicyControl = 0;
	//}
	sprintf_s(methodPolicy, "<methodPolicy control=\"%d\" method=\"auto\"/>", methodPolicyControl);


	sprintf_s(tmpChar, sizeof(char) * length, "%s\n%s\n%s\n%s\n%s\n", timePolicy, barcodePolicy, linePolicy, typePolicy, methodPolicy);

	//sprintf_s(tmpChar, "%s\n%s\n%s\n%s\n%s\n", timePolicy, barcodePolicy, linePolicy, typePolicy, methodPolicy);

	sprintf_s(tmpXml, sizeof(char) * length, queryXml, ipStr, m_pParamManager->GetLoginUserName(), m_pParamManager->GetLoginPasswd(), tmpTime, tmpChar);



	/*
	将过滤xml发送到服务器端
	*/



	delete []tmpChar;
	tmpChar = nullptr;

	delete[]tmpXml;
	tmpXml = nullptr;
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
