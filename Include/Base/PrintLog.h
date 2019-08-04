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
#include "Func.h"
#include "IntTypes.h"
#include <string>

namespace Public{
namespace Base{

enum LOG_Level{
	LOG_Level_NONE,
	LOG_Level_FATAL,
	LOG_Level_ERROR,
	LOG_Level_WARN,
	LOG_Level_INFO,
	LOG_Level_TRACE,
	LOG_Level_DEBUG
};

struct LogPrintInfo
{
	std::string			logstr;
	LOG_Level			loglev;
	std::string			logprex;
};


/// ��ӡ����ص���������
/// ����ΪҪ��ӡ���ַ���
typedef Function1<void,const LogPrintInfo&> LogPrinterProc;

///���ô�ӡ�ȼ�
void BASE_API setprintloglevel(LOG_Level level);

//��ӡ����
void BASE_API printer(LOG_Level level,const char* filename,const char* func,int line,const char* fmt, ...);

/// ���ô�ӡ������ص�����
/// \param printer [in]  ����ص�����, Ϊ��ʱ���ô�ӡ�������׼����豸
/// \retval 0 �ɹ�
/// \retval -1 ʧ��
int BASE_API setlogprinter(void* userflag,const LogPrinterProc& printer);

/// �����ӡ������ص�����
int BASE_API cleanlogprinter(void* userflag);


/// ��¼������Ϣ������6����ֱ�ӵ��ã�����ʵ��debgf��
/// \return ���ش�ӡ���ֽ���
#define logdebug(fmt,...) printer(LOG_Level_DEBUG,__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)

/// ��¼������Ϣ������5����ֱ�ӵ��ã�����ʵ��tracef��
/// \return ���ش�ӡ���ֽ���
#define logtrace(fmt, ...) printer(LOG_Level_TRACE,__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)

/// ��¼������Ϣ������4����ֱ�ӵ��ã�����ʵ��infof��
/// \return ���ش�ӡ���ֽ���
#define loginfo(fmt, ...) printer(LOG_Level_INFO,__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)

/// ��¼������Ϣ������3����ֱ�ӵ��ã�����ʵ��warnf��
/// \return ���ش�ӡ���ֽ���
#define logwarn(fmt, ...) printer(LOG_Level_WARN,__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)

/// ��¼������Ϣ������2����ֱ�ӵ��ã�����ʵ��errorf��
/// \return ���ش�ӡ���ֽ���
#define logerror(fmt, ...) printer(LOG_Level_ERROR,__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)

/// ��¼����������Ϣ������1����ֱ�ӵ��ã�����ʵ��fatalf��
/// \return ���ش�ӡ���ֽ���
#define logfatal(fmt, ...) printer(LOG_Level_FATAL,__FILE__,__FUNCTION__,__LINE__,fmt,##__VA_ARGS__)


} // namespace Base
} // namespace Public

#endif //__BASE_PRINT_LOG_H__


