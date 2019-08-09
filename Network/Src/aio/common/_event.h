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

struct OtherDeleteSocket
{
	shared_ptr<Socket> sock;

	static void DelteSocketCallback(void* param)
	{
		OtherDeleteSocket* delobj = (OtherDeleteSocket*)param;
		if (delobj) SAFE_DELETE(delobj);
	}
};

class Event
{
public:
	Event(EventType _pooltype):pooltype(_pooltype){}
	virtual ~Event() {}

	//�����¼���int Ϊ�¼���ǰ��ֵ��statusΪ״̬
	void doEvent1(int bytes, bool status)
	{
		if (userthread == NULL || !userthread->callbackThreadUsedStart()) return;

		shared_ptr<Socket> sockptr = sock.lock();
		if (sockptr == NULL)
		{
			return;
		}

		doEvent(sockptr, bytes, status);

		userthread->callbackThreadUsedEnd();

		//z�����鷳�ˣ��¼��ص�����Ҫɾ���Լ�����ɾ���Լ�����Ҫ�ȴ��¼��ص�����������
		//1����ioworker�̳߳���ȥɾ��
		//2��������ɾ��
		if (sockptr.use_count() == 1)
		{
			OtherDeleteSocket* delobj = new OtherDeleteSocket;
			delobj->sock = sockptr;

			if (!sockptr->ioWorker()->postEvent(OtherDeleteSocket::DelteSocketCallback, delobj))
			{
				OTHER_DELETE(delobj);
			}
		}
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