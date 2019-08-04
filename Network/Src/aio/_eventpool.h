#pragma  once
#ifdef WIN32

#include <winsock2.h>
typedef int socklen_t;

#endif

#include "Base/Base.h"
#include "Network/Socket.h"
#include "_userthread.h"

using namespace Public::Base;
using namespace Public::Network;




//������ʱ��أ�����¼�

struct NewSocketInfo
{
	int		newsock;
	NetAddr otheraddr;
};

class Event
{
public:
#ifdef WIN32
	OVERLAPPED		overlped;
	WSABUF			wbuf;
	SOCKADDR		addr;
	int 			addrlen;
	DWORD			dwBytes;
	DWORD			dwFlags;
	int				newsock;
#endif
public:
	Event()
	{
#ifdef WIN32
		memset(&overlped, 0, sizeof(overlped));
		memset(&wbuf, 0, sizeof(wbuf));
		memset(&addr, 0, sizeof(addr));
		dwBytes = dwFlags = 0;
		newsock = 0;

		addr.sa_family = AF_INET;
		addrlen = sizeof(SOCKADDR);
#endif
	}
	virtual ~Event() {}

	virtual bool init(const shared_ptr<Socket>& _sock, const shared_ptr<UserThread>& _userthread)
	{
		weak_sock = _sock;
		userthread = _userthread;

		return true;
	}

	//�����¼���int Ϊ�¼���ǰ��ֵ��statusΪ״̬
	void doEvent(int bytes, bool status)
	{
		shared_ptr<Socket> sock = weak_sock.lock();
		if (sock == NULL || !userthread->callbackThreadUsedStart())
		{
			return;
		}

		doEvent(sock, bytes, status);

		userthread->callbackThreadUsedEnd();
	}

	//�ж��¼��Ƿ���Ч
	bool isValid() { return weak_sock.lock() != NULL; }
private:
	virtual void doEvent(const shared_ptr<Socket>& sock, int bytes, bool status) = 0;
private:
	weak_ptr<Socket> weak_sock;
	shared_ptr<UserThread> userthread;
};

//�¼��أ���ŵ�ǰ����ʹ�õ��¼�����ʱ�ж��¼��Ƿ����
class EventPool
{
public:
	EventPool() 
	{
		checkTimer = make_shared<Timer>("EventPool");
		checkTimer->start(Timer::Proc(&EventPool::doCheckTimerProc, this), 0, 10000);
	}
	~EventPool() 
	{
		checkTimer = NULL;
	}
	//�����¼�
	void pushEvent(void* eventid, const shared_ptr<Event>& event)
	{
		Guard locker(mutex);

		eventmap[eventid] = event;
	}
	//ȡ���¼�
	shared_ptr<Event> popEvent(void* eventid)
	{
		Guard locker(mutex);

		std::map<void*, shared_ptr<Event> >::iterator iter = eventmap.find(eventid);
		if (iter == eventmap.end()) return shared_ptr<Event>();

		return iter->second;
	}
private:
	void doCheckTimerProc(unsigned long)
	{
		std::map<void*, shared_ptr<Event> > eventmaptmp;
		{
			Guard locker(mutex);
			eventmaptmp = eventmap;
		}

		//��Ч���¼��б�
		std::set<void*>	invalidList;

		//����¼��Ƿ���Ч
		for (std::map<void*, shared_ptr<Event> >::iterator iter = eventmaptmp.begin(); iter != eventmaptmp.end(); iter++)
		{
			if (!iter->second->isValid())
			{
				invalidList.insert(iter->first);
			}
		}

		//ɾ����Ч���¼�
		{
			Guard locker(mutex);
			for (std::set<void*> ::iterator iter = invalidList.begin(); iter != invalidList.end(); iter++)
			{
				eventmap.erase(*iter);
			}
		}
	}
private:
	Mutex								mutex;
	std::map<void*, shared_ptr<Event> > eventmap;

	shared_ptr<Timer>					checkTimer;
};