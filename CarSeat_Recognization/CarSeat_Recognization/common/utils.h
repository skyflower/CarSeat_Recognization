#pragma once
#include <iostream>
#include <Windows.h>

#ifdef OPENCV
#include <opencv2\core\core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif // OPENCV




#define MAX_CHAR_LENGTH 256


namespace utils
{

	char* WcharToChar(wchar_t* wc);
	wchar_t* CharToWchar(char* c);
	std::wstring StrToWStr(const std::string str);
	std::string WStrToStr(const std::wstring wstr);

#ifdef OPENCV
	void showImageHist(const cv::Mat & img);

	cv::Rect splitFrameGrayImage(const cv::Mat &img);

	cv::Rect calcRect(const std::vector<cv::Point2f> &point);

	double calcArea(const cv::Rect &rect);

	double calcAvgPixel(const cv::Mat &img, int minPixelValue, int maxPixelValue);

#endif // OPENCV

};

