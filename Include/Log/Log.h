#ifndef __PUBLIC_LOG_H__
#define __PUBLIC_LOG_H__
#include "Log/Defs.h"
#include "Base/Base.h"
using namespace Public::Base;

namespace Public{
namespace Log{

class LOG_API LogSystem
{
public:
	/// ��ӡ Log�� �汾��Ϣ
	static void printLibVersion();
	static void init(const std::string& appName,const std::string& logpath = "", LOG_Level logLevel = LOG_Level_DEBUG);
	static void uninit();
};

};
};

#endif //__PUBLIC_LOG_H__
