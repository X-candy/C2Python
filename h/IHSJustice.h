#pragma once
#ifndef __libICOLORDETECTION__CILibColorDetection__
#define __libICOLORDETECTION__CILibColorDetection__

#include <stdio.h>
#include <vector>
#include "HSType.h"

using namespace std;

#ifdef __cplusplus
#ifndef DLL_EXPORT_C_DECL
#define DLL_EXPORT_C_DECL extern "C" __declspec(dllexport)
#endif

#ifndef DLL_IMPORT_C_DECL
#define DLL_IMPORT_C_DECL extern "C" __declspec(dllimport)
#endif

#ifndef DLL_EXPORT_DECL
#define DLL_EXPORT_DECL extern __declspec(dllexport)
#endif

#ifndef DLL_IMPORT_DECL
#define DLL_IMPORT_DECL extern __declspec(dllimport)
#endif

#ifndef DLL_EXPORT_CLASS_DECL
#define DLL_EXPORT_CLASS_DECL __declspec(dllexport)
#endif

#ifndef DLL_IMPORT_CLASS_DECL
#define DLL_IMPORT_CLASS_DECL __declspec(dllimport)
#endif

#else
#ifndef DLL_EXPORT_DECL
#define DLL_EXPORT_DECL __declspec(dllexport)
#endif

#ifndef DLL_IMPORT_DECL
#define DLL_IMPORT_DECL __declspec(dllimport)
#endif
#endif

#define LIBHSJ_EXPORTS
#ifdef LIBHSJ_EXPORTS
#define LIBHSJ_CLASS    DLL_EXPORT_CLASS_DECL
#define LIBHSJ_API      DLL_EXPORT_DECL
#else
#define LIBHSJ_CLASS    DLL_IMPORT_CLASS_DECL
#define LIBHSJ_API      DLL_IMPORT_DECL
#endif

typedef struct CHSJParams
{
	//图像宽
	int Img_nWidth;
	//图像高
	int Img_nHeight;

	//兴趣区域，如果是全图像处理，输入全图像的四个顶点
	HSPoint vecROI[32];
	int nVecROI;
	//报警敏感度
	int nSensity;

	HSRect maxFilter;
	HSRect minFilter;


	CHSJParams()
	{
		Img_nWidth = 0;
		Img_nHeight = 0;

		memset(vecROI, 0, sizeof(HSPoint) * 32);
		memset(&maxFilter, 0, sizeof(HSRect));
		memset(&minFilter, 0, sizeof(HSRect));
		nVecROI = 0;
		nSensity = 0;
	}
};

typedef struct CHSJReturn
{
	HSRect reportRc[32];
	int nReportRc;

	unsigned char* pImg_data;
	int nImg_lens;

	CHSJReturn()
	{
		memset(reportRc, 0, sizeof(HSRect) * 32);
		nReportRc = 0;
		pImg_data = NULL;
		nImg_lens = 0;
	}
};


class LIBHSJ_CLASS CILibHSJ
{
public:
	virtual int FillBuffer(unsigned char* _ImgData, int _nImgLens, CHSJReturn* _report) = 0;
	virtual int SetParams(CHSJParams _params) = 0;
};

LIBHSJ_API CILibHSJ* CreatelibHSJ();
LIBHSJ_API int DestorylibHSJ(CILibHSJ* _pHSJ);
#endif