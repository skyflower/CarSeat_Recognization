#include "stdafx.h"

#include "ImageManager.h"



CImageManager::CImageManager():m_bTerminate(true)
{
	m_pFiles = new std::list<imageOperator>;	
}

void CImageManager::OperatorImage(const imageOperator & tmp)
{
	switch (tmp.m_operator)
	{
	case OPERTAOR::MOVEFILE:
		MoveFile(tmp.m_srcFile.c_str(), tmp.m_dstFile.c_str());
		break;
	case OPERTAOR::RENAME:
		_wrename(tmp.m_srcFile.c_str(), tmp.m_dstFile.c_str());
		break;
	case OPERTAOR::RENAME_MOVEFILE:
		_wrename(tmp.m_srcFile.c_str(), tmp.m_dstFile.c_str());
		break;
	default:
		break;
	}
}


CImageManager * CImageManager::GetInstance()
{
	static CImageManager instance;
	return &instance;
}

void CImageManager::Start()
{
	m_thread = std::thread(&CImageManager::Run, this);
}

CImageManager::~CImageManager()
{
	m_bTerminate = false;
	if (m_thread.joinable())
	{
		m_thread.join();
	}

	delete m_pFiles;
	//m_pFiles = NULL
}

void CImageManager::PushFile(CImageManager::OPERTAOR type, pathType srcFile, pathType dstFile)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	imageOperator tmp;
	tmp.m_operator = type;
	tmp.m_srcFile = srcFile;
	tmp.m_dstFile = dstFile;
	m_pFiles->push_back(tmp);
}

void CImageManager::Run(CImageManager * pThis)
{
	while (pThis->m_bTerminate)
	{
		std::unique_lock<std::mutex> lck(pThis->m_mutex, std::defer_lock);
		if (lck.try_lock())
		{
			if (pThis->m_pFiles->size() == 0)
			{
				lck.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
				continue;
			}
			imageOperator tmp = *pThis->m_pFiles->begin();
			pThis->m_pFiles->pop_front();

			lck.unlock();

			pThis->OperatorImage(tmp);
		}
	}
}

