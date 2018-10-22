#pragma once
#include "ChildView.h"

class CQueryFrame :
	public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CQueryFrame)
public:
	CQueryFrame();

private:
	//CStatic mQueryHead;
	CListCtrl mQueryResult;
	
	enum
	{
		QUERY_LIST_CTRL_ID = 101
	};
	// 特性
public:

	void queryCtrlMsg(int nCode);

	// 操作
public:

	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// 实现
public:
	// 框架工作区的视图。
	CChildView m_wndView;
	virtual ~CQueryFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成的消息映射函数
protected:
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};

