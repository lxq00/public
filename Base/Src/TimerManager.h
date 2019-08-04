#ifndef __TIMERMANAGER_H__
#define __TIMERMANAGER_H__
#include "Base/IntTypes.h"
#include "Base/Thread.h"
#include "Base/Time.h"
#include "Base/Guard.h"
#include "Base/Semaphore.h"
namespace Public {
namespace Base {

class TimerObject
{
public:
	TimerObject() {}
	virtual ~TimerObject() {}

	virtual bool checkIsNeedRun() = 0;
	virtual bool runFunc() = 0;
	virtual int getPeriod() = 0;
	virtual bool reset() = 0;
};

class TimerThread;
class ITimerThreadManager
{
public:
	ITimerThreadManager() {}
	virtual ~ITimerThreadManager() {}
	virtual bool resetTimer(TimerObject * pTimer, uint64_t usedTime) = 0;
	virtual TimerObject * getNeedRunTimer() = 0;
	virtual bool putTimerTimeoutAndDestory(TimerThread *thread) = 0;
};

#define TIMERTHREADTIMEOUT		60000
#define MINTHREADPOOLSIZE		2
#define MAXTHREADPOOLSIZE		10

/// ʹ��ʹ���̳߳�ģʽ
class TimerThread : public Thread
{
public:
	TimerThread(ITimerThreadManager* _manager) :Thread("[Pooled]", priorTop + 5),manager(_manager) ,prevUsedTime(Time::getCurrentMilliSecond())
	{
	}
	~TimerThread()
	{
		Thread::cancelThread();
		destroyThread();
	}
private:
	void threadProc()
	{
		while (looping())
		{
			TimerObject * runtimer = manager->getNeedRunTimer();
			if (runtimer != NULL)
			{
				uint64_t startTime = Time::getCurrentMilliSecond();
				runtimer->runFunc();
				uint64_t stopTime = Time::getCurrentMilliSecond();
				manager->resetTimer(runtimer, stopTime >= startTime ? stopTime - startTime : 0);

				prevUsedTime = Time::getCurrentMilliSecond();
			}

			uint64_t nowtime = Time::getCurrentMilliSecond();
			if (nowtime < prevUsedTime || nowtime > prevUsedTime + TIMERTHREADTIMEOUT)
			{
				if (manager->putTimerTimeoutAndDestory(this))
				{
					break;
				}

				prevUsedTime = nowtime;
			}
		}
	}
private:
	ITimerThreadManager *	manager;
	uint64_t 				prevUsedTime;
};


class TimerManager : public Thread,public ITimerThreadManager
{
	struct TimerRunInfo
	{
		uint32_t	runTimes;
		uint64_t	usedTime;
	};
public:
	TimerManager(): Thread("TimerManager", Thread::priorTop), prevPrintTime(0)
	{
		curTime = Time::getCurrentMilliSecond();

		createThread();
	}
	/// ��������
	~TimerManager()
	{
		destroyThread();

		timerThreadPool.clear();
		destoryTimerThreadList.clear();
		timerList.clear();
	}
	bool addTimer(TimerObject * pTimer)
	{
		Guard locker(timerMutex);

		std::map<TimerObject*, TimerRunInfo>::iterator iter = timerList.find(pTimer);
		if (iter == timerList.end())
		{
			TimerRunInfo info = { 0,0 };
			timerList[pTimer] = info;
		}

		return true;
	}
	bool removeTimer(TimerObject * pTimer)
	{
		Guard locker(timerMutex);
		waitAndDoingTimer.erase(pTimer);
		timerList.erase(pTimer);

		return true;
	}
	static shared_ptr<TimerManager> getManager()
	{
		static Mutex				  timerMutex;
		static weak_ptr<TimerManager> manager;

		{
			Guard locker(timerMutex);
			shared_ptr<TimerManager> timermanager = manager.lock();
			if (timermanager == NULL)
			{
				timermanager = shared_ptr<TimerManager>(new TimerManager());
				manager = timermanager;
			}

			return timermanager;
		}
	}
private:
	virtual TimerObject * getNeedRunTimer()
	{
		timerSem.pend(100);
		
		Guard locker(timerMutex);
		if (needRunTimer.size() <= 0) return NULL;

		TimerObject * timer = needRunTimer.front();
		needRunTimer.pop_front();

		std::set<TimerObject*>::iterator iter = waitAndDoingTimer.find(timer);
		if (iter == waitAndDoingTimer.end()) return NULL;

		return timer;
	}



	virtual bool putTimerTimeoutAndDestory(TimerThread *thread)
	{
		Guard locker(timerMutex);
		if (timerThreadPool.size() <= MINTHREADPOOLSIZE) return false;

		std::map<TimerThread*, shared_ptr<TimerThread> >::iterator iter = timerThreadPool.find(thread);
		if (iter != timerThreadPool.end())
		{
			destoryTimerThreadList.insert(iter->second);
			timerThreadPool.erase(iter);
		}

		return true;
	}

