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
	// ����
public:

	void queryCtrlMsg(int nCode);

	// ����
public:

	// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// ʵ��
public:
	// ��ܹ���������ͼ��
	CChildView m_wndView;
	virtual ~CQueryFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};

