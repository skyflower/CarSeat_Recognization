#include "stdafx.h"
#include "RecogInfoCtrl.h"
//#include "ConditonDlg.h"
#include "ConditionFilter.h"
#include "Query_Module.h"
#include "./common/ParamManager.h"


IMPLEMENT_DYNCREATE(CRecogInfoCtrl, CListCtrl)


BEGIN_MESSAGE_MAP(CRecogInfoCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CRecogInfoCtrl::OnNMDblclk)
	ON_WM_CREATE()
END_MESSAGE_MAP()


CRecogInfoCtrl::CRecogInfoCtrl():m_pData(nullptr)
{
}


CRecogInfoCtrl::~CRecogInfoCtrl()
{
}

void CRecogInfoCtrl::initColumn()
{
	InsertColumn(0, _T("ʱ��"), LVCFMT_CENTER, 150);
	InsertColumn(1, _T("������"), LVCFMT_CENTER, 150);
	InsertColumn(2, _T("������"), LVCFMT_CENTER, 50);
	InsertColumn(3, _T("����������"), LVCFMT_CENTER, 150);
	InsertColumn(4, _T("ʶ������"), LVCFMT_CENTER, 150);
	InsertColumn(5, _T("��������"), LVCFMT_CENTER, 150);
	InsertColumn(6, _T("����Ա�û���"), LVCFMT_CENTER, 100);
	InsertColumn(7, _T("���߱��"), LVCFMT_CENTER, 100);
}


bool CRecogInfoCtrl::InsertItem(std::vector<RecogResultW> *pRecog)
{
	if ((pRecog == nullptr) || (pRecog->size() == 0))
	{
		return false;
	}
	if (m_pData != nullptr)
	{
		delete m_pData;
		m_pData = nullptr;
	}
	m_pData = new std::vector<RecogResultW>(*pRecog);

	DeleteAllItems();
	int len = pRecog->size();
	for (size_t i = 0; i < len; ++i)
	{
		RecogResultW &tmp = pRecog->at(i);
		CListCtrl::InsertItem(i, tmp.m_szTime, i);

		SetItemText(i, 1, tmp.m_szBarcode);
		SetItemText(i, 2, tmp.m_szInternalType);
		SetItemText(i, 3, tmp.m_szTypeByBarcode);
		SetItemText(i, 4, tmp.m_szTypeByRecog);
		SetItemText(i, 5, tmp.m_szTypeByUsrInput);
		SetItemText(i, 6, tmp.m_szUsrName);
		SetItemText(i, 7, tmp.m_szLineName);
	}
	return true;
}

bool CRecogInfoCtrl::ClearAllItem()
{
	this->DeleteAllItems();
	delete m_pData;
	m_pData = nullptr;
	return true;
}


void CRecogInfoCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int dbClickIndex = pNMItemActivate->iItem;
	TRACE1("dbClickIndex = %d\n", dbClickIndex);
	if ((m_pData != nullptr) && (m_pData->size() > dbClickIndex))
	{

		RecogResultW &tmp = m_pData->at(dbClickIndex);
		if (_waccess_s(tmp.m_szImagePath, 0x0) != 0)
		{
			RecogResultA tmpRecogResultA;
			utils::RecogResultWToC(tmp, tmpRecogResultA);
			CConditionFilterA filter;
			memset(&filter, 0, sizeof(CConditionFilterA));
			memcpy(filter.mBarcodeBegin, tmpRecogResultA.m_szBarcode, strlen(tmpRecogResultA.m_szBarcode));
			memcpy(filter.mBarcodeEnd, tmpRecogResultA.m_szBarcode, strlen(tmpRecogResultA.m_szBarcode));
			CParamManager *pParamManager = CParamManager::GetInstance();
			unsigned int ip = pParamManager->GetLocalIP();
			const std::string &userName = pParamManager->GetLoginUserName();
			const std::string &passwd = pParamManager->GetLoginPasswd();

			/*char *searchXml = generateSearchXml(filter, ip, userName, passwd);
			if (searchXml == nullptr)
			{
				AfxMessageBox(L"���ɲ�ѯXMLʧ��");
				return;
			}

			delete[]searchXml;
			searchXml = nullptr;*/
			CNetworkTask *queryTask = new CNetworkTask(pParamManager->GetServerIP(), pParamManager->GetServerPort());

			std::vector<RecogResultW> *selectedRecog = queryTask->QueryRecogInfo(filter, ip, userName, passwd);
			if ((selectedRecog != nullptr) && (selectedRecog->size() == 1))
			{
				memcpy(tmp.m_szImagePath, selectedRecog->at(0).m_szImagePath, sizeof(wchar_t) * wcslen(selectedRecog->at(0).m_szImagePath));
			}
			delete selectedRecog;
			selectedRecog = nullptr;

			delete queryTask;
			queryTask = nullptr;
		}
		wchar_t detailInfo[300];
		memset(detailInfo, 0, sizeof(detailInfo));
		swprintf_s(detailInfo, sizeof(detailInfo) / sizeof(wchar_t), L"ʱ��:%s\t����:%s\n������:%s\t����������:%s\nʶ������:%s\t����Ա:%s",
			tmp.m_szTime, tmp.m_szLineName, tmp.m_szBarcode, tmp.m_szTypeByBarcode, tmp.m_szTypeByRecog, tmp.m_szUsrName);
		
		CDetailInfoDlg dlg;
		dlg.SetDisplayInfo(detailInfo, tmp.m_szImagePath);
		
		dlg.DoModal();
	}


	*pResult = 0;
}


int CRecogInfoCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������


	return 0;
}
