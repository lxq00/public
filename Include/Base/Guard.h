//
//  Copyright (c)1998-2014, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: Guard.h 3 2013-01-21 06:57:38Z  $
//


#ifndef __BASE_GUARD_H__
#define __BASE_GUARD_H__

#include "Mutex.h"
#include "RecursiveMutex.h"
#include "ReadWriteMutex.h"
#include "Shared_ptr.h"
namespace Public{
namespace Base{


/// \class Guard
/// \brief �ػ�����.

class Guard
{
	Guard(Guard const&);
	Guard& operator=(Guard const&);

public:
	/// ���캯��,�Զ�����CMutex::enter
	/// \param mutex ����������,CGuard��������������.
	inline Guard(Mutex& mutexparam)
		:mutex(&mutexparam)
	{
		if(mutex != NULL)
		{
			mutex->enter();
		}		
	};
	inline Guard(const shared_ptr<IMutexInterface>& ptr)
		:mutex(ptr.get())
	{
		if (mutex != NULL)
		{
			mutex->enter();
		}
	};
	inline Guard(IMutexInterface* ptr)
		:mutex(ptr)
	{
		if(mutex != NULL)
		{
			mutex->enter();
		}		
	};

	/// ��������,�Զ�����CMutex::leave
	inline ~Guard()
	{
		if(mutex != NULL)
		{
			mutex->leave();
		}
	};

private:
	IMutexInterface *mutex;	///< ��Ҫ�Զ����õĻ���������
};

////////////////////////////////////////////////////////////////////////////////
/// \class RecursiveGuard
/// \brief �ݹ��ػ�����
class RecursiveGuard
{
	RecursiveGuard(RecursiveGuard const&);
	RecursiveGuard& operator=(RecursiveGuard const&);

public:
	/// ���캯��,�Զ�����CMutex::enter
	/// \param mutex [in] ����������,CGuard��������������.
	inline RecursiveGuard(RecursiveMutex& mutexparam)
		:mutex(mutexparam)
	{
		mutex.enter();
	};

	/// ��������,�Զ�����CMutex::leave
	inline ~RecursiveGuard()
	{
		mutex.leave();
	};

private:
	RecursiveMutex &mutex;	///< ��Ҫ�Զ����õĻ���������
};

////////////////////////////////////////////////////////////////////////////////
/// \class GuardReadMutex
/// \brief �����ػ�����
class GuardRead
{
	GuardRead(GuardRead const&);
	GuardRead& operator=(GuardRead const&);

public:
	/// ���캯��,�Զ����� CRwMutex::EnterReading
	/// \param mutex [in]����������,CGuard��������������.
	inline GuardRead(ReadWriteMutex& mutexparam)
		: mutex(mutexparam)
	{
		mutex.enterRead();
	};

	/// ��������,�Զ����� CReadWriteMutex::leave
	inline ~GuardRead()
	{
		mutex.leaveRead();
	};

private:
	ReadWriteMutex& mutex;	///< ��Ҫ�Զ����õĻ���������
};

////////////////////////////////////////////////////////////////////////////////
/// \class GuardReadMutex
/// \brief ��д���ػ�����
class GuardWrite
{
	GuardWrite(GuardWrite const&);
	GuardWrite& operator=(GuardWrite const&);

public:
	/// ���캯��,�Զ����� CReadWriteMutex::EnterWriting
	/// \param mutex [in]����������,CGuard��������������.
	inline GuardWrite(ReadWriteMutex& mutexparam)
		: mutex(mutexparam)
	{
		mutex.enterWrite();
	};

	/// ��������,�Զ����� CReadWriteMutex::leave
	inline ~GuardWrite()
	{
		mutex.leaveWrite();
	};

private:
	ReadWriteMutex& mutex;	///< ��Ҫ�Զ����õĻ���������
};

} // namespace Base
} // namespace Public

#endif //__BASE_GUARD_H__

