//
//  Copyright (c)1998-2014, Public Technology
//  All Rights Reserved.
//
//
//	Description:
//	$Id: String.h 227 2013-10-30 01:10:30Z  $

#ifndef __BASE_STRING_H__
#define __BASE_STRING_H__

#include <stdio.h>
#include <string.h>
#include "Defs.h"
#include "Shared_ptr.h"
#include "Value.h"
#include "Function.h"

namespace Public
{
namespace Base
{
///	��ȫ���ַ�������
///	\param [out] dest:Ŀ��buffer,���Ŀ��bufferû��'\0'�������������һ���ַ�Ϊ'\0',����0
///	\param [in]	dstBufLen,Ŀ��buffer�ռ��С,�ú������д��dstBufLen-1���ַ���������д���ַ��������'\0'�ַ�
///	\param [in] src:
///	\param [in] srcCopyCount: ����src�ĳ���
///		ִ����ɺ������copy����ô��Destһ��������0����
///	\retval ���صĽ���Ǵ�src copy��dest�е��ַ���Ŀ
//size_t BASE_API strncat(char* dest, size_t dstBufLen, const char* src, const size_t srcCopyCount);

///	��ȫ���ַ���copy����
///	\param [out] dst,Ŀ��buffer
///	\param [in]	dstBufLen,Ŀ��buffer�ռ��С,�ú������д��dstBufLen-1���ַ���������д���ַ��������'\0'�ַ�
///	\param [in]	src,Դbuffer
///	\param [in] srcCopyCount
///	\retval Ҫcopy���ַ�����,��dstBufLen-1�ռ����������£����copy���ַ���ĿΪsrcCopyCount,�����ں������'\0'�ַ�
size_t BASE_API strncpy(char *dst, size_t dstBufLen, const char *src, size_t srcCopyCount);

/////	��ǿ��snprintf����֤'\0'������ʵ��д�볤�ȣ�
/////	����֧�� len += snprintf_x( buf + len, maxlen - len, "xxxx", ... ); ������д��
/////	��ʵ��buffer����ʱ����֤\'0'������maxlen - 1����ԭ��snprintf��VC�᷵��-1�Ҳ���֤'\0'��gcc�᷵�ؼ���buffer�㹻ʱ��д�볤�ȣ�
/////	��������maxlen-1ʱ�޷����ֳ��ȸոպû��ǳ����ˣ����Լ򻯶������������߶�������������
/////	Ҳ��������Ҫ�޳����ұ�֤'\0'ʱ���ַ���������ȡ��strncpy����ע��ԭ��strncpy����֤'\0'��
/////	�� strncpy( dst, src, siz - 1 ); dst[size - 1] = '\0'; �൱�� snprintf( dst, siz, "%s", src );
/////	\param [out] buf �������
/////	\param [in] maxlen �����������ֽ���
/////	\param [in] fmt ��ʽ�ַ���
/////	\retval ����ʵ��д�볤��
//int  BASE_API snprintf_x(char* buf, int maxlen, const char* fmt, ... );

enum CharSetEncoding
{
	CharSetEncoding_Unknown = 0,
	CharSetEncoding_UTF8,
	CharSetEncoding_GBK,
};

template <typename T>
inline void processData(Value &value, ...) { value = Value(typeid(T).name()); }

template <typename T = Value>
inline void processData(Value &value, const Value &val) { value = val; }

// ������ֹ�����Ļ�����
template <typename T>
void processValues(std::vector<Value> &values, const T &arg)
{
	Value value;
	processData<T>(value, arg);

	values.push_back(value);
}

// �ɱ��������ģ��
template <typename T, typename... Ts>
void processValues(std::vector<Value> &values, const T &arg, Ts... args)
{
	Value value;
	processData<T>(value, arg);

	values.push_back(value);

	processValues(values, args...); // �����Ȼ��ݹ�
}

//String�����ڲ���������ָ��
class BASE_API String
{
	struct StringInternal;
	StringInternal *internal;

public:
	//���캯��
	String(const shared_ptr<IMempoolInterface> &mempool = shared_ptr<IMempoolInterface>());
	String(const char *str, const shared_ptr<IMempoolInterface> &mempool = shared_ptr<IMempoolInterface>());
	String(const char *str, size_t len, const shared_ptr<IMempoolInterface> &mempool = shared_ptr<IMempoolInterface>());
	String(const std::string &str, const shared_ptr<IMempoolInterface> &mempool = shared_ptr<IMempoolInterface>());
	String(const String &str);
	~String();

	//���ض����ַ
	const char *c_str() const;

	char *c_str();
	//���ض������ݳ���
	size_t length() const;
	//�����������ݳ���
	void resize(size_t size);
	//�������ݳ��ȣ�ԭʼ���ݻᶪ,length��Ϊ0
	char *alloc(size_t size);
	void clear();
	bool empty() const;

	operator std::string() const;
	bool operator==(const String &tmp) const;
	bool operator!=(const String &tmp) const;

	//����������
	String &operator=(const char *str);
	String &operator=(const std::string &str);
	String &operator=(const String &str);

	String &operator+=(char ch);
	String &operator+=(const char *str);
	String &operator+=(const std::string &str);
	String &operator+=(const String &str);

	//׷������
	String &append(char ch);
	String &append(const char *str, size_t size = 0);
	String &append(const std::string &str);
	String &append(const String &str);

public:
	//�ַ���תСд
	static std::string tolower(const std::string &src);

	//�ַ���ת��д
	static std::string toupper(const std::string &src);

	/// Ansi(Gb2312) ת���ַ���Ϊutf8��ʽ ��չ�ӿ�(linux ֻ֧��ת������������1024bytes)
	/// \param [in] src Դ�ַ���
	/// \retval ��string ת��ʧ��
	///         �ǿ�string ת�����
	static std::string ansi2utf8(const std::string &src);

	/// utf8ת���ַ���Ϊ Ansi(Gb2312)��ʽ  ��չ�ӿ�(linux ֻ֧��ת������������1024bytes)
	/// \param [in] src Դ�ַ���
	/// \retval ��string ת��ʧ��
	///         �ǿ�string ת�����
	static std::string utf82ansi(const std::string &src);

	//split() �������ڰ�һ���ַ����ָ���ַ�������
	static std::vector<std::string> split(const std::string &src, const std::string &howmany);
	static std::vector<std::string> split(const char *src, size_t len, const std::string &howmany);

	//��������,Ĭ������ո�/�س�/tabl
	static void trim(std::string &src);
	static void trim_if(std::string &src, const Function<bool(char)> &iffunc);
	static std::string trim_copy(const std::string &src);
	static std::string trim_copy_if(const std::string &src, const Function<bool(char)> &iffunc);

	// ������,
	static void ltrim(std::string &src);
	static void ltrim_if(std::string &src, const Function<bool(char)> &iffunc);
	static std::string ltrim_copy(const std::string &src);
	static std::string ltrim_copy_if(const std::string &src, const Function<bool(char)> &iffunc);
	// ������,
	static void rtrim(std::string &src);
	static void rtrim_if(std::string &src, const Function<bool(char)> &iffunc);
	static std::string rtrim_copy(const std::string &src);
	static std::string rtrim_copy_if(const std::string &src, const Function<bool(char)> &iffunc);

	//���ַ����д�ͷ��ʼ����ָ���ַ���������λ�ã�δ�ҵ�����-1
	static size_t indexOf(const std::string &src, const std::string &fromindex);
	static size_t indexOf(const char *src, size_t len, const std::string &fromindex);

	//���ַ����д�ͷ��ʼ����ָ���ַ���������λ�ã�δ�ҵ�����-1 ���Դ�Сд
	static size_t indexOfByCase(const std::string &src, const std::string &fromindex);
	static size_t indexOfByCase(const char *src, size_t len, const std::string &fromindex);

	//���ַ����д�β��ʼ����ָ���ַ���������λ�ã�δ�ҵ�����-1
	static size_t lastIndexOf(const std::string &src, const std::string &fromindex);
	static size_t lastIndexOf(const char *src, size_t len, const std::string &fromindex);

	//���ַ����д�β��ʼ����ָ���ַ���������λ�ã�δ�ҵ�����-1 ���Դ�Сд
	static size_t lastIndexOfByCase(const std::string &src, const std::string &fromindex);
	static size_t lastIndexOfByCase(const char *src, size_t len, const std::string &fromindex);

	static std::string replace(const std::string &src, const std::string &substr, const std::string &replacement);

	static void _snprintf_x(char *buffer, int maxsize, const char *fmt, const std::vector<Value> &values);

	static int strcasecmp(const std::string &s1, const std::string &s2);
	static int strcasecmp(const char *s1, const std::string &s2);
	static int strcasecmp(const std::string &s1, const char *s2);
	static int strcasecmp(const char *s1, const char *s2);

	static int strncasecmp(const std::string &s1, const std::string &s2, int len);
	static int strncasecmp(const char *s1, const std::string &s2, int len);
	static int strncasecmp(const std::string &s1, const char *s2, int len);
	static int strncasecmp(const char *s1, const char *s2, int len);

	static bool iequals(const std::string &s1, const std::string &s2);
	static bool iequals(const char *s1, const std::string &s2);
	static bool iequals(const std::string &s1, const char *s2);
	static bool iequals(const char *s1, const char *s2);
};

#define snprintf_x(buffer, maxsize, fmt, ...)              \
	{                                                      \
		std::vector<Value> values;                         \
		processValues(values, fmt, ##__VA_ARGS__);         \
		String::_snprintf_x(buffer, maxsize, fmt, values); \
	}

inline String operator+(const char *str1, const String &str2)
{
	String dstsrc(str1);

	return dstsrc += str2;
}
inline String operator+(const String &str1, const char *str2)
{
	String dstsrc(str1);

	return dstsrc += str2;
}
inline String operator+(const std::string &str1, const String &str2)
{
	String dstsrc(str1);

	return dstsrc += str2;
}
inline String operator+(const String &str1, const std::string &str2)
{
	String dstsrc(str1);

	return dstsrc += str2;
}

} // namespace Base
} // namespace Public

#endif // __BASE_STRING_H__
