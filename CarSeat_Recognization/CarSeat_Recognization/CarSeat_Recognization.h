
// CarSeat_Recognization.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCarSeat_RecognizationApp: 
// �йش����ʵ�֣������ CarSeat_Recognization.cpp
//

class CCarSeat_RecognizationApp : public CWinApp
{
public:
	CCarSeat_RecognizationApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCarSeat_RecognizationApp theApp;