#include "../stdafx.h"
#include "utils.h"


namespace utils
{

	char* WcharToChar(wchar_t* wc)
	{

		int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
		char *m_char = new char[len + 1];
		memset(m_char, 0, sizeof(char) * (len + 1));

		WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
		m_char[len] = '\0';
		return m_char;
	}
	wchar_t* CharToWchar(char* c)
	{

		int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
		wchar_t *m_wchar = new wchar_t[len + 1];
		memset(m_wchar, 0, sizeof(wchar_t) * (len + 1));

		MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
		m_wchar[len] = '\0';
		return m_wchar;
	}

	std::wstring StrToWStr(const std::string str)
	{
		//wchar_t* CharToWchar(char* c);
		wchar_t *pWChar = CharToWchar(const_cast<char*>(str.c_str()));
		std::wstring tmpStr(pWChar);
		delete[]pWChar;
		pWChar = nullptr;
		return tmpStr;
	}
	std::string WStrToStr(const std::wstring wstr)
	{
		char *pChar = WcharToChar(const_cast<wchar_t*>(wstr.c_str()));

		std::string tmpStr(pChar);
		delete[]pChar;
		pChar = nullptr;

		return tmpStr;
	}

#ifdef OPENCV
	void showImageHist(const cv::Mat & srcImage)
	{
		int histBinNum = 255;

		//设定取值范围
		float range[] = { 0, 255 };
		const float* histRange = { range };

		bool uniform = true;
		bool accumulate = false;

		//声明三个通道的hist数组
		cv::Mat red_hist;

		//计算直方图
		cv::calcHist(&srcImage, 1, 0, cv::Mat(), red_hist, 1, &histBinNum, &histRange, uniform, accumulate);

		//创建直方图窗口
		int hist_w = 400;
		int hist_h = 400;
		int bin_w = cvRound((double)srcImage.cols / histBinNum);

		cv::Mat histImage(srcImage.cols, srcImage.rows, CV_8UC3, cv::Scalar(0, 0, 0));

		//将直方图归一化到范围[0, histImage.rows]
		normalize(red_hist, red_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		//循环绘制直方图
		for (int i = 1; i < histBinNum; i++)
		{
			cv::line(histImage, cv::Point(bin_w*(i - 1), srcImage.rows - cvRound(red_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), srcImage.rows - cvRound(red_hist.at<float>(i))), cv::Scalar(0, 0, 255), 2, 8, 0);
		}

		cv::namedWindow("原图像", cv::WINDOW_AUTOSIZE);
		cv::imshow("原图像", srcImage);

		cv::namedWindow("图像直方图", cv::WINDOW_AUTOSIZE);
		cv::imshow("图像直方图", histImage);

		cv::waitKey(0);
	}
	cv::Rect splitFrameGrayImage(const cv::Mat & img)
	{
		if ((img.rows < 2) || (img.cols < 2) || (img.channels() != 1))
		{
			std::cout << "splitFrameGrayImage Error: " << img.rows << "\t" << img.cols << "\t" << img.channels() << std::endl;
			return cv::Rect(cv::Point(0, 0), cv::Point(img.cols - 1, img.rows - 1));
		}
		std::function<bool(cv::Mat)> is_allZero = [&](cv::Mat &file)->bool
		{
			bool flag = true;
			size_t nSize = 0;
			unsigned char *pdata = file.data;
			for (size_t j = 0; j < file.cols; j += 2)
			{
				int k = pdata[j];
				if ((k != 0) && (k != 0xFF))
				{
					return false;
				}
			}

			return true;
		};

		size_t beginRow = 0;
		for (size_t i = 0; i < img.rows; i += 2)
		{
			if (is_allZero(img.row(i)) == false)
			{
				beginRow = i;
				break;
			}
		}
		size_t endRow = img.rows - 1;
		for (int i = img.rows - 1; i > -1; i -= 2)
		{
			if (is_allZero(img.row(i)) == false)
			{
				endRow = i;
				break;
			}
		}

		size_t beginCol = 0;
		for (int i = 0; i < img.cols; i += 2)
		{
			if (is_allZero(img.col(i).t()) == false)
			{
				beginCol = i;
				break;
			}
		}

		size_t endCol = img.cols - 1;
		for (int i = img.cols - 1; i > -1; i -= 2)
		{
			if (is_allZero(img.col(i).t()) == false)
			{
				endCol = i;
				break;
			}
		}

		if ((beginRow < endRow) && (beginCol < endCol))
		{
			return cv::Rect(cv::Point(beginCol, beginRow), cv::Point(endCol, endRow));
		}

		return cv::Rect(cv::Point(0, 0), cv::Point(img.cols - 1, img.rows - 1));
	}
	cv::Rect calcRect(const std::vector<cv::Point2f>& point)
	{
		double minX = INT_MAX;
		double maxX = -INT_MAX;
		double minY = INT_MAX;
		double maxY = -INT_MAX;
		size_t nSize = point.size();
		for (size_t i = 0; i < nSize; ++i)
		{
			const cv::Point2f &p = point[i];
			if (p.x > maxX)
			{
				maxX = p.x;
			}
			if (p.x < minX)
			{
				minX = p.x;
			}
			if (p.y > maxY)
			{
				maxY = p.y;
			}
			if (p.y < minY)
			{
				minY = p.y;
			}
		}

		return cv::Rect(cv::Point2f(minX, minY), cv::Point2f(maxX, maxY));
	}
	double calcArea(const cv::Rect & rect)
	{
		return rect.height * rect.width;
	}
	double calcAvgPixel(const cv::Mat & img, int minPixelValue, int maxPixelValue)
	{
		int histBinNum = 255;

		//设定取值范围
		float range[] = { minPixelValue, maxPixelValue };
		const float* histRange = { range };

		bool uniform = true;
		bool accumulate = false;

		cv::Mat srcGrayHist;

		//计算直方图
		cv::calcHist(&img, 1, 0, cv::Mat(), srcGrayHist, 1, &histBinNum, &histRange, uniform, accumulate);

		double srcAvgValue = 0;
		size_t srcPixelCount = 0;

		float *pData = nullptr;
		pData = (float*)srcGrayHist.data;
		for (size_t i = 0; i < srcGrayHist.rows; ++i)
		{
			if ((i > range[0]) && (i < range[1]))
			{
				srcPixelCount += pData[i];
				srcAvgValue += pData[i] * i;
			}
		}
		if (srcPixelCount == 0)
		{
			return INT_MAX;
		}

		srcAvgValue = static_cast<double>(srcAvgValue) / srcPixelCount;

		return srcAvgValue;
	}
#endif // OPENCV
}
