#include "RecogResultManager.h"
#include <functional>
#include <fstream>


CRecogResultManager* CRecogResultManager::m_pInstance = nullptr;

CRecogResultManager::CRecogResultManager():
	m_pRecogResult(nullptr)
{
	memset(m_szName, 0, sizeof(m_szName));
	strcpy_s(m_szName, "recogResult.csv");
	init();
}

void CRecogResultManager::init()
{
	if (m_pRecogResult == nullptr)
	{
		m_pRecogResult = new std::unordered_map<size_t, RecogResult>;
	}
	/*
	初始化本地文件，对于日期时间超过一个月以上的识别结果不予初始化,not implement
	*/
	char *content = nullptr;
	size_t length = 0;
	if (false == utils::readFile(m_szName, content, length))
	{
		if (content != nullptr)
		{
			delete[]content;
			content = nullptr;
			return;
		}
	}
	
	std::string tmpContent(content);
	length = tmpContent.size();
	
	delete[]content;
	content = nullptr;

	content = const_cast<char*>(tmpContent.c_str());
	char tmpChar[MAX_CHAR_LENGTH * 3];
	memset(tmpChar, 0, sizeof(char) * 3 * MAX_CHAR_LENGTH);
	
	int begin = 0;
	struct RecogResult tmpResult;
	while (1)
	{
		const char val = '\n';
		const char * end = std::find(content + begin, content + length, val);
		if (end == content + length)
		{
			break;
		}
		memset(tmpChar, 0, sizeof(char) * 3 * MAX_CHAR_LENGTH);
		int i = end - content - begin;
		memcpy(tmpChar, content + begin, i);
		tmpChar[i] = ',';
		if (parseLine(tmpChar, tmpResult) == true)
		{
			size_t tmpHashValue = m_pHashFunc(tmpResult.m_szBarcode);
			m_pRecogResult->insert(std::make_pair(tmpHashValue, tmpResult));
		}
		begin = begin + i + 1;
	}
}


/// 回写到硬盘
bool CRecogResultManager::serialize()
{
	if ((m_pRecogResult != nullptr) || (m_pRecogResult->size() == 0))
	{
		return;
	}
	std::fstream fs(m_szName, std::ios::in | std::ios::out);
	std::unordered_map<size_t, struct RecogResult>::const_iterator iter = m_pRecogResult->begin();
	for (; iter != m_pRecogResult->end(); ++iter)
	{
		fs << iter->second.m_szBarcode << ","	\
			<<  iter->second.m_szTime << ","	\
			<< iter->second.m_szTypeByRecog << ",";
		fs << iter->second.m_szTypeByBarcode << ","		\
			<< iter->second.m_bIsCorrect << ",";
		fs << iter->second.m_szRecogMethod << "," \
			<< iter->second.m_szCameraName << "," \
			<< iter->second.m_szLineName << ",";
		fs << iter->second.m_szUsrName << ","	\
			<< iter->second.m_szImagePath << "\n";
	}
	fs.close();
	return false;
}

bool CRecogResultManager::parseLine(char * line, RecogResult & a)
{
	if (line == nullptr)
	{
		return false;
	}
	
	memset(&a, 0, sizeof(a));
	do
	{
		char quote = ',';
		char *begin = line;
		char *end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szBarcode, begin, (end - begin));
		begin = end + 1;
		end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szTime, begin, end - begin);
		begin = end + 1;
		end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szTypeByRecog, begin, end - begin);
		begin = end + 1;
		end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szTypeByBarcode, begin, end - begin);
		begin = end + 1;
		end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		char tmp[10] = { 0 };
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, begin, end - begin);
		//int i = atoi(tmp);
		if (atoi(tmp) == 0)
		{
			a.m_bIsCorrect = false;
		}
		else
		{
			a.m_bIsCorrect = true;
		}
		begin = end + 1;
		end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szRecogMethod, begin, end - begin);
		begin = end + 1;
		end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szCameraName, begin, end - begin);
		begin = end + 1;
		end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szLineName, begin, end - begin);
		begin = end + 1;
		end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szUsrName, begin, end - begin);
		begin = end + 1;
		end = strchr(begin, quote);
		if (end == NULL)
		{
			break;
		}
		memcpy(a.m_szImagePath, begin, end - begin);
		begin = end + 1;
		
		memcpy(a.m_szUsrName, begin, end - begin);




	} while (0);
	return true;
}


CRecogResultManager::~CRecogResultManager()
{
	serialize();
	if (m_pRecogResult != nullptr)
	{
		delete m_pRecogResult;
		m_pRecogResult = nullptr;
	}
}


CRecogResultManager* CRecogResultManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CRecogResultManager;
	}
	return m_pInstance;
}

const RecogResult * CRecogResultManager::findByBarcode(const char * barcode) const
{
	if ((m_pRecogResult == nullptr) || (barcode == nullptr))
	{
		return nullptr;
	}
	size_t tmpValue = m_pHashFunc(barcode);

	std::unordered_map<size_t, RecogResult>::const_iterator iter = m_pRecogResult->find(tmpValue);

	if (iter == m_pRecogResult->end())
	{
		return nullptr;
	}
	return (const RecogResult*)(&(*iter));
}

bool CRecogResultManager::add(const RecogResult & a)
{
	if (m_pRecogResult == nullptr)
	{
		m_pRecogResult = new std::unordered_map<size_t, RecogResult>;
	}
	size_t tmpValue = m_pHashFunc(a.m_szBarcode);
	m_pRecogResult->insert(std::make_pair(tmpValue, a));
	
	return false;
}
