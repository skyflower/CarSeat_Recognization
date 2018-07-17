#include "RecogResultManager.h"


CRecogResultManager* CRecogResultManager::m_pInstance = nullptr;

CRecogResultManager::CRecogResultManager():
	m_pRecogResult(nullptr)
{
	init();
}

void CRecogResultManager::init()
{
	if (m_pRecogResult == nullptr)
	{
		m_pRecogResult = new std::unordered_map<size_t, RecogResult>;
	}
	/*
	��ʼ�������ļ�����������ʱ�䳬��һ�������ϵ�ʶ���������ʼ��,not implement
	*/

}


CRecogResultManager::~CRecogResultManager()
{
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
