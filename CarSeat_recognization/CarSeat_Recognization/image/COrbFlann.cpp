#include "COrbFlann.h"
#include "PatternManager.h"
#include <mutex>
#include <fstream>
#include "../common/utils.h"

#ifdef OPENCV
using namespace cv;
#endif //OPENCV
using namespace std;

COrbFlannMatch * COrbFlannMatch::m_pInstance = nullptr;

extern std::fstream Log;


COrbFlannMatch::COrbFlannMatch() :
	CBaseMatch(), m_bDisplay(false),
	m_pCurType(CPatternManager::ImageType::IMAGE_BACKREST)
{
#ifdef OPENCV
	m_pPatternKeyPoints = nullptr;
	m_pPatternDesc = nullptr;
	detector = ORB(1000, 2.0f, 8, 31, 0, 2, ORB::kBytes, 31);
	extractor = OrbDescriptorExtractor();
	// extractor;
#endif // OPENCV
}


COrbFlannMatch::~COrbFlannMatch()
{
#ifdef OPENCV
	if (m_pPatternKeyPoints != nullptr)
	{
		for (auto iter = m_pPatternKeyPoints->begin(); iter != m_pPatternKeyPoints->end(); ++iter)
		{
			(*iter)->clear();
			delete (*iter);
			(*iter) = nullptr;
		}
		m_pPatternKeyPoints->clear();
		delete m_pPatternKeyPoints;
		m_pPatternKeyPoints = nullptr;
	}
	if (m_pPatternDesc != nullptr)
	{
		m_pPatternDesc->clear();
		delete m_pPatternDesc;
		m_pPatternDesc = nullptr;
	}
#endif // OPENCV
}

COrbFlannMatch *COrbFlannMatch::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new COrbFlannMatch();
	}
	return m_pInstance;
}

void COrbFlannMatch::SetDisplayStatus(bool status)
{
	m_bDisplay = status;
}

bool COrbFlannMatch::GetDisplayStatus()
{
	return m_bDisplay;
}

void COrbFlannMatch::SetCurImageType(CPatternManager::ImageType type)
{
	m_pCurType = type;
}

CPatternManager::ImageType COrbFlannMatch::GetCurImageType()
{
	return m_pCurType;
}

bool COrbFlannMatch::preMatch()
{
#ifdef OPENCV
	if (m_pPatternKeyPoints == nullptr)
	{
		m_pPatternKeyPoints = new std::vector<std::vector<KeyPoint>*>;
	}
	if (m_pPatternDesc == nullptr)
	{
		m_pPatternDesc = new std::vector<cv::Mat>;
	}
	
	CPatternManager *pPatternManager = CPatternManager::GetInstance();
	size_t nPatternCount = pPatternManager->GetPatternCount(m_pCurType);

	for (size_t i = 0; i < nPatternCount; ++i)
	{
		cv::Mat patternImg = pPatternManager->GetPatternByIndex(i, m_pCurType);
		
		vector<KeyPoint> *keypoints_1 = new vector<KeyPoint>;
		Mat descriptors_1;

		detector.detect(patternImg, *keypoints_1);

		extractor.compute(patternImg, *keypoints_1, descriptors_1);
		m_pPatternKeyPoints->push_back(keypoints_1);
		m_pPatternDesc->push_back(descriptors_1);
	}
#endif // OPENCV
	return true;
}

#ifdef OPENCV

