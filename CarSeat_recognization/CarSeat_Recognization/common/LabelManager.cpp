#include "LabelManager.h"
#include <cstdlib>
#include "./Log.h"
#include <sys/stat.h>
#include "./utils.h"
//#include <Windows.h>
 
bool CLabelManager::m_bInitFlag = false;

CLabelManager::CLabelManager()
{
	m_pBarcode = nullptr;
	m_pClassifyType = nullptr;
	memset(m_strLoginName, 0, sizeof(m_strLoginName));
	memset(m_strLoginPasswd, 0, sizeof(m_strLoginPasswd));
	m_bLoginAutoSave = false;
	memset(m_configFile, 0, sizeof(m_configFile));
	strcpy_s(m_configFile, "labelConfig.txt");

	
	if (m_bInitFlag == false)
	{
		m_bInitFlag = init();
		m_bSerialize = false;
	}
}


CLabelManager::~CLabelManager()
{
	if (m_bSerialize == true)
	{
		serialize();
	}
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

std::string CLabelManager::GetInternalTypeByBarcode(std::string barcode)
{
	if (barcode.size() < 5)
	{
		return std::string();
	}

	char tmp[10] = { 0 };
	memset(tmp, 0, sizeof(tmp));

	for (int i = 0; (i < 3) && (i + 5 < barcode.size()); ++i)
	{
		tmp[i] = barcode[i + 5];
	}

	return std::string(tmp);
}

std::string CLabelManager::GetInternalTypeByClassifyType(std::string type)
{
	if (m_pClassifyType != nullptr)
	{
		std::unordered_map<std::string, std::string>::const_iterator iter = m_pClassifyType->begin();
		for (; iter != m_pClassifyType->end(); ++iter)
		{
			if (iter->second == type)
			{
				return iter->first;
			}
		}
	}
	return std::string();
}

bool CLabelManager::init()
{
	size_t nSize = 0;
	char *content = nullptr;
	utils::readFile(m_configFile, content, nSize);
	if ((content == nullptr) || (nSize == 0))
	{
		if (content != nullptr)
		{
			delete[]content;
			content = nullptr;
		}
		return false;
	}

	char tmpValue[MAX_USR_NAME_AND_PASSWD] = { 0 };
	memset(tmpValue, 0, sizeof(tmpValue));

	if (true == utils::getValueByName(content, "LoginUsrName", m_strLoginName))
	{
		
		//memcpy(m_strLoginName, tmpValue, strlen(tmpValue));
		
	}

	memset(tmpValue, 0, sizeof(tmpValue));
	if (true == utils::getValueByName(content, "LoginPasswd", m_strLoginPasswd))
	{
		
		//memcpy(m_strLoginPasswd, tmpValue, strlen(tmpValue));
		
	}

	memset(tmpValue, 0, sizeof(tmpValue));
	if (true == utils::getValueByName(content, "LoginInfoAutoSave", tmpValue))
	{
		
		if (tmpValue[0] == '1')
		{
			m_bLoginAutoSave = true;
		}
		else if(tmpValue[0] == '0')
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
		m_pBarcode = new std::unordered_map<std::string, std::string>;
	}
	if (m_pClassifyType == nullptr)
	{
		m_pClassifyType = new std::unordered_map<std::string, std::string>;
	}

	char *str[] = { "barcodeTable", "classifyType" };
	std::unordered_map<std::string, std::string> *p[2];
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
	fopen_s(&fp, m_configFile, "wb");
	if (fp == nullptr)
	{
		WriteError("Failed labelConfig.txt.bak");
		return false;
	}

	const size_t Length = 2000;
	char *tmpStr = new char[Length];
	memset(tmpStr, 0, sizeof(char) * Length);

	if ((m_pBarcode != nullptr) && (m_pBarcode->size() > 0))
	{
		strcat_s(tmpStr, Length, "barcodeTable={");
		//strcat_s(tmpStr, Length, "")
		size_t tmpSize = m_pBarcode->size();
		int i = 0;
		std::unordered_map<std::string, std::string>::const_iterator iter = m_pBarcode->begin();
		for (; i < tmpSize - 1; ++i)
		{
			sprintf_s(tmpStr, Length, "%s\"%s\":\"%s\",\n", tmpStr, iter->first.c_str(), \
				iter->second.c_str());
			++iter;
		}
		sprintf_s(tmpStr, Length, "%s\"%s\":\"%s\"}\n", tmpStr, iter->first.c_str(), \
			iter->second.c_str());
	}
	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(char) * Length);


	if ((m_pClassifyType != nullptr) && (m_pClassifyType->size() > 0))
	{
		strcat_s(tmpStr, Length, "classifyType={");
		size_t tmpSize = m_pClassifyType->size();
		int i = 0;
		std::unordered_map<std::string, std::string>::const_iterator iter = m_pClassifyType->begin();
		for (; i < tmpSize - 1; ++i)
		{
			sprintf_s(tmpStr, Length, "%s\"%s\":\"%s\",\n", tmpStr, iter->first.c_str(), \
				iter->second.c_str());
			++iter;
		}
		sprintf_s(tmpStr, Length, "%s\"%s\":\"%s\"}\n", tmpStr, iter->first.c_str(), \
			iter->second.c_str());
	}
	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(char) * Length);

	sprintf_s(tmpStr, Length, "\nExposureTimeMax=\"%d\"\nExposureTimeMin=\"%d\"\n", \
		m_nExposureTimeMax, m_nExposureTimeMin);

	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	memset(tmpStr, 0, sizeof(char) * Length);


	sprintf_s(tmpStr, Length, "\nLoginUsrName=\"%s\"\nLoginPasswd=\"%s\"\nLoginInfoAutoSave=\"%d\"\n", \
		m_strLoginName, m_strLoginPasswd, (m_bLoginAutoSave == true ? 1 : 0));
	fwrite(tmpStr, sizeof(char), strlen(tmpStr), fp);
	//memset(tmpStr, 0, sizeof(wchar_t) * Length);

	delete[]tmpStr;
	tmpStr = nullptr;

	return true;
}


