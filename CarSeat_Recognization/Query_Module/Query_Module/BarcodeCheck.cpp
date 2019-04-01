// BarcodeCheck.cpp : 实现文件
//

#include "stdafx.h"
#include "Query_Module.h"
#include "BarcodeCheck.h"
#include "afxdialogex.h"
#include "CheckListDlg.h"
#include <map>


// CBarcodeCheck 对话框

IMPLEMENT_DYNAMIC(CBarcodeCheck, CDialogEx)

CBarcodeCheck::CBarcodeCheck(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BARCODE, pParent)
{
	m_pBarcode = nullptr;
	m_pClassify = nullptr;
	m_bRevise = false;
}

CBarcodeCheck::~CBarcodeCheck()
{
}

void CBarcodeCheck::SetTypePointer(std::unordered_map<std::string, std::string>* barcode, 
	std::unordered_map<std::string, std::string>* classifyType)
{
	if (barcode != nullptr)
	{
		m_pBarcode = barcode;
	}
	if (classifyType != nullptr)
	{
		m_pClassify = classifyType;
	}
}

void CBarcodeCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BARCODE_CHECKLIST, m_BarcodeList);
}


BEGIN_MESSAGE_MAP(CBarcodeCheck, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBarcodeCheck::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBarcodeCheck::OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_BARCODE_CHECKLIST, &CBarcodeCheck::OnNMDblclkBarcodeChecklist)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ONE_LINE, &CBarcodeCheck::OnBnClickedButtonAddOneLine)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_SELECTED, &CBarcodeCheck::OnBnClickedButtonDeleteSelected)
END_MESSAGE_MAP()


// CBarcodeCheck 消息处理程序


void CBarcodeCheck::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bRevise == true)
	{
		wchar_t barcodeBuf[32];
		wchar_t internalTypeBuf[128];
		wchar_t externalTypeBuf[256];
		int itemCount = m_BarcodeList.GetItemCount();

		bool flag = false; /// false，需要询问, true，不需要询问
		m_pBarcode->clear();
		m_pClassify->clear();
		
		for (int i = 0; i < itemCount; ++i)
		{
			memset(barcodeBuf, 0, sizeof(barcodeBuf));
			memset(internalTypeBuf, 0, sizeof(internalTypeBuf));
			memset(externalTypeBuf, 0, sizeof(externalTypeBuf));

			m_BarcodeList.GetItemText(i, 0, barcodeBuf, sizeof(barcodeBuf) / sizeof(wchar_t));
			m_BarcodeList.GetItemText(i, 1, internalTypeBuf, sizeof(internalTypeBuf) / sizeof(wchar_t));
			m_BarcodeList.GetItemText(i, 2, externalTypeBuf, sizeof(externalTypeBuf) / sizeof(wchar_t));

			
			if ((wcslen(barcodeBuf) == 0) || (wcslen(internalTypeBuf) == 0) || (wcslen(externalTypeBuf) == 0))
			{
				continue;
			}

			char *tmpBarcode = utils::WCharToChar(barcodeBuf);
			char *tmpInternal = utils::WCharToChar(internalTypeBuf);
			char *tmpExternal = utils::WCharToChar(externalTypeBuf);
			std::string tmpStrBarcode(tmpBarcode);
			std::string tmpStrInternal(tmpInternal);
			std::string tmpStrExternal(tmpExternal);

			delete tmpBarcode;
			tmpBarcode = nullptr;
			delete tmpInternal;
			tmpInternal = nullptr;
			delete tmpExternal;
			tmpExternal = nullptr;

			if ((tmpStrBarcode.size() == 0) || (tmpStrInternal.size() == 0) || (tmpStrExternal.size() == 0))
			{
				continue;
			}
			(*m_pBarcode)[tmpStrBarcode] = tmpStrInternal;
			(*m_pClassify)[tmpStrInternal] = tmpStrExternal;
			
		}
		CParamManager::GetInstance()->SetAutoSaveFalg(true);
	}

	CDialogEx::OnOK();
}