double COrbFlannMatch::_auxMatch(size_t srcIndex, const cv::Mat &testImg)
{
	cv::Mat srcImg = CPatternManager::GetInstance()->GetPatternByIndex(srcIndex, m_pCurType);
	//cv::imshow("srcImg:", srcImg);
	//cv::waitKey(0);

	size_t startTime = clock();
	if ((testImg.data == NULL)||	\
		(testImg.cols < CBaseMatch::GetImageMinSize()) ||	\
		(testImg.rows < CBaseMatch::GetImageMinSize()))
	{
		return INT_MAX;
	}

	
	//-- Step 1: Detect the keypoints using ORB Detector
	std::vector<cv::KeyPoint> *srcKeypoints = m_pPatternKeyPoints->at(srcIndex);
	cv::Mat * srcDesc = &m_pPatternDesc->at(srcIndex);

	std::vector<KeyPoint> keypoints_2;

	//detector.detect(img_1, keypoints_1);
	detector.detect(testImg, keypoints_2);
	if (keypoints_2.size() == 0)
	{
		std::cout << "keyPoints Size Error" << std::endl;
		Log << "keyPoints Size Error" << std::endl;
		return INT_MAX;
	}

	//cv::initModule_nonfree();
	//-- Step 2: Calculate descriptors 
	Mat descriptors_2;

	//extractor.compute(img_1, keypoints_1, descriptors_1);
	extractor.compute(testImg, keypoints_2, descriptors_2);
	if ((descriptors_2.rows == 0) || (descriptors_2.cols == 0))
	{
		std::cout << "compute descriptors Error" << std::endl;
		Log << "compute descriptors Error" << std::endl;
		return INT_MAX;
	}

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

	std::vector< DMatch > matches, matches2;
	matcher->match(*srcDesc, descriptors_2, matches);
	matcher->match(descriptors_2, *srcDesc, matches2);

	std::vector< DMatch > good_matches1, good_matches2, better_matches;
	good_matches1 = filter_distance(*srcDesc, matches);
	good_matches2 = filter_distance(descriptors_2, matches2);

	for (int i = 0; i<good_matches1.size(); i++)
	{
		DMatch temp1 = good_matches1[i];
		for (int j = 0; j < good_matches2.size(); j++)
		{
			DMatch temp2 = good_matches2[j];
			if (temp1.queryIdx == temp2.trainIdx && temp2.queryIdx == temp1.trainIdx) 
			{
				better_matches.push_back(temp1);
				break;
			}
		}
	}

	std::vector<Point2f> obj;
	obj.reserve(better_matches.size());
	std::vector<Point2f> scene;
	scene.reserve(better_matches.size());

	for (int i = 0; i < better_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back((*srcKeypoints)[better_matches[i].queryIdx].pt);
		scene.push_back(keypoints_2[better_matches[i].trainIdx].pt);
	}
	if ((obj.size() < 4) || (scene.size() < 4))
	{
		std::cout << "keyPoints less than 4" << std::endl;
		Log << "keyPoints less than 4" << std::endl;
		return INT_MAX;
	}
	//findFundamentalMat()
	Mat H;
	try
	{
		H = cv::findHomography(obj, scene, CV_RANSAC);
	}
	catch (const cv::Exception &exe)
	{
		std::cout << "findHomography Error" << std::endl;
		Log << "findHomography Error" << std::endl;
		return INT_MAX;
	}

	double tmpInterval = CBaseMatch::GetMaxInterval();
	double *pData = (double*)H.data;
	Log << pData[0] << "," << pData[1] << "," << pData[3] << "," << pData[4] << std::endl;
	std::cout << pData[0] << "," << pData[1] << "," << pData[3] << "," << pData[4] << std::endl;

	double tmpScaleFactor = abs(pData[0] - pData[4]) / 2;
	double tmpRotateFactor = max(abs(pData[3]), abs(pData[1]));
	double result = max(tmpScaleFactor, tmpRotateFactor);
	//std::cout << "Match result: = " << result << std::endl;
	//Log << "Match result: = " << result << std::endl;
	size_t endTime = clock();
	std::cout << "Match Over,Time: " << (endTime - startTime + 0.0001) / CLOCKS_PER_SEC << std::endl;


	cv::Rect srcRect;// = util::splitFrameGrayImage(srcImg);
	srcRect.x = srcRect.y = 0;
	srcRect.width = srcImg.cols;
	srcRect.height = srcImg.rows;

	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(srcRect.x, srcRect.y);
	obj_corners[1] = cvPoint(srcRect.x + srcRect.width, srcRect.y);
	obj_corners[2] = cvPoint(srcRect.x + srcRect.width, srcRect.y + srcRect.height);
	obj_corners[3] = cvPoint(srcRect.x, srcRect.y + srcRect.height);
	std::vector<Point2f> scene_corners(4);

	cv::perspectiveTransform(obj_corners, scene_corners, H);

	cv::Rect testRect = util::calcRect(scene_corners);
	if ((testRect.x < 0) || (testRect.y < 0) || (testRect.width < 0) || (testRect.height < 0) \
		|| (testRect.x + testRect.width > testImg.cols) \
		|| (testRect.y + testRect.height > testImg.rows))
	{
		std::cout << "perspectiveTransform Error" << std::endl;
		Log << "perspectiveTransform Error" << std::endl;
		return INT_MAX;
	}
	

	if (false == testImageHist(srcImg, testImg(testRect)))
	{
		std::cout << "testImageHist Error" << std::endl;
		Log << "testImageHist Error" << std::endl;
		return INT_MAX;
	}

	if (false == testArea(srcRect, testRect))
	{
		std::cout << "AreaError" << std::endl;
		Log << "AreaError" << std::endl;
		return INT_MAX;
	}

	if (m_bDisplay)
	{
		cv::Mat output;
		cv::drawMatches(srcImg, *srcKeypoints, testImg, keypoints_2, better_matches, output);
		line(output, scene_corners[0] + Point2f(srcImg.cols, 0), scene_corners[1] + Point2f(srcImg.cols, 0), Scalar(0, 255, 0), 4);
		line(output, scene_corners[1] + Point2f(srcImg.cols, 0), scene_corners[2] + Point2f(srcImg.cols, 0), Scalar(0, 255, 0), 4);
		line(output, scene_corners[2] + Point2f(srcImg.cols, 0), scene_corners[3] + Point2f(srcImg.cols, 0), Scalar(0, 255, 0), 4);
		line(output, scene_corners[3] + Point2f(srcImg.cols, 0), scene_corners[0] + Point2f(srcImg.cols, 0), Scalar(0, 255, 0), 4);

		imshow("Matches result", output);
		waitKey(0);
	}
	
	return result;
}

