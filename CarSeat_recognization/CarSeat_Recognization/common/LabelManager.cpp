#include "LabelManager.h"
#include <cstdlib>
#include "./Log.h"
#include <sys/stat.h>
#include "./utils.h"
#include <Windows.h>

bool CLabelManager::m_bInitFlag = false;

CLabelManager::CLabelManager()
{
	m_pBarcode = nullptr;
	m_pClassifyType = nullptr;
	memset(m_strLoginName, 0, sizeof(m_strLoginName));
	memset(m_strLoginPasswd, 0, sizeof(m_strLoginPasswd));
	m_bLoginAutoSave = false;
	
	if (m_bInitFlag == false)
	{
		m_bInitFlag = init();
		m_bSerialize = false;
	}
}


CLabelManager::~CLabelManager()
{
	if (m_pBarcode != nullptr)
	{
		delete m_pBarcode;
		m_pBarcode = nullptr;
	}
	if (m_pClassifyType != nullptr)
	{
		delete m_pClassifyType;
		m_pClassifyType = nullptr;
	}
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

	char tmpValue[MAX_USR_NAME_AND_PASSWD] = { 0 };
	memset(tmpValue, 0, sizeof(tmpValue));

	if (true == utils::getValueByName(content, "LoginUsrName", tmpValue))
	{
		wchar_t *tmp = utils::CharToWchar(tmpValue);
		if (tmp != nullptr)
		{
			wmemcpy(m_strLoginName, tmp, wcslen(tmp));
			delete[]tmp;
			tmp = nullptr;
		}
		
	}

	memset(tmpValue, 0, sizeof(tmpValue));
	if (true == utils::getValueByName(content, "LoginPasswd", tmpValue))
	{
		wchar_t *tmp = utils::CharToWchar(tmpValue);
		if (tmp != nullptr)
		{
			wmemcpy(m_strLoginPasswd, tmp, wcslen(tmp));
			delete[]tmp;
			tmp = nullptr;
		}
	}

	memset(tmpValue, 0, sizeof(tmpValue));
	if (true == utils::getValueByName(content, "LoginInfoAutoSave", tmpValue))
	{
		if (tmpValue[0] == L'1')
		{
			m_bLoginAutoSave = true;
		}
		else
		{
			m_bLoginAutoSave = false;
		}
	}

	memset(tmpValue, 0, sizeof(tmpValue));
	if (true == utils::getValueByName(content, "ExposureTimeMax", tmpValue))
	{
		m_nExposureTimeMax = atoi(tmpValue);
	}

	memset(tmpValue, 0, sizeof(tmpValue));
	if (true == utils::getValueByName(content, "ExposureTimeMin", tmpValue))
	{
		m_nExposureTimeMin = atoi(tmpValue);
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
	if (m_bSerialize == false)
	{
		return false;
	}
	FILE *fp = nullptr;
	fopen_s(&fp, "labelConfig.txt", "wb");
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

	wsprintfW(tmpStr, L"\nExposureTimeMax=%d\nExposureTimeMin=%d\n", \
		m_nExposureTimeMax, m_nExposureTimeMin);
	fwrite(tmpStr, sizeof(wchar_t), wcslen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(wchar_t) * Length);


	wsprintfW(tmpStr, L"\nLoginUsrName=%s\nLoginPasswd=%s\nLoginInfoAutoSave=%d\n", \
		m_strLoginName, m_strLoginPasswd, (m_bLoginAutoSave == true ? 1 : 0));
	fwrite(tmpStr, sizeof(wchar_t), wcslen(tmpStr), fp);
	//memset(tmpStr, 0, sizeof(wchar_t) * Length);


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

std::vector<std::wstring> CLabelManager::GetExternalType()
{
	std::vector<std::wstring> pVec;
	if (m_pBarcode != nullptr)
	{
		std::unordered_map < std::wstring, std::wstring>::const_iterator iter = m_pBarcode->begin();
		for (; iter != m_pBarcode->end(); ++iter)
		{
			pVec.push_back(iter->second);
		}
	}
	return pVec;
}


//std::vector<std::wstring> CLabelManager::GetInternalType()
//{
//	std::vector<std::wstring> pVec;
//	return pVec;
//}


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
bool CLabelManager::SetLoginUsrName(const wchar_t * name)
{
	if ((name == nullptr) || (wcslen(name) > MAX_USR_NAME_AND_PASSWD))
	{
		return false;
	}
	memset(m_strLoginName, 0, sizeof(m_strLoginName));
	wmemcpy(m_strLoginName, name, wcslen(name));
	m_bSerialize = true;
	return true;
}
bool CLabelManager::SetLoginPasswd(const wchar_t * passwd)
{
	if ((passwd == nullptr) || (wcslen(passwd) > MAX_USR_NAME_AND_PASSWD))
	{
		return false;
	}
	memset(m_strLoginPasswd, 0, sizeof(m_strLoginPasswd));
	wmemcpy(m_strLoginPasswd, passwd, wcslen(passwd));
	m_bSerialize = true;
	return true;
}
bool CLabelManager::GetLoginAutoSave()
{
	return m_bLoginAutoSave;
}
void CLabelManager::SetLoginAutoSave(bool autoSave)
{
	if (autoSave == m_bLoginAutoSave)
	{
		return;
	}
	m_bLoginAutoSave = autoSave;
	m_bSerialize = true;
}
int CLabelManager::GetExposureTimeMax()
{
	return m_nExposureTimeMax;
}
int CLabelManager::GetExposureTimeMin()
{
	return m_nExposureTimeMin;
}
const wchar_t * CLabelManager::GetLoginUsrName() const
{
	return m_strLoginName;
}
const wchar_t * CLabelManager::GetLoginPasswd() const
{
	return m_strLoginPasswd;
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

std::wstring CLabelManager::GetClassifyTypeByExternal(std::wstring externalType)
{
	std::wstring barcode;
	if (m_pBarcode != nullptr)
	{
		
		for (auto &k : (*m_pBarcode))
		{
			if (k.second == externalType)
			{
				barcode = k.first;
				break;
			}
		}
	}
	if (barcode.size() > 0)
	{
		for (auto &k : (*m_pClassifyType))
		{
			if (k.second == barcode)
			{
				return k.first;
			}
		}
	}
	return std::wstring();
}
