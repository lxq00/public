#ifndef __ASIOSOCKET_OBJCET_USERTHREADINFO_DEFINE_H__
#define __ASIOSOCKET_OBJCET_USERTHREADINFO_DEFINE_H__
#include "ASIOServerPool.h"
#include "Network/Socket.h"
namespace Public {
namespace Network {

class UserThreadInfo
{
public:
	UserThreadInfo():mustQuit(false),usedCallbackThreadId(0),usedCallbackThreadNum(0){}
	~UserThreadInfo(){}
	void quit() 
	{
		Guard locker(mutex);
		mustQuit = true;
	}
	void waitAllOtherCallbackThreadUsedEnd()////�ȴ����лص��߳�ʹ�ý���
	{
		uint32_t currThreadId = Thread::getCurrentThreadID();
		while(1)
		{
			{
				Guard locker(mutex);
				if(usedCallbackThreadNum == 0 || (usedCallbackThreadNum == 1 && usedCallbackThreadId == currThreadId))
				{
					///���û���߳�ʹ�ã��������Լ��̹߳ر��Լ������Թر�
					break;
				}
			}
			Thread::sleep(10);
		}
	}
	bool callbackThreadUsedStart()///��¼��ǰ�ص��߳�ʹ����Ϣ
	{
		Guard locker(mutex);
		if (mustQuit) return false;

		uint32_t currThreadId = Thread::getCurrentThreadID();
		usedCallbackThreadId += currThreadId;
		++usedCallbackThreadNum;

		return true;
	}
	bool callbackThreadUsedEnd() ///��ǰ�ص��߳�ʹ�ý���
	{
		Guard locker(mutex);
		
		uint32_t currThreadId = Thread::getCurrentThreadID();
		usedCallbackThreadId -= currThreadId;
		--usedCallbackThreadNum;

		return true;
	}
protected:
	Mutex											mutex;
	bool											mustQuit;				///�ǲ����Ѿ����ر�
	uint64_t										usedCallbackThreadId;	//����ʹ�ûص����߳�ID��
	uint32_t										usedCallbackThreadNum;	//����ʹ�ûص����߳���
};

}
}


#endif //__ASIOSOCKET_OBJCET_H__
