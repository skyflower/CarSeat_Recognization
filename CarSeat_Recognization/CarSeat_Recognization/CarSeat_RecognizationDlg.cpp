
// CarSeat_RecognizationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CarSeat_RecognizationDlg.h"
#include "afxdialogex.h"
// add start by xiexinpeng
#include <string>
#include "stdlib.h"
#include "windows.h"
#include <atlstr.h>
#include "OPC.h"
using namespace std;
// add end by xiexinpeng
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


// CCarSeat_RecognizationDlg 对话框

BEGIN_DHTML_EVENT_MAP(CCarSeat_RecognizationDlg)
	
END_DHTML_EVENT_MAP()


CCarSeat_RecognizationDlg::CCarSeat_RecognizationDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(IDD_CARSEAT_RECOGNIZATION_DIALOG, IDR_HTML_CARSEAT_RECOGNIZATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSuccessCount = 0;
	m_nFailCount = 0;
}

void CCarSeat_RecognizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_PATTERN, m_ImagePattern);
	DDX_Control(pDX, IDC_IMAGE_REC, m_ImageRec);
	DDX_Control(pDX, IDC_BARCODE, m_barCode);
	DDX_Control(pDX, IDC_REG_RATIO, m_RegRatio);
}

BEGIN_MESSAGE_MAP(CCarSeat_RecognizationDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CCarSeat_RecognizationDlg 消息处理程序

BOOL CCarSeat_RecognizationDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

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

    // 连接kepserver add start by xiexinpeng	
    // kepserver 连接类
    COPC Opc;
	COleVariant* WriteItem = NULL;
	// 需要填具体PLC标志位
	string flag = "PLC_flag";
	string server="OPCServer.WinCC";
	wchar_t* opcServer;
	int size = MultiByteToWideChar(CP_OEMCP, 0, server.c_str(), strlen(server.c_str()) + 1, NULL, 0);
	opcServer=new wchar_t[size];
	MultiByteToWideChar(CP_OEMCP, 0, server.c_str(), strlen(server.c_str()) + 1, opcServer, size);	
	Opc.AddServerName(opcServer);
	
	if( TRUE == Opc.ConnectServer() )
	{
		Opc.bOPCConnect=true;
		int size = MultiByteToWideChar(CP_OEMCP, 0, flag.c_str(), strlen(flag.c_str()) + 1, NULL, 0);
		wchar_t* opcflag = new wchar_t[size];
		MultiByteToWideChar(CP_OEMCP, 0, flag.c_str(), strlen(flag.c_str()) + 1, opcflag, size);
		WriteItem[0] = SysAllocString(opcflag);
		Opc.InitialOPC(opcServer , 1 , WriteItem);
		Opc.PreWrite();	
	}
	// add end by xiexinpeng
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCarSeat_RecognizationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCarSeat_RecognizationDlg::OnPaint()
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
		CDHtmlDialog::OnPaint();
	}
	m_barCode.SetWindowTextW(L"K215-黑色-菱形纹理");

	double ratio = 1.0;
	if (m_nFailCount + m_nSuccessCount != 0)
	{
		ratio = double(m_nSuccessCount) / double(m_nFailCount + m_nSuccessCount);
	}
	WCHAR result[200];
	memset(result, 0, sizeof(WCHAR) * 200);
	wprintf_s(result, L"Success:%d\nFailed:%d\nSuccess Rate:%f%%", m_nSuccessCount, m_nFailCount, ratio);

	m_RegRatio.SetWindowTextW(result);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCarSeat_RecognizationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
