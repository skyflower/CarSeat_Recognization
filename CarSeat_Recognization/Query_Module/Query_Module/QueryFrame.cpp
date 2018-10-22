// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// ChildFrm.cpp : CChildFrame ���ʵ��
//

#include "stdafx.h"
#include "Query_Module.h"

#include "QueryFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CQueryFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CQueryFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_CLOSE, &CQueryFrame::OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CChildFrame ����/����

CQueryFrame::CQueryFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CQueryFrame::~CQueryFrame()
{
}


BOOL CQueryFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CChildFrame ���

#ifdef _DEBUG
void CQueryFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CQueryFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame ��Ϣ�������
void CQueryFrame::OnFileClose()
{
	// ��Ҫ�رտ�ܣ�ֻ�跢�� WM_CLOSE��
	// ���൱�ڴ�ϵͳ�˵���ѡ��رա�
	SendMessage(WM_CLOSE);
}

int CQueryFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}

	//mQueryResult.Create();		LVS_REPORT  LVS_LIST
	mQueryResult.Create(WS_BORDER | WS_DLGFRAME | WS_VISIBLE | LVS_EX_CHECKBOXES|LVS_REPORT/*|LVS_NOLABELWRAP*/ | LVS_SHOWSELALWAYS, \
		CRect(0, 0, 780, 580), this, QUERY_LIST_CTRL_ID);


	mQueryResult.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);


	mQueryResult.InsertColumn(0, _T("����"), LVCFMT_LEFT, 255);
	mQueryResult.InsertColumn(1, _T("�Ա�"), LVCFMT_LEFT, 255);
	mQueryResult.InsertColumn(2, _T("��������"), LVCFMT_LEFT, 270);


	mQueryResult.InsertItem(0, L"����С", 0);	
	mQueryResult.SetItemText(0, 1, L"��");	
	mQueryResult.SetItemText(0, 2, L"1992");	
	mQueryResult.InsertItem(1, L"����", 1);	
	mQueryResult.SetItemText(1, 1, L"��");	
	mQueryResult.SetItemText(1, 2, L"1987");	
	mQueryResult.InsertItem(2, L"����", 2);	
	mQueryResult.SetItemText(2, 1, L"��");
	mQueryResult.SetItemText(2, 2, L"1980");


	return 0;
}

void CQueryFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWndEx::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL CQueryFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	//NM_CLICK;
	// mQueryResult�ؼ���Ϣ
	if (nID == QUERY_LIST_CTRL_ID)
	{
		//TRACE1("nID = %d, nCode = %d\n", nID, nCode);
		queryCtrlMsg(nCode);
		return TRUE;
	}

	// ����ִ��Ĭ�ϴ���
	return CMDIChildWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CQueryFrame::queryCtrlMsg(int nCode)
{
	//TRACE1("nCode = %d\n", nCode);
	int hi = HIWORD(nCode);
	int lo = LOWORD(nCode);
	//TRACE2("hi = %d, lo = %d\n", hi, lo);
	NM_CLICK;
	switch (nCode)
	{
	case NM_DBLCLK:
		TRACE0("double click\n");
		break;
	case NM_CLICK:
		TRACE0("single left click\n");
		break;
	case NM_SETFOCUS:
		TRACE0("set focus\n");
		break;
	default:
		break;
	}
}
