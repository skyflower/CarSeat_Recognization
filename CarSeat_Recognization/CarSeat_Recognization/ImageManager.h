#pragma once

#include <iostream>
#include <list>
#include <thread>
#include <mutex>


class CImageManager;

struct imageOperator
{
	CImageManager::OPERTAOR m_operator;
	CImageManager::pathType m_srcFile;
	CImageManager::pathType m_dstFile;
};


class CImageManager
{
public:

	enum OPERTAOR
	{
		RENAME,
		MOVEFILE,
		RENAME_MOVEFILE
	};



	typedef std::wstring pathType;

	CImageManager *GetInstance();

	void Start();

	

	
	~CImageManager();

	void PushFile(CImageManager::OPERTAOR type, pathType srcFile, pathType dstFile);

	static void Run(CImageManager *pThis);

private:

	CImageManager();

	void OperatorImage(const imageOperator &tmp);

	std::list<imageOperator> *m_pFiles;

	std::thread m_thread;
	std::mutex m_mutex;

	bool m_bTerminate;
};

