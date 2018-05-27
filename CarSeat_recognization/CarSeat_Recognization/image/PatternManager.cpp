#include "PatternManager.h"

#ifdef OPENCV
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/core/core.hpp>
#endif // OPENCV

#include "BaseMatch.h"
#include "../common/utils.h"
#include <Shlwapi.h>

CPatternManager *CPatternManager::m_pInstance = nullptr;

CPatternManager *CPatternManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CPatternManager;
	}
	return m_pInstance;
}

CPatternManager::CPatternManager()
{
#ifdef OPENCV
	m_pPatternBackRest = new std::unordered_multimap<CTypeIDManager::typeID, cv::Mat>;
	m_pPatternCushion = new std::unordered_multimap<CTypeIDManager::typeID, cv::Mat>;
	//m_pPatternImg = new std::unordered_multimap<CTypeIDManager::typeID, cv::Mat>;

	m_pPatternImg = nullptr;
#endif // OPENCV

	m_pTypeIDManager = CTypeIDManager::GetInstance();
}


CPatternManager::~CPatternManager()
{
#ifdef OPENCV
	if (m_pPatternBackRest)
	{
		m_pPatternBackRest->clear();
		delete m_pPatternBackRest;
		m_pPatternBackRest = nullptr;
	}
	if (m_pPatternCushion)
	{
		m_pPatternCushion->clear();
		delete m_pPatternCushion;
		m_pPatternCushion = nullptr;
	}
	if (m_pPatternImg)
	{
		m_pPatternImg->clear();
		delete m_pPatternImg;
		m_pPatternImg = nullptr;
	}
#endif // OPENCV

	
}

bool CPatternManager::AddPatternImg(const wchar_t *file, const wchar_t *descriptor, CPatternManager::ImageType type)
{
	//char *m_char = nullptr;
	std::wstring tmpWStr(file);
#ifdef OPENCV
	try
	{

		std::string tmpStr = utils::WStrToStr(tmpWStr);

		cv::Mat tmpPattern = cv::imread(tmpStr.c_str());
		cv::Mat tmpObject;
		if ((tmpPattern.channels() == 3) || (tmpPattern.channels() == 4))
		{
			cv::cvtColor(tmpPattern, tmpObject, CV_BGR2GRAY);
		}
		
		int scaleFactor =  CBaseMatch::GetScaleFactor();
		cv::Mat patternMat;
		cv::resize(tmpObject, patternMat, cv::Size(tmpObject.cols / scaleFactor, tmpObject.rows / scaleFactor), 0, 0, cv::INTER_LANCZOS4);
		AddPatternImg(patternMat, descriptor, type);


		
		return true;
	}
	catch (const cv::Exception & exec)
	{
		std::cout << "AddPattern Failed: " << utils::WStrToStr(tmpWStr).c_str() << std::endl;
		return false;
	}
#endif // OPENCV
	return false;
}

#ifdef OPENCV
bool CPatternManager::AddPatternImg(cv::Mat& img, const wchar_t *descriptor, CPatternManager::ImageType type)
{
	std::unordered_multimap<CTypeIDManager::typeID, cv::Mat> *pInsert = nullptr;;
	if (type == ImageType::IMAGE_BACKREST)
	{
		if (m_pPatternBackRest == nullptr)
		{
			m_pPatternBackRest = new std::unordered_multimap<CTypeIDManager::typeID, cv::Mat>;
		}
		pInsert = m_pPatternBackRest;
	}
	else if(type == ImageType::IMAGE_CUSHION)
	{
		if (m_pPatternCushion == nullptr)
		{
			m_pPatternCushion = new std::unordered_multimap<CTypeIDManager::typeID, cv::Mat>;
		}
		pInsert = m_pPatternCushion;
	}
	else
	{
		return false;
	}

	cv::Rect rect = utils::splitFrameGrayImage(img);
	std::wstring descStr(descriptor);
	if (m_pTypeIDManager == nullptr)
	{
		m_pTypeIDManager = CTypeIDManager::GetInstance();
	}
	CTypeIDManager::typeID id = m_pTypeIDManager->GetTypeID(descStr);
	if (id == 0)
	{
		id = m_pTypeIDManager->AddType(descStr);
	}
	pInsert->insert(std::make_pair(id, img(rect)));

	return false;
}
#endif // OPENCV