std::vector<std::string> CLabelManager::GetBarcode()
{
	std::vector<std::string> pVec;
	if (m_pBarcode != nullptr)
	{
		std::unordered_map < std::string, std::string>::const_iterator iter = m_pBarcode->begin();
		for (; iter != m_pBarcode->end(); ++iter)
		{
			pVec.push_back(iter->first);
		}
	}
	return pVec;
}

std::vector<std::string> CLabelManager::GetExternalType()
{
	std::vector<std::string> pVec;
	if (m_pBarcode != nullptr)
	{
		std::unordered_map < std::string, std::string>::const_iterator iter = m_pBarcode->begin();
		for (; iter != m_pBarcode->end(); ++iter)
		{
			pVec.push_back(iter->second);
		}
	}
	return pVec;
}


//std::vector<std::string> CLabelManager::GetInternalType()
//{
//	std::vector<std::string> pVec;
//	return pVec;
//}


std::vector<std::string> CLabelManager::GetClassifyType()
{
	std::vector<std::string> pVec;
	if (m_pClassifyType != nullptr)
	{
		std::unordered_map < std::string, std::string>::const_iterator iter = m_pClassifyType->begin();
		for (; iter != m_pClassifyType->end(); ++iter)
		{
			pVec.push_back(iter->first);
		}
	}
	return pVec;
}
bool CLabelManager::SetLoginUsrName(const char * name)
{
	if ((name == nullptr) || (strlen(name) > MAX_USR_NAME_AND_PASSWD))
	{
		return false;
	}
	memset(m_strLoginName, 0, sizeof(m_strLoginName));
	memcpy(m_strLoginName, name, strlen(name));
	m_bSerialize = true;
	return true;
}
bool CLabelManager::SetLoginPasswd(const char * passwd)
{
	if ((passwd == nullptr) || (strlen(passwd) > MAX_USR_NAME_AND_PASSWD))
	{
		return false;
	}
	memset(m_strLoginPasswd, 0, sizeof(m_strLoginPasswd));
	memcpy(m_strLoginPasswd, passwd, strlen(passwd));
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
const char * CLabelManager::GetLoginUsrName() const
{
	return m_strLoginName;
}
const char * CLabelManager::GetLoginPasswd() const
{
	return m_strLoginPasswd;
}
void CLabelManager::UpdateBarcode(const char *xmlContent, size_t len)
{

}
std::string CLabelManager::GetExternalTypeByBarcode(std::string barcode)
{
	/*
	截取中间的6-8位作为条形码类型
	*/
	
	std::string tmpInternalType = GetInternalTypeByBarcode(barcode);

	if (m_pBarcode != nullptr)
	{
		std::unordered_map<std::string, std::string>::const_iterator iter = m_pBarcode->find(tmpInternalType);
		if (iter != m_pBarcode->end())
		{
			return iter->second;
		}
	}
	return std::string();
}
std::string CLabelManager::GetExternalTypeByClassifyType(std::string classifyType)
{
	std::string internalType;
	if (m_pClassifyType != nullptr)
	{
		std::unordered_map<std::string, std::string>::const_iterator iter = m_pClassifyType->find(classifyType);
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
		std::unordered_map<std::string, std::string>::const_iterator iter = m_pBarcode->find(internalType);
		if (iter != m_pBarcode->end())
		{
			return iter->second;
		}
	}
	return std::string();
}

std::string CLabelManager::GetClassifyTypeByExternal(std::string externalType)
{
	std::string barcode;
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
	return std::string();
}
