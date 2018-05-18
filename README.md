该示例主要演示了c++/c在调用python时进行的是数据通信


当处于debug状态下，会报如下错误：
error LNK2019: 无法解析的外部符号 __imp__Py_NegativeRefcount，该符号在函数 "public: int __cdecl CPyHSJ::CallFunc(wchar_t *,int,char * *,int *)" (?CallFunc@CPyHSJ@@QEAAHPEA_WHPEAPEADPEAH@Z) 中被引用
error LNK2001: 无法解析的外部符号 __imp__Py_RefTotal

引起原因是：
1 注释掉object.h第56行
//#define Py_TRACE_REFS
2 注释掉pyconfig.h 375行
//#define Py_DEBUG


Release下面运行情况正常不会出现上述错误
