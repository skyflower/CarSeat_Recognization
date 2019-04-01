
// generateRegisterCodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "generateRegisterCode.h"
#include "generateRegisterCodeDlg.h"
#include "afxdialogex.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#pragma comment(lib, "SoftwareRegisterDll.lib")


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


// CgenerateRegisterCodeDlg �Ի���



CgenerateRegisterCodeDlg::CgenerateRegisterCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GENERATEREGISTERCODE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgenerateRegisterCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CgenerateRegisterCodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DECIPHER, &CgenerateRegisterCodeDlg::OnBnClickedButtonDecipher)
END_MESSAGE_MAP()


// CgenerateRegisterCodeDlg ��Ϣ�������

BOOL CgenerateRegisterCodeDlg::OnInitDialog()
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
	//CBaseRegister *pRegister = createRegisterManager();
	//int ret = pRegister->CheckRegisterCode(nullptr, 0);
	//TRACE1("ret = %d\n", ret);

	char *tmpCode = getDeviceMachineCode();
	
	wchar_t *pSzWCode = utils::CharToWChar(tmpCode);
	if (pSzWCode != nullptr)
	{
		GetDlgItem(IDC_EDIT_MACHINE_CODE)->SetWindowTextW(pSzWCode);
		delete pSzWCode;
		pSzWCode = nullptr;
	}

	std::string tmpRandomStr = utils::randomStr(16);
	std::wstring tmpWRandomStr = utils::StrToWStr(tmpRandomStr);
	GetDlgItem(IDC_EDIT_RANDOM_KEYS)->SetWindowTextW(tmpWRandomStr.c_str());

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CgenerateRegisterCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CgenerateRegisterCodeDlg::OnPaint()
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
HCURSOR CgenerateRegisterCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



registerNode CgenerateRegisterCodeDlg::getRegisterInfo()
{
	//char *szCpuID = getCPU_ID();
	//char *szDiskID = getDiskID();
	registerNode node;
	memset(&node, 0, sizeof(node));
	node.m_nVersion = 1;
	//sprintf(node.m_szMachineCode, "%s%s", szCpuID, szDiskID);
	SYSTEMTIME curTime;
	GetSystemTime(&curTime);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_BEGIN_TIME))->GetTime(&curTime);
	
	int durationDay = GetDlgItemInt(IDC_EDIT_TIME_DURATION, NULL);
	int registerLimit = GetDlgItemInt(IDC_EDIT_REGISTER_LIMIT, NULL);
	wchar_t randomKey[100];
	memset(randomKey, 0, sizeof(randomKey));
	((CEdit*)GetDlgItem(IDC_EDIT_RANDOM_KEYS))->GetWindowText(randomKey, sizeof(randomKey)/sizeof(randomKey[0]));

	if (wcslen(randomKey) != 0)
	{
		char *tmp = utils::WCharToChar(randomKey);
		if (tmp != nullptr)
		{
			memcpy(node.m_szRandomKey, tmp, strlen(tmp));
		}
		delete[]tmp;
		tmp = nullptr;
	}

	wchar_t machineCode[1024];
	GetDlgItemText(IDC_EDIT_MACHINE_CODE, machineCode, sizeof(machineCode) / sizeof(machineCode[0]));

	node.m_nRegisterLimit = registerLimit;

	if (node.m_nRegisterLimit > 1)
	{
		node.m_type = registerType::MULTI_DEVICE;
	}
	else
	{
		node.m_type = registerType::SINGLE_DEVICE;
	}
	
	if (wcslen(machineCode) != 0)
	{
		char *tmp = utils::WCharToChar(machineCode);
		if (tmp != nullptr)
		{
			memcpy(node.m_szMachineCode, tmp, strlen(tmp));
		}
		delete[]tmp;
		tmp = nullptr;
	}
	time_t tmpSecTime;
	struct tm tmpTime;
	tmpTime.tm_year = curTime.wYear - 1900;
	tmpTime.tm_mon = curTime.wMonth - 1;
	tmpTime.tm_mday = curTime.wDay;
	tmpTime.tm_hour = curTime.wHour;
	tmpTime.tm_sec = curTime.wSecond;
	tmpTime.tm_min = curTime.wMinute;
	tmpTime.tm_wday = curTime.wDayOfWeek;
	
	tmpSecTime = mktime(&tmpTime);
	
	node.m_nBeginTime = tmpSecTime;
	node.m_szEndTime = durationDay * 86400 + tmpSecTime;

	return node;
}

void CgenerateRegisterCodeDlg::OnBnClickedButtonDecipher()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	registerNode node = getRegisterInfo();
	
	CBaseRegister *pRegister = createRegisterManager();
	if (pRegister != nullptr)
	{
		int len = sizeof(node);
		char *code = pRegister->GenerateRegisterCode(&node, &len);
		if (code == nullptr)
		{
			destroyRegisterManager(pRegister);
			pRegister = nullptr;
			return;
		}

		char *buffer = new char[2 * len + 2];
		memset(buffer, 0, sizeof(char) * (2 * len + 2));
		unsigned char *pUchar = (unsigned char*)code;
		for (int i = 0; i < len; ++i)
		{
			int tmp = pUchar[i];
			sprintf_s(buffer + 2 * i, 2 * len - 2 * i+2, "%02X", tmp);
		}


		wchar_t *wCode = utils::CharToWChar(buffer);
		GetDlgItem(IDC_EDIT_REGISTER_CODE)->SetWindowTextW(wCode);
		
		delete[]wCode;
		wCode = nullptr;
		
		delete[]code;
		code = nullptr;

		delete[]buffer;
		buffer = nullptr;

		destroyRegisterManager(pRegister);

		pRegister = nullptr;
	}

}