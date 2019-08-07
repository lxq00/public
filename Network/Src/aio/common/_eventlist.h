#pragma  once
#include "Base/Base.h"
#include "Network/Network.h"
#include "_userthread.h"

using namespace Public::Base;
using namespace Public::Network;

struct NewSocketInfo
{
	int		newsocket;
	NetAddr	otheraddr;
};


typedef enum {
	EventType_Read,
	EventType_Write,
}EventType;

class Event
{
public:
	Event(EventType _pooltype):pooltype(_pooltype){}
	virtual ~Event() {}

	//�����¼���int Ϊ�¼���ǰ��ֵ��statusΪ״̬
	void doEvent(int bytes, bool status)
	{
		shared_ptr<Socket> sockptr = sock.lock();
		if (sockptr == NULL || !userthread || !userthread->callbackThreadUsedStart())
		{
			return;
		}

		doEvent(sockptr, bytes, status);

		userthread->callbackThreadUsedEnd();
	}

	//�ж��¼��Ƿ���Ч
	virtual bool isValid() { return sock.lock() != NULL; }
	EventType type() const { return pooltype; }

	virtual bool init(const shared_ptr<Socket>& _sock, const shared_ptr<_UserThread>& _userthread)
	{
		sock = _sock;
		userthread = _userthread;
	
		return true;
	}	
private:
	virtual void doEvent(const shared_ptr<Socket>& sock, int bytes, bool status) = 0;

private:
	weak_ptr<Socket>		sock;
	shared_ptr<_UserThread> userthread;
	EventType				pooltype;
};


//�¼��أ���ŵ�ǰ����ʹ�õ��¼�����ʱ�ж��¼��Ƿ����
class _EventList
{
public:
	_EventList()
	{
		checkTimer = make_shared<Timer>("EventPool");
		checkTimer->start(Timer::Proc(&_EventList::doCheckTimerProc, this), 0, 10000);
	}
	~_EventList()
	{
		checkTimer = NULL;
		eventmap.clear();
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
		shared_ptr<Event> event;
		{
			Guard locker(mutex);

			std::map<void*, shared_ptr<Event> >::iterator iter = eventmap.find(eventid);
			if (iter == eventmap.end()) return shared_ptr<Event>();

			event = iter->second;
			eventmap.erase(iter);
		}
		return event;
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