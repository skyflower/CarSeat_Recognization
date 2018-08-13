
// CarSeat_Recognization.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CarSeat_RecognizationDlg.h"
#include <locale>
#include "LoginDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarSeat_RecognizationApp

BEGIN_MESSAGE_MAP(CCarSeat_RecognizationApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCarSeat_RecognizationApp 构造

CCarSeat_RecognizationApp::CCarSeat_RecognizationApp():
	m_pCameraManager(nullptr)
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_pParamManager = nullptr;
	m_pLog = nullptr;
	//m_pNetworkTask = nullptr;
	m_pClassify = nullptr;
	m_pLabelManager = nullptr;
	_tsetlocale(LC_ALL, _T("chs"));
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCarSeat_RecognizationApp 对象

CCarSeat_RecognizationApp theApp;


// CCarSeat_RecognizationApp 初始化

BOOL CCarSeat_RecognizationApp::InitInstance()
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

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	//初始化参数模块，标签模块，网络模块，识别模块
	initSystem();

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	
	CCarSeat_RecognizationDlg dlg;
	
	INT_PTR nResponse = IDOK;
	if (LoginSystem() == true)
	{
		
		m_pMainWnd = &dlg;

		dlg.SetImageClassify(m_pClassify);
		dlg.SetLabelManager(m_pLabelManager);

		m_UIThread = std::thread(&CCarSeat_RecognizationDlg::run, &dlg);

		nResponse = dlg.DoModal();
	}
	
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	

	/*if (m_UIThread.joinable())
	{
		dlg.terminate();
		m_UIThread.join();
	}*/

	DeInitSystem();
	
	
	return FALSE;
}

bool CCarSeat_RecognizationApp::LoginSystem()
{
	CLoginDlg loginDlg;
	loginDlg.SetLabelManager(m_pLabelManager);

	INT_PTR nResponse = loginDlg.DoModal();
	if (nResponse == IDOK)
	{
		return true;
	}
	return false;
}

void CCarSeat_RecognizationApp::initSystem()
{
	m_pParamManager = CParamManager::GetInstance();
	m_pLog = CLog::GetInstance();
	//m_pNetworkTask = CNetworkTask::GetInstance();
	//if (m_pNetworkTask != nullptr)
	//{
	//	m_NetworkThread = std::thread(std::bind(&CNetworkTask::run, m_pNetworkTask));
	//}
	m_pClassify = new CImageClassify(m_pParamManager->GetGraphFile(), m_pParamManager->GetLabelFile());
	m_pClassify->initPython("label_image_command_line_two", "seatClassify");
	//m_pNetworkTask->SetImageClassify(m_pClassify);

	if (m_pClassify != nullptr)
	{
		//m_pClassifyThread = std::thread(std::bind(&CImageClassify::run, m_pClassify));
	}

	m_pLabelManager = new CLabelManager();
	m_pCameraManager = CCameraManager::GetInstance();

}

void CCarSeat_RecognizationApp::DeInitSystem()
{
	/*if (m_pClassifyThread.joinable())
	{
		m_pClassify->terminate();
		m_pClassifyThread.join();
	}

	if (m_NetworkThread.joinable())
	{
		CNetworkTask::message msg;
		m_pNetworkTask->SetThreadStatus(false);
		msg.serverIp = -1;
		msg.serverPort = -1;
		m_pNetworkTask->SendMessageTo(&msg);
		m_NetworkThread.join();
	}*/

	/*if (m_pNetworkTask != nullptr)
	{
		delete m_pNetworkTask;
		m_pNetworkTask = nullptr;
	}*/
	if (m_UIThread.joinable())
	{
		m_UIThread.join();
	}

	if (m_pClassify != nullptr)
	{
		delete m_pClassify;
		m_pClassify = nullptr;
	}
	if (m_pLabelManager != nullptr)
	{
		delete m_pLabelManager;
		m_pLabelManager = nullptr;
	}
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
	if (m_pCameraManager != nullptr)
	{
		delete m_pCameraManager;
		m_pCameraManager = nullptr;
	}
	
}
