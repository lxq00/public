//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: SystemCall.cpp 11 2013-01-22 08:42:03Z  $
//

#ifndef WIN32
	#include <errno.h>
	#include <sys/wait.h>
	#include <sys/types.h>
	#include <unistd.h>
#else
	#include <process.h>
#endif

#include <stdlib.h>
#include "Base/PrintLog.h"


namespace {

using namespace Public::Base;

#ifdef __linux__
inline int linux_system(const char* command)
{
	if (command == NULL)
	{
		return (1);
	}

	int status = -1;
	pid_t pid = vfork();

	if (pid < 0)
	{
	//	errorf("vfork() failed>>>>\n");
		status = -1;
	}
	else if (pid == 0)
	{
	//	tracef("inclild ppid=%d pid=%d>>>>>\n", getppid(), getpid());
		execl("/bin/sh", "sh", "-c", command, (char*)0);
		_exit(127);	// �ӽ���ִ�������򲻻�ִ�д����
	}
	else
	{
		//tracef("in parent pid =%d>>>>\n", getppid());
		while (waitpid(pid, &status, 0) < 0)
		{
			if (errno != EINTR)
			{
				status = -1;
				break;
			}
		}
	}

	return status;
}
#else
inline int windows_system(const char* cmd)
{
	if (cmd == NULL)
	{
		return -1;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;

	std::string cmdtmp = "cmd.exe /c ";
	cmdtmp = cmdtmp + cmd;

	if (!CreateProcess(NULL, (char*)cmdtmp.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) //ɾ���ǿ��ļ���
	{
		return false;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}
#endif // __linux__

} // namespace noname

namespace Public{
namespace Base {

int BASE_API SystemCall(const std::string& command)
{
#ifndef WIN32
	return linux_system(command.c_str());
#else
	return windows_system(command.c_str());
#endif
}

} // namespace Base
} // namespace Public

