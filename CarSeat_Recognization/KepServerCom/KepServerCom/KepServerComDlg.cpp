
// KepServerComDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KepServerCom.h"
#include "KepServerComDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	//CKepServerComDlg* pObject = (CKepServerComDlg*)pParam;
	COPC *dOpc = (COPC*)pParam;
	CDataCtrl DataCtrl;
	//CString str;
	//str.Format(_T("%d"), pObject->Opc.bOPCConnect);
	//AfxMessageBox(str);
	//DataCtrl.DataControl(&(pObject->Opc));
	DataCtrl.DataControl(dOpc);
	//delete[] pObject->ReadItem;
	//delete[] pObject->WriteItem;

	return 0;   // thread completed successfully  
}


void CKepServerComDlg::OnBnClickedConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	// server name
	CString Cstr = m_ServerName;
	//wchar_t* opcServer = nullptr;
	BSTR opcServer = nullptr;
	//opcServer = new wchar_t[20];
	opcServer = Cstr.AllocSysString();
	Opc.AddServerName(opcServer);

	if (TRUE == Opc.ConnectServer())
	{
		Opc.bOPCConnect = true;
		CString read_flag = m_ReadFlagName;
		CString write_flag = m_WriteFlagName;
		ReadItem = new COleVariant[1];
		WriteItem = new COleVariant[1];
		ReadItem[0] = COleVariant(read_flag);
		WriteItem[0] = COleVariant(write_flag);
		Opc.InitialOPC(opcServer, 1, 1, ReadItem, WriteItem);
		Opc.PreRead();
		Opc.PreWrite();
		m_ConncetResult = L"Success";
		UpdateData(false);
		/*�Ƕ��߳�*/
		//CDataCtrl DataCtrl;
		//DataCtrl.DataControl(&Opc);
		/*���߳�*/
		COPC* pOpc = new COPC;
		pOpc = &Opc;
		AfxBeginThread(MyThreadProc, (LPVOID)pOpc);
		//delete[] ReadItem;
		//delete[] WriteItem;
	}
	else 
	{
		m_ConncetResult = L"Failed";
		UpdateData(false);
	}
}