	bool resetTimer(TimerObject * pTimer, uint64_t usedTime)
	{
		Guard locker(timerMutex);

		waitAndDoingTimer.erase(pTimer);
		std::map<TimerObject*, TimerRunInfo>::iterator iter = timerList.find(pTimer);
		if (iter != timerList.end())
		{
			iter->first->reset();
			if (iter->first->getPeriod() == 0)
			{
				timerList.erase(iter);
				return false;
			}

			iter->second.runTimes++;
			iter->second.usedTime += usedTime;
		}


		return true;
	}
	void threadProc()
	{
#define MAXTIMERSLEEPTIME		10

		while (looping())
		{
			setTimeout(10000); // ���ó�ʱʱ��Ϊ10���ӣ���ʱ���Ź�������
			Thread::sleep(10);

			
			{
				Guard locker(timerMutex);

				uint64_t OldTime = curTime;
				curTime = Time::getCurrentMilliSecond();

				// ��ʱû�иı䣬��������Ϊ��ʱ�����Ȳ���
				if (curTime == OldTime)
				{
					continue;
				}

				if (curTime < OldTime)
				{
					//	assert(0); // overflowd
				}


				for (std::map<TimerObject*, TimerRunInfo>::iterator iter = timerList.begin(); iter != timerList.end(); iter++)
				{
					std::set<TimerObject*>::iterator witer = waitAndDoingTimer.find(iter->first);
					if (witer != waitAndDoingTimer.end()) continue;

					if (iter->first->checkIsNeedRun())
					{
						needRunTimer.push_back(iter->first);
						waitAndDoingTimer.insert(iter->first);

						timerSem.post();
					}
				}
			}

			checkTimerThread();
		}
	}
	
	void checkTimerThread()
	{
		{
			std::set<shared_ptr<TimerThread> > freeList;
			{
				Guard locker(timerMutex);
				freeList = destoryTimerThreadList;
				destoryTimerThreadList.clear();
			}
		}


		{
			Guard locker(timerMutex);
			int newthreadsize = 0;
			if (timerThreadPool.size() < MINTHREADPOOLSIZE)
			{
				//��횱��C2��
				newthreadsize = (int)(MINTHREADPOOLSIZE - timerThreadPool.size());
			}
			if (timerThreadPool.size() < MAXTHREADPOOLSIZE)
			{
				//thread 2 -> needRunTimer.size() <=5
				//thread 4 -> needRunTimer.size() 5 ~ 10
				//thread 6 -> needRunTimer.size() 10 ~ 30
				//thread 8 -> needRunTimer.size() 30 ~ 80
				//thread 10 -> needRunTimer.size() > 80

				int neednewthreadsize = 0;
				if (needRunTimer.size() > 5 && needRunTimer.size() <= 10 && timerThreadPool.size() < 4)
				{
					neednewthreadsize = int(4 - timerThreadPool.size());
				}
				else if (needRunTimer.size() > 10 && needRunTimer.size() <= 30 && timerThreadPool.size() < 6)
				{
					neednewthreadsize = int(6 - timerThreadPool.size());
				}
				else if (needRunTimer.size() > 30 && needRunTimer.size() <= 80 && timerThreadPool.size() < 8)
				{
					neednewthreadsize = int(8 - timerThreadPool.size());
				}
				else if(needRunTimer.size() > 80)
				{
					neednewthreadsize = int(MAXTHREADPOOLSIZE - timerThreadPool.size());
				}

				newthreadsize = max(newthreadsize, neednewthreadsize);
			}
			if (newthreadsize + timerThreadPool.size() > MAXTHREADPOOLSIZE)
			{
				newthreadsize = int(MAXTHREADPOOLSIZE - timerThreadPool.size());
			}

			for (int i = 0; i < newthreadsize; i++)
			{
				TimerThread * p = new TimerThread(this);
				shared_ptr<TimerThread> timerthread(p);
				if (timerthread->createThread())
				{
					timerThreadPool[p] = timerthread;
				}
			}
		}
	}
private:
	std::map<TimerThread*,shared_ptr<TimerThread> > 			timerThreadPool;
	std::set<shared_ptr<TimerThread> > 							destoryTimerThreadList;


	std::map<TimerObject*, TimerRunInfo> 				timerList;
	std::list<TimerObject*>							needRunTimer;
	std::set<TimerObject*>								waitAndDoingTimer;
	Mutex 														timerMutex;
	Semaphore													timerSem;

	uint64_t													prevPrintTime;

public:
	uint64_t 										curTime;
};

} // namespace Base
} // namespace Public


#endif //__TIMERMANAGER_H__
