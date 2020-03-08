//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: Base64.h 3 2013-01-21 06:57:38Z  $

#ifndef __BASE_BASE64_H__
#define __BASE_BASE64_H__

#include "Defs.h"
#include "Base/IntTypes.h"

namespace Public {
namespace Base {

/// \class Base64
/// \brief Base64�㷨��صķ�����
class BASE_API Base64 	
{
public:
	/// Base64 ����
	/// \param [in] src ����ǰ�����ݻ���
	/// \reval ���ر������ı��ֽ�
	static std::string encode(const std::string& src);

	/// Base64 ����
	/// \param [in] src ���������ݻ���
	/// \reval ���ؽ����������ֽ�
	static std::string decode(const std::string& src);
};

class BASE_API Base32
{
public:
	/// Base32 ����
	/// \param [in] src ����ǰ�����ݻ���
	/// \reval ���ر������ı��ֽ�
	static std::string encode(const std::string& src);

	/// Base32 ����
	/// \param [in] src ���������ݻ���
	/// \reval ���ؽ����������ֽ�
	static std::string decode(const std::string& src);
};

class BASE_API Base16
{
public:
	/// Base16 ����
	/// \param [in] src ����ǰ�����ݻ���
	/// \reval ���ر������ı��ֽ�
	static std::string encode(const std::string& src);

	/// Base16 ����
	/// \param [in] src ���������ݻ���
	/// \reval ���ؽ����������ֽ�
	static std::string decode(const std::string& src);
};


} // namespace Base
} // namespace Public

#endif// __BASE_BASE64_H__

