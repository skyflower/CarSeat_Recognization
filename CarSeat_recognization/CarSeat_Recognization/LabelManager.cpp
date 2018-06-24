#include "LabelManager.h"
#include <cstdlib>
#include "./common/Log.h"
#include <sys/stat.h>
#include "./common/utils.h"
#include <Windows.h>

bool CLabelManager::m_bInitFlag = false;

CLabelManager::CLabelManager()
{
	m_pBarcode = nullptr;
	m_pClassifyType = nullptr;
	
	if (m_bInitFlag == false)
	{
		m_bInitFlag = init();
		m_bSerialize = false;
	}
}


CLabelManager::~CLabelManager()
{
}

std::wstring CLabelManager::GetInternalTypeByBarcode(std::wstring barcode)
{
	return barcode;
}

std::wstring CLabelManager::GetInternalTypeByClassifyType(std::wstring type)
{
	if (m_pClassifyType != nullptr)
	{
		std::unordered_map<std::wstring, std::wstring>::const_iterator iter = m_pClassifyType->begin();
		for (; iter != m_pClassifyType->end(); ++iter)
		{
			if (iter->second == type)
			{
				return iter->first;
			}
		}
	}
	return std::wstring();
}

bool CLabelManager::init()
{
	size_t nSize = 0;
	char *content = nullptr;
	utils::readFile("labelConfig.txt", content, nSize);
	if ((content == nullptr) || (nSize == 0))
	{
		return false;
	}

	// barcodeTable
	// classifyType

	if (m_pBarcode == nullptr)
	{
		m_pBarcode = new std::unordered_map<std::wstring, std::wstring>;
	}
	if (m_pClassifyType == nullptr)
	{
		m_pClassifyType = new std::unordered_map<std::wstring, std::wstring>;
	}

	char *str[] = { "barcodeTable", "classifyType" };
	std::unordered_map<std::wstring, std::wstring> *p[2];
	p[0] = m_pBarcode;
	p[1] = m_pClassifyType;

	char *tmpChar = content;
	for (size_t i = 0; i < 2; ++i)
	{
		do
		{
			char *begin = strstr(tmpChar, str[i]);
			if (begin == NULL)
			{
				break;
			}
			
			char *end = strchr(begin + 1, '}');
			if (end == NULL)
			{
				break;
			}
			if (end - begin > 1)
			{
				size_t tmpLength = end - begin + 10;
				char *tmpContent = new char[tmpLength];
				memset(tmpContent, 0, sizeof(char) * tmpLength);
				memcpy(tmpContent, begin, end - begin + 1);

				utils::parseMap(tmpContent, str[i], p[i]);
				tmpChar = end + 1;

				if (tmpContent != nullptr)
				{
					delete[]tmpContent;
					tmpContent = nullptr;
				}
			}

		} while (0);
	}
	
	if (content != nullptr)
	{
		delete[]content;
		content = nullptr;
	}
	return true;
}

bool CLabelManager::serialize()
{
	FILE *fp = nullptr;
	fopen_s(&fp, "labelConfig.txt.bak", "wb");
	if (fp == nullptr)
	{
		WriteError("Failed labelConfig.txt.bak");
		return false;
	}

	const size_t Length = 2000;
	wchar_t *tmpStr = new wchar_t[Length];
	memset(tmpStr, 0, sizeof(wchar_t) * Length);

	if ((m_pBarcode != nullptr) && (m_pBarcode->size() > 0))
	{
		lstrcatW(tmpStr, L"barcodeTable={");
		size_t tmpSize = m_pBarcode->size();
		int i = 0;
		std::unordered_map<std::wstring, std::wstring>::const_iterator iter = m_pBarcode->begin();
		for (; i < tmpSize - 1; ++i)
		{
			wsprintfW(tmpStr, L"%s\"%s\":\"%s\",\n", tmpStr, iter->first.c_str(), \
				iter->second.c_str());
			++iter;
		}
		wsprintfW(tmpStr, L"%s\"%s\":\"%s\"}\n", tmpStr, iter->first.c_str(), \
			iter->second.c_str());
	}
	fwrite(tmpStr, sizeof(wchar_t), wcslen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(wchar_t) * Length);

	if ((m_pClassifyType != nullptr) && (m_pClassifyType->size() > 0))
	{
		lstrcatW(tmpStr, L"classifyType={");
		size_t tmpSize = m_pClassifyType->size();
		int i = 0;
		std::unordered_map<std::wstring, std::wstring>::const_iterator iter = m_pClassifyType->begin();
		for (; i < tmpSize - 1; ++i)
		{
			wsprintfW(tmpStr, L"%s\"%s\":\"%s\",\n", tmpStr, iter->first.c_str(), \
				iter->second.c_str());
			++iter;
		}
		wsprintfW(tmpStr, L"%s\"%s\":\"%s\"}\n", tmpStr, iter->first.c_str(), \
			iter->second.c_str());
	}
	fwrite(tmpStr, sizeof(wchar_t), wcslen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(wchar_t) * Length);


	return true;
}


std::vector<std::wstring> CLabelManager::GetBarcode()
{
	std::vector<std::wstring> pVec;
	if (m_pBarcode != nullptr)
	{
		std::unordered_map < std::wstring, std::wstring>::const_iterator iter = m_pBarcode->begin();
		for (; iter != m_pBarcode->end(); ++iter)
		{
			pVec.push_back(iter->first);
		}
	}
	return pVec;
}


std::vector<std::wstring> CLabelManager::GetInternalType()
{
	std::vector<std::wstring> pVec;
	return pVec;
}


std::vector<std::wstring> CLabelManager::GetClassifyType()
{
	std::vector<std::wstring> pVec;
	if (m_pClassifyType != nullptr)
	{
		std::unordered_map < std::wstring, std::wstring>::const_iterator iter = m_pClassifyType->begin();
		for (; iter != m_pClassifyType->end(); ++iter)
		{
			pVec.push_back(iter->first);
		}
	}
	return pVec;
}
void CLabelManager::UpdateBarcode(const char *xmlContent, size_t len)
{

}
std::wstring CLabelManager::GetExternalTypeByBarcode(std::wstring barcode)
{
	if (m_pBarcode != nullptr)
	{
		std::unordered_map<std::wstring, std::wstring>::const_iterator iter = m_pBarcode->find(barcode);
		if (iter != m_pBarcode->end())
		{
			return iter->second;
		}
	}
	return std::wstring();
}
std::wstring CLabelManager::GetExternalTypeByClassifyType(std::wstring classifyType)
{
	std::wstring internalType;
	if (m_pClassifyType != nullptr)
	{
		std::unordered_map<std::wstring, std::wstring>::const_iterator iter = m_pClassifyType->find(classifyType);
		if (iter != m_pClassifyType->end())
		{
			internalType = iter->second;
		}
	}
	if (internalType.size() == 0)
	{
		return internalType;
	}
	if (m_pBarcode != nullptr)
	{
		std::unordered_map<std::wstring, std::wstring>::const_iterator iter = m_pBarcode->find(internalType);
		if (iter != m_pBarcode->end())
		{
			return iter->second;
		}
	}
	return std::wstring();
}
