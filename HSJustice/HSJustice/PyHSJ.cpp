#include "PyHSJ.h"


CPyHSJ::CPyHSJ()
{
	m_pModule = NULL;
	m_pFunc = NULL;
	m_strPythonName = NULL;

	m_bIsInitial = false;
}


CPyHSJ::~CPyHSJ()
{
	Py_Finalize();
}

char* CPyHSJ::WChar2Char(wchar_t* _strWChar)
{
	int iSize = WideCharToMultiByte(CP_ACP, 0, _strWChar, -1, NULL, 0, NULL, NULL);
	char* strChar = (char*)malloc(iSize*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, _strWChar, -1, strChar, iSize, NULL, NULL);
	return strChar;
}

int CPyHSJ::Initial(wchar_t* _strProgramName, wchar_t* _strPythonExePath, wchar_t* _strPythonExeName)
{
	Py_SetPythonHome(L"C:/Users/xcandy/AppData/Local/conda/conda/envs/tensorflow");
	Py_SetProgramName(_strProgramName);
	Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化  

	PySys_SetArgv(1, &_strProgramName);

	PyRun_SimpleString("import sys\n"
		"print(sys.argv)");

	PyRun_SimpleString("import os");
	PyRun_SimpleString("import sys;");

	char strPythonFilePath[1024] = { 0 };
	sprintf_s(strPythonFilePath, "sys.path.append('%s')", WChar2Char(_strPythonExePath));
	PyRun_SimpleString(strPythonFilePath);

	m_strPythonName = WChar2Char(_strPythonExeName);
	m_pModule = PyImport_ImportModule(m_strPythonName);//这里是要调用的文件名  
	if (m_pModule == NULL || PyErr_Occurred()){
		PyErr_Print();
		return -1;
	}

	m_bIsInitial = true;
	return 0;
}


std::string UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴   
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}

int CPyHSJ::parseList(PyObject *_pyList, std::vector<std::vector<double>>  &_out)
{
	if (PyList_Check(_pyList)){ //检查是否为List对象
		int SizeOfList = PyList_Size(_pyList);//List对象的大小
		for (int Index_i = 0; Index_i < SizeOfList; Index_i++){
			PyObject *ListItem = PyList_GetItem(_pyList, Index_i);//获取List对象中的每一个元素
			int NumOfItems = PyList_Size(ListItem);//List对象子元素的大小

			std::vector<double> vplistItem;
			for (int Index_k = 0; Index_k < NumOfItems; Index_k++){
				double temp = PyFloat_AsDouble(PyList_GetItem(ListItem, Index_k));
				vplistItem.push_back(temp);
			}
			_out.push_back(vplistItem);
			
			Py_DECREF(ListItem); //释放空间
		}
	}
	return 0;
}

int CPyHSJ::parseTuple(PyObject *_pyTuple, std::vector<std::vector<double>> &_out) 
{
	if (PyTuple_Check(_pyTuple))
	{
		int sizeOfTuple = PyTuple_Size(_pyTuple);

		for (int i = 0; i < sizeOfTuple; i++)
		{
			PyObject *tupleItem = PyTuple_GetItem(_pyTuple, i);
			parseList(tupleItem, _out);
			//if(PyInt_Check(tupleItem))
			{
				//std::cout << PyInt_FromLong(tupleItem) << std::endl;      //输出元素
			}
			Py_DECREF(tupleItem);
		}
	}
	else
	{
		parseList(_pyTuple, _out);
	}
	return 0;
}


int CPyHSJ::CallFunc(wchar_t* _strFuncName, int _args, char** _argv, int* _argv_lens)
{
	if (m_bIsInitial == false)
	{
		printf("Please Initial Python envs\n");
		return -1;
	}

	if (m_pModule == NULL || PyErr_Occurred()){
		PyErr_Print();
		return -1;
	}

	char* strFuncName = WChar2Char(_strFuncName);
	m_pFunc = PyObject_GetAttrString(m_pModule, strFuncName);//这里是要调用的函数名  

	PyObject *pArgs = PyTuple_New(_args);

	char* image = (char*)malloc(_argv_lens[0]);
	memcpy(image, _argv[0], _argv_lens[0]);

	for (int i = 0; i < _args; i++)
	{
		PyObject * res = PyByteArray_FromStringAndSize(_argv[i], _argv_lens[i]);
		PyTuple_SetItem(pArgs, 0, res);
	}


	PyObject *pResult = PyEval_CallObject(m_pFunc, pArgs);//调用函数
	if (pResult == NULL || PyErr_Occurred()){
		printf("Fillbuffer error\n");
		PyErr_Print();
		return -1;
	}
	std::vector<std::vector<double>> pyOut;

	for (int i = 0; i < pyOut.size(); i++)
	{
		pyOut[i].clear();
	}
	pyOut.clear();

	parseTuple(pResult, pyOut);
	for (int i = 0; i < pyOut.size(); i++)
	{
		for (int j = 0; j < pyOut[i].size();j++)
		{
			printf("%f  ", pyOut[i][j]);
		}
		printf("\n");
	}
	

	Py_DECREF(pResult);
	Py_DECREF(pArgs);
	return 1;

}