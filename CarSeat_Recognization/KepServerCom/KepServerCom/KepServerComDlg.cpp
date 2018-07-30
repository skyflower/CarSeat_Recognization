
// KepServerComDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KepServerCom.h"
#include "KepServerComDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CKepServerComDlg 对话框



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


// CKepServerComDlg 消息处理程序

BOOL CKepServerComDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKepServerComDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO: 在此添加控件通知处理程序代码
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
		/*非多线程*/
		//CDataCtrl DataCtrl;
		//DataCtrl.DataControl(&Opc);
		/*多线程*/
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
