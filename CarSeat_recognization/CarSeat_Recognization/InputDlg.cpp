// InputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "InputDlg.h"
#include "afxdialogex.h"
#include "./common/ParamManager.h"


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
END_MESSAGE_MAP()


// CInputDlg 消息处理程序


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
	std::vector<std::wstring> pVec = m_pLabelManager->GetBarcode();
	if (pVec.size() > 0)
	{
		std::vector<std::wstring>::const_iterator iter = pVec.begin();
		for (; iter != pVec.end(); ++iter)
		{
			pCombo->AddString(iter->c_str());
		}
	}
	if (m_StTestImage.GetSafeHwnd() != NULL)
	{
		m_StTestImage.SetBitmap((HBITMAP)(*m_pTestImage));
	}
	

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

std::wstring CInputDlg::GetInputType()
{
	return std::wstring(m_szReType);
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

void CInputDlg::SetTestImagePath(std::wstring path)
{
	if (_waccess_s(path.c_str(), 0x04) == 0)
	{
		m_pTestImage->Destroy();
		HRESULT ret = m_pTestImage->Load(path.c_str());
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

	int index = m_TypeCombo.GetCurSel();

	//wchar_t selType[256] = { 0 };
	memset(m_szReType, 0, sizeof(m_szReType));

	m_TypeCombo.GetLBText(index, m_szReType);

	std::string tmpPatternDir(m_pParamManager->GetPatternImagePath());

	std::wstring tmpWPatternDir = utils::StrToWStr(tmpPatternDir);

	tmpWPatternDir = tmpWPatternDir + L"\\" + m_szReType + L".jpg";

	if (_waccess_s(tmpWPatternDir.c_str(), 0x04) == 0)
	{
		m_pPatternImage->Destroy();
		HRESULT ret = m_pPatternImage->Load(tmpWPatternDir.c_str());
		if (ret != S_OK)
		{
			return;
		}
		if (m_StPatternImage.GetSafeHwnd() != NULL)
		{
			m_StPatternImage.SetBitmap((HBITMAP)(*m_pPatternImage));
		}

	}


	//m_pPatternImage->Load(tmpWPatternDir.c_str());
	//m_StPatternImage.SetBitmap((HBITMAP)(*m_pPatternImage));

}
