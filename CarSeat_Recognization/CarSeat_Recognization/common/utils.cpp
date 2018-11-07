#include "../stdafx.h"
#include "utils.h"
#include <io.h>




namespace utils
{

	char* WcharToChar(wchar_t* wc)
	{
		if ((wc == nullptr) || (wcslen(wc) == 0))
		{
			return nullptr;
		}
		
		int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
		char *m_char = new char[len + 1];
		memset(m_char, 0, sizeof(char) * (len + 1));

		WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
		m_char[len] = '\0';
		return m_char;
	}
	wchar_t* CharToWchar(char* c)
	{
		if ((c == nullptr) || (strlen(c) == 0))
		{
			return nullptr;
		}
		int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
		wchar_t *m_wchar = new wchar_t[len + 1];
		memset(m_wchar, 0, sizeof(wchar_t) * (len + 1));

		MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
		m_wchar[len] = '\0';
		return m_wchar;
	}

	std::wstring StrToWStr(const std::string str)
	{
		wchar_t *pWChar = CharToWchar(const_cast<char*>(str.c_str()));
		if (pWChar == nullptr)
		{
			return std::wstring();
		}
		std::wstring tmpStr(pWChar);
		delete[]pWChar;
		pWChar = nullptr;

		return tmpStr;
	}
	std::string WStrToStr(const std::wstring wstr)
	{
		char *pChar = WcharToChar(const_cast<wchar_t*>(wstr.c_str()));
		if (pChar == nullptr)
		{
			return std::string();
		}
		std::string tmpStr(pChar);
		delete[]pChar;
		pChar = nullptr;

		return tmpStr;
	}

