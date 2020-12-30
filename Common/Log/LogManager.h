#pragma once
#include "Logger.h"


#define _WINSOCKAPI_

#include <windows.h>
#include <atomic>
#include <thread>
#include<iostream>
#define _CRT_SECURE_NO_WARNINGS
class CLoggerManager
{
private:
	CLoggerManager();

	~CLoggerManager();


public:
	static CLoggerManager* GetInstance();

	void AddLog(NLogger* pLogger);

	void DoWriteLog();

	void StartWriteLogThread();

	void Stop();

	static void Destory()
	{
		if (nullptr != s_pLogManager)
		{
			delete s_pLogManager;
			s_pLogManager = nullptr;
		}
	};

private:
	// This is important
	class GarbageCollector  // ����������
	{
	public:

		~GarbageCollector()
		{
			// We can destory all the resouce here, eg:db connector, file handle and so on
			if (s_pLogManager != nullptr)
			{
				delete s_pLogManager;
				s_pLogManager = nullptr;
			}
		}
	};
	static GarbageCollector  gc;  //����������ľ�̬��Ա



private:
	static CLoggerManager* s_pLogManager;

	std::vector<NLogger*> m_loggerList;

	std::unique_ptr<std::thread> m_thread;

	std::atomic< bool> m_bRunning; //�߳��Ƿ�������

};

