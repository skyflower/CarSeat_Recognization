#include "TypeIDManager.h"


CTypeIDManager *CTypeIDManager::m_pInstance = nullptr;

CTypeIDManager::CTypeIDManager():m_pTypeID(nullptr)
{
	
}


CTypeIDManager::~CTypeIDManager()
{
}

CTypeIDManager::typeID CTypeIDManager::AddType(std::wstring desc)
{
	if (m_pTypeID == nullptr)
	{
		m_pTypeID = new std::unordered_map<typeID, std::wstring>;
	}
	typeID tmpID = __calcTypeID(desc);
	m_pTypeID->insert(std::make_pair(tmpID, desc));
	return tmpID;
}

CTypeIDManager::typeID CTypeIDManager::GetTypeID(std::wstring desc)
{
	if (m_pTypeID != nullptr)
	{
		std::unordered_map<typeID, std::wstring>::iterator iter = m_pTypeID->begin();
		for (; iter != m_pTypeID->end(); ++iter)
		{
			if (iter->second == desc)
			{
				return iter->first;
			}
		}
	}
	return typeID(0);
}

std::wstring CTypeIDManager::GetTypeString(typeID type)
{
	if (m_pTypeID != nullptr)
	{
		std::unordered_map<typeID, std::wstring>::iterator iter = m_pTypeID->find(type);
		if (iter == m_pTypeID->end())
		{
			return std::wstring();
		}
		return iter->second;
	}
	return std::wstring();
}

CTypeIDManager * CTypeIDManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CTypeIDManager;
	}
	return m_pInstance;
}

CTypeIDManager::typeID CTypeIDManager::__calcTypeID(const std::wstring &desc)
{
	if (desc.size() == 0)
	{
		return 0;
	}
	static unsigned int id = 1;
	id++;
	return id;

	unsigned int  value = 0x78163452;
	for (unsigned int i = 0; i < desc.size(); ++i)
	{
		value = (((value << 4) ^ (desc[i])) + desc[i]) & 0xFFFFFFFF;
	}
	return value;
}