void CBarcodeCheck::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CBarcodeCheck::OnNMDblclkBarcodeChecklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	checkListNode data;
	int index = pNMItemActivate->iItem;
	m_BarcodeList.GetItemText(index, 0, data.m_szBarcode, sizeof(data.m_szBarcode) / sizeof(wchar_t));
	m_BarcodeList.GetItemText(index, 1, data.m_szInternalType, sizeof(data.m_szInternalType) / sizeof(wchar_t));
	m_BarcodeList.GetItemText(index, 2, data.m_szExternalType, sizeof(data.m_szExternalType) / sizeof(wchar_t));
	m_BarcodeList.GetItemText(index, 3, data.m_szImagePath, sizeof(data.m_szImagePath) / sizeof(wchar_t));
	
	CCheckListDlg dlg;

	WriteInfo("index = %d", index);

	dlg.SetCheckListData(data);
	
	if (IDOK == dlg.DoModal())
	{
		checkListNode revisedData = dlg.GetCheckListData();
		if (memcmp(&data, &revisedData, sizeof(checkListNode)) != 0)
		{
			m_bRevise = true;

			data = revisedData;

			m_BarcodeList.SetItemText(index, 0, data.m_szBarcode);
			m_BarcodeList.SetItemText(index, 1, data.m_szInternalType);
			m_BarcodeList.SetItemText(index, 2, data.m_szExternalType);
			m_BarcodeList.SetItemText(index, 3, data.m_szImagePath);
		}
	}
	*pResult = 0;
}


BOOL CBarcodeCheck::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_BarcodeList.SetExtendedStyle(WS_BORDER | WS_DLGFRAME | WS_VISIBLE | LVS_EX_CHECKBOXES | LVS_REPORT/*|LVS_NOLABELWRAP*/ | LVS_SHOWSELALWAYS |LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

	m_BarcodeList.InsertColumn(0, _T("三字码"), LVCFMT_CENTER, 100);
	m_BarcodeList.InsertColumn(1, _T("内部类型"), LVCFMT_CENTER, 300);
	m_BarcodeList.InsertColumn(2, _T("外部类型"), LVCFMT_CENTER, 300);
	m_BarcodeList.InsertColumn(3, _T("模板图像路径"), LVCFMT_CENTER, 400);

	if ((m_pBarcode != nullptr) && (m_pBarcode->size() > 0))
	{
		int i = 0;
		std::map<std::string, std::string> *tmpSorted = new std::map<std::string, std::string>(m_pBarcode->begin(), m_pBarcode->end());

		for (std::map<std::string, std::string>::iterator iter = tmpSorted->begin();
			iter != tmpSorted->end(); ++iter)
		{
			std::wstring wBarcode = utils::StrToWStr(iter->first);
			std::wstring wInternalType = utils::StrToWStr(iter->second);
			m_BarcodeList.InsertItem(i, wBarcode.c_str(), i);
			m_BarcodeList.SetItemText(i, 1, wInternalType.c_str());
			if (m_pClassify != nullptr)
			{
				std::unordered_map<std::string, std::string>::iterator tmpIter = m_pClassify->find(iter->second);
				if (tmpIter != m_pClassify->end())
				{
					std::wstring wExternalType = utils::StrToWStr(tmpIter->second);
					m_BarcodeList.SetItemText(i, 2, wExternalType.c_str());
				}
			}
			++i;
		}
		tmpSorted->clear();
		delete tmpSorted;
		tmpSorted = nullptr;

	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CBarcodeCheck::OnBnClickedButtonAddOneLine()
{
	// TODO: 在此添加控件通知处理程序代码
	int rowCount = m_BarcodeList.GetItemCount();
	m_BarcodeList.InsertItem(rowCount, L"请输入三字码", rowCount);
	m_BarcodeList.SetItemText(rowCount, 1, L"请填写内部类型");
	m_BarcodeList.SetItemText(rowCount, 2, L"请填写外部类型");
	
	m_bRevise = true;
}


void CBarcodeCheck::OnBnClickedButtonDeleteSelected()
{
	// TODO: 在此添加控件通知处理程序代码
	std::list<int> selected;
	int rowCount = m_BarcodeList.GetItemCount();
	for (int i = 0; i < rowCount; ++i)
	{
		if (m_BarcodeList.GetCheck(i) == TRUE)
		{
			selected.push_front(i);
		}
	}
	for (std::list<int>::iterator iter = selected.begin(); iter != selected.end(); ++iter)
	{
		m_BarcodeList.DeleteItem(*iter);
	}
	if (selected.size() > 0)
	{
		m_bRevise = true;
	}
}
