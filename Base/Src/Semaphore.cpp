//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: Semaphore.cpp 33 2013-02-05 09:43:35Z  $
//
#if defined(__linux__)
#include <semaphore.h>
#include <time.h>
#endif

#include <assert.h>
#include "Base/Semaphore.h"
#include "Base/PrintLog.h"
#ifdef WIN32
	#include <Windows.h>
	#define INCREMENT_AMOUNT	1
	#define LONG_MAX      2147483647L   /* maximum (signed) long value */
#elif defined(__linux__)
	#include <semaphore.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <errno.h>
#else
	#error "Unknown Platform"
#endif


namespace Public{
namespace Base {


struct Semaphore::SemaphoreInternal
{
#ifdef WIN32
	void* handle;
#elif defined(__linux__)
	sem_t *	sem;
#endif
};


Semaphore::Semaphore(int initialCount)
{
	internal = new SemaphoreInternal;

#ifdef WIN32
	internal->handle = CreateSemaphore(NULL, initialCount, LONG_MAX, NULL);
	BASE_Assert(internal->handle);

#elif defined(__linux__)
	internal->sem = (sem_t *)malloc(sizeof(sem_t));
	BASE_Assert(internal->sem);
	int ret=sem_init(internal->sem, 0, initialCount);
	BASE_Assert(ret != -1);
	(void)ret;	// ��������δʹ�ñ��뾯��
#endif
}

Semaphore::~Semaphore()
{
#ifdef WIN32
	CloseHandle(internal->handle);

#elif defined(__linux__)
	BASE_Assert(internal->sem);
	int ret = sem_destroy(internal->sem);
	BASE_Assert(ret ==0);
	(void)ret;	// ��������δʹ�ñ��뾯��
	free(internal->sem);

#endif
	delete internal;
}

int Semaphore::pend()
{
#ifdef WIN32
	BASE_Assert(internal->handle);
	return WaitForSingleObject(internal->handle, INFINITE);

#elif defined(__linux__)
	BASE_Assert(internal->sem);
	int ret = 0;
	while((ret = sem_wait(internal->sem)) != 0 && errno == EINTR);

	if(ret != 0)
	{
		logerror("Semaphore::pend() errno %d\r\n",errno);

		perror("sem_wait");
	}
	
	return ret;

#endif
}


/// �����ź�������������Ǵ�0�ۼӣ��ỽ����ȴ����еĵ�һ���߳�
/// \timeout [in] ��ʱʱ��,��λ����
/// \return >=0 ��ǰ�ź�������
///         <0  ��ʱ

int Semaphore::pend(uint32_t timeout)
{

#ifdef WIN32
	BASE_Assert(internal->handle);
	DWORD ret = WaitForSingleObject(internal->handle, timeout);
	if (ret == WAIT_OBJECT_0)
		return 0;
	else 
		return -1;
#elif defined(__linux__)
	BASE_Assert(internal->sem);
	int ret = 0;
	struct timespec timeSpec;
	clock_gettime(CLOCK_REALTIME,&timeSpec);
	timeSpec.tv_sec += (timeout < 1000)?1:(timeout/1000);
	while((ret = sem_timedwait(internal->sem, &timeSpec)) != 0 && errno == EINTR);
	
	return ret;
#endif	
}
	
int Semaphore::post()
{
#ifdef WIN32
	LONG cnt = 0;// ����Ĳ�����������ø��ź�����ǰ�ı�ʹ�õ�����
	return ReleaseSemaphore(internal->handle, INCREMENT_AMOUNT, &cnt);

#elif defined(__linux__)
	assert(internal->sem);
	return sem_post(internal->sem);

#endif
}

} // namespace Base
} // namespace Public
