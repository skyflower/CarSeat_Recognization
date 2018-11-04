﻿#include "ImageClassify.h"
#include "../common/Log.h"

CImageClassify::CImageClassify(const char * graphFile, const char * labelFile):
#ifdef PYTHON_TENSORFLOW
	m_pPyName(nullptr),
	m_pPyModule(nullptr),
	m_pPyFunc(nullptr),
	m_pPyDict(nullptr),
#endif // PYTHON_TENSORFLOW
	 
	m_bRunning(true),
	m_Mutex()
{
	memset(m_szGraph, 0, sizeof(m_szGraph));
	memset(m_szLabel, 0, sizeof(m_szLabel));

	memcpy(m_szGraph, graphFile, strlen(graphFile));
	memcpy(m_szLabel, labelFile, strlen(labelFile));
	memset(m_szCacheImagePath, 0, sizeof(m_szCacheImagePath));

	m_pReadyImage = new std::list<std::string>;
	m_pType = new std::unordered_map<size_t, std::string>;
}

CImageClassify::~CImageClassify()
{
	destory();
	if (m_pReadyImage != nullptr)
	{
		delete m_pReadyImage;
		m_pReadyImage = nullptr;
	}
	if (m_pType != nullptr)
	{
		delete m_pType;
		m_pType = nullptr;
	}
}


bool CImageClassify::initPython(const char *modulName,const char *functionName, const char* cachePath)
{
#ifdef PYTHON_TENSORFLOW

	if (cachePath != nullptr)
	{
		memcpy(m_szCacheImagePath, cachePath, strlen(cachePath));
	}
	//WriteInfo("inter python init");
	Py_Initialize();
	if (!Py_IsInitialized())
	{
		WriteError("Py Initizlize Failed");
		return false;
	}
	//WriteInfo("exit python init, success");
	char tmpChar[100] = { 0 };
	sprintf_s(tmpChar, sizeof(tmpChar), "import %s", modulName);
	PyRun_SimpleString(tmpChar);
	//WriteInfo("python import %s", modulName);

	m_pPyName = PyUnicode_FromString(modulName);
	if (m_pPyName == nullptr)
	{
		WriteError("moduleName failed, %s", modulName);
		return false;
	}
	
	m_pPyModule = PyImport_Import(m_pPyName);

	//WriteInfo("PyImport_Import import %s", modulName);
	if (m_pPyModule == nullptr)
	{
		WriteError("get moudle handle error");
		return false;
	}
	

	m_pPyDict = PyModule_GetDict(m_pPyModule);
	
	if (!m_pPyDict)
	{
		WriteError("get moudledict handle error");
		return false;
	}
	//WriteInfo("PyModule_GetDict success");
	m_pPyFunc = PyDict_GetItemString(m_pPyDict, functionName);

	if (!m_pPyFunc || !PyCallable_Check(m_pPyFunc))
	{
		WriteError("can't find function [%s]", functionName);
		
		return false;
	}
#endif // PYTHON_TENSORFLOW
	return true;
}

void CImageClassify::pushImage(const char * filePath)
{
	std::string tmp(filePath);
	//std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	for (;;)
	{
		if (m_Mutex.try_lock())
		{
			m_pReadyImage->push_back(tmp);
			m_Mutex.unlock();
			break;
		}
	}
}

std::string CImageClassify::GetImageType(const char * filePath)
{
	size_t tmpHashValue = hashValue(filePath);
	if ((m_pType != nullptr) && (m_pType->size() > 0))
	{
		std::unordered_map<size_t, std::string>::const_iterator iter = m_pType->find(tmpHashValue);
		if (iter == m_pType->end())
		{
			return std::string();
		}
		return iter->second;
	}
	return std::string();
}

bool CImageClassify::terminate()
{
	m_bRunning = false;
	return true;
}

