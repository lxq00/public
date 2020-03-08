//
//  Copyright (c)1998-2014, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: PrintLog.h 3 2013-01-21 06:57:38Z  $
//

#ifndef __BASE_PRINT_LOG_H__
#define __BASE_PRINT_LOG_H__

#include <stdio.h>
#include "Defs.h"
#include "Function.h"
#include "IntTypes.h"
#include <string>
#include "Base/Time.h"
#include "Base/Shared_ptr.h"
#include "String.h"

namespace Public
{
namespace Base
{

enum LOG_Level
{
	LOG_Level_NONE = 0,

	LOG_Level_FATAL, //����
	LOG_Level_ERROR, //����
	LOG_Level_WARN,  //����
	LOG_Level_INFO,  //��Ϣ
	LOG_Level_TRACE, //����
	LOG_Level_DEBUG, //����
};

struct LogPrintInfo
{
	LOG_Level loglev;
	std::string loglevstr;

	std::string filename;
	std::string func;
	int line;

	Time time;

	std::string logstr;

	std::string logdetails;
};

/// ��ӡ����ص���������
/// ����ΪҪ��ӡ���ַ���
typedef Function<void(const shared_ptr<LogPrintInfo> &)> LogPrinterProc;

///���ô�ӡ�ȼ�
void BASE_API setprintloglevel(LOG_Level level);

//��ӡ����
void BASE_API printer(LOG_Level level, const char *filename, const char *func, int line, const char *fmt, const std::vector<Value> &values);

/// ���ô�ӡ������ص�����
/// \param printer [in]  ����ص�����, Ϊ��ʱ���ô�ӡ�������׼����豸
/// \retval 0 �ɹ�
/// \retval -1 ʧ��
int BASE_API setlogprinter(void *userflag, const LogPrinterProc &printer);

/// �����ӡ������ص�����
int BASE_API cleanlogprinter(void *userflag);

#define logprinter(level, filename, func, line, fmt, ...)  \
	{                                                      \
		std::vector<Value> values;                         \
		processValues(values, fmt, ##__VA_ARGS__);         \
		printer(level, filename, func, line, fmt, values); \
	}

/// ��¼������Ϣ������6����ֱ�ӵ��ã�����ʵ��debgf��
/// \return ���ش�ӡ���ֽ���
#define logdebug(fmt, ...) logprinter(LOG_Level_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

/// ��¼������Ϣ������5����ֱ�ӵ��ã�����ʵ��tracef��
/// \return ���ش�ӡ���ֽ���
#define logtrace(fmt, ...) logprinter(LOG_Level_TRACE, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

/// ��¼������Ϣ������4����ֱ�ӵ��ã�����ʵ��infof��
/// \return ���ش�ӡ���ֽ���
#define loginfo(fmt, ...) logprinter(LOG_Level_INFO, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

/// ��¼������Ϣ������3����ֱ�ӵ��ã�����ʵ��warnf��
/// \return ���ش�ӡ���ֽ���
#define logwarn(fmt, ...) logprinter(LOG_Level_WARN, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

/// ��¼������Ϣ������2����ֱ�ӵ��ã�����ʵ��errorf��
/// \return ���ش�ӡ���ֽ���
#define logerror(fmt, ...) logprinter(LOG_Level_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

/// ��¼����������Ϣ������1����ֱ�ӵ��ã�����ʵ��fatalf��
/// \return ���ش�ӡ���ֽ���
#define logfatal(fmt, ...) logprinter(LOG_Level_FATAL, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

} // namespace Base
} // namespace Public

#endif //__BASE_PRINT_LOG_H__
