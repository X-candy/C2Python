��ʾ����Ҫ��ʾ��c++/c�ڵ���pythonʱ���е�������ͨ��


������debug״̬�£��ᱨ���´���
error LNK2019: �޷��������ⲿ���� __imp__Py_NegativeRefcount���÷����ں��� "public: int __cdecl CPyHSJ::CallFunc(wchar_t *,int,char * *,int *)" (?CallFunc@CPyHSJ@@QEAAHPEA_WHPEAPEADPEAH@Z) �б�����
error LNK2001: �޷��������ⲿ���� __imp__Py_RefTotal

����ԭ���ǣ�
1 ע�͵�object.h��56��
//#define Py_TRACE_REFS
2 ע�͵�pyconfig.h 375��
//#define Py_DEBUG


Release������������������������������
