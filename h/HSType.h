
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef _HS_ALGO
#define _HS_ALGO
//Point Structure
typedef struct HSPoint
{
	int x;
	int y;
	HSPoint()
	{
		x=0;
		y=0;
	}
}HSPoint;
//Rectangle Structure
typedef struct HSRect
{
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	int n; 
}HSRect;
//Image Structure
typedef struct HSIMG
{
	int width;
	int height;
	int size;
	int channel;
	int offset;
	int bn;
	unsigned long frametime;
	unsigned char* data;
}HSIMG;

typedef struct HS_DEBUG_RECT{
	int nLineWeight;
	int nColorR;
	int nColorG;
	int nColorB;
	int nRectCount;
	HSRect hsRect[32];
}HS_DEBUG_RECT;

typedef struct HS_DEBUG_POINT{
	int nLineWeight;
	int nColorR;
	int nColorG;
	int nColorB;
	int nPointCount;
	HSPoint hsPoint[32];
}HS_DEBUG_POINT;


//10代表类型个数
typedef struct HS_DEBUG_PARAM
{
	int nDebugRect;
	HS_DEBUG_RECT struDebugRect[10];
	int nDebugPoint;
	HS_DEBUG_POINT struDebugPoint[10];
}HS_DEBUG_PARAM;


//Image Type
enum HS_IMGSTRUCT
{
	HS_CIF = 0,//size:352*288
	HS_QCIF = 1,//size:176*144
	HS_QQCIF = 2,//size:88*72
};
#endif