std::unordered_map<CTypeIDManager::typeID, double> COrbFlannMatch::PatternImage(const cv::Mat & testImg)
{
	std::unordered_map<CTypeIDManager::typeID, double> resultValue;
	if (m_pCurType == CPatternManager::ImageType::IMAGE_BACKREST)
	{
		Log << "backrestMatch" << std::endl;
	}
	else if (m_pCurType == CPatternManager::ImageType::IMAGE_CUSHION)
	{
		Log << "cushionMatch" << std::endl;
	}

	CPatternManager *pPatternManager = CPatternManager::GetInstance();
	size_t nPatternCount = pPatternManager->GetPatternCount(m_pCurType);
	double result = INT_MAX;

	CTypeIDManager *pIDManager =  CTypeIDManager::GetInstance();

	for (size_t i = 0; i < nPatternCount; ++i)
	{

		std::wstring patternStr = pPatternManager->GetDescriptorByIndex(i, m_pCurType);
		//if (patternStr != L"D2_black_pvc_hole_cloth")
		//{
		//	continue;
		//}
		std::cout << "oncePatternBegin\t" << util::WStrToStr(patternStr).c_str() << std::endl;
		Log << "oncePatternBegin\t" << util::WStrToStr(patternStr).c_str() << std::endl;

		//cv::Mat inputImg = preProcess(testImg);
		double tmpResult = _auxMatch(i, testImg);
		std::cout << "matchResult: " << tmpResult << std::endl;
		Log << "matchResult: " << tmpResult << std::endl;

		if (tmpResult < CBaseMatch::GetMaxInterval())
		{
			CTypeIDManager::typeID id = pIDManager->GetTypeID(patternStr);
			if (id == CTypeIDManager::typeID(0))
			{
				id = pIDManager->AddType(patternStr);
			}
			std::unordered_map<CTypeIDManager::typeID, double>::iterator iter = resultValue.find(id);
			if ((iter != resultValue.end()) && (iter->second > tmpResult))
			{
				iter->second = tmpResult;
			}
			else if (iter == resultValue.end())
			{
				resultValue.insert(std::make_pair(id, tmpResult));
			}
		}
	}
	return resultValue;
}

