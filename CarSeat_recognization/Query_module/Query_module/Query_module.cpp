// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// Query_Module.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Query_Module.h"
#include "MainFrm.h"
#include "LoginDlg.h"
#include "./common/utils.h"
#include "ChildFrm.h"


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


// CQuery_ModuleApp ����

CQuery_ModuleApp::CQuery_ModuleApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Query_Module.AppID.NoVersion"));

	m_pLog = CLog::GetInstance();
	m_pParamManager = CParamManager::GetInstance();


	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
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

// Ψһ��һ�� CQuery_ModuleApp ����

CQuery_ModuleApp theApp;


// CQuery_ModuleApp ��ʼ��

BOOL CQuery_ModuleApp::InitInstance()
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

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ������ MDI ��ܴ���
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// ��ͼ���ع��� MDI �˵��Ϳ�ݼ���
	//TODO: ��Ӹ��ӳ�Ա�����������ض�
	//	Ӧ�ó��������Ҫ�ĸ��Ӳ˵����͵ĵ���
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_Query_ModuleTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_Query_ModuleTYPE));


	



	// �������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;
}

int CQuery_ModuleApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
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

// CQuery_ModuleApp ��Ϣ�������

void CQuery_ModuleApp::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	pFrame->LockWindowUpdate();
	// �����µ� MDI �Ӵ���
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_Query_ModuleTYPE, m_hMDIMenu, m_hMDIAccel);
	pFrame->UnlockWindowUpdate();
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CQuery_ModuleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CQuery_ModuleApp �Զ������/���淽��

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


// CQuery_ModuleApp ��Ϣ�������
void CQuery_ModuleApp::OnButtonChoose()
{
	// TODO: �ڴ���������������
	int ret = mConditionDlg.DoModal();

	if (ret == IDCANCEL)
	{
		return;
	}
	CConditionFilter filter = mConditionDlg.GetFilterCondition();
	TRACE1("ret = %d\n", ret);



}


void CQuery_ModuleApp::OnButtonBarcode()
{
	// TODO: �ڴ���������������


}


void CQuery_ModuleApp::OnButtonLogin()
{
	// TODO: �ڴ���������������
	CLoginDlg dlg;
	int ret = dlg.DoModal();
	if (ret == IDCANCEL)
	{
		return;
	}

	int length = 0;
	if (dlg.GetLoginUserName(mUsrName, length) == false)
	{
		AfxMessageBox(L"��ȡ�û���ʧ��");
		return;
	}
	if (dlg.GetLoginPasswd(mPasswd, length) == false)
	{
		AfxMessageBox(L"��ȡ����ʧ��");
		return;
	}

	bool tmpSaveFlag = dlg.GetAutoSaveFlag();
	m_pParamManager->SetLoginUserName(std::wstring(mUsrName));
	m_pParamManager->SetLoginPasswd(std::wstring(mPasswd));
}


void CQuery_ModuleApp::OnUpdateButtonChoose(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	std::wstring tmpUserName = m_pParamManager->GetLoginUserName();
	if ((tmpUserName.size() == 0) ||(wcslen(mUsrName) == 0))
	{
		pCmdUI->Enable(FALSE);
	}
}


void CQuery_ModuleApp::OnUpdateButtonBarcode(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	std::wstring tmpUserName = m_pParamManager->GetLoginUserName();
	if ((tmpUserName.size() == 0) || (wcslen(mUsrName) == 0))
	{
		pCmdUI->Enable(FALSE);
	}
}


void CQuery_ModuleApp::OnUpdateButtonLogin(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

}
