#ifndef __STATICXMMDBUFFER_H__
#define __STATICXMMDBUFFER_H__
#include "Base/Mutex.h"
#include "Base/Semaphore.h"
#include "Base/DynamicMemPool.h"
#include "Base/StaticMemPool.h"
#include "Base/ShareMem.h"
#include "Base/Defs.h"
#include "Base/Func.h"
#include <string>
using namespace  std;
namespace Public{
namespace Base{

///���̼�����������
class BASE_API NamedMutex:public IMutexInterface
{
	struct NamedMutexInternal;
	NamedMutex(const NamedMutex&);
	NamedMutex& operator=(const NamedMutex&);
public:
	NamedMutex(const std::string& shareName);
	~NamedMutex();
	
	bool tryEnter();
	bool enter();
	bool leave();
private:
	NamedMutexInternal* internal;
};

///���̼������ź���
class BASE_API NamedSemaphore:public ISemaphoreInterface
{
	NamedSemaphore(const NamedSemaphore&);
	NamedSemaphore& operator=(const NamedSemaphore&);
	struct NamedSemaphoreInternal;
public:
	NamedSemaphore(const std::string& shareName);
	~NamedSemaphore();

	int pend();

	int pend(uint32_t timeout);

	int post();
private:
	NamedSemaphoreInternal* internal;
};

///���̼乲���ڴ�
class BASE_API ShareMEMBuffer
{
	struct ShareMEMBufferInternal;
public:
	typedef Function2<void,void*,int>		ReadMEMCallback;
private:
	ShareMEMBuffer(const std::string& shareName, int createBlock, int maxBlock, int rblockSize, int bocknum, int memMaxSize, bool create, void* startAddr, const ReadMEMCallback& callback);
public:
	~ShareMEMBuffer();
	
	static shared_ptr<ShareMEMBuffer> create(const std::string& shareName,int writeBlockSize,int wrteBlockNum,int readBlockSize,int readBlockNum,int memMaxSize,const ReadMEMCallback& callback = NULL);
	static shared_ptr<ShareMEMBuffer> open(const std::string& shareName,int readBlockSize,int readBlockNum,int writeBlockSize,int writeBlockNum,int memMaxSize,void* startAddr,const ReadMEMCallback& callback);
	
	int write(void* block,int size);

	void* startAddr();
private:
	ShareMEMBufferInternal* internal;
};


///���̼乲���ڴ�
class BASE_API ShareMEMPool
{
	struct ShareMEMPoolInternal;
private:
	ShareMEMPool(const std::string& shareName, uint32_t mempoolsize, bool create);
public:
	~ShareMEMPool();

	static shared_ptr<ShareMEMPool> create(const std::string& shareName, uint32_t mempoolsize);
	static shared_ptr<ShareMEMPool> open(const std::string& shareName, uint32_t mempoolsize);

	IMempoolInterface* getMempool();
private:
	ShareMEMPoolInternal* internal;
};


};
};

#endif //__STATICXMMDBUFFER_H__
