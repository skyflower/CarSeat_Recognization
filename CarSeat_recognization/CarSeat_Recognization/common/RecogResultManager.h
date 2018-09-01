#pragma once

#include <iostream>
#include <Windows.h>
#include <list>
#include "utils.h"

/*

*/
template<typename type>
class RecogResult
{
public:
	RecogResult();
	~RecogResult();
	type m_szBarcode[32];		//	完整条形码
	type m_szTime[32];			//	结果形成的时间戳，包含本地日期和时间
	type m_szTypeByRecog[32];	//	识别结果的外部类型
	type m_szTypeByBarcode[32];	//	根据条形码拿到的外部类型
	type m_szTypeByUsrInput[32];	//当识别错误的时候，产线管理员输入的外部类型
	type m_szRecogMethod[8];	//	识别方法，目前只是"auto"
	type m_szCameraName[20];	//	拍照相机的名字
	type m_szLineName[8];		//	产线名称
	type m_szUsrName[20];		//	产线管理员用户名
	type m_szImagePath[256];	//	照片路径

	bool m_bIsCorrect;			//	识别结果是否一致

	static bool RecogToText(RecogResult & a, type *buffer);
	static bool TextToRecog(RecogResult & a, type *buffer);
};


template<typename type>
RecogResult<type>::RecogResult():m_bIsCorrect(false)
{
	memset(m_szBarcode, 0, sizeof(m_szBarcode));
	memset(m_szTime, 0, sizeof(m_szTime));
	memset(m_szTypeByRecog, 0, sizeof(m_szTypeByRecog));
	memset(m_szTypeByBarcode, 0, sizeof(m_szTypeByBarcode));
	memset(m_szTypeByUsrInput, 0, sizeof(m_szTypeByUsrInput));
	memset(m_szRecogMethod, 0, sizeof(m_szRecogMethod));
	memset(m_szCameraName, 0, sizeof(m_szCameraName));
	memset(m_szLineName, 0, sizeof(m_szLineName));
	memset(m_szUsrName, 0, sizeof(m_szUsrName));
	memset(m_szImagePath, 0, sizeof(m_szImagePath));
}

template<typename type>
RecogResult<type>::~RecogResult()
{

}

template<typename type>
bool RecogResult<type>::RecogToText(RecogResult<type> &a, type *buffer)
{
	//std::stringstream fs;
	std::basic_stringstream<type, std::char_traits<type>, std::allocator<type> > fs;
	fs << a.m_szBarcode << ","	\
		<< a.m_szTime << ","	\
		<< a.m_szTypeByRecog << ",";
	fs << a.m_szTypeByBarcode << ","		\
		<< a.m_szTypeByUsrInput << ","	\
		<< a.m_bIsCorrect << ",";
	fs << a.m_szRecogMethod << "," \
		<< a.m_szCameraName << "," \
		<< a.m_szLineName << ",";
	fs << a.m_szUsrName << ","	\
		<< a.m_szImagePath << "\n";
	
	std::basic_string<type> b = fs.str();
	memcpy(buffer, b.c_str(), sizeof(type) * b.size());
	return true;
}

template<typename type>
bool RecogResult<type>::TextToRecog(RecogResult<type> &a, type *buffer)
{
	if (buffer == nullptr)
	{
		return false;
	}
	type quote;
	
	type delimate;
	if (typeid(type) == typeid(','))
	{
		quote = ',';
		delimate = '\0';
	}
	else if (typeid(type) == typeid(L','))
	{
		quote = L',';
		delimate = L'\0';
	}
	size_t length = 0;
	while (buffer[length] != delimate)
	{
		++length;
	}
	bool flag = false;

	do
	{
		type *begin = buffer;
		
		int64_t tmpLength = length;
		type *end = std::find(begin, begin + tmpLength, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szBarcode, begin, (end - begin) * sizeof(type));
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;

		end = std::find(begin, begin + tmpLength, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szTime, begin, (end - begin) * sizeof(type));
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;

		end = std::find(begin, begin + tmpLength, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szTypeByRecog, begin, (end - begin) * sizeof(type));
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;

		end = std::find(begin, begin + tmpLength, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szTypeByBarcode, begin, (end - begin) * sizeof(type));
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;

		end = std::find(begin, begin + tmpLength, quote);
		if (end == NULL)
		{
			break;
		}
		if (end - begin > 0)
		{
			memcpy(a.m_szTypeByUsrInput, begin, (end - begin) * sizeof(type));
		}
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;

		end = std::find(begin, begin + tmpLength, quote);
		if (end == NULL)
		{
			break;
		}
		type tmp[10] = { 0 };
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, begin, (end - begin) * sizeof(type));

		if (tmp[0] == '0')
		{
			a.m_bIsCorrect = false;
		}
		else if(tmp[0] == '1')
		{
			a.m_bIsCorrect = true;
		}
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;

		end = std::find(begin, begin + tmpLength, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szRecogMethod, begin, (end - begin) * sizeof(type));
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;
		end = std::find(begin, begin + tmpLength, quote);

		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szCameraName, begin, (end - begin) * sizeof(type));
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;
		end = std::find(begin, begin + tmpLength, quote);

		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szLineName, begin, (end - begin) * sizeof(type));
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;

		end = std::find(begin, begin + tmpLength, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szUsrName, begin, (end - begin) * sizeof(type));
		tmpLength = tmpLength - (end - begin) - 1;
		begin = end + 1;
		end = std::find(begin, begin + tmpLength, quote);

		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szImagePath, begin, (end - begin) * sizeof(type));
		begin = end + 1;
		return true;
	} while (0);
	return flag;
}


typedef RecogResult<char> RecogResultA;
typedef RecogResult<wchar_t> RecogResultW;


class CRecogResultManager
{
public:
	
	~CRecogResultManager();

	static CRecogResultManager *GetInstance();

	const RecogResultA* findByBarcode(const char *barcode)const;

	bool add(const RecogResultA &a);

private:

	CRecogResultManager();

	void init();
	bool serialize();

	bool parseLine(char *line, RecogResultA &a);

	void output(RecogResultA &a);

	char m_szName[256];

	bool m_bAutoSave;

	static CRecogResultManager *m_pInstance;

	std::list<RecogResultA> *m_pRecogResult;

};

