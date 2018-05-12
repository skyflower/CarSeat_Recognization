int callPython()
{
	int result = -1;

	std::string type;

	char module[] = "label_image_changed";
	char func[] = "tensorflow_main";

	pythonInterface m_interface;
	m_interface.initPython(module,func);

	time_t classfiy_time = clock();
	type = m_interface.compute();
	std::cout << "classify time is :" << (double)((clock() - classfiy_time)) << std::endl;


	//初始化
	/*PyObject *pName, *pValue,*pModule, *pFunc;
	Py_Initialize();

	char module_path[] = ".";
	PyObject *sys_path = PySys_GetObject("path");
	PyList_Append(sys_path, PyUnicode_FromString(module_path));

	pName = PyUnicode_FromString(module);

	if(pName == NULL)
	{
		 PyErr_Print();
	}
	pModule = PyImport_Import(pName);

	Py_DECREF(pName);

	if(pModule != NULL)
	{
		pFunc = PyObject_GetAttrString(pModule, func);
		if (pFunc && PyCallable_Check(pFunc))
		{	
			pValue = PyObject_CallObject(pFunc, NULL);
			if (pValue != NULL) 
			{
				type = PyUnicode_AsUTF8(pValue);
				printf("defect type : %s\n", PyUnicode_AsUTF8(pValue));
				Py_DECREF(pValue);
			}
			else 
			{
				PyErr_Print();
				Py_DECREF(pFunc);
				Py_DECREF(pModule);

				fprintf(stderr,"Call failed\n");
				return result;
			}
		}
	}

	Py_DECREF(pFunc);
	Py_DECREF(pModule);
	Py_Finalize();*/

	if(type == "scratches")
	{
		return DEFECT_TYPE::scratches;
	}
	else if(type == "damage")
	{
		return DEFECT_TYPE::damage;
	}
	else if(type == "fold")
	{
		return DEFECT_TYPE::fold;
	}
	else if(type == "pinch")
	{
		return DEFECT_TYPE::pinch;
	}
	else if(type == "pig")
	{
		return DEFECT_TYPE::pig;
	}
	else if(type == "dirty")
	{
		return DEFECT_TYPE::dirty;
	}
	else if(type == "toner_strip")
	{
		return DEFECT_TYPE::toner_strip;
	}

	return result;
}