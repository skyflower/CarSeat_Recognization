// QueryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "QueryDlg.h"
#include "afxdialogex.h"
#include "./common/utils.h"


// CQueryDlg 对话框

IMPLEMENT_DYNAMIC(CQueryDlg, CDialogEx)

CQueryDlg::CQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_QUERY, pParent)
	, m_szQueryBarcode(_T(""))
	, m_szQueryResult(_T(""))
{
	m_pRecogManager = CRecogResultManager::GetInstance();
}

CQueryDlg::~CQueryDlg()
{

}

void CQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_ST_QUERY_BARCODE, m_szStatic);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_QueryButton);
	DDX_Text(pDX, IDC_ED_QUERY_BARCODE, m_szQueryBarcode);
	DDX_Control(pDX, IDC_QUERY_IMAGE, m_ImageDisplay);
	DDX_Text(pDX, IDC_IMAGE_RESULT, m_szQueryResult);
}


BEGIN_MESSAGE_MAP(CQueryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CQueryDlg::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CQueryDlg 消息处理程序


void CQueryDlg::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString tmp;
	UpdateData(TRUE);	//从空间传给变量
	//m_szQueryBarcode
	if (m_pRecogManager != nullptr)
	{
		wchar_t tmp[MAX_CHAR_LENGTH] = { 0 };
		memset(tmp, 0, sizeof(wchar_t) * MAX_CHAR_LENGTH);
		wchar_t *pointer = m_szQueryBarcode.GetBuffer();
		memcpy(tmp, pointer, sizeof(wchar_t) * m_szQueryBarcode.GetLength());
		m_szQueryBarcode.ReleaseBuffer();
		char *value = utils::WcharToChar(tmp);

		const RecogResult *p = m_pRecogManager->findByBarcode(value);

		delete[]value;
		value = nullptr;

		if (p == nullptr)
		{
			AfxMessageBox(L"请输入正确的条形码");
			return;
		}

		RecogResult cRecog = *p;
		RecogResultW wRecog;
		if (utils::RecogResultCToW(cRecog, wRecog) == false)
		{
			return;
		}

		m_szQueryResult.Format(L"时间:%s\n条形码:%s\n条形码类型:%s\n识别类型%s\n产线管理员：%s\n产线编号:%s",
			wRecog.m_szTime, wRecog.m_szBarcode, wRecog.m_szTypeByBarcode, \
			wRecog.m_szTypeByRecog, wRecog.m_szUsrName, wRecog.m_szLineName);

		UpdateData(FALSE);

		//wchar_t *tmpWchar = utils::CharToWchar(const_cast<char*>(p->m_szImagePath));
		if (wcslen(wRecog.m_szImagePath) != 0)
		{
			HRESULT ret = m_Image.Load(wRecog.m_szImagePath);
			if (ret != 0)
			{
				AfxMessageBox(L"图像路径不正确");
			}
			else
			{
				m_ImageDisplay.SetBitmap((HBITMAP)m_Image);
			}

			//delete[]tmpWchar;
			//tmpWchar = nullptr;
		}
	}
}
