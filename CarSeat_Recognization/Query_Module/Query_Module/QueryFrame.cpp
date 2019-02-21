// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "Query_Module.h"

#include "QueryFrame.h"
#include "RecogResult.h"

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

// CChildFrame 构造/析构

CQueryFrame::CQueryFrame()
{
	// TODO: 在此添加成员初始化代码
}

CQueryFrame::~CQueryFrame()
{
}


BOOL CQueryFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CChildFrame 诊断

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

// CChildFrame 消息处理程序
void CQueryFrame::OnFileClose()
{
	// 若要关闭框架，只需发送 WM_CLOSE，
	// 这相当于从系统菜单中选择关闭。
	SendMessage(WM_CLOSE);
}

int CQueryFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	//mQueryResult.Create();		LVS_REPORT  LVS_LIST
	mQueryResult.Create(WS_BORDER | WS_DLGFRAME | WS_VISIBLE | LVS_EX_CHECKBOXES|LVS_REPORT/*|LVS_NOLABELWRAP*/ | LVS_SHOWSELALWAYS, \
		CRect(0, 0, 1020, 880), this, QUERY_LIST_CTRL_ID);

	mQueryResult.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

	mQueryResult.initColumn();

	return 0;
}

void CQueryFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWndEx::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL CQueryFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	//NM_CLICK;
	// mQueryResult控件消息
	if (nID == QUERY_LIST_CTRL_ID)
	{
		//TRACE2("nID = %d, nCode = %d\n", nID, nCode);
		//TRACE1("NM_FIRST = %d\n", NM_FIRST);
		queryCtrlMsg(nCode);
		return TRUE;
	}

	// 否则，执行默认处理
	return CMDIChildWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CQueryFrame::queryCtrlMsg(int nCode)
{
	//TRACE1("nCode = %d\n", nCode);
	int hi = HIWORD(nCode);
	int lo = LOWORD(nCode);
	//TRACE2("hi = %d, lo = %d\n", hi, lo);
	//NM_CLICK;
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



bool CQueryFrame::InsertItem(std::vector<RecogResultW> *pRecog)
{
	mQueryResult.InsertItem(pRecog);
	/*mQueryResult.InsertColumn(0, _T("时间"), LVCFMT_CENTER, 150);
	mQueryResult.InsertColumn(1, _T("条形码"), LVCFMT_CENTER, 150);
	mQueryResult.InsertColumn(2, _T("对照码"), LVCFMT_CENTER, 50);
	mQueryResult.InsertColumn(3, _T("对照码"), LVCFMT_CENTER, 50);
	mQueryResult.InsertColumn(4, _T("识别类型"), LVCFMT_CENTER, 150);
	mQueryResult.InsertColumn(5, _T("输入类型"), LVCFMT_CENTER, 150);
	mQueryResult.InsertColumn(6, _T("管理员用户名"), LVCFMT_CENTER, 100);
	mQueryResult.InsertColumn(7, _T("产线编号"), LVCFMT_CENTER, 100);*/
	
	return true;
}