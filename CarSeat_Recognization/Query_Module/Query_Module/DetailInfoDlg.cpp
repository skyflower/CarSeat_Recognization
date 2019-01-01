// DetailInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Query_Module.h"
#include "DetailInfoDlg.h"
#include "afxdialogex.h"


// CDetailInfoDlg 对话框

IMPLEMENT_DYNAMIC(CDetailInfoDlg, CDialogEx)

CDetailInfoDlg::CDetailInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RECOG_INFO, pParent)
	, m_szDetailInfo(_T(""))
{

}

CDetailInfoDlg::~CDetailInfoDlg()
{
}

void CDetailInfoDlg::SetDisplayInfo(wchar_t * info, wchar_t * imagePath)
{
	if ((info != nullptr) && (imagePath != nullptr))
	{
		m_szDetailInfo = CString(info);

		//UpdateData(FALSE);
		if ((HBITMAP)m_Image != NULL)
		{
			m_Image.Destroy();
			
		}
		HRESULT ret = m_Image.Load(imagePath);
		if (ret != 0)
		{
			AfxMessageBox(L"图像路径不正确");
		}
		else
		{
			//displayImage(&m_Image, &m_RecogImage);
		}
	}
}

void CDetailInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DETAIL_INFO, m_szDetailInfo);
	DDX_Control(pDX, IDC_RECOG_IMAGE, m_RecogImage);
}


BEGIN_MESSAGE_MAP(CDetailInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// CDetailInfoDlg 消息处理程序



void CDetailInfoDlg::displayImage(CImage * pImage, CStatic * pStatic)
{
	if ((pImage == nullptr) || (pStatic == nullptr))
	{
		return;
	}
	if (pStatic->GetSafeHwnd() != NULL)
	{
		RECT rect;
		pStatic->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pStatic->SetBitmap((HBITMAP)(*pImage));

		pStatic->MoveWindow(rect.left, rect.top, rect.right - rect.left, \
			rect.bottom - rect.top, TRUE);

		Invalidate();
	}
}

BOOL CDetailInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//m_RecogImage.Create(L"image", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(50, 80, 150, 150), this);

	m_RecogImage.ModifyStyle(0xF, SS_BITMAP);
	if ((HBITMAP)m_Image != NULL)
	{
		displayImage(&m_Image, &m_RecogImage);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}