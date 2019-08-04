//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: MemPool.h 255 2014-01-23 03:28:32Z  $
//
#ifndef __DYNAMIC_MEMORYPOOL_H__
#define __DYNAMIC_MEMORYPOOL_H__
#include "Base/Defs.h"
#include "Base/IntTypes.h"
namespace Public{
namespace Base{

///�ڴ���趨������Publicmedia��Ҳ���������ط�ʹ��
class BASE_API DynamicMemPool :public IMempoolInterface
{
	class MemPoolInternal;
public:
	DynamicMemPool();
	~DynamicMemPool();

	/// �ڴ���䡢ʧ�ܷ���NULL
	void* Malloc(uint32_t size,uint32_t& realsize);
	
	/// �ڴ��ͷ�
	bool Free(void*);

	uint32_t usedBufferSize();

	/// �ڴ�ʹ�ô�ӡ
//	void dump();

	///�����ڴ���ܴ�С��Ĭ��Ϊ128M
//	static void config(uint32_t allSize);
private:
	MemPoolInternal*	internal;
};


} // namespace Base
} // namespace Public


#endif //__DYNAMIC_MEMORYPOOL_H__
