#include "RecogResultManager.h"
#include <functional>
#include <fstream>
#include "Log.h"


CRecogResultManager* CRecogResultManager::m_pInstance = nullptr;

CRecogResultManager::CRecogResultManager():
	m_pRecogResult(nullptr),
	m_bAutoSave(false)
{
	memset(m_szName, 0, sizeof(m_szName));
	strcpy_s(m_szName, "recogResult.txt");
	init();
}

void CRecogResultManager::init()
{
	if (m_pRecogResult == nullptr)
	{
		m_pRecogResult = new std::list<RecogResultA>;
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
	if (content == nullptr)
	{
		return;
	}
	
	std::string tmpContent(content);
	length = tmpContent.size();
	
	delete[]content;
	content = nullptr;

	content = const_cast<char*>(tmpContent.c_str());
	char tmpChar[MAX_CHAR_LENGTH * 3];
	memset(tmpChar, 0, sizeof(char) * 3 * MAX_CHAR_LENGTH);
	
	int begin = 0;
	RecogResultA tmpResult;
	while (1)
	{
		const char val = '\n';
		const char * end = std::find(content + begin, content + length, val);
		if (end == content + length)
		{
			break;
		}
		if (end - content - begin < 10)
		{
			break;
		}
		memset(tmpChar, 0, sizeof(char) * 3 * MAX_CHAR_LENGTH);
		int i = end - content - begin;
		memcpy(tmpChar, content + begin, i);
		if (tmpChar[i - 1] <= 0x20)
		{
			tmpChar[i - 1] = ',';
		}
		else
		{
			tmpChar[i] = ',';
		}
		if (parseLine(tmpChar, tmpResult) == true)
		{
			//size_t tmpHashValue = m_pHashFunc(tmpResult.m_szBarcode);
			m_pRecogResult->push_back(tmpResult);
		}
		//output(tmpResult);
		begin = begin + i + 1;
	}
}


/// 回写到硬盘
bool CRecogResultManager::serialize()
{
	if ((m_pRecogResult == nullptr) || (m_pRecogResult->size() == 0))
	{
		return true;
	}
	std::fstream fs(m_szName, std::ios::in | std::ios::out);
	std::list<RecogResultA>::const_iterator iter = m_pRecogResult->begin();
	for (; iter != m_pRecogResult->end(); ++iter)
	{
		fs << iter->m_szBarcode << ","	\
			<<  iter->m_szTime << ","	\
			<< iter->m_szTypeByRecog << ",";
		fs << iter->m_szTypeByBarcode << ","		\
			<< iter->m_szTypeByUsrInput << ","	\
			<< iter->m_bIsCorrect << ",";
		fs << iter->m_szRecogMethod << "," \
			<< iter->m_szCameraName << "," \
			<< iter->m_szLineName << ",";
		fs << iter->m_szUsrName << ","	\
			<< iter->m_szImagePath << "\n";
	}
	fs.close();
	return true;
}

bool CRecogResultManager::parseLine(char * line, RecogResultA & a)
{
	bool value = RecogResultA::TextToRecog(a, line);
	return value;
}

void CRecogResultManager::output(RecogResultA & a)
{
	char tmp[MAX_CHAR_LENGTH] = { 0 };
	memset(tmp, 0, sizeof(tmp));
	sprintf_s(tmp, sizeof(tmp), "%s,%s,path = %s", a.m_szBarcode, a.m_szTime, a.m_szImagePath);
	WriteInfo("%s", tmp);
}


CRecogResultManager::~CRecogResultManager()
{
	if (m_bAutoSave == true)
	{
		serialize();
	}
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

const RecogResultA * CRecogResultManager::findByBarcode(const char * barcode) const
{
	if ((m_pRecogResult == nullptr) || (barcode == nullptr))
	{
		return nullptr; 
	}
	//size_t tmpValue = m_pHashFunc(barcode);

	std::list<RecogResultA>::const_iterator iter = m_pRecogResult->cbegin();
	size_t tmpLen = strlen(barcode);
	while (iter != m_pRecogResult->cend())
	{
		int ret = strncmp(iter->m_szBarcode, barcode, tmpLen);
		if (ret == 0)
		{
			break;
		}
		//WriteInfo("ret = %d, %s, %s, %d", ret, iter->m_szBarcode, barcode, strlen(barcode));
		++iter;
	}
	//, m_pRecogResult->end());

	if (iter == m_pRecogResult->cend())
	{
		return nullptr;
	}
	return (const RecogResultA*)(&(*iter));
}

bool CRecogResultManager::add(const RecogResultA & a)
{
	if (m_pRecogResult == nullptr)
	{
		m_pRecogResult = new std::list<RecogResultA>;
	}
	//size_t tmpValue = m_pHashFunc(a.m_szBarcode);
	m_pRecogResult->push_front(a);
	m_bAutoSave = true;
	
	return true;
}
