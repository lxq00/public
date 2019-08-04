//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: Base.cpp 11 2013-01-22 08:42:03Z  $
//

#ifndef WIN32
#include <sys/types.h>
#include <unistd.h>
#endif

#include "Base/Base.h"
#include "../version.inl"

namespace Public {
namespace Base {

/// ��ӡ Base�� �汾��Ϣ
void BaseSystem::printVersion()
{
	Public::Base::AppVersion appVersion("Base Lib", r_major, r_minor, r_build, versionalias, __DATE__);
	appVersion.print();
}

void BaseSystem::autoExitDelayer(uint64_t delaySecond /* = 30 */)
{
#ifdef WIN32
	uint32_t processId = GetCurrentProcessId();
	char tmpStr[512] = { 0 };
	snprintf_x(tmpStr, 512, "ping -n %llu 127.0.0.1 >nul&&taskkill /pid %u /f",
		delaySecond,
		processId
	);

	char systemPath[256] = { 0 };
	GetSystemDirectory(systemPath, 256);	

	std::deque<std::string> execargv = { "/c" ,tmpStr };

	Process::createProcess((std::string(systemPath)+PATH_SEPARATOR+std::string("cmd.exe")).c_str(), execargv);

#else //#ifdef x86

	uint32_t processId = getpid();
	char tmpStr[512] = { 0 };
	snprintf_x(tmpStr, 512, "sleep %llu&&kill -9 %u",
		delaySecond,
		processId
	);

	std::deque<std::string> execargv;
	execargv.push_back("-c");
	execargv.push_back(tmpStr);

	Process::createProcess("/bin/sh",execargv);
#endif
}

static BaseSystem::closeEventCallback	closeEvent = NULL;
static void*								closeParam = NULL;
static bool 								systemIsClose = false;
static Semaphore							closeSem;
static BaseSystem::CloseEvent			closeCmd;

void BaseSystem::consoleCommandClose(CloseEvent cmd)
{
	closeCmd = cmd;
	systemIsClose = true;
	closeSem.post();
}

void BaseSystem::waitSystemClose()
{
	closeSem.pend();
	if(systemIsClose)
	{
		closeEvent(closeParam,closeCmd);
	}
}

#ifdef WIN32
#include <Windows.h>
bool consoleHandler(DWORD event)
{
	if(systemIsClose)
	{
		return true;
	}
	switch(event)
	{
	case CTRL_C_EVENT:
		BaseSystem::consoleCommandClose(BaseSystem::CloseEvent_CTRLC);
		break;
	case CTRL_BREAK_EVENT:
		BaseSystem::consoleCommandClose(BaseSystem::CloseEvent_CTRLBREAK);
		break;
	case CTRL_CLOSE_EVENT:
		BaseSystem::consoleCommandClose(BaseSystem::CloseEvent_ONCLOSE);
		break;
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_LOGOFF_EVENT:
		BaseSystem::consoleCommandClose(BaseSystem::CloseEvent_LOGOFF);
		break;
	default:
		break;
	}

	return true;
}
#else
#include <signal.h>
void consoleHandler(int event)
{
	if(systemIsClose)
	{
		return;
	}
	switch(event)
	{
	case SIGQUIT:
		BaseSystem::consoleCommandClose(BaseSystem::CloseEvent_CTRLBREAK);
		break;
	case SIGSTOP:
	case SIGTERM:
	case SIGINT:
		BaseSystem::consoleCommandClose(BaseSystem::CloseEvent_CTRLC);
		break;
	}
}
#endif

void BaseSystem::init(const closeEventCallback& _closeEvent,void* userdata)
{
	closeEvent = _closeEvent;
	closeParam = userdata;
#ifdef WIN32
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)consoleHandler,true);
#else
	signal(SIGPIPE,SIG_IGN);
	signal(SIGQUIT,consoleHandler);
	signal(SIGSTOP,consoleHandler);
	signal(SIGTERM,consoleHandler);
	signal(SIGINT,consoleHandler);
#endif
}

void BaseSystem::uninit()
{
	
}

} // namespace Base
} // namespace Public

