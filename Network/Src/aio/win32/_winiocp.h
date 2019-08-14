#pragma  once

#ifdef WIN32

#include "_winevent.h"
#include "../common/_pool.h"

//这里是处理事件的线程池

struct _WinIOCP:public _PoolResource
{
public:
	_WinIOCP(SOCKET socketfd, const shared_ptr<Socket>& sock, const shared_ptr<_UserThread>& userthread, const shared_ptr<_Pool>& pool, HANDLE iocp)
		:_PoolResource(socketfd,sock,userthread,pool)
	{
		::CreateIoCompletionPort((HANDLE)socketfd, iocp, NULL, 0);
	}
	virtual bool postEvent(Event* event)
	{
		if (!_PoolResource::postEvent(event))
		{
			return false;
		}

		return true;
	}
	~_WinIOCP()
	{
		::PostQueuedCompletionStatus((HANDLE)socket(), 0, NULL, NULL);
		::PostQueuedCompletionStatus((HANDLE)socket(), 0, NULL, NULL);
	}
};

class _SystemPoll :public _Pool,public enable_shared_from_this<_SystemPoll>
{
	struct RunParam
	{
		DWORD bytes;
		BOOL ret ;
		WinEvent* winevent;
	};
public:
	_SystemPoll(uint32_t maxthreadnum):_Pool(1)
	{
		iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (iocp == NULL)
		{
			return;
		}

		int cpunum = Host::getProcessorNum();

		for (int i = 0; i < maxthreadnum; i++)
//		for (int i = 0; i < 1; i++)
		{
			shared_ptr<Thread> t = ThreadEx::creatThreadEx("_SystemPoll",ThreadEx::Proc(&_SystemPoll::threadRunProc, this), NULL, Thread::priorTop, Thread::policyRealtime);
			t->createThread();

			int mask = 0x01 << i;
	//		SetThreadAffinityMask((HANDLE)t->handle(), 0x01 << i);

			threadlist.push_back(t);
		}
	}

	~_SystemPoll()
	{
		for (std::list<shared_ptr<Thread> >::iterator iter = threadlist.begin(); iter != threadlist.end(); iter++)
		{
			(*iter)->cancelThread();
		}

		for (std::list<shared_ptr<Thread> >::iterator iter = threadlist.begin(); iter != threadlist.end(); iter++)
		{
			::PostQueuedCompletionStatus(iocp, 0, NULL, NULL);
		}

		threadlist.clear();

		if (iocp)
			CloseHandle(iocp);
		iocp = NULL;
	}

	virtual shared_ptr<_PoolResource> addResource(SOCKET socketfd, const shared_ptr<Socket>& _sock, const shared_ptr<_UserThread>& _userthread)
	{
		//_WinIOCP(int socketfd, const shared_ptr<Socket>& sock, const shared_ptr<_UserThread>& userthread, const shared_ptr<_Pool>& pool, PTP_WIN32_IO_CALLBACK callback,void* pv, TP_CALLBACK_ENVIRON* tp_env)
		shared_ptr< _WinIOCP> iocpres = shared_ptr<_WinIOCP>(new _WinIOCP(socketfd, _sock,_userthread,shared_from_this(), iocp));
		return iocpres;
	}

	void threadRunProc(Thread* t, void* param)
	{
		if (iocp == NULL) return;

		ThreadPool::Proc tpfun(&_SystemPoll::threadPoolRunProc, this);

		while (t->looping())
		{
			DWORD bytes = 0;
			ULONG_PTR key = 0;
			OVERLAPPED* poverlaped = NULL;

			BOOL ret = ::GetQueuedCompletionStatus(iocp, &bytes, &key, &poverlaped, INFINITE);

			WinEvent* winevent = CONTAINING_RECORD(poverlaped, WinEvent, overlped);

			if (winevent == NULL || poverlaped == NULL) return;

			//RunParam* runparam = new RunParam;
			//runparam->bytes = bytes;
			//runparam->winevent = winevent;
			//runparam->ret = ret;

			//dispatch(tpfun, runparam);

			winevent->doEvent1((DWORD)bytes, ret);
		}
	}
	void threadPoolRunProc(void* param)
	{
		RunParam* runparam = (RunParam*)param;

		if (runparam == NULL) return;

		runparam->winevent->doEvent1(runparam->bytes, runparam->ret);

		SAFE_DELETE(runparam);
	}
private:
	HANDLE	iocp;

	std::list<shared_ptr<Thread> > threadlist;
};

#endif