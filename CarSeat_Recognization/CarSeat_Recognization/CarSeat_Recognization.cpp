
// CarSeat_Recognization.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CarSeat_RecognizationDlg.h"
#include <locale>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarSeat_RecognizationApp

BEGIN_MESSAGE_MAP(CCarSeat_RecognizationApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCarSeat_RecognizationApp ����

CCarSeat_RecognizationApp::CCarSeat_RecognizationApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_pParamManager = nullptr;
	m_pLog = nullptr;
	m_pNetworkTask = nullptr;
	m_pClassify = nullptr;
	_tsetlocale(LC_ALL, _T("chs"));
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCarSeat_RecognizationApp ����

CCarSeat_RecognizationApp theApp;


// CCarSeat_RecognizationApp ��ʼ��

BOOL CCarSeat_RecognizationApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	m_pParamManager = CParamManager::GetInstance();
	m_pLog = CLog::GetInstance();
	m_pNetworkTask = CNetworkTask::GetInstance();
	if (m_pNetworkTask != nullptr)
	{
		m_NetworkThread = std::thread(std::bind(&CNetworkTask::run, m_pNetworkTask));
	}
	m_pClassify = new CImageClassify(m_pParamManager->GetGraphFile(), m_pParamManager->GetLabelFile());
	m_pClassify->initPython("label_image_command_line", "seatClassify");
	m_pNetworkTask->SetImageClassify(m_pClassify);

	if (m_pClassify != nullptr)
	{
		m_pClassifyThread = std::thread(std::bind(&CImageClassify::run, m_pClassify));
	}

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CCarSeat_RecognizationDlg dlg;
	m_pMainWnd = &dlg;
	dlg.SetImageClassify(m_pClassify);

	m_UIThread = std::thread(&CCarSeat_RecognizationDlg::run, &dlg);
	

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	
	if (m_UIThread.joinable())
	{
		dlg.terminate();
		m_UIThread.join();
	}
	if (m_pParamManager != nullptr)
	{
		delete m_pParamManager;
		m_pParamManager = nullptr;
	}
	if (m_pClassifyThread.joinable())
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
	}
	
	if (m_pNetworkTask != nullptr)
	{
		delete m_pNetworkTask;
		m_pNetworkTask = nullptr;
	}
	if (m_pClassify != nullptr)
	{
		delete m_pClassify;
		m_pClassify = nullptr;
	}
	if (m_pLog != nullptr)
	{
		delete m_pLog;
		m_pLog = nullptr;
	}
	return FALSE;
}
