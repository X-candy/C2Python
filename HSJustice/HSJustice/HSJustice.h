#pragma once
#include "../../h/IHSJustice.h"
#include "PyHSJ.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define  RESIZE_SCALE 1


#define  OPENCV_VERSION 310

#if _DEBUG
#if(OPENCV_VERSION==310)
#pragma comment(lib, "opencv_world310d.lib")
#else
#pragma comment(lib, "opencv_world300d.lib")
#endif // OPENCV_VERSION
#else
#if(OPENCV_VERSION==310)
#pragma comment(lib, "opencv_world310.lib")
#else
#pragma comment(lib, "opencv_world300.lib")
#endif // OPENCV_VERSION 
#endif

extern "C"
{
#include "Python.h"  
}

#if _DEBUG
#pragma comment(lib, "python35_d.lib")
#else
#pragma comment(lib, "python35.lib")
#endif

class CHSJ:public CILibHSJ
{
public:
	CHSJ();
	~CHSJ();
	int FillBuffer(unsigned char* _ImgData, int _nImgLens, CHSJReturn* _report);
	int SetParams(CHSJParams _params);
	int SetRoi(vector<Point> _vecROI);
	int PackReturn(CHSJReturn* _report);

	Mat m_InputImg;
	int m_nSensity;
	int m_nImgWidth;
	int m_nImgHeight;

	Mat m_ProcessImg;
	Mat m_Input;
	Mat m_ROIMask;
	int m_ProcessWidth;
	int m_ProcessHeight;

	Rect m_maxFilter;
	Rect m_minFilter;
	int m_nFrameCounts;


	Rect m_ReportRC[32];
	int m_nReportRCCount;

	unsigned char* m_pJpgData;

	CPyHSJ m_IPyHSJ;
};


LIBHSJ_API CILibHSJ* CreatelibHSJ()
{
	CILibHSJ* pILibHSJ = (CILibHSJ*)new CHSJ();

	return pILibHSJ;
}

LIBHSJ_API int DestorylibHSJ(CILibHSJ* _pILibHSJ)
{
	CHSJ* plibHSJ = (CHSJ*)_pILibHSJ;
	delete plibHSJ;
	return 0;
}