﻿#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <condition_variable>
#include <atomic>
#include <sstream>
#include <list>
 

// 添加日志类,记录此系统的所有日志。
//	需求.1.满足多线程的运行场景
//		2.长时间运行日志不能丢失。按照时间日期分别保存
//		3.需要记录函数名称以及行数.


class CLog
{
public:
	enum class LogType
	{
		WARNING,
		INFO,
		FATAL_ERROR,
		EXIT,
	};
	struct LogMessage
	{
		LogType type;
		char pFunc[100];
		unsigned int mLine;
		char data[2000];
	};

	static CLog *GetInstance();
	static const CLog& GetReference();
	
	static int Write(LogType type, char* func, int line, char *fmt, ...);	

	virtual void run();
	bool GetFlag();
private:
	enum
	{
		MAX_LOG_NUM = 10
	};

	CLog();
	void init();
	void SetFlag(bool flag);

	static CLog *m_pInstance;

	//LogMessage *m_pMessage;
	std::list<LogMessage> *m_pListMessage;
	std::mutex m_pMutex;
	//size_t m_nCurBlank;
	//size_t m_nCurValid;
	std::atomic<int> m_nMsgCount;
	std::condition_variable m_pCond;
	std::fstream m_pLog;
	
	std::thread m_pThread;

public:
	~CLog();

};


//  log日志调用接口
#define WriteInfo(x, ...) CLog::Write(CLog::LogType::INFO, __FUNCTION__, __LINE__, x, __VA_ARGS__)
#define WriteError(x, ...) CLog::Write(CLog::LogType::FATAL_ERROR, __FUNCTION__, __LINE__, x, __VA_ARGS__)
#define WriteWarning(x, ...) CLog::Write(CLog::LogType::WARNING, __FUNCTION__, __LINE__, x, __VA_ARGS__)

// 不允许调用下面这个接口
#define WriteExit(x, ...) CLog::Write(CLog::LogType::EXIT, __FUNCTION__, __LINE__, x, __VA_ARGS__)

//#define WriteLog(x)		CLog::GetReference() << __FUNCTION__ << "  " << __LINE__ << "  " << x;

//
//template<typename type>
//inline const CLog & CLog::operator<<(type value) const
//{
//	// TODO: 在此处插入 return 语句
//	CLog *pLog = GetInstance();
//	if (pLog == nullptr)
//	{
//		return *this;
//	}
//	std::unique_lock<std::mutex> lck(pLog->m_pMutex, std::defer_lock);
//	int cnt = 0;
//	if (lck.try_lock())
//	{
//		m_pStrBuf << value;
//		//m_pIostream << value;
//		
//		size_t tmp = pLog->m_nCurBlank;
//		LogMessage *pMesg = &pLog->m_pMessage[tmp];
//		memset(pMesg, 0, sizeof(LogMessage));
//		pMesg->type = LogType::INFO;
//		//memcpy(pMesg->pFunc, func, strnlen_s(func, sizeof(pMesg->pFunc)));
//		pMesg->mLine = -1;
//		va_list argptr;
//
//		//memcpy(pMesg->data, m_pStrBuf.str().c_str(), m_pStrBuf.gcount());
//		pLog->m_nCurBlank = (tmp + 1) % MAX_LOG_NUM;
//		pLog->SetFlag(true);
//		pLog->m_pCond.notify_all();
//	}
//
//	return *this;
//}
