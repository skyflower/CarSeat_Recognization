
// KepServerTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKepServerTestApp: 
// �йش����ʵ�֣������ KepServerTest.cpp
//

class CKepServerTestApp : public CWinApp
{
public:
	CKepServerTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKepServerTestApp theApp;