//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//
//	Description:
//	$Id: AtomicCount.h 80 2013-04-11 07:05:56Z  $
//

#ifndef __BASE_ATOMIC_COUNT_H__
#define __BASE_ATOMIC_COUNT_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#include <Windows.h>
#endif
#include "Base/Defs.h"

namespace Public {
namespace Base {
	
/// \class AtomicCount
/// \brief ԭ�Ӽ�����


class BASE_API AtomicCount
{
		/// ��ֹ�������캯���͸�ֵ����
    AtomicCount( AtomicCount const & );
    AtomicCount & operator=( AtomicCount const & );
public:
	typedef int	ValueType;
public:
	AtomicCount();
	/// ԭ�Ӽ��㹹�캯��
	/// \param v [in] ��ʼ��ʱԭ�Ӽ�����ֵ
	explicit AtomicCount( ValueType initValue);
	~AtomicCount();

	/// ǰֵ������������
    ValueType operator++();

	/// ��ֵ������������
	ValueType operator++(int);

	/// ǰֵ�ݼ�ȥ��������
    ValueType operator--();

	/// ��ֵ�ݼ�ȥ��������
	ValueType operator--(int);

	ValueType value() const;

	/// ���غ������ò���
    operator ValueType() const;

	///�ж�ԭ�Ӽ�����0
	bool operator !() const;
private:
#ifdef WIN32
	typedef volatile LONG  countType;
#else
	typedef int countType;
#endif
	countType		count;
};

/// \class AtomicCount
/// \brief ԭ�Ӽ�����

inline AtomicCount::AtomicCount() :count(0) {}

inline AtomicCount::AtomicCount(ValueType v) : count(v) {}

inline AtomicCount::~AtomicCount() {}

inline AtomicCount::ValueType AtomicCount::value() const
{
	return count;
}

inline AtomicCount::operator AtomicCount::ValueType() const
{
	return count;
}

inline bool AtomicCount::operator !() const
{
	return count == 0;
}

#ifdef WIN32
inline AtomicCount::ValueType AtomicCount::operator ++()
{
	return InterlockedIncrement(&count);
}
inline AtomicCount::ValueType AtomicCount::operator ++(int)
{
	AtomicCount::ValueType result = InterlockedIncrement(&count);

	return --result;
}
inline AtomicCount::ValueType AtomicCount::operator --()
{
	return InterlockedDecrement(&count);
}
inline AtomicCount::ValueType AtomicCount::operator --(int)
{
	AtomicCount::ValueType result = InterlockedDecrement(&count);

	return ++result;
}
#else
inline AtomicCount::ValueType AtomicCount::operator ++()
{
	return __sync_add_and_fetch(&count, 1);
}
inline AtomicCount::ValueType AtomicCount::operator ++(int)
{
	return __sync_fetch_and_add(&count, 1);
}
inline AtomicCount::ValueType AtomicCount::operator --()
{
	return __sync_sub_and_fetch(&count, 1);
}
inline AtomicCount::ValueType AtomicCount::operator --(int)
{
	return __sync_fetch_and_sub(&count, 1);
}
#endif

} // namespace Base
} // namespace Public

#endif // __BASE_ATOMIC_COUNT_H__


