#pragma once
#pragma once
#include <Shlwapi.h> 
#include <iostream>  
#include <process.h>  
#include <Shlwapi.h> 
#include <vector>
#pragma comment(lib, "Shlwapi.lib") 
extern "C"
{
#include "Python.h"  
}

class CPyHSJ
{
public:
	CPyHSJ();
	~CPyHSJ();

	PyObject *m_pModule;
	PyObject *m_pFunc;
	bool m_bIsInitial;

	wchar_t* m_strProgramName;
	wchar_t* m_PythonHome;

	char* m_strPythonName;

	int Initial(wchar_t* _strProgramName, wchar_t* _strPythonExePath, wchar_t* _strPythonExeName);

	int CallFunc(wchar_t* _strFuncName, int _args, char** _argv, int* _argv_lens);
	int parseList(PyObject *_pyList, std::vector<std::vector<double>>  &_out);
	int parseTuple(PyObject *_pyTuple, std::vector<std::vector<double>> &_out);

	char* WChar2Char(wchar_t* _strWChar);
};