void CImageClassify::run()
{
	//std::unique_lock<std::mutex> lock(m_Mutex, std::defer_lock);
	char tmpPath[MAX_CHAR_LENGTH] = { 0 };
	
	while (m_bRunning)
	{
		if (m_Mutex.try_lock())
		{
			memset(tmpPath, 0, sizeof(char) * MAX_CHAR_LENGTH);
			if (m_pReadyImage->empty() == false)
			{
				memcpy(tmpPath, m_pReadyImage->begin()->c_str(), m_pReadyImage->begin()->size());
				m_pReadyImage->pop_front();
			}
			m_Mutex.unlock();
			if (strlen(tmpPath) > 0)
			{
				std::string tmpType = compute(tmpPath);
				if (m_pType == nullptr)
				{
					m_pType = new std::unordered_map<size_t, std::string>;
				}
				size_t tmpHashValue = hashValue(tmpPath);
				m_pType->insert(std::make_pair(tmpHashValue, tmpType));

			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

std::string CImageClassify::compute(const char *filePath)
{
	char *tmpUpType = NULL;
	float tmpUpValue = 0;
	float tmpDownValue = 0;
	char *tmpDownType = NULL;
	
#ifdef PYTHON_TENSORFLOW

	if ((m_pPyFunc == nullptr) || (filePath == nullptr) ||	\
		(strlen(m_szGraph) == 0) || (strlen(m_szLabel) == 0))
	{
		WriteError("error, m_szGraph = %s, m_szLabel = %s, filePath = %s", m_szGraph, m_szLabel, filePath);
		WriteError("m_szCacheImagePath = %s", m_szCacheImagePath);
		return std::string();
	}
	PyObject *presult = PyEval_CallFunction(m_pPyFunc, "ssss", filePath, m_szLabel, m_szGraph, m_szCacheImagePath);

	Py_ssize_t len = 0;
	if (presult != nullptr)
	{
		int ok = PyArg_ParseTuple(presult, "sfsf", &tmpUpType, &tmpUpValue, &tmpDownType, &tmpDownValue);
	}
	else
	{
		WriteError("presult is null");
	}

	
#endif // PYTHON_TENSORFLOW

	WriteInfo("File = [%s], UP : type = [%s], value = [%f], DOWN: type = [%s], value = [%f]", filePath, tmpUpType, tmpUpValue, tmpDownType, tmpDownValue);

	if ((tmpUpType == nullptr) || (tmpDownType == nullptr))
	{
		return std::string();
	}

	// 先采用统一识别方式判断，如果不等，则采用上下分开识别方式判断
	if (strncmp(tmpUpType, tmpDownType, strlen(tmpUpType)) == 0)
	{
		return std::string(tmpUpType);
	}

	/*
	按照命名方式截取，*_up, *_down，前面的整体类型
	*/
	size_t tmpDownLength = strlen(tmpDownType);
	size_t tmpUpLength = strlen(tmpUpType);

	if ((tmpUpLength <= 3) || (tmpDownLength <= 5))
	{
		return std::string();
	}
	
	if (strncmp(tmpUpType, tmpDownType, strlen(tmpUpType) - 3) != 0)
	{
		return std::string();
	}
	char tmpResultType[100] = { 0 };
	memset(tmpResultType, 0, sizeof(tmpResultType));
	memcpy(tmpResultType, tmpUpType, sizeof(char) * (tmpUpLength - 3));

	std::string strBuffer(tmpResultType);

	return strBuffer;
}

void CImageClassify::destory()
{
#ifdef PYTHON_TENSORFLOW



	if (m_pPyFunc != nullptr)
	{
		Py_DECREF(m_pPyFunc);
	}
	if (m_pPyModule != nullptr)
	{
		Py_DECREF(m_pPyModule);
	}
	if (m_pPyName != nullptr)
	{
		Py_DECREF(m_pPyName);
	}
	
	if (m_pPyDict != nullptr)
	{
		Py_DECREF(m_pPyDict);
	}
	Py_Finalize();
#endif // PYTHON_TENSORFLOW

}

size_t CImageClassify::hashValue(const char * filePath)
{	
	size_t Value = m_pHashFunc(filePath);

	return Value;
}
