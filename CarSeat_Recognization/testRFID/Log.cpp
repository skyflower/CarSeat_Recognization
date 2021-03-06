﻿
#include "Log.h"
#include "utils.h"
#include <cstdlib>
#include <io.h>
#include <Windows.h>

CLog *CLog::m_pInstance = nullptr;


CLog * CLog::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CLog();
	}
	return m_pInstance;
}
 
const CLog & CLog::GetReference()
{
	// TODO: 在此处插入 return 语句
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CLog();
	}
	return *m_pInstance;
}

int CLog::Write(CLog::LogType a, char* func, int line, char *fmt, ...)
{
	CLog *pLog = GetInstance();
	if (pLog->m_pThread.native_handle() == nullptr)
	{
		pLog->m_pThread = std::thread(&CLog::run, pLog);
		
	}
	if (pLog == nullptr)
	{
		return -1;
	}
	std::unique_lock<std::mutex> lck(pLog->m_pMutex , std::defer_lock);
	int cnt = 0;
	if (lck.try_lock())
	{
		//size_t tmp = pLog->m_nCurBlank;
		LogMessage pMesg;// = &pLog->m_pMessage[tmp];
		memset(&pMesg, 0, sizeof(LogMessage));
		pMesg.type = a;
		memcpy(pMesg.pFunc, func, strnlen_s(func, sizeof(pMesg.pFunc)));
		pMesg.mLine = line;
		va_list argptr;

		__crt_va_start(argptr, fmt);
		cnt = vsprintf_s(pMesg.data, fmt, argptr);
		__crt_va_end(argptr);
		//pLog->m_nCurBlank = (tmp + 1) % MAX_LOG_NUM;
		//pLog->SetFlag(true);
		pLog->m_pListMessage->push_back(pMesg);
		pLog->m_nMsgCount++;
		pLog->m_pCond.notify_all();
	}
	
	return cnt;
}


void CLog::run()
{
	LogMessage buffer;
	char currentTime[100];
	SYSTEMTIME curTime;
	
	while (1)
	{
		CLog *pLog = CLog::GetInstance();
		std::unique_lock<std::mutex> lck(m_pMutex, std::defer_lock);
		if (!lck.try_lock())
		{
			continue;
		}
		
		m_pCond.wait(lck, [&pLog] {return pLog->m_nMsgCount > 0; });

		//m_pMessage[m_nCurValid];
		
		/*memcpy(&buffer, &m_pMessage[m_nCurValid], sizeof(LogMessage));
		memset(&m_pMessage[m_nCurValid], 0, sizeof(LogMessage));
		m_nCurValid = (m_nCurValid + 1) % MAX_LOG_NUM;
		if (m_nCurBlank == m_nCurValid)
		{
			pLog->SetFlag(false);
		}*/
		memset(&buffer, 0, sizeof(LogMessage));
		std::list<LogMessage>::iterator iter = pLog->m_pListMessage->begin();
		memcpy(&buffer, &*iter, sizeof(LogMessage));
		pLog->m_pListMessage->pop_front();
		--m_nMsgCount;
		lck.unlock();
		if (m_pLog.good())
		{
			if (buffer.type == LogType::FATAL_ERROR)
			{
				m_pLog << "Error\t";
			}
			else if (buffer.type == LogType::INFO)
			{
				m_pLog << "Info\t";
			}
			else if (buffer.type == LogType::WARNING)
			{
				m_pLog << "Warning\t";
			}
			else if (buffer.type == LogType::EXIT)
			{
				break;
			}
			
			memset(&curTime, 0, sizeof(curTime));
			GetLocalTime(&curTime);

			memset(currentTime, 0, sizeof(currentTime));

			sprintf_s(currentTime, "%04d%02d%02d:%02d%02d%02d:%d", \
				curTime.wYear, curTime.wMonth, curTime.wDay, \
				curTime.wHour, curTime.wMinute, curTime.wSecond, curTime.wMilliseconds);

			m_pLog << currentTime << "\t";


			m_pLog << "[" << buffer.pFunc << "," << buffer.mLine << "] " << buffer.data << "\n";
			m_pLog.flush();
		}
	}
	m_pLog.close();
}

CLog::CLog()
{
	init();
}

void CLog::init()
{
	m_pListMessage = new std::list<LogMessage>;
	m_nMsgCount = 0;

	
	time_t  time1 = time(NULL);//获取系统时间，单位为秒;

	struct tm tmpTime;
	localtime_s(&tmpTime, &time1);//转换成tm类型的结构体;

	char logFileName[100] = { 0 };
	memset(logFileName, 0, sizeof(logFileName));

	char *LogDir = "./Log/";

	if (_access(LogDir, 0) != 0)
	{
		wchar_t *tmpPath = utils::CharToWchar(LogDir);
		if (tmpPath != nullptr)
		{
			_wmkdir(tmpPath);
			delete tmpPath;
			tmpPath = nullptr;
		}
	}

	sprintf_s(logFileName, "%stestRFID_%04d%02d%02d_%02d%02d%02d.txt", \
		LogDir, tmpTime.tm_year + 1900, tmpTime.tm_mon + 1, tmpTime.tm_mday, \
		tmpTime.tm_hour, tmpTime.tm_min, tmpTime.tm_sec);

	m_pLog.open(logFileName, std::ios::trunc | std::ios::in | std::ios::out | std::ios::binary);
}


CLog::~CLog()
{
	if (m_pThread.joinable())
	{
		WriteExit("End");
		m_pThread.join();
	}
	//std::chrono::duration<int, std::milli> a = std::chrono::milliseconds(100);
	//std::this_thread::sleep_for(a);
	//m_pLog.close();
	if (m_pListMessage != NULL)
	{
		delete m_pListMessage;
		m_pListMessage = NULL;
	}
	/*if (m_pMessage != nullptr)
	{
		delete[]m_pMessage;
		m_pMessage = nullptr;
	}*/
}
