#include "HSJustice.h"


CHSJ::CHSJ()
{
	wchar_t strPythonPath[1024] = L"C:/Users/xcandy/AppData/Local/conda/conda/envs/tensorflow";
	wchar_t strPythonExePath[1024] = L"../../PythonExe";
	wchar_t strPythonExeName[1024] = L"pytest";
	m_IPyHSJ.Initial(strPythonExeName, strPythonExePath, strPythonExeName);
}


CHSJ::~CHSJ()
{
}

int CHSJ::PackReturn(CHSJReturn* _report)
{
	_report->nReportRc = m_nReportRCCount;
	if (_report->nReportRc > 0)
	{
		Mat drawMat;
		m_Input.copyTo(drawMat);
		for (int i = 0; i < m_nReportRCCount; i++)
		{
			_report->reportRc[i].xmin = m_ReportRC[i].x * RESIZE_SCALE;
			_report->reportRc[i].xmax = (m_ReportRC[i].x + m_ReportRC[i].width)* RESIZE_SCALE;
			_report->reportRc[i].ymin = m_ReportRC[i].y* RESIZE_SCALE;
			_report->reportRc[i].ymax = (m_ReportRC[i].y + m_ReportRC[i].height)* RESIZE_SCALE;


			rectangle(drawMat, Rect(_report->reportRc[i].xmin, _report->reportRc[i].ymin,
				_report->reportRc[i].xmax - _report->reportRc[i].xmin, _report->reportRc[i].ymax - _report->reportRc[i].ymin),
				CV_RGB(255, 255, 0), 2);
		}

		vector<uchar> buff;//buffer for coding
		vector<int> param = vector<int>(2);
		param[0] = CV_IMWRITE_JPEG_QUALITY;
		param[1] = 95;//default(95) 0-100

		imencode(".jpg", drawMat, buff, param);


		if (m_pJpgData != NULL)
		{
			free(m_pJpgData);
		}
		_report->nImg_lens = buff.size();

		m_pJpgData = (uchar*)malloc(buff.size());
		memcpy(_report->pImg_data, buff.data(), buff.size());
		_report->pImg_data = m_pJpgData;

		if (_report->nImg_lens == 0)
		{
			printf("===========||||======imgsize:%d ,%d============\n", buff.size(), _report->nImg_lens);
		}
	}
	return 0;
}


int CHSJ::FillBuffer(unsigned char* _ImgData, int _nImgLens, CHSJReturn* _report)
{
	m_nFrameCounts++;

	memcpy(m_InputImg.data, _ImgData, _nImgLens);
	resize(m_InputImg, m_ProcessImg, cvSize(m_ProcessWidth, m_ProcessHeight));


#ifndef _DEBUG
	flip(m_ProcessImg, m_ProcessImg, -1);
	flip(m_ProcessImg, m_ProcessImg, 1);
#endif 
	//Scalar low(0, 0, 255);
	//Scalar high(255, 255, 255);
	//ShowColorBar(low, high);
	//Scalar high1(0, 0, 0);
	//ShowColorBar(low, high1);

	imshow("Input", m_ProcessImg);
	m_ProcessImg.copyTo(m_Input);
	bitwise_and(m_ProcessImg, m_ROIMask, m_ProcessImg);

	Rect rc;
	int nRc = 0;
	//DetectionRedRegion(m_ProcessImg, m_ReportRC, m_nReportRCCount);

	wchar_t strPythonFuncName[1024] = L"Test";
	char* arrArgv[10] = { 0 };
	int argv_lens[10] = { 0 };
	int argcCount = 1;

	arrArgv[argcCount - 1] = (char*)m_ProcessImg.data;
	argv_lens[argcCount - 1] = m_ProcessImg.dataend - m_ProcessImg.datastart;
	m_IPyHSJ.CallFunc(strPythonFuncName, argcCount, arrArgv, argv_lens);
	PackReturn(_report);

	return 1;
}


int CHSJ::SetParams(CHSJParams _params)
{
	m_nImgWidth = _params.Img_nWidth;
	m_nImgHeight = _params.Img_nHeight;

	m_InputImg = Mat(m_nImgHeight, m_nImgWidth, CV_8UC3);

	m_ProcessWidth = m_nImgWidth / RESIZE_SCALE;
	m_ProcessHeight = m_nImgHeight / RESIZE_SCALE;
	//resize(m_InputImg, m_ProcessImg, m_InputImg.size() / 2);
	resize(m_InputImg, m_ProcessImg, cvSize(m_ProcessWidth, m_ProcessHeight));

	vector<Point> roi_point;

	for (int i = 0; i < (int)_params.nVecROI; i++)
	{
		Point temp_pt = Point(_params.vecROI[i].x, _params.vecROI[i].y);
		cout << "I" << i << "," << temp_pt << endl;
		roi_point.push_back(temp_pt);
	}

	SetRoi(roi_point);

	m_nSensity = _params.nSensity;
	cout << "m_nSensity = " << m_nSensity << endl;

	if (_params.maxFilter.n != 0)
	{
		m_maxFilter.x = _params.maxFilter.xmin / RESIZE_SCALE;
		m_maxFilter.y = _params.maxFilter.ymin / RESIZE_SCALE;
		m_maxFilter.width = (_params.maxFilter.xmax - _params.maxFilter.xmin) / RESIZE_SCALE;
		m_maxFilter.height = (_params.maxFilter.ymax - _params.maxFilter.ymin) / RESIZE_SCALE;
	}


	if (_params.minFilter.n != 0)
	{
		m_minFilter.x = _params.minFilter.xmin / RESIZE_SCALE;
		m_minFilter.y = _params.minFilter.ymin / RESIZE_SCALE;
		m_minFilter.width = (_params.minFilter.xmax - _params.minFilter.xmin) / RESIZE_SCALE;
		m_minFilter.height = (_params.minFilter.ymax - _params.minFilter.ymin) / RESIZE_SCALE;
	}

	return 1;
}

int CHSJ::SetRoi(vector<Point> _vecROI)
{
	m_ROIMask = Mat::zeros(m_ProcessImg.rows, m_ProcessImg.cols, CV_8UC3);

	if (_vecROI.size() == 0)
	{
		m_ROIMask.setTo(255);
		return 1;
	}


	Mat temp_mat = Mat::zeros(m_ProcessImg.rows, m_ProcessImg.cols, CV_8UC1);
	int nVecROISize = _vecROI.size();
	for (int i = 0; i < nVecROISize; i++)
	{
		line(temp_mat, _vecROI[i%_vecROI.size()], _vecROI[(i + 1) % _vecROI.size()], Scalar(255, 255, 255), 2);
	}

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(temp_mat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(m_ROIMask, contours, i, CV_RGB(255, 255, 255), CV_FILLED);
	}

	return 1;
}

