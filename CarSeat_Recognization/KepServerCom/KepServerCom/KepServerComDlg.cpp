
// KepServerComDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KepServerCom.h"
#include "KepServerComDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
bool pServerConnected = false;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
typedef struct OpcData {
	CString ServerName;
	CString ReadTag;
	CString WriteTag;
}OpcData_t;

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


// CKepServerComDlg �Ի���



CKepServerComDlg::CKepServerComDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KEPSERVERCOM_DIALOG, pParent)
	, m_ServerName(_T("Kepware.KEPServerEX.V5"))
	, m_ReadFlagName(_T("Channel1.Device1.Tag1"))
	, m_ConncetResult(_T(""))
	, m_WriteFlagName(_T("Channel1.Device1.Tag2"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKepServerComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONNECT, B_ServerConnect);
	DDX_Text(pDX, IDC_KEPSERVERNAME, m_ServerName);
	DDX_Text(pDX, IDC_READFLAG, m_ReadFlagName);
	DDX_Text(pDX, IDC_RESULT, m_ConncetResult);
	DDX_Text(pDX, IDC_WRITEFLAG, m_WriteFlagName);
}

BEGIN_MESSAGE_MAP(CKepServerComDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CKepServerComDlg::OnBnClickedConnect)
//	ON_WM_TIMER()
//ON_WM_TIMER()
END_MESSAGE_MAP()


// CKepServerComDlg ��Ϣ�������

BOOL CKepServerComDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CKepServerComDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CKepServerComDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CKepServerComDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT MyThreadProc(LPVOID pParam)
{
	OpcData_t* pOD = (OpcData_t*)pParam;
	// server name
	CString Cstr = pOD->ServerName;
	//wchar_t* opcServer = nullptr;
	BSTR opcServer = nullptr;
	//opcServer = new wchar_t[20];
	opcServer = Cstr.AllocSysString();
	COPC Opc;
	Opc.AddServerName(opcServer);

	if (TRUE == Opc.ConnectServer())
	{
		Opc.bOPCConnect = true;
		pServerConnected = true;
		CString read_flag = pOD->ReadTag;
		CString write_flag = pOD->WriteTag;
		COleVariant *ReadItem = new COleVariant[1];
		COleVariant *WriteItem = new COleVariant[1];
		ReadItem[0] = COleVariant(read_flag);
		WriteItem[0] = COleVariant(write_flag);
		Opc.InitialOPC(opcServer, 1, 1, ReadItem, WriteItem);
		Opc.PreRead();
		Opc.PreWrite();
		CDataCtrl CD;
		CD.DataControl(&Opc);
		delete[] ReadItem;
		delete[] WriteItem;
	}

	return 0;   // thread completed successfully  
}

void CKepServerComDlg::DoEvent()
{
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  //ȡ��Ϣ������Ӧ�ó������Ϣ���У�PM_REMOVEȡ��֮�����Ϣ�������Ƴ�  
	{
		//����Ϣ  
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

void CKepServerComDlg::OnBnClickedConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	OpcData_t *OD = new OpcData_t;
	OD->ServerName = m_ServerName;
	OD->ReadTag = m_ReadFlagName;
	OD->WriteTag = m_WriteFlagName;
	CWinThread* pThread = AfxBeginThread(MyThreadProc, (LPVOID)OD);
	Sleep(200);
	if (pServerConnected)
	{
		m_ConncetResult = L"Success";
		UpdateData(false);
		DWORD dwRet;
		DoEvent();
		do
		{
			dwRet = ::MsgWaitForMultipleObjects(1, &pThread->m_hThread, FALSE, INFINITE, QS_ALLINPUT);
			if (dwRet != WAIT_OBJECT_0)
			{
				DoEvent();
			}
		} while ((dwRet != WAIT_OBJECT_0) && (dwRet != WAIT_FAILED));
		delete OD;
	}
	else 
	{
		m_ConncetResult = L"Failed";
		UpdateData(false);
	}

}
