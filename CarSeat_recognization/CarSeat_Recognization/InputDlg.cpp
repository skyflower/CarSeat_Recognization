// InputDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "InputDlg.h"
#include "afxdialogex.h"
#include "./common/ParamManager.h"


// CInputDlg �Ի���

IMPLEMENT_DYNAMIC(CInputDlg, CDialogEx)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_INPUT_TYPE, pParent)
{

}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COLOR, m_ColorCombo);
	DDX_Control(pDX, IDC_COMBO_OUTLINE, m_OutlineCombo);
	DDX_Control(pDX, IDC_COMBO_TEXTURE, m_TexCombo);
	DDX_Control(pDX, IDC_PATTERN_IMAGE, m_StPatternImage);
	DDX_Control(pDX, IDC_TEST_IMAGE, m_StTestImage);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialogEx)
END_MESSAGE_MAP()


// CInputDlg ��Ϣ�������


BOOL CInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CParamManager *pManager = CParamManager::GetInstance();
	if (pManager == nullptr)
	{
		return FALSE;
	}
	std::vector<std::wstring> *pVector = pManager->GetColorParameter();
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

	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