	// bool RecogResultCToW(RecogResultA & a, RecogResultW & b)
	bool RecogResultCToW(RecogResult<char> &a, RecogResult<wchar_t> &b)
	{
		bool flag = false;
		do
		{
			memset(&b, 0, sizeof(b));
			wchar_t *tmp = utils::CharToWchar(a.m_szBarcode);
			if (tmp != nullptr)
			{
				memcpy(b.m_szBarcode, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::CharToWchar(a.m_szRecogMethod);
			if (tmp != nullptr)
			{
				memcpy(b.m_szRecogMethod, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::CharToWchar(a.m_szCameraName);
			if (tmp != nullptr)
			{
				memcpy(b.m_szCameraName, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			

			tmp = utils::CharToWchar(a.m_szImagePath);
			if (tmp != nullptr)
			{
				memcpy(b.m_szImagePath, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}

			tmp = utils::CharToWchar(a.m_szTypeByUsrInput);
			if (tmp != nullptr)
			{
				memcpy(b.m_szTypeByUsrInput, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::CharToWchar(a.m_szLineName);
			if (tmp != nullptr)
			{
				memcpy(b.m_szLineName, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::CharToWchar(a.m_szTime);
			if (tmp != nullptr)
			{
				memcpy(b.m_szTime, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			tmp = utils::CharToWchar(a.m_szInternalType);
			if (tmp != nullptr)
			{
				memcpy(b.m_szInternalType, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}


			tmp = utils::CharToWchar(a.m_szTypeByBarcode);
			if (tmp != nullptr)
			{
				memcpy(b.m_szTypeByBarcode, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}

			tmp = utils::CharToWchar(a.m_szInternalType);
			if (tmp != nullptr)
			{
				memcpy(b.m_szInternalType, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			

			tmp = utils::CharToWchar(a.m_szTypeByRecog);
			if (tmp != nullptr)
			{
				memcpy(b.m_szTypeByRecog, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::CharToWchar(a.m_szUsrName);
			if (tmp != nullptr)
			{
				memcpy(b.m_szUsrName, tmp, sizeof(wchar_t) * wcslen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}

			b.m_bIsCorrect = a.m_bIsCorrect;
			flag = true;

		} while (0);
		return flag;
	}

	//bool RecogResultWToC(RecogResultW & a, RecogResultA & b)
	bool RecogResultWToC(RecogResult<wchar_t> &a, RecogResult<char> &b)
	{
		bool flag = false;
		do
		{
			memset(&b, 0, sizeof(b));
			char *tmp = utils::WcharToChar(a.m_szBarcode);
			if (tmp != nullptr)
			{
				memcpy(b.m_szBarcode, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::WcharToChar(a.m_szRecogMethod);
			if (tmp != nullptr)
			{
				memcpy(b.m_szRecogMethod, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}

			tmp = utils::WcharToChar(a.m_szTypeByUsrInput);
			if (tmp != nullptr)
			{
				memcpy(b.m_szTypeByUsrInput, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::WcharToChar(a.m_szCameraName);
			if (tmp != nullptr)
			{
				memcpy(b.m_szCameraName, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}

			tmp = utils::WcharToChar(a.m_szInternalType);
			if (tmp != nullptr)
			{
				memcpy(b.m_szInternalType, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::WcharToChar(a.m_szImagePath);
			if (tmp != nullptr)
			{
				memcpy(b.m_szImagePath, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::WcharToChar(a.m_szLineName);
			if (tmp != nullptr)
			{
				memcpy(b.m_szLineName, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}

			tmp = utils::WcharToChar(a.m_szTime);
			if (tmp != nullptr)
			{
				memcpy(b.m_szTime, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::WcharToChar(a.m_szTypeByBarcode);
			if (tmp != nullptr)
			{
				memcpy(b.m_szTypeByBarcode, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::WcharToChar(a.m_szTypeByRecog);
			if (tmp != nullptr)
			{
				memcpy(b.m_szTypeByRecog, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			tmp = utils::WcharToChar(a.m_szUsrName);
			if (tmp != nullptr)
			{
				memcpy(b.m_szUsrName, tmp, sizeof(wchar_t) * strlen(tmp));
				delete[]tmp;
				tmp = nullptr;
			}
			
			b.m_bIsCorrect = a.m_bIsCorrect;
			flag = true;

		} while (0);
		return flag;
	}


	bool delBlankSpace(char * text, size_t len)
	{
		if ((text == nullptr) || (len == 0))
		{
			return true;
		}
		size_t i = 0;
		size_t j = 0;
		for (; j < len; )
		{
			if (text[j] > 0x20)
			{
				text[i] = text[j];
				++i;
				++j;
			}
			else if (text[j] <= 0x20)
			{
				++j;
			}
		}
		if (i < len)
		{
			memset(text + i, 0, sizeof(char) * (len - i));
		}
		char s = '#';
		i = 0;
		while (text[i] != '\0')
		{
			if (text[i] == s)
			{
				text[i] = ' ';
			}
			++i;
		}
		return true;
	}

	bool readFile(char * file, char * &content, size_t & FileSize)
	{
		FILE *fp = nullptr;
		content = nullptr;
		FileSize = 0;
		fopen_s(&fp, file, "rb");
		if (fp == nullptr)
		{
			return false;
		}
		fseek(fp, 0, SEEK_END);

		size_t nSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		content = new char[nSize + 1];
		if (content == nullptr)
		{
			return false;
		}
		memset(content, 0, sizeof(char) * (nSize + 1));
		fread(content, 1, nSize, fp);
		fclose(fp);

		FileSize = nSize;
		return true;
	}



	bool parseVector(const char *content, const char * name, std::vector<std::string>* pVector)
	{
		if ((content == nullptr) || (name == nullptr) || (pVector == nullptr))
		{
			return false;
		}
		char *p = const_cast<char*>(strstr(content, name));
		if (p != NULL)
		{
			char *line = strchr(p, '=');
			char *end = strchr(p, '\n');
			if ((line != NULL) && (end != NULL))
			{

				if (parseLineSegment(line + 1, end - line - 1, pVector) == true)
				{



				}
			}
		}
		return true;
	}

	bool parseLineSegment(const char * pContent, size_t length, std::vector<std::string>* pData)
	{
		if ((pContent == nullptr) || (length < 1) || (pData == nullptr))
		{
			return false;
		}

		char *p = const_cast<char*>(pContent);
		char tmpStr[100];
		char c = '\"';
		while (p < pContent + length)
		{
			char* begin = strchr(p, c);
			if ((begin == NULL) || (begin >= pContent + length))
			{
				break;
			}
			char* end = strchr(begin + 1, c);
			if ((end == NULL) || (end >= pContent + length))
			{
				break;
			}
			p = end + 1;
			if (end - begin - 1 != 0)
			{
				memset(tmpStr, 0, sizeof(tmpStr));
				memcpy(tmpStr, begin + 1, sizeof(char)*(end - begin - 1));
				utils::delBlankSpace(tmpStr, strlen(tmpStr));
				
				pData->push_back(std::string(tmpStr));
				
			}
		}
		return true;
	}

	int parseIp(const char * content, const char * name)
	{
		if ((content == nullptr) || (name == nullptr))
		{
			return 0;
		}
		char *p = const_cast<char*>(strstr(content, name));
		if (p == nullptr)
		{
			return 0;
		}
		const char *quote = strchr(p, '"');
		if (quote == nullptr)
		{
			return 0;
		}
		const char *endline = strchr(quote + 1, '"');
		char str[100];
		memset(str, 0, sizeof(str));
		memcpy(str, quote + 1, endline - quote - 1);
		utils::delBlankSpace(str, strlen(str));

		TRACE1("ServerIp = %s", str);
		unsigned int one = 0;
		unsigned int two = 0;
		unsigned int three = 0;
		unsigned int four = 0;
		sscanf_s(str, "%d.%d.%d.%d", &one, &two, &three, &four);
		unsigned int tmpServerIp = (one << 24) | (two << 16) | (three << 8) | four;

		return tmpServerIp;
	}

	size_t parseMap(const char * content, const char * name, std::unordered_map<std::string, std::string>* pMap)
	{
		if ((content == nullptr) || (name == nullptr) || (pMap == nullptr))
		{
			return 0;
		}
		const char *p = strstr(content, name);
		if (p == nullptr)
		{
			return 0;
		}
		const char *endLine = strchr(p, '}');
		if (endLine == nullptr)
		{
			return 0;
		}
		char tmpStr[50] = { 0 };
		while (p < endLine)
		{
			const char *first = strchr(p, '\"');
			if ((first > endLine) || (first == nullptr))
			{
				return pMap->size();
			}
			const char *second = strchr(first + 1, '\"');
			if ((second > endLine) || (second == nullptr))
			{
				return pMap->size();
			}
			memset(tmpStr, 0, sizeof(tmpStr));
			size_t tmpLength = second - first - 1;
			memcpy(tmpStr, first + 1, tmpLength);
			utils::delBlankSpace(tmpStr, strlen(tmpStr));

			TRACE1("tmpStr = [%s]\n", tmpStr);
			std::string keyChar(tmpStr);
			//std::wstring keyWChar = utils::StrToWStr(keyChar);

			p = second + 1;

			first = strchr(p, '\"');
			if ((first > endLine) || (first == nullptr))
			{
				return pMap->size();
			}
			second = strchr(first + 1, '\"');
			if ((second > endLine) || (second == nullptr))
			{
				return pMap->size();
			}
			memset(tmpStr, 0, sizeof(tmpStr));
			tmpLength = second - first - 1;
			memcpy(tmpStr, first + 1, tmpLength);
			//utils::delBlankSpace(tmpStr, strlen(tmpStr));
			TRACE1("tmpStr = [%s]\n", tmpStr);
			std::string valueChar(tmpStr);
			//std::wstring valueWChar = utils::StrToWStr(valueChar);

			pMap->insert(std::make_pair(keyChar, valueChar));

			p = second + 1;

		}

		return pMap->size();
	}

	bool getValueByName(const char *content, const char * name, char * value)
	{
		if ((name == nullptr) || (value == nullptr) || (content == nullptr))
		{
			return false;
		}

		char *p = strstr(const_cast<char*>(content), name);
		if (p == NULL)
		{
			return false;
		}
		char *begin = strstr(p, "\"");
		if (begin == NULL)
		{
			return false;
		}
		char *end = strstr(begin + 1, "\"");
		if ((begin == NULL))
		{
			return false;
		}
		memcpy_s(value, MAX_CHAR_LENGTH, begin + 1, end - begin - 1);
		utils::delBlankSpace(value, strlen(value));

		return true;
	}

	bool mkdir(const char* dir)
	{
		utils::delBlankSpace(const_cast<char*>(dir), strlen(dir));

		do
		{
			if (_access(dir, 6) == 0)
			{
				break;
			}
			
			wchar_t *wDir = utils::CharToWchar(const_cast<char*>(dir));
			if (wDir == nullptr)
			{
				break;
			}

			CreateDirectory(wDir, NULL);

			delete[]wDir;
			wDir = nullptr;
		} while (0);
		return false;
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
