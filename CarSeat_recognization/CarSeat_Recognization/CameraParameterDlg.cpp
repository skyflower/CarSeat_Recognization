// CameraParameter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CarSeat_Recognization.h"
#include "CameraParameterDlg.h"
#include "afxdialogex.h"


// CCameraParameterDlg �Ի���

IMPLEMENT_DYNAMIC(CCameraParameterDlg, CDialogEx)

CCameraParameterDlg::CCameraParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CAMERA_PARAMETER, pParent)
	, m_dGainDB(0)
	, m_uExposureTime(0)
	, m_fCameraFPS(0)
	, m_pLineCamera(nullptr)
	, m_nExposureTimeUpper(0)
{

}

CCameraParameterDlg::~CCameraParameterDlg()
{
}

void CCameraParameterDlg::SetLineCamera(CLineCamera * pCamera)
{
	if (pCamera == nullptr)
	{
		return;
	}
	m_pLineCamera = pCamera;
	m_uExposureTime = m_pLineCamera->GetExposureTime();
	m_fCameraFPS = m_pLineCamera->GetFrameRate();
	m_dGainDB = m_pLineCamera->GetGain();
	
}

void CCameraParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_DB, m_dGainDB);
	DDV_MinMaxDouble(pDX, m_dGainDB, 0, 255);
	DDX_Text(pDX, IDC_ED_EXPOSURE_TIME, m_uExposureTime);
	DDV_MinMaxUInt(pDX, m_uExposureTime, 0, 2000000);
	DDX_Text(pDX, IDC_ED_FPS, m_fCameraFPS);
	DDV_MinMaxFloat(pDX, m_fCameraFPS, 1, 100);
	DDX_Text(pDX, IDC_ED_EXPOSURE_TIME_UPPER, m_nExposureTimeUpper);
}


BEGIN_MESSAGE_MAP(CCameraParameterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GET_PARAMETER, &CCameraParameterDlg::OnBnClickedButtonGetParameter)
	ON_BN_CLICKED(IDC_BUTTON_SET_PARAMETER, &CCameraParameterDlg::OnBnClickedButtonSetParameter)
END_MESSAGE_MAP()


// CCameraParameterDlg ��Ϣ�������


void CCameraParameterDlg::OnBnClickedButtonGetParameter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pLineCamera == nullptr)
	{
		return;
	}
	m_uExposureTime = m_pLineCamera->GetExposureTime();
	m_fCameraFPS = m_pLineCamera->GetFrameRate();
	m_dGainDB = m_pLineCamera->GetGain();
	UpdateData(FALSE); // false ����ֵ�ӱ��������ؼ�
}


void CCameraParameterDlg::OnBnClickedButtonSetParameter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pLineCamera == nullptr)
	{
		return;
	}
	UpdateData(TRUE); // false ����ֵ�ӿؼ���������
	m_pLineCamera->SetExposureTime(m_uExposureTime);
	m_pLineCamera->SetFrameRate(m_fCameraFPS);
	m_pLineCamera->SetGain(m_dGainDB);
}


BOOL CCameraParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
