// InputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "InputDlg.h"
#include "afxdialogex.h"
#include "./common/ParamManager.h"
#include <io.h>


// CInputDlg 对话框

IMPLEMENT_DYNAMIC(CInputDlg, CDialogEx)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_INPUT_TYPE, pParent),
	m_pLabelManager(nullptr),
	m_pParamManager(nullptr),
	m_pTestImage(nullptr),
	m_pPatternImage(nullptr)
{
	if (m_pPatternImage == nullptr)
	{
		m_pPatternImage = new CImage();
	}
	if (m_pTestImage == nullptr)
	{
		m_pTestImage = new CImage();
	}

}

CInputDlg::~CInputDlg()
{
	if (m_pTestImage != nullptr)
	{
		delete m_pTestImage;
		m_pTestImage = nullptr;
	}
	if (m_pPatternImage != nullptr)
	{
		delete m_pPatternImage;
		m_pPatternImage = nullptr;
	}
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATTERN_IMAGE, m_StPatternImage);
	DDX_Control(pDX, IDC_TEST_IMAGE, m_StTestImage);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_TypeCombo);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CInputDlg::OnSelchangeComboType)
	ON_STN_CLICKED(IDC_PATTERN_IMAGE, &CInputDlg::OnStnClickedPatternImage)
END_MESSAGE_MAP()


// CInputDlg 消息处理程序


void CInputDlg::displayImage(CImage * pImage, CStatic * pStatic)
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

void CInputDlg::testPrint()
{
	if (m_StTestImage.GetSafeHwnd() != NULL)
	{
		RECT rect;
		m_StTestImage.GetWindowRect(&rect);
		ScreenToClient(&rect);

		//m_StTestImage.GetClientRect(&rect);
		WriteInfo("m_StTestImage bottom = %d, top = %d", rect.bottom, rect.top);
		WriteInfo("m_StTestImage left = %d, right = %d", rect.left, rect.right);
	}
	if (m_StPatternImage.GetSafeHwnd() != NULL)
	{
		RECT rect;
		m_StPatternImage.GetWindowRect(&rect);
		ScreenToClient(&rect);
		//m_StPatternImage.GetClientRect(&rect);

		WriteInfo("m_StPatternImage bottom = %d, top = %d", rect.bottom, rect.top);
		WriteInfo("m_StPatternImage left = %d, right = %d", rect.left, rect.right);
	}
}

BOOL CInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/*CParamManager *pManager = CParamManager::GetInstance();
	if (pManager == nullptr)
	{
		return FALSE;
	}*/

	if (m_pLabelManager == nullptr)
	{
		return FALSE;
	}

	CComboBox *pCombo = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE));
	std::vector<std::string> pVec = m_pLabelManager->GetExternalType();
	if (pVec.size() > 0)
	{
		std::vector<std::string>::const_iterator iter = pVec.begin();
		for (; iter != pVec.end(); ++iter)
		{
			std::wstring tmpStr = utils::StrToWStr(*iter);
			pCombo->AddString(tmpStr.c_str());
		}
	}
	m_StTestImage.ModifyStyle(0xF, SS_BITMAP);//设置静态控件的样式，使得它可以使用位图
	m_StPatternImage.ModifyStyle(0xF, SS_BITMAP);

	//m_hDC;
	


	displayImage(m_pTestImage, &m_StTestImage);

	/*if (m_StTestImage.GetSafeHwnd() != NULL)
	{
		HBITMAP hbitMap = (HBITMAP)(*m_pTestImage);
		m_StTestImage.SetBitmap(hbitMap);
		m_StTestImage.ShowWindow(TRUE);
	}*/
	

	/*std::vector<std::wstring> *pVector = pManager->GetColorParameter();
	if (pVector != nullptr)
	{
		size_t nSize = pVector->size();
		for (size_t i = 0; i < nSize; ++i)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_COLOR))->AddString(pVector->at(i).c_str());
		}
		
	}

	pVector = pManager->GetTextureParameter();
	if (pVector != nullptr)
	{
		size_t nSize = pVector->size();
		for (size_t i = 0; i < nSize; ++i)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_TEXTURE))->AddString(pVector->at(i).c_str());
		}

	}

	pVector = pManager->GetOutlineParameter();
	if (pVector != nullptr)
	{
		size_t nSize = pVector->size();
		for (size_t i = 0; i < nSize; ++i)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_OUTLINE))->AddString(pVector->at(i).c_str());
		}

	}*/


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

