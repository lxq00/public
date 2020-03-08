//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: Thread.cpp 226 2013-10-29 09:10:03Z  $
//

#include <assert.h>
#include <list>

#ifdef WIN32
#include <Windows.h>
#include <stdio.h>
#include <process.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <stdio.h>
#include <algorithm>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#define TSK_DEF_STACK_SIZE 64 * 1024
#include <sys/syscall.h>
#include <signal.h>
#include <stdarg.h>
#else
#error "Unknown Platform"
#endif
#include "ThreadManager.h"
#include "Base/ThreadEx.h"
#include "Base/Timer.h"
namespace Public
{
namespace Base
{

//����ϵͳ����
#ifdef WIN32
	struct TimePerionInit
	{
		TimePerionInit()
		{
			::timeBeginPeriod(1);
		}
		~TimePerionInit()
		{
			::timeEndPeriod(1);
		}
	}_timeperiod;
#endif

//static ThreadErrorManager threadErrorManager;
//static Mutex ThreadManagerInfoMutex;
class ThreadManagerInfo : public Thread
{

#define PRINTTHREADINFOMAXLONGTIME 5 * 60 * 1000

	struct ThreadInfo
	{
		std::string threadName;
		uint32_t threadId;
		uint32_t poolCount;
	};

public:
	ThreadManagerInfo() : Thread("")
	{
		createThread();
	}
	~ThreadManagerInfo()
	{
		destroyThread();
	}

	void addThread(const Thread *addr, const std::string &name, uint32_t threadId)
	{
		ThreadInfo info;
		info.poolCount = 0;
		info.threadId = threadId;
		info.threadName = name;

		Guard locker(mutex);
		threadInfo[addr] = info;
	}
	void delThread(const Thread *addr)
	{
		Guard locker(mutex);
		threadInfo.erase(addr);
	}
	void threadPool(const Thread *addr)
	{
		Guard locker(mutex);
		std::map<const Thread *, ThreadInfo>::iterator iter = threadInfo.find(addr);
		if (iter == threadInfo.end())
		{
			return;
		}
		iter->second.poolCount++;
	}

private:
	void threadProc()
	{
		while (looping())
		{
			Thread::sleep(100);
			if (Time::getCurrentMilliSecond() >= prevTime && Time::getCurrentMilliSecond() - prevTime < PRINTTHREADINFOMAXLONGTIME)
			{
				continue;
			}
			prevTime = Time::getCurrentMilliSecond();
			std::map<const Thread *, ThreadInfo> threadlist;
			{
				Guard locker(mutex);
				for (std::map<const Thread *, ThreadInfo>::iterator iter = threadInfo.begin(); iter != threadInfo.end(); iter++)
				{
					threadlist[iter->first] = iter->second;
					iter->second.poolCount = 0;
				}
			}
			logtrace("Base Thread: ----- print Thread Run Info Start--------");
			uint32_t threadnum = 0;
			for (std::map<const Thread *, ThreadInfo>::iterator iter = threadlist.begin(); iter != threadlist.end(); iter++, threadnum++)
			{
				logtrace("Base Thread:	%d/%d	id:#%d	name:%s poolCount:%d pooltime:%dms", threadnum + 1, threadlist.size(), iter->second.threadId, iter->second.threadName.c_str(), iter->second.poolCount, PRINTTHREADINFOMAXLONGTIME);
			}
			logtrace("Base Thread: ----- print Thread Run Info End--------");
		}
	}

private:
	Mutex mutex;
	std::map<const Thread *, ThreadInfo> threadInfo;
	uint64_t prevTime;
};

struct ThreadInternal
{
	thread_handle_t handle;

