// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <Windows.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <time.h>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;


#ifdef _FPCLASS_SNAN
#define NaN sqrt(-1.0)
#define IsNaN(x) _isnan(x)
#else
#define NaN (0.0 / 0.0)
#define IsNaN(x) ((x) != (x))
#endif
#ifdef _FPCLASS_NINF
#define INF DBL_MAX
#define PINF DBL_MAX
#define NINF -DBL_MAX
#define IsINF(x) _finite(x)
#else
#define INF	-log(0) //�����
#define PINF	INF	//�������
#define NINF	-INF	//�������
#define isINF(x)	(((x)==PINF)||((x)==NINF))
#endif
#define DIVIDE_EPS ((double)1e-16)


#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

#define  OPENCV_VERSION 310

#if _DEBUG
#if(OPENCV_VERSION==310)
#pragma comment(lib, "opencv_world310d.lib")
#else
#pragma comment(lib, "opencv_world300d.lib")
#endif // OPENCV_VERSION

#else

#if(OPENCV_VERSION==310)
//#pragma comment(lib, "opencv_video310.lib")
//#pragma comment(lib, "opencv_core310.lib")
//#pragma comment(lib, "opencv_imgproc310.lib")
//#pragma comment(lib, "opencv_highgui310.lib")
//#pragma comment(lib, "opencv_imgcodecs310.lib")
//#pragma comment(lib, "opencv_videoio310.lib")
#pragma comment(lib, "opencv_world310.lib")
#else
#pragma comment(lib, "opencv_world300.lib")
#endif // OPENCV_VERSION 

#endif

#define RETURN_SUCESS 1
#define RETURN_ERROR  0
#define CONST_EPS 0.0000000000001 //��ֹ��0