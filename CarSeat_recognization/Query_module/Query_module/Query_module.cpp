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
	����ѯ�����Լ��û��������뷢�͵���������
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


	// ʱ��ѡ�����
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


	//����ѡ�����
	char linePolicy[policyLength] = { 0 };
	memset(linePolicy, 0, sizeof(linePolicy));
	int linePolicyControl = 1;
	if ((strlen(filter.mLineBegin) == 0) && (strlen(filter.mLineEnd) == 0))
	{
		linePolicyControl = 0;
	}
	sprintf_s(linePolicy, "<linePolicy control=\"%d\" startLine=\"%s\" endLine=\"%s\"/>",
		linePolicyControl, filter.mLineBegin, filter.mLineEnd);



	// ��ʽ�����������
	char barcodePolicy[policyLength] = { 0 };
	memset(barcodePolicy, 0, sizeof(barcodePolicy));
	int barcodePolicyControl = 1;
	if ((strlen(filter.mBarcodeBegin) == 0) && (strlen(filter.mBarcodeEnd) == 0))
	{
		barcodePolicyControl = 0;
	}
	sprintf_s(barcodePolicy, "<barcodePolicy control=\"%d\" startBarcode=\"%s\" endBarcode=\"%s\"/>",
		barcodePolicyControl, filter.mBarcodeBegin, filter.mBarcodeEnd);


	// �������Ͳ���
	char typePolicy[policyLength] = { 0 };
	memset(typePolicy, 0, sizeof(typePolicy));
	int typePolicyControl = 1;
	if ((strlen(filter.mSeatType) == 0))
	{
		typePolicyControl = 0;
	}
	sprintf_s(typePolicy, "<typePolicy control=\"%d\" type=\"%s\"/>",
		typePolicyControl, filter.mSeatType);


	// ʶ�����Ͳ��ԣ�Ŀǰֻ֧���Զ�ʶ��
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
	������xml���͵���������
	*/



	delete []tmpChar;
	tmpChar = nullptr;

	delete[]tmpXml;
	tmpXml = nullptr;
}


void CQuery_ModuleApp::OnButtonBarcode()
{
	// TODO: �ڴ���������������
	/*
	��������ձ�
	*/

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

	/*
	�ж��û����Ƿ���ȷ��
	
	*/
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
