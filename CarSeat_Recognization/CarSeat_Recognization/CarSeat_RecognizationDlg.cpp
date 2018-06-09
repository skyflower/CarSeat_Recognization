// CarSeat_RecognizationDlg.cpp :
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CarSeat_RecognizationDlg.h"
#include "afxdialogex.h"
#include "InputDlg.h"
#include "./common/utils.h"
// add start by xiexinpeng
#include <string>
#include "kepserver/OPC.h"
// add end by xiexinpeng
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

//
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


// CCarSeat_RecognizationDlg

BEGIN_DHTML_EVENT_MAP(CCarSeat_RecognizationDlg)

END_DHTML_EVENT_MAP()


CCarSeat_RecognizationDlg::CCarSeat_RecognizationDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(IDD_CARSEAT_RECOGNIZATION_DIALOG, IDR_HTML_CARSEAT_RECOGNIZATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSuccessCount = 0;
	m_nFailCount = 0;
	m_pFont.CreatePointFont(24, L"楷体");

	m_pParamManager = nullptr;
	m_pNetworkTask = nullptr;
	m_pClassify = nullptr;
}

void CCarSeat_RecognizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_PATTERN, m_ImagePattern);
	DDX_Control(pDX, IDC_IMAGE_REC, m_ImageRec);
	DDX_Control(pDX, IDC_BARCODE, m_barCode);
	DDX_Control(pDX, IDC_REG_RATIO, m_RegRatio);
	DDX_Control(pDX, IDC_IMAGE_PATTERN, m_ImagePattern);
}


BEGIN_MESSAGE_MAP(CCarSeat_RecognizationDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_USRINPUT, &CCarSeat_RecognizationDlg::OnUsrinput)
END_MESSAGE_MAP()


// CCarSeat_RecognizationDlg
BOOL CCarSeat_RecognizationDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// IDM_ABOUTBOX
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

	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);
	this->OnSetFont(&m_pFont);


    // kepserver add start by xiexinpeng	
    // kepserver
    COPC Opc;
	// server name
	CString Cstr = L"Kepware.KEPServerEX.V5";
	
	BSTR opcServer = nullptr;
	
	opcServer = Cstr.AllocSysString();
	
	Opc.AddServerName(opcServer);
	
	if(TRUE == Opc.ConnectServer())
	{
		Opc.bOPCConnect=true;
		CString flag = L"Channel1.Device1.Tag2";
		Opc.InitialOPC(opcServer, 1, &COleVariant(flag));
		Opc.PreWrite();	
		//wirte data demo
		float test = 500;
		Opc.WriteData(1, 0, &COleVariant(test));
	}
	if (opcServer != nullptr)
	{
		::SysFreeString(opcServer);
	}
	// add end by xiexinpeng

	CFont* curFont = this->GetFont();


	char tmpStr[] = "K215-黑色-菱形纹理";
	WCHAR result[200];

	m_barCode.SetWindowTextW(L"K215-黑色-菱形纹理");
		
	double ratio = 1.0;
	if (m_nFailCount + m_nSuccessCount != 0)
	{
		ratio = double(m_nSuccessCount) / double(m_nFailCount + m_nSuccessCount);
	}
	memset(result, 0, sizeof(WCHAR) * 200);
	wsprintfW(result, L"Success:%d\nFailed:%d\nSuccess Rate:%f%", m_nSuccessCount, m_nFailCount, ratio);

	m_RegRatio.SetWindowTextW(result);

	//m_pUIThread = std::thread(run, this);

	return TRUE;  //
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


void CCarSeat_RecognizationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); //

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDHtmlDialog::OnPaint();
	}
	
}

HCURSOR CCarSeat_RecognizationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCarSeat_RecognizationDlg::run()
{
	std::wstring preImagePath;
	m_pNetworkTask = CNetworkTask::GetInstance();
	m_pParamManager = CParamManager::GetInstance();

	while (1)
	{
		std::wstring currentImage = m_pNetworkTask->GetCurrentImagePath();
		break;
	}
}

void CCarSeat_RecognizationDlg::SetImageClassify(CImageClassify * pClassify)
{
	m_pClassify = pClassify;
}

void CCarSeat_RecognizationDlg::OnUsrinput()
{
	// TODO: 在此添加命令处理程序代码
	CInputDlg dlg;
	INT_PTR msg = dlg.DoModal();
	if (msg == IDOK)
	{

	}
	else if(msg == IDCANCEL)
	{

	}

}