bool CPatternManager::AddPatternPath(const wchar_t * file, const wchar_t * descriptor)
{
	wchar_t tmpFileName[256] = { 0 };
	swprintf_s(tmpFileName, L"%s\\model.jpg", file);

	if(PathFileExists(tmpFileName))
	{
		AddPatternImg(tmpFileName, descriptor, ImageType::IMAGE_BACKREST);
	}

	memset(tmpFileName, 0, sizeof(wchar_t) * 256);
	swprintf_s(tmpFileName, L"%s\\cushion_model.jpg", file);
	
	if (PathFileExists(tmpFileName))
	{
		AddPatternImg(tmpFileName, descriptor, ImageType::IMAGE_CUSHION);
	}
	return true;
}

size_t CPatternManager::GetPatternCount(CPatternManager::ImageType type)
{
#ifdef OPENCV
	if (type == ImageType::IMAGE_BACKREST)
	{
		return m_pPatternBackRest->size();
	}
	else if(type == ImageType::IMAGE_CUSHION)
	{
		return m_pPatternCushion->size();
	}
#endif // OPENCV
	return 0;
}

#ifdef OPENCV

cv::Mat CPatternManager::GetPatternByIndex(size_t index, CPatternManager::ImageType type)
{
	size_t i = 0;
	std::unordered_multimap<CTypeIDManager::typeID, cv::Mat> *tmpPointer = nullptr;
	if (type == ImageType::IMAGE_BACKREST)
	{
		tmpPointer = m_pPatternBackRest;
	}
	else if (type == ImageType::IMAGE_CUSHION)
	{
		tmpPointer = m_pPatternCushion;
	}
	else
	{
		return cv::Mat();
	}
	for (auto iter = tmpPointer->begin(); iter != tmpPointer->end(); iter++)
	{
		if (i == index)
		{
			return iter->second;
		}
		++i;
	}
	return cv::Mat();
}

cv::Mat CPatternManager::GetPatternByDescriptor(const wchar_t *descriptor, CPatternManager::ImageType type)
{
	std::unordered_multimap<CTypeIDManager::typeID, cv::Mat> *tmpPointer = nullptr;
	if (type == ImageType::IMAGE_BACKREST)
	{
		tmpPointer = m_pPatternBackRest;
	}
	else if (type == ImageType::IMAGE_CUSHION)
	{
		tmpPointer = m_pPatternCushion;
	}
	else
	{
		return cv::Mat();
	}
	if ((descriptor == nullptr) || (tmpPointer == nullptr) || (tmpPointer->empty()))
	{
		return cv::Mat();
	}
	std::wstring tmpStr(descriptor);
	CTypeIDManager::typeID id = m_pTypeIDManager->GetTypeID(tmpStr);

	for (auto iter = tmpPointer->begin(); iter != tmpPointer->end(); iter++)
	{
		if (iter->first == id)
		{
			return iter->second;
		}
	}
	return cv::Mat();
}
#endif // OPENCV

std::wstring CPatternManager::GetDescriptorByIndex(size_t index, CPatternManager::ImageType type)
{
#ifdef OPENCV
	std::unordered_multimap<CTypeIDManager::typeID, cv::Mat> *tmpPointer = nullptr;
	if (type == ImageType::IMAGE_BACKREST)
	{
		tmpPointer = m_pPatternBackRest;
	}
	else if (type == ImageType::IMAGE_CUSHION)
	{
		tmpPointer = m_pPatternCushion;
	}
	else
	{
		return std::wstring();
	}
	if ((tmpPointer == nullptr) || (tmpPointer->size() < index))
	{
		return std::wstring();
	}
	
	size_t i = 0;
	CTypeIDManager::typeID id = 0;
	for (auto iter = tmpPointer->begin(); iter != tmpPointer->end(); iter++)
	{
		if (i == index)
		{
			id = iter->first;
		}
		++i;
	}
	if (id != 0)
	{
		return m_pTypeIDManager->GetTypeString(id);
	}
#endif // OPENCV
	return std::wstring();
}
