////////////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017, DuanBin, all rights reserved.
//
// Author	: DuanBin
// Create	: 2018-04-13 11:06
// Mail		: duanchristian2015@gmail.com
// Version	: 1.0
//
//
// Description	: python接口
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PYTHON_INTERFACE_H_
#define _PYTHON_INTERFACE_H_

#include <string>
#include <Python.h>
#include "../common/utils.h"
#include <unordered_map>
#include <list>
#include <thread>
#include <mutex>

class CImageClassify
{
public:

	CImageClassify(const char*graphFile, const char* labelFile);
	~CImageClassify();

	bool initPython(const char *modulName,const char *functionName);

	void pushImage(const char*filePath);

	std::wstring GetImageType(const char*filePath);


	//  终止线程
	bool terminate();
	
	void run();
	
private:

	void destory();

	std::wstring compute(const char *filePath);

	size_t hashValue(const char* filePath);

	PyObject * m_pPyName;
	PyObject * m_pPyModule;
	PyObject * m_pPyFunc;
	PyObject * m_pPyDict;
	std::hash<const char*> m_pHashFunc;

	// 
	std::unordered_map<size_t, std::wstring> *m_pType;
	
	std::list<std::string> *m_pReadyImage;
	std::mutex m_Mutex;
	//std::thread m_pThread;
	bool m_bRunning;

private:
	enum
	{
		MAX_NAME_LENGTH = 50
	};
	char m_szGraph[MAX_CHAR_LENGTH];
	char m_szLabel[MAX_CHAR_LENGTH];
};


#endif
