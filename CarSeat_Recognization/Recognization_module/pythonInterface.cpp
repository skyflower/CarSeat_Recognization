#include "pythonInterface.h"

/* test python */
void pythonInterface::initPython(const char *modulName,const char *functionName)
{
	module = modulName;
	func = functionName;

	if(!Py_IsInitialized())
	{
		Py_InitializeEx(0);
	}

	//Py_DECREF(pName);

	

}

std::string pythonInterface::compute()
{

	char module_path[] = ".";
	PyObject *sys_path = PySys_GetObject("path");
	PyList_Append(sys_path, PyUnicode_FromString(module_path));

	pName = PyUnicode_FromString(module);
	pModule = PyImport_Import(pName);

	if(pModule != NULL)
	{
		pFunc = PyObject_GetAttrString(pModule, func);
		if (pFunc && PyCallable_Check(pFunc))
		{	
			pValue = PyObject_CallObject(pFunc, NULL);
			if (pValue != NULL) 
			{
				printf("defect type : %s\n", PyUnicode_AsUTF8(pValue));
				Py_DECREF(pValue);
			}
			else 
			{
				PyErr_Print();
				destory();
				fprintf(stderr,"Call failed\n");
				return std::string("");
			}
		}
	}

	return PyUnicode_AsUTF8(pValue);
}

void pythonInterface::destory()
{
	Py_DECREF(pFunc);
	Py_DECREF(pModule);

	Py_Finalize();
}