	int running;
	int policy;
	int priority;
	int stacksize;
	int id;
	std::string name;
	Semaphore createSem;
	Semaphore quitSem;	 ///< ���ź���������ֹͬһ��������߳�ͬʱ��������Σ�
	uint64_t expectedtime; ///< Ԥ��ִ�н���ʱ��ʱ�䣬0��ʾ��Ԥ��
	bool loop;
	Mutex mutex; ///< ������
	XM_ErrorInfo errinfo;

#ifdef WIN32
	static void threadEnryPointer(void *param)
#else
	static void *threadEnryPointer(void *param)
#endif // WIN32
	{
		Thread *threadptr = static_cast<Thread *>(param);
		if (threadptr == NULL || threadptr->internal == NULL)
		{
#ifdef WIN32
			return;
#else
			return NULL;
#endif
		}
#if defined(__linux__) || defined(__APPLE__)
		if (threadptr->internal->policy == Thread::policyNormal)
		{
			int priority = -19 + threadptr->internal->priority * 40 / (Thread::priorBottom + 1);

			setpriority(PRIO_PROCESS, 0, priority);
		}
#endif

		threadptr->internal->running = true;
		threadptr->internal->id = Public::Base::Thread::getCurrentThreadID();
		threadptr->internal->createSem.post();

		threadptr->threadProc();

		{
			Guard guard(threadptr->internal->mutex);
			//	threadErrorManager.cleanThreadErrorInfo(threadptr->internal->id);
			threadptr->internal->running = false;
			threadptr->internal->quitSem.post();
		}

#ifdef WIN32
		_endthread();
#elif defined(__linux__) || defined(__APPLE__)
		pthread_exit(0);
		return 0;
#endif
	}

