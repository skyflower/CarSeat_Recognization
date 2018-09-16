#pragma once

#include <iostream>

#ifdef OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif // OPENCV

#include <vector>
#include <unordered_map>
#include "TypeIDManager.h"

class CPatternManager
{
public:
	enum class ImageType
	{
		IMAGE_BACKREST,
		IMAGE_CUSHION
	};
	typedef size_t PatternID;

	~CPatternManager();

	bool AddPatternImg(const wchar_t *file, const wchar_t *descriptor, CPatternManager::ImageType type = ImageType::IMAGE_BACKREST);
#ifdef OPENCV
	bool AddPatternImg(cv::Mat& img, const wchar_t *descriptor, CPatternManager::ImageType type = ImageType::IMAGE_BACKREST);
	cv::Mat GetPatternByIndex(size_t index, CPatternManager::ImageType type);
	cv::Mat GetPatternByDescriptor(const wchar_t *descriptor, CPatternManager::ImageType type);

#endif // OPENCV

	
	bool AddPatternPath(const wchar_t *file, const wchar_t *descriptor);

	size_t GetPatternCount(CPatternManager::ImageType type);
	std::wstring GetDescriptorByIndex(size_t index, CPatternManager::ImageType type);

	static CPatternManager *GetInstance();

private:

	static CPatternManager * m_pInstance;
	CTypeIDManager *m_pTypeIDManager;

	CPatternManager();
	
#ifdef OPENCV

	std::unordered_multimap<CTypeIDManager::typeID, cv::Mat> *m_pPatternImg;
	std::unordered_multimap<CTypeIDManager::typeID, cv::Mat> *m_pPatternBackRest;
	std::unordered_multimap<CTypeIDManager::typeID, cv::Mat> *m_pPatternCushion;

#endif // OPENCV

	
};

