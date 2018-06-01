#include "ImageClassify.h"
#include "../common/Log.h"

CImageClassify::CImageClassify(const char * graphFile, const char * labelFile)
{
	memset(m_szGraph, 0, sizeof(m_szGraph));
	memset(m_szLabel, 0, sizeof(m_szLabel));

	memcpy(m_szGraph, graphFile, strlen(graphFile));
	memcpy(m_szLabel, labelFile, strlen(labelFile));

}

CImageClassify::~CImageClassify()
{
	destory();
}


bool CImageClassify::initPython(const char *modulName,const char *functionName)
{
	Py_Initialize();
	if (!Py_IsInitialized())
	{
		WriteError("Py Initizlize Failed");
		return false;
	}

	PyRun_SimpleString("import label_image_command_line");

	//PyObject *pName = NULL, *pMoudle = NULL, *pDict = NULL, *pFunc = NULL;
	//PyObject *tmpModel = nullptr;

	pName = PyUnicode_FromString(modulName);
	
	pModule = PyImport_Import(pName);
	//tmpModel = PyImport_ImportModule("label_image_command_line");
	if (pModule == nullptr)
	{
		WriteError("get moudle handle error");
		return false;
	}

	pDict = PyModule_GetDict(pModule);

	if (!pDict)
	{
		WriteError("get moudledict handle error");
		return false;
	}

	pFunc = PyDict_GetItemString(pDict, functionName);

	if (!pFunc || !PyCallable_Check(pFunc))
	{
		WriteError("can't find function [add]");
		
		return false;
	}
	return true;
}

std::wstring CImageClassify::compute(const char *filePath)
{
	if ((pFunc == nullptr) || (filePath == nullptr) ||	\
		(strlen(m_szGraph) == 0) || (strlen(m_szGraph) == 0))
	{
		return std::wstring();
	}
	PyObject *presult = PyEval_CallFunction(pFunc, "sss", filePath, m_szLabel, m_szGraph);

	char *buffer = NULL;
	Py_ssize_t len = 0;
	float reValue = 0;

	int ok = PyArg_ParseTuple(presult, "sf", &buffer, &reValue);
	
	std::string tmpBuffer(buffer);

	wchar_t *tmpWType = utils::CharToWchar(buffer);

	std::wstring wstrBuffer(tmpWType);
	return wstrBuffer;
}

void CImageClassify::destory()
{
	Py_DECREF(pFunc);
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_DECREF(pDict);

	Py_Finalize();
}
