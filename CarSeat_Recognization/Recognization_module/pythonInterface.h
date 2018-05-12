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

class pythonInterface
{
public:
public:

	pythonInterface() {};
	~pythonInterface(){};

	void initPython(const char *modulName,const char *functionName);

	std::string compute();

	void destory();

private:
	PyObject *pName, *pValue,*pModule, *pFunc;

private:
	const char *module;
	const char *func;

};


#endif
