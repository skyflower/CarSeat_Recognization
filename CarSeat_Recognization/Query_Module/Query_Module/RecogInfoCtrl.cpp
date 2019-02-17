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
	InsertColumn(0, _T("时间"), LVCFMT_CENTER, 150);
	InsertColumn(1, _T("条形码"), LVCFMT_CENTER, 150);
	InsertColumn(2, _T("对照码"), LVCFMT_CENTER, 50);
	InsertColumn(3, _T("条形码类型"), LVCFMT_CENTER, 150);
	InsertColumn(4, _T("识别类型"), LVCFMT_CENTER, 150);
	InsertColumn(5, _T("输入类型"), LVCFMT_CENTER, 150);
	InsertColumn(6, _T("管理员用户名"), LVCFMT_CENTER, 100);
	InsertColumn(7, _T("产线编号"), LVCFMT_CENTER, 100);
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
	// TODO: 在此添加控件通知处理程序代码
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
				AfxMessageBox(L"生成查询XML失败");
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
		swprintf_s(detailInfo, sizeof(detailInfo) / sizeof(wchar_t), L"时间:%s\t产线:%s\n条形码:%s\t条形码类型:%s\n识别类型:%s\t管理员:%s",
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

	// TODO:  在此添加您专用的创建代码


	return 0;
}
