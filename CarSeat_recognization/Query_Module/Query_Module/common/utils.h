#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <unordered_map>


#define MAX_CHAR_LENGTH 256


namespace utils
{

	char* WcharToChar(wchar_t* wc);
	wchar_t* CharToWchar(char* c);
	std::wstring StrToWStr(const std::string str);
	std::string WStrToStr(const std::wstring wstr);

	bool delBlankSpace(char *text, size_t len);

	bool readFile(char *file, char *&content, size_t &FileSize);


	// XXX={"AAA", "BBBB", "CCC"}
	bool parseVector(const char *content, const char*name, std::vector<std::wstring> *pVector);
	
	//
	bool parseLineSegment(const char *name, size_t, std::vector<std::wstring> *pVec);
	
	//XXX=192.168.1.153
	int parseIp(const char *content, const char *name);
	
	// XXX={"AAA":"MMM", "BBB":"NNN", "CCC":"LLL"}
	int parseMap(const char*content, const char*name, std::unordered_map<std::wstring, std::wstring>*pMap);

	// XXX=AAA
	bool getValueByName(const char *content, const char*name, char*value);

#ifdef OPENCV
	void showImageHist(const cv::Mat & img);

	cv::Rect splitFrameGrayImage(const cv::Mat &img);

	cv::Rect calcRect(const std::vector<cv::Point2f> &point);

	double calcArea(const cv::Rect &rect);

	double calcAvgPixel(const cv::Mat &img, int minPixelValue, int maxPixelValue);

#endif // OPENCV

};
