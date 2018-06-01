////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017, DuanBin, all rights reserved.
//
// Author	: DuanBin
// Create	: 2018-04-13 11:06
// Mail		: duanchristian2015@gmail.com
// Version	: 1.0
//
//
// Description	: python½Ó¿Ú
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PYTHON_INTERFACE_H_
#define _PYTHON_INTERFACE_H_

#include <string>
#include <Python.h>
#include "../common/utils.h"

class CImageClassify
{
public:
public:

	CImageClassify(const char*graphFile, const char* labelFile);
	~CImageClassify();

	bool initPython(const char *modulName,const char *functionName);

	std::wstring compute(const char *filePath);

	void destory();

private:
	PyObject * pName;
	PyObject * pModule;
	PyObject * pFunc;
	PyObject * pDict;

private:
	enum
	{
		MAX_NAME_LENGTH = 50
	};
	char m_szGraph[MAX_CHAR_LENGTH];
	char m_szLabel[MAX_CHAR_LENGTH];
	//char m_szModule[MAX_NAME_LENGTH];
	//char m_szFuncName[MAX_NAME_LENGTH];
};


#endif