bool COrbFlannMatch::testImageHist(const cv::Mat & srcImg, const cv::Mat & testImg)
{
	float range[] = { 5, 250 };
	double srcAvgValue = util::calcAvgPixel(srcImg, range[0], range[1]);
	double testAvgValue = util::calcAvgPixel(testImg, range[0], range[1]);

	std::cout << "srcAvgValue: " << srcAvgValue << "\ttestAvgValue: " << testAvgValue << std::endl;
	Log << "srcAvgValue: " << srcAvgValue << "\ttestAvgValue: " << testAvgValue << std::endl;

	if (abs(srcAvgValue - testAvgValue) > CBaseMatch::GetHistLimit())
	{
		return false;
	}
	return true;
}

bool COrbFlannMatch::testArea(const cv::Rect &srcRect, const cv::Rect & testRect)
{
	double srcArea = srcRect.width * srcRect.height;
	double testArea = testRect.width * testRect.height;
	std::cout << "srcArea: " << srcArea << "\ttestArea: " << testArea << std::endl;
	if (abs(srcArea - testArea) > CBaseMatch::GetAreaLimit() * srcArea)
	{
		return false;
	}
	return true;
}

std::vector<DMatch> COrbFlannMatch::filter_distance(Mat descriptors, std::vector< DMatch > matches)
{
	double max_dist = 0; 
	double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors.rows; i++)  
	{
		double dist = matches[i].distance;
		if (dist < min_dist)
		{
			min_dist = dist;
		}
		if (dist > max_dist)
		{
			max_dist = dist;
		}
	}

	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors.rows; i++)
	{
		if (matches[i].distance <= max(5 * min_dist, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}
	return good_matches;
}

#endif // OPENCV

std::wstring COrbFlannMatch::match(const wchar_t *testFile)
{
	static std::once_flag oc;
	std::call_once(oc, [&]() { this->preMatch(); });

	if (testFile == nullptr)
	{
		std::cout << "parameters invalid, please check" << std::endl;
		return std::wstring();
	}

	// testImg;
	
	std::wstring tmpStr(testFile);
#ifdef OPENCV
	Mat img_scene;
	try
	{
		cv::Mat testImg = cv::imread(util::WStrToStr(tmpStr).c_str(), CV_LOAD_IMAGE_COLOR);

		Mat tmpScene;
		if ((testImg.channels() == 3) || (testImg.channels() == 4))
		{
			cvtColor(testImg, tmpScene, CV_BGR2GRAY);
		}
		if (!tmpScene.data)
		{
			std::cout << " --(!) Error reading images " << std::endl;
			return std::wstring();
		}

		size_t scaleFactor = CBaseMatch::GetScaleFactor();

		resize(tmpScene, img_scene, Size(tmpScene.cols / scaleFactor, tmpScene.rows / scaleFactor), 0, 0, INTER_LANCZOS4);

	}
	catch (const cv::Exception &exec)
	{
		return std::wstring();
	}
	//m_bDisplay = true;
	m_pCurType = CPatternManager::ImageType::IMAGE_BACKREST;
	std::unordered_map<CTypeIDManager::typeID, double> backrest = PatternImage(img_scene);
#endif // OPENCV
	//m_pCurType = CPatternManager::ImageType::IMAGE_CUSHION;
	//std::unordered_map<CTypeIDManager::typeID, double> cushion = PatternImage(img_scene);

	CTypeIDManager::typeID id = 0;
	double result = INT_MAX;
#ifdef OPENCV
	for (auto i : backrest)
	{
		//auto k = cushion.find(i.first);
		//if (k != cushion.end())
		{
			//double tmpResult = max(i.second, result);
			if (i.second < result)
			{
				id = i.first;
				result = i.second;
			}
		}
	}
#endif // OPENCV
	std::wstring outputStr = CTypeIDManager::GetInstance()->GetTypeString(id);
	return outputStr;
}

#ifdef OPENCV
cv::Mat COrbFlannMatch::preProcess(const cv::Mat &src)
{
	if (m_pCurType == CPatternManager::ImageType::IMAGE_CUSHION)
	{
		 cv::Mat result = src(cv::Rect(0, src.rows / 2, src.cols, src.rows / 2));
		 //cv::imshow("cushion", result);
		 //cv::waitKey(0);
		 return result;
	}
	else if (m_pCurType == CPatternManager::ImageType::IMAGE_BACKREST)
	{
		cv::Mat result = src(cv::Rect(0, 0, src.cols, src.rows * 2 / 3));
		return result;
	}
	return src;
}

#endif // OPENCV
