// ConditonDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Query_Module.h"
#include "ConditonDlg.h"
#include "afxdialogex.h"


// CConditonDlg �Ի���

IMPLEMENT_DYNAMIC(CConditonDlg, CDialogEx)

CConditonDlg::CConditonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONDITION, pParent)
{

}

CConditonDlg::~CConditonDlg()
{
}

CConditionFilter CConditonDlg::GetFilterCondition()
{
	return mFilter;
}

void CConditonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConditonDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConditonDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConditonDlg ��Ϣ�������



void CConditonDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	memset(&mFilter, 0, sizeof(CConditionFilter));

	wchar_t text[256];

	//���ڿ�ʼ
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_DATE_BEGIN, text, sizeof(text));
	memcpy(mFilter.mDateBeign, text, wcslen(text));

	//���ڽ���
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_DATE_END, text, sizeof(text));
	memcpy(mFilter.mDateEnd, text, wcslen(text));


	//ʱ�俪ʼ
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_TIME_BEGIN, text, sizeof(text));
	memcpy(mFilter.mTimeBegin, text, wcslen(text));

	//ʱ�����
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_TIME_END, text, sizeof(text));
	memcpy(mFilter.mTimeEnd, text, wcslen(text));


	//���߿�ʼ
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_LINE_BEGIN, text, sizeof(text));
	memcpy(mFilter.mLineBegin, text, wcslen(text));

	//���߽���
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_LINE_END, text, sizeof(text));
	memcpy(mFilter.mLineEnd, text, wcslen(text));

	//�����뿪ʼ
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_BARCODE_BEGIN, text, sizeof(text));
	memcpy(mFilter.mBarcodeBegin, text, wcslen(text));

	//���������
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_BARCODE_END, text, sizeof(text));
	memcpy(mFilter.mBarcodeEnd, text, wcslen(text));

	//��������
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_SEAT_TYPE, text, sizeof(text));
	memcpy(mFilter.mSeatType, text, wcslen(text));

	//ʶ������
	memset(text, 0, sizeof(text));
	GetDlgItemText(IDC_METHOD_TYPE, text, sizeof(text));
	memcpy(mFilter.mMethodType, text, wcslen(text));

	CDialogEx::OnOK();
}