	///���ϵͳ���߳��Ƿ���� true �̻߳�����
	bool checkThreadIsExist()
	{
		if (!running)
		{
			return false;
		}
#ifdef WIN32
		DWORD exitCode = 0;
		if (!GetExitCodeThread(handle, &exitCode) || exitCode == STILL_ACTIVE) //��ȡʧ�ܣ����ߴ���ʾ����
		{
			return true;
		}

		return false;
#elif defined(__linux__) || defined(__APPLE__)
		int killRet = pthread_kill(handle, 0); //0�źŲ���ɱ����Ϊϵͳ���Եģ�ר�ż�����߲����ߵ�
		if (killRet == ESRCH)
		{
			return false;
		}

		return true;
#endif
	}
};

#define DEFAULTTHREADNAME "Nonamed"

Thread::Thread(const std::string &name, int priority /* = priorDefault */, int policy /* = policyNormal */, int stackSize /* = 0 */)
{
	internal = new ThreadInternal;

	internal->priority = priority;
	internal->policy = policy;
	internal->stacksize = stackSize;
	internal->errinfo.errorCode = 0;
	internal->name = name == "" ? DEFAULTTHREADNAME : name;
	internal->id = -1;
	internal->running = false;
	internal->loop = false;
	internal->expectedtime = 0;
	internal->handle = 0;
}

Thread::~Thread()
{
	destroyThread();

	{
		///��һ�£��ȴ���Щ��ס�ͷŵĶ����ͷ�
		Guard locker(internal->mutex);
	}
	delete internal;
}

void Thread::setThreadName(const std::string &name)
{
	internal->name = name;
}

bool Thread::createThread()
{
	Guard guard(internal->mutex);

	if (internal->running)
	{
		return false;
	}
	// ��ô���Ȩ������loop��־
	internal->loop = true;

	// ��Ҫ�ٴγ�ʼ����Щ״̬
	internal->id = -1;
	internal->expectedtime = 0;

#ifdef WIN32
	int prior = 0;
	int priorMin = THREAD_PRIORITY_IDLE;
	int priorMax = THREAD_PRIORITY_TIME_CRITICAL;

	if (internal->policy == policyRealtime)
	{
		prior = THREAD_PRIORITY_TIME_CRITICAL;
	}
	else
	{
		prior = priorMax - (internal->priority - Thread::priorTop) * (priorMax - priorMin) / (Thread::priorBottom - Thread::priorTop);
		if (prior <= THREAD_PRIORITY_IDLE)
		{
			prior = THREAD_PRIORITY_IDLE;
		}
		else if (prior <= THREAD_PRIORITY_LOWEST)
		{
			prior = THREAD_PRIORITY_LOWEST;
		}
		else if (prior <= THREAD_PRIORITY_BELOW_NORMAL)
		{
			prior = THREAD_PRIORITY_BELOW_NORMAL;
		}
		else if (prior <= THREAD_PRIORITY_NORMAL)
		{
			prior = THREAD_PRIORITY_NORMAL;
		}
		else if (prior >= THREAD_PRIORITY_TIME_CRITICAL)
		{
			prior = THREAD_PRIORITY_TIME_CRITICAL;
		}
		else if (prior >= THREAD_PRIORITY_HIGHEST)
		{
			prior = THREAD_PRIORITY_HIGHEST;
		}
		else if (prior >= THREAD_PRIORITY_ABOVE_NORMAL)
		{
			prior = THREAD_PRIORITY_ABOVE_NORMAL;
		}
		else if (prior >= THREAD_PRIORITY_NORMAL)
		{
			prior = THREAD_PRIORITY_NORMAL;
		}
	}

	internal->handle = (HANDLE)_beginthread(ThreadInternal::threadEnryPointer, internal->stacksize, (void *)this);
	BASE_Assert((internal->handle != (HANDLE)0 && internal->handle != (HANDLE)-1));
	if (internal->handle == NULL || internal->handle == (HANDLE)-1)
	{
		internal->loop = false;
		internal->handle = 0;
	}
	else
	{
		BOOL ret = SetThreadPriority(internal->handle, prior);
		BASE_Assert(ret);
	}

	bool succ = (internal->handle != NULL);

#elif defined(__linux__) || defined(__APPLE__)
	if (internal->stacksize < TSK_DEF_STACK_SIZE)
	{
		internal->stacksize = TSK_DEF_STACK_SIZE;
	}

	pthread_attr_t attr;
	int ret = pthread_attr_init(&attr);
	BASE_Assert(ret == 0);

	if (internal->policy == policyRealtime)
	{
		ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
		BASE_Assert(ret == 0);

		// ����linuxʵʱ�߳����ȼ�
		struct sched_param param;
		int priorMin = sched_get_priority_min(SCHED_FIFO);
		int priorMax = sched_get_priority_max(SCHED_FIFO);

		param.sched_priority = priorMax -
							   (internal->priority - Thread::priorTop) * (priorMax - priorMin) / (Thread::priorBottom - Thread::priorTop);

		//infof("fifo sched priority = %d\n", param.sched_priority);
		ret = pthread_attr_setschedparam(&attr, &param);
		BASE_Assert(ret == 0);
	}
	else
	{
		if (internal->policy != policyNormal)
		{
			//	warnf("Thread::CreateThread policy isn't set properly, policy = %d", internal->policy);
		}

		ret = pthread_attr_setschedpolicy(&attr, SCHED_RR);
		BASE_Assert(ret == 0);
	}
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	//	pthread_attr_setstacksize(&attr,internal->stacksize);
	ret = pthread_create(&internal->handle, &attr,
						 (void *(*)(void *))ThreadInternal::threadEnryPointer, (void *)this);
	BASE_Assert(ret == 0);

	pthread_detach(internal->handle);
	pthread_attr_destroy(&attr);
	//assert(ret == 0);

	if (ret)
	{
		internal->loop = false;
	}

	bool succ = (ret == 0);

#endif

	if (internal->createSem.pend(10000) < 0) ///5���̶߳���û����
	{
		//�����߳���ɱ�Լ�
#ifdef WIN32
		TerminateThread(internal->handle, 0);
		CloseHandle(internal->handle);
		internal->handle = NULL;
#elif defined(__linux__)
		void *status;
		::pthread_cancel(internal->handle);
		::pthread_join(internal->handle, &status);
		internal->handle = 0;
#endif
		internal->running = false;
		return false;
	}

	if (internal->name != DEFAULTTHREADNAME)
	{
		//	ThreadManagerInfo::instalce()->addThread(this,internal->name,internal->id);
	}

	return succ;
}

bool Thread::destroyThread()
{
	if (internal->name != DEFAULTTHREADNAME)
	{
		//	ThreadManagerInfo::instalce()->delThread(this);
	}

	{
		Guard locker(internal->mutex);
		if (!internal->running)
		{
			return false;
		}
		internal->loop = false;
	}

	while (internal->checkThreadIsExist())
	{
		internal->quitSem.pend(100); //һֱ���߳�ֱ������
	}
	internal->running = false;
	internal->handle = 0;

	return true;
}

bool Thread::terminateThread()
{
	if (internal->name != DEFAULTTHREADNAME)
	{
		//	ThreadManagerInfo::instalce()->delThread(this);
	}

	internal->loop = false;

	Guard guard(internal->mutex);
	if (!internal->running)
	{
		return true;
	}

	//threadErrorManager.cleanThreadErrorInfo(internal->id);

	bool isTerminateMySelft = (internal->id == Thread::getCurrentThreadID());

	internal->id = 0;
	internal->running = false;
	if (isTerminateMySelft)
	{
		//�Լ��߳�ɱ�Լ�
#ifdef WIN32
#elif defined(__linux__)
		::pthread_exit(0);
#endif
	}
	else
	{
		//�����߳���ɱ�Լ�
#ifdef WIN32
		::TerminateThread(internal->handle, 0);
		CloseHandle(internal->handle);
#elif defined(__linux__) || defined(__APPLE__)
		void *status = NULL;
		::pthread_cancel(internal->handle);
		::pthread_join(internal->handle, &status);
#endif
		internal->handle = 0;
	}

	return true;
}

/// ȡ���̣߳������߳��˳���־����������ʽ�����ȴ��߳̽���
bool Thread::cancelThread()
{
	if (internal->name != DEFAULTTHREADNAME)
	{
		//	ThreadManagerInfo::instalce()->delThread(this);
	}

	Guard guard(internal->mutex);
	if (!internal->running)
	{
		return true;
	}

	internal->loop = false;

	return true;
}

bool Thread::isThreadOver()
{
	return !internal->running;
}

int Thread::getThreadID()
{
	return internal->id;
}
thread_handle_t Thread::handle() const
{
	return internal->handle;
}
void Thread::setTimeout(int milliSeconds)
{
	Guard guard(internal->mutex);

	if (milliSeconds == 0) // ���Ԥ��ʱ��
	{
		internal->expectedtime = 0;
	}
	else
	{
		internal->expectedtime = Time::getCurrentMilliSecond() + (int)milliSeconds;
	}
}

bool Thread::isTimeout()
{
	Guard guard(internal->mutex);

	return (internal->expectedtime != 0 && internal->expectedtime < Time::getCurrentMilliSecond());
}

int Thread::getCurrentThreadID()
{
#ifdef WIN32
	return (int)GetCurrentThreadId();
#elif defined(__linux__) || defined(__APPLE__)
	//	return (int)syscall(SYS_gettid);
	//����mac�� ��error: cast from pointer to smaller type 'int' loses information�� �Ĵ���
	return (int)(long)pthread_self();
#endif
}

bool Thread::looping() const
{
	if (internal->name != DEFAULTTHREADNAME)
	{
		//	ThreadManagerInfo::instalce()->threadPool(this);
	}

	return internal->loop;
}

void Thread::sleep(int ms)
{
#ifdef WIN32
	Sleep(ms);
#elif defined(__linux__) || defined(__APPLE__)
	if (ms > 0)
	{
		usleep(ms * 1000);
	}
	else
	{
		sched_yield();
	}
#else
#error "Unknown Platform"
#endif
}

class ThreadProcThread : public Thread
{
public:
	ThreadProcThread(const std::string &name, const ThreadEx::Proc &_proc, void *_param, int priority, int policy, int stackSize)
		: Thread(name, priority, policy, stackSize), proc(_proc), param(_param)
	{
	}
	void threadProc()
	{
		proc(this, param);
	}

private:
	ThreadEx::Proc proc;
	void *param;
};

shared_ptr<Thread> ThreadEx::creatThreadEx(const std::string &name, const ThreadEx::Proc &proc, void *param, int priority, int policy, int stackSize)
{
	ThreadProcThread *thread = new ThreadProcThread(name, proc, param, priority, policy, stackSize);
	return shared_ptr<Thread>(thread);
}
//
//ThreadErrorManager::ThreadErrorManager(){}
//
//ThreadErrorManager::~ThreadErrorManager(){}
//
//bool ThreadErrorManager::cleanThreadErrorInfo(uint32_t threadId)
//{
//	Guard locker(mutex);
//
//	threadErrorMap.erase(threadId);
//
//	return true;
//}
//
//bool ThreadErrorManager::setThreadErrorInfo(uint32_t threadId,const XM_ErrorInfo& info)
//{
//	Guard locker(mutex);
//
//	threadErrorMap[threadId] = info;
//
//	return true;
//}
//
//bool ThreadErrorManager::getThreadErrorInfo(uint32_t threadId,XM_ErrorInfo& info)
//{
//	Guard locker(mutex);
//
//	std::map<uint32_t, XM_ErrorInfo>::iterator iter = threadErrorMap.find(threadId);
//	if(iter == threadErrorMap.end())
//	{
//		return false;
//	}
//
//	info = iter->second;
//
//	return true;
//}
//
//bool XM_SetLastErrorInfo(int errCode, const char *fmt, ...)
//{
//	char info[ErrorInfo_MaxLength] = {0};
//	va_list arg;
//	va_start(arg,fmt);
//	vsprintf(info,fmt,arg);
//	va_end(arg);
//
//	return XM_SetLastError(errCode,info);
//}
///// ���߳��������Ĵ�����
///// \param errCode [in] ������
///// \param info [in] �������������Ϣ
///// \retval true �ɹ�
///// \retval false ʧ��
//bool XM_SetLastError(int errCode, const char *info)
//{
//	XM_ErrorInfo errinfo;
//
//	errinfo.errorCode = errCode;
//	errinfo.info = info;
//
//	return XM_SetLastErrorEx(errinfo);
//}
//
//
///// ���߳��������Ĵ�����
///// \param errCode [in] ������
///// \param info [in] �������������Ϣ
///// \retval true �ɹ�
///// \retval false ʧ��
//bool XM_SetLastErrorEx(const XM_ErrorInfo &lastinfo)
//{
//	return threadErrorManager.setThreadErrorInfo(Thread::getCurrentThreadID(),lastinfo);
//}
//
///// ������Ĵ�����
///// \param errinfo [out] ������
///// \retval true �ɹ�
///// \retval false ʧ��
//bool XM_GetLastError(XM_ErrorInfo &errinfo)
//{
//	errinfo.errorCode = 0;
//	errinfo.info.clear();
//	errinfo.stacks.clear();
//
//	return threadErrorManager.getThreadErrorInfo(Thread::getCurrentThreadID(),errinfo);
//}
//
///// ������Ĵ�����
///// \retval true �ɹ�
///// \retval false ʧ��
//bool XM_ClearLastError()
//{
//	return threadErrorManager.cleanThreadErrorInfo(Thread::getCurrentThreadID());
//}
//
///// ��������Ϣ����Ӹ��ӵĵ���ջ��Ϣ
///// \param detail [in] ������Ϣ
///// \retval true �ɹ�
///// \retval false ʧ��
//bool XM_AddLastErrorStack(const char *detail)
//{
//	if(detail == NULL)
//	{
//		return false;
//	}
//
//	XM_ErrorInfo info;
//	if(!XM_GetLastError(info))
//	{
//		return false;
//	}
//
//	info.stacks = detail;
//
//	return XM_SetLastErrorEx(info);
//}

} // namespace Base
} // namespace Public
