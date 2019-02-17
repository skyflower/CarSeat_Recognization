// CheckListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Query_Module.h"
#include "CheckListDlg.h"
#include "afxdialogex.h"


// CCheckListDlg �Ի���

IMPLEMENT_DYNAMIC(CCheckListDlg, CDialogEx)

CCheckListDlg::CCheckListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_REVISE_TYPE, pParent)
{
	memset(&data, 0, sizeof(data));
}

CCheckListDlg::~CCheckListDlg()
{
}

void CCheckListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCheckListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_IMAGE, &CCheckListDlg::OnBnClickedButtonChooseImage)
	ON_BN_CLICKED(IDOK, &CCheckListDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCheckListDlg ��Ϣ�������


void CCheckListDlg::OnBnClickedButtonChooseImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	wchar_t szFilter[] = L"BMP Files (*.bmp)|*.bmp|JPG Files (*.jpg;*.jpeg;*.jpe)|*.jpg;*.jpeg;*.jpe|PNG Files (*.png)|*.png|TIF Files(*.tif;*.tiff)|*.tif;*.tiff|GIF Files (*.gif)|*.gif|DIB Files(*.dib)|*.dib| All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, (LPCTSTR)"BMP", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if (dlg.DoModal() == IDOK)
	{
		//������д���ļ�����ļ���Ӧ�Ĳ��� 
		
		CString filepath = dlg.GetPathName();
		CString fileextname = dlg.GetFileExt();
		//CString filename = dlg.GetFileName();

		fileextname.MakeLower();

		if ((fileextname == "bmp") || (fileextname == "jpeg") || (fileextname == "jpe") || (fileextname == "jpg") || \
			(fileextname == "png") || (fileextname == "dib") || (fileextname == "tif") || (fileextname == "tiff") || (fileextname == "gif"))
		{
			//displayimage();
			GetDlgItem(IDC_STATIC_PATTERN_IMAGE_PATH)->SetWindowTextW(filepath);
			wmemcpy(data.m_szImagePath, filepath.GetBuffer(), filepath.GetLength());
			filepath.ReleaseBuffer();
		}
		else
		{
			AfxMessageBox(L"do not support file format, please retry");
		}
	}
}


void CCheckListDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	memset(&data, 0, sizeof(data));

	memset(data.m_szBarcode, 0, sizeof(data.m_szBarcode));
	GetDlgItemText(IDC_EDIT_BARCODE, data.m_szBarcode, sizeof(data.m_szBarcode)/sizeof(wchar_t));
	if (wcslen(data.m_szBarcode) == 0)
	{
		AfxMessageBox(L"������������");
		return;
	}

	memset(data.m_szInternalType, 0, sizeof(data.m_szInternalType));
	GetDlgItemText(IDC_EDIT_INTERNAL_TYPE, data.m_szInternalType, sizeof(data.m_szInternalType) / sizeof(wchar_t));
	if (wcslen(data.m_szInternalType) == 0)
	{
		AfxMessageBox(L"�������ⲿ����");
		return;
	}

	

	memset(data.m_szExternalType, 0, sizeof(data.m_szExternalType));
	GetDlgItemText(IDC_EDIT_EXTERNAL_TYPE, data.m_szExternalType, sizeof(data.m_szExternalType) / sizeof(wchar_t));
	if (wcslen(data.m_szExternalType) == 0)
	{
		AfxMessageBox(L"�������ⲿ����");
		return;
	}

	memset(data.m_szImagePath, 0, sizeof(data.m_szImagePath));
	GetDlgItemText(IDC_STATIC_PATTERN_IMAGE_PATH, data.m_szImagePath, sizeof(data.m_szImagePath) / sizeof(wchar_t));
	if (wcslen(data.m_szImagePath) == 0)
	{
		//AfxMessageBox(L"��ѡ��ģ��ͼ��·��");
		//return;

	}

	CDialogEx::OnOK();
}

checkListNode CCheckListDlg::GetCheckListData()
{
	return data;
}

void CCheckListDlg::SetCheckListData(const checkListNode &tmp)
{
	if (wcslen(tmp.m_szImagePath) != 0)
	{
		wmemcpy(data.m_szImagePath, tmp.m_szImagePath, wcslen(tmp.m_szImagePath));
	}
	if (wcslen(tmp.m_szBarcode) != 0)
	{
		wmemcpy(data.m_szBarcode, tmp.m_szBarcode, wcslen(tmp.m_szBarcode));
	}

	if (wcslen(tmp.m_szInternalType) != 0)
	{
		wmemcpy(data.m_szInternalType, tmp.m_szInternalType, wcslen(tmp.m_szInternalType));
	}

	if (wcslen(tmp.m_szExternalType) != 0)
	{
		wmemcpy(data.m_szExternalType, tmp.m_szExternalType, wcslen(tmp.m_szExternalType));
	}
}



BOOL CCheckListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetDlgItemText(IDC_EDIT_BARCODE, data.m_szBarcode);
	SetDlgItemText(IDC_EDIT_INTERNAL_TYPE, data.m_szInternalType);
	SetDlgItemText(IDC_EDIT_EXTERNAL_TYPE, data.m_szExternalType);
	SetDlgItemText(IDC_STATIC_PATTERN_IMAGE_PATH, data.m_szImagePath);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