std::string CInputDlg::GetInputType()
{
	return std::string(m_szReType);
	//CComboBox *pCombo = ((CComboBox*)GetDlgItem(IDC_COMBO_TYPE));
	//if ((pCombo != NULL) && (pCombo->GetCount() > 0))
	//{
	//	//pCombo->get
	//	wchar_t tmpStr[50] = { 0 };
	//	int nIndex = pCombo->GetCurSel();
	//	int nCount = pCombo->GetCount();
	//	if ((nIndex != CB_ERR) && (nCount > 1))
	//	{
	//		pCombo->GetLBText(nIndex, tmpStr);
	//	}
	//	return std::wstring(tmpStr);
	//}
	//return std::wstring();
}

void CInputDlg::SetManagePointer(CParamManager * pParamManager, CLabelManager * pLabelManager)
{
	if (pParamManager != nullptr)
	{
		m_pParamManager = pParamManager;
	}
	if (pLabelManager != nullptr)
	{
		m_pLabelManager = pLabelManager;
	}
	
}

void CInputDlg::SetTestImagePath(std::string path)
{
	
	if (_access(path.c_str(), 0x04) == 0)
	{
		m_pTestImage->Destroy();
		HRESULT  ret = S_OK;

		std::wstring tmpPath = utils::StrToWStr(path);
		
		ret = m_pTestImage->Load(tmpPath.c_str());
		if (ret != S_OK)
		{
			return;
		}
		if (m_StTestImage.GetSafeHwnd() != NULL)
		{
			m_StTestImage.SetBitmap((HBITMAP)(*m_pTestImage));
		}
	}
	
}


void CInputDlg::OnSelchangeComboType()
{
	// TODO: 在此添加控件通知处理程序代码

#ifdef _DEBUG
	//testPrint();
#endif // _DEBUG

	
	int index = m_TypeCombo.GetCurSel();

	wchar_t selType[256] = { 0 };
	memset(selType, 0, sizeof(selType));

	m_TypeCombo.GetLBText(index, selType);

	if (wcslen(selType) == 0)
	{
		return;
	}
	char *tmpPointer = utils::WcharToChar(selType);
	memset(m_szReType, 0, sizeof(m_szReType));
	memcpy(m_szReType, tmpPointer, strlen(tmpPointer));

	delete[]tmpPointer;
	tmpPointer = nullptr;

	std::string tmpClassifyType = m_pLabelManager->GetClassifyTypeByExternal(std::string(m_szReType));


	std::string tmpPatternDir(m_pParamManager->GetPatternImagePath());

	//std::wstring tmpWPatternDir = utils::StrToWStr(tmpPatternDir);

	tmpPatternDir = tmpPatternDir + "\\" + tmpClassifyType + ".jpg";
	

	if (_access_s(tmpPatternDir.c_str(), 0x04) == 0)
	{
		m_pPatternImage->Destroy();
		std::wstring tmpPatternPath = utils::StrToWStr(tmpPatternDir);
		HRESULT ret = m_pPatternImage->Load(tmpPatternPath.c_str());
		if (ret != S_OK)
		{
			return;
		}
		displayImage(m_pPatternImage, &m_StPatternImage);
	}
}


void CInputDlg::OnStnClickedPatternImage()
{
	// TODO: 在此添加控件通知处理程序代码
}
