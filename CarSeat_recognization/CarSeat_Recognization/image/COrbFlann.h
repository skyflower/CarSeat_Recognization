#pragma once
#include <iostream>
#ifdef OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/legacy/legacy.hpp>
#endif //OPENCV
#include "BaseMatch.h"
#include "PatternManager.h"
#include <ctime>


class COrbFlannMatch : public CBaseMatch
{
public:
	
	~COrbFlannMatch();
	std::wstring match(const wchar_t *testFile);

	static COrbFlannMatch *GetInstance();

	void SetDisplayStatus(bool status);
	bool GetDisplayStatus();
	void SetCurImageType(CPatternManager::ImageType type);
	CPatternManager::ImageType GetCurImageType();

private:
	static COrbFlannMatch * m_pInstance;

	bool preMatch();

#ifdef OPENCV
	double _auxMatch(size_t srcIndex, const cv::Mat &testImg);

	std::unordered_map<CTypeIDManager::typeID, double> PatternImage(const cv::Mat &testImg);

	bool testImageHist(const cv::Mat &srcImg, const cv::Mat &testImg);
	bool testArea(const cv::Rect &srcRect, const cv::Rect & testRect);

	std::vector<std::vector<cv::KeyPoint>*>* m_pPatternKeyPoints;
	std::vector<cv::Mat> *m_pPatternDesc;

	

	cv::Mat preProcess(const cv::Mat& src);

	std::vector<cv::DMatch> filter_distance(cv::Mat descriptors, std::vector< cv::DMatch > matches);
	
	cv::ORB detector;
	cv::OrbDescriptorExtractor extractor;
#endif // OPENCV

	bool m_bDisplay;
	CPatternManager::ImageType m_pCurType;

private:
	COrbFlannMatch();
};

