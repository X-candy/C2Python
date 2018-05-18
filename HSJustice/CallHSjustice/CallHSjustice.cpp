// RedDetection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../h/IHSJustice.h"
#if _DEBUG
#pragma comment(lib, "D:/HSProject/HSAlgorithm/Source/HSJustice/HSJustice/x64/Debug/HSJustice.lib")
#else
#pragma comment(lib, "D:/HSProject/HSAlgorithm/Source/HSJustice/HSJustice/x64/Release/HSJustice.lib")
#endif
int KeyBroadAction(int &step_flag)
{
	int c = cvWaitKey(1);
	if ((char)c == ' ' || step_flag != 0)
	{
		while (1)
		{
			int next = cvWaitKey(40);
			if ((char)next == ' ')
			{
				step_flag = 1;
				break;
			}
			if ((char)next == 'g')
			{
				step_flag = 0;
				break;
			}
			if ((char)next == 27)
			{
				c = next;
				break;
			}
		}
	}
	if ((char)c == 27/*|| NULL == cvGetWindowHandle("Video")*/)
	{
		return -1;
	}
	return 0;
}

Point StartPt = Point(0, 0);
Point EndPt = Point(0, 0);
vector<Point> roiVec;
Mat g_frame;
bool bIsSetRect = false;
void onMouse(int event, int x, int y, int, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		cout << x << "," << y << endl;
		StartPt = Point(x, y);
		bIsSetRect = false;
	}

	if (event == CV_EVENT_LBUTTONUP)
	{
		cout << x << "," << y << endl;
		EndPt = Point(x, y);
		bIsSetRect = true;
	}
	if (bIsSetRect == true)
	{
		return;
	}

	Mat Frame;
	g_frame.copyTo(Frame);
	cv::rectangle(Frame, StartPt, Point(x, y), CV_RGB(255, 0, 0));
	//imshow("frame", Frame);
	//waitKey(1);
}


char* WChar2Char(wchar_t* _strWChar)
{
	int iSize = WideCharToMultiByte(CP_ACP, 0, _strWChar, -1, NULL, 0, NULL, NULL);
	char* strChar = (char*)malloc(iSize*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, _strWChar, -1, strChar, iSize, NULL, NULL);
	return strChar;
}


int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR szFilePath[100000];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	cout << szFilePath << endl;

	long jumped = 0;
	VideoCapture videoCapture;
	argv[1] =L"E:\\1.录像\\红色视频/大厅.mp4", jumped = 180;

	videoCapture.open(WChar2Char(argv[1]));
	//videoCapture.open(0);
	if (!videoCapture.isOpened())
	{
		cout << "打开文件错误" << endl;
		system("pause");
		return 0;
	}
	videoCapture.set(CV_CAP_PROP_POS_FRAMES, jumped);

	Mat frame;
	videoCapture >> g_frame;
	//imshow("frame", g_frame);

	long nFrameCounts = 0;
	int step_flag = 0;

	//////////////////////////////////////////////////////////////////////////
	CILibHSJ * pLibHSJ = CreatelibHSJ();
	CHSJParams params;
	params.Img_nWidth = 352;
	params.Img_nHeight = 288;
	params.nSensity = 10;

	params.maxFilter.n = 1;
	params.maxFilter.xmin = 5;
	params.maxFilter.xmax = 500;
	params.maxFilter.ymin = 5;
	params.maxFilter.ymax = 200;

	params.minFilter.n = 1;
	params.minFilter.xmin = 5;
	params.minFilter.xmax = 35;
	params.minFilter.ymin = 5;
	params.minFilter.ymax = 35;

	pLibHSJ->SetParams(params);

	CHSJReturn report;


	//////////////////////////////////////////////////////////////////////////
	while (1)
	{
		videoCapture >> frame;
		if (frame.empty())
		{
			system("pause");
			break;
		}

		resize(frame, frame, Size(352, 288));
		//cvtColor(frame, testMat, CV_BGR2RGB);

		int64 starttick = getTickCount();
		////////////////////处理入口///////////////////////////////////////////////////

		int nLens = frame.dataend - frame.datastart;
		unsigned char* imgData = frame.data;

		pLibHSJ->FillBuffer(imgData, nLens, &report);

		///////////////////////////////////////////////////////////////////////
		int64 endtick = getTickCount();
		int64 nCostTime = endtick - starttick;
		nCostTime = nCostTime / 1000;
		cout << "nFrameCounts----" << nFrameCounts \
			<< "--nCostTime-------" << nCostTime \
			<< "ms" << endl;


		if (KeyBroadAction(step_flag) < 0)	break;
		nFrameCounts++;
	}
	return 0;

}
