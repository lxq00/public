//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: Process.cpp 120 2013-05-22 07:47:37Z  $
//

#ifndef WIN32
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#if defined(__linux__)
#include <sys/prctl.h>
#endif

#else
#include "Base/Base.h"
#include <TlHelp32.h>

#define PSAPI_VERSION 1
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#endif
#include <string>
#include "Base/Process.h"
#include "Base/PrintLog.h"
#include "Base/String.h"

namespace Public
{
namespace Base
{

class ProcessImpl : public Process
{
public:
	/// ���캯��
	ProcessImpl(const std::string &name, const std::vector<std::string> &argv, bool relation)
		: handle(0)
	{
#ifdef WIN32
		jobhandle = 0;
		STARTUPINFOA StartupInfo = {0};
		PROCESS_INFORMATION ProcessInformation = {0};
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_HIDE;
#ifdef DEBUG
		StartupInfo.wShowWindow = SW_SHOWNORMAL;
#endif
		char param[512] = "";
		for (size_t i = 0; i < argv.size(); i++)
		{
			snprintf_x(param, 512, "%s %s", param, argv[i].c_str());
		}

		if (!CreateProcessA((LPSTR)name.c_str(), ((argv.size() > 0) ? (LPSTR)(param) : NULL), NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInformation))
		{
			DWORD ret = GetLastError();
			logwarn("Can't open process:%s: errno (%d) \n", name.c_str(), ret);
			isok = false;
			handle = NULL;
			return;
		}
		handle = ProcessInformation.hProcess;
		pid = ProcessInformation.dwProcessId;

		if (relation)
		{
			createRelation();
		}

		CloseHandle(ProcessInformation.hThread);
#else
		handle = fork();
		pid = (int)handle;
		if (handle == 0) // sub process
		{
			if (relation)
			{
#if defined(__linux__)
				prctl(PR_SET_PDEATHSIG, SIGHUP);
#endif
			}
			if (argv.size() > 0)
			{
				char *args[argv.size() + 2];
				args[0] = (char *)name.c_str();
				size_t i = 1;
				for (; i < argv.size() + 1; i++)
				{
					args[i] = (char *)argv[i - 1].c_str();
				}
				args[i] = NULL;
				execv(name.c_str(), args);
			}
			else
			{
				execl(name.c_str(), name.c_str(), (char *)NULL);
			}
			//logwarn("Can't run sub process:%s:%s \n", name, strerror(errno));
			_exit(1);
		}
		if (handle == -1)
		{
			// logwarn("Can't open process:%s:%s \n", name.c_str() strerror(errno));
			isok = false;
			return;
		}
#endif
		isok = true;
	};

	/// ����������
	~ProcessImpl()
	{
		kill();
#ifdef WIN32
		if (jobhandle != 0)
		{
			CloseHandle(jobhandle);
		}
#endif
	};

	/// ɱ������
	/// \retval true �ɹ�
	/// \retval false ʧ��
	bool kill()
	{
#ifdef WIN32
		if (isok && handle)
		{
			if (TerminateProcess(handle, 255) == TRUE)
			{
				WaitForSingleObject(handle, 10 * 1000);
				CloseHandle(handle);
				isok = false;
				handle = NULL;
				return true;
			}
			WaitForSingleObject(handle, 10 * 1000);
			CloseHandle(handle);
			isok = false;
			handle = NULL;
		}
#else
		if (isok && handle > 0)
		{
			if (::kill(handle, SIGKILL) == 0)
			{
				int status;
				waitpid(handle, &status, 0);
				isok = false;
				handle = -1;
				return true;
			}
			isok = false;
			handle = -1;
		}
#endif
		return false;
	}
	inline bool isrunning() const
	{
		return isok;
	}
	ProcessHandle getHandle()
	{
		return handle;
	}

	int getPid()
	{
		return (int)pid;
	}
	bool exists()
	{
		return Process::existByPid(pid);
	}

private:
	void createRelation()
	{
#ifdef WIN32
		jobhandle = CreateJobObject(NULL, NULL);
		if (jobhandle)
		{
			JOBOBJECT_EXTENDED_LIMIT_INFORMATION extlimit;
			extlimit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
			SetInformationJobObject(jobhandle, JobObjectExtendedLimitInformation, &extlimit, sizeof(extlimit));
			AssignProcessToJobObject(jobhandle, handle);
		}
#else

#endif
	}

private:
#ifdef WIN32
	HANDLE jobhandle;
#endif
	ProcessHandle handle;
	int pid;

	bool isok;
};

shared_ptr<Process> Process::createProcess(const std::string &name, const std::vector<std::string> &argv, bool relation)
{
	shared_ptr<ProcessImpl> impl = make_shared<ProcessImpl>(name, argv, relation);
	if (impl->isrunning())
	{
		return impl;
	}
	else
	{
		return shared_ptr<Process>();
	}

	return shared_ptr<Process>();
}

bool Process::existByPid(int pid)
{
#ifdef WIN32
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	bool isFind = false;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	if (!Process32First(hsnapshot, &pe))
	{
		CloseHandle(hsnapshot);
		return false;
	}
	if (pe.th32ProcessID == pid)
	{
		CloseHandle(hsnapshot);
		return true;
	}

	while (1)
	{
		pe.dwSize = sizeof(pe);
		if (!Process32Next(hsnapshot, &pe))
		{
			break;
		}
		if (pe.th32ProcessID == pid)
		{
			isFind = true;
			break;
		}
	}
	CloseHandle(hsnapshot);

	return isFind;
#else
	return ::kill(pid, 0) != -1;
#endif
}

bool Process::killByPid(int pid)
{
#ifdef WIN32
	HANDLE hp = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, FALSE, pid);
	if (WaitForSingleObject(hp, 5000) != WAIT_OBJECT_0)
	{
		TerminateProcess(hp, 0);
	}
	return true;
#else
	return ::kill((pid_t)pid, 9) == 0;
#endif
}

uint32_t Process::getProcessId()
{
#ifdef WIN32
	return GetCurrentProcessId();
#else
	return getpid();
#endif
}

struct BaseProcessInfo : public ProcessInfo
{
	uint32_t pagefileMem;
	uint32_t workmem;
	uint32_t threads;
	uint32_t cpu;

	BaseProcessInfo(shared_ptr<Process> process, uint32_t timeout) : pagefileMem(-1), workmem(-1), threads(-1), cpu(-1)
	{
#ifdef WIN32
		getProcessInfo(process->getHandle(), timeout);
#else
		getProcessInfo(process->getPid(), timeout);
#endif
	}
	BaseProcessInfo(uint32_t pid, uint32_t timeout) : pagefileMem(-1), workmem(-1), threads(-1), cpu(-1)
	{
#ifdef WIN32
		if (pid == 0)
			pid = GetCurrentProcessId();
		HANDLE hProcessHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		getProcessInfo(hProcessHandle, timeout);
		CloseHandle(hProcessHandle);
#else
		if (pid == 0)
			pid = getpid();
		getProcessInfo(pid, timeout);
#endif
	}

	//�ύ�ڴ棬��λM
	bool pagefileMemory(uint32_t &mem)
	{
		if (pagefileMem != (uint32_t)-1)
		{
			mem = pagefileMem;
		}

		return pagefileMem != (uint32_t)-1;
	}

	//�����ڴ棬��λM
	bool workMemory(uint32_t &mem)
	{
		if (workmem != (uint32_t)-1)
		{
			mem = workmem;
		}
		return workmem != (uint32_t)-1;
	}

	//�����߳���
	bool threadNum(uint16_t &num)
	{
		if (threads != (uint32_t)-1)
		{
			num = threads;
		}
		return threads != (uint32_t)-1;
	}

	//��ǰCPUʹ���� 0 ~ 100,timeout ʹ�ó�ʱʱ�� ��λms
	bool cpuUsage(uint16_t &usage)
	{
		if (cpu != (uint32_t)-1)
		{
			usage = cpu;
		}

		return cpu != (uint32_t)-1;
	}
#ifdef WIN32
	void getProcessInfo(HANDLE procHandle, uint32_t timeout)
	{
		getProcessMemInfo(procHandle, pagefileMem, workmem);
		getProcessThreads(procHandle, threads);
		cpu = getcpuage(procHandle, timeout);
	}

	// ��ȡ�����ڴ��С
	bool getProcessMemInfo(HANDLE procHandle, uint32_t &pagefileMemory, uint32_t &workMemory)
	{
		PROCESS_MEMORY_COUNTERS memoryInfo = {0};
		memoryInfo.cb = sizeof(PROCESS_MEMORY_COUNTERS);
		if (!GetProcessMemoryInfo(procHandle, &memoryInfo, sizeof(PROCESS_MEMORY_COUNTERS)))
		{
			return false;
		}

		pagefileMemory = (uint32_t)(memoryInfo.PagefileUsage / 1024 / 1024);
		workMemory = (uint32_t)(memoryInfo.WorkingSetSize / 1024 / 1024);
		return true;
	}
	int getProcessThreads(HANDLE hProcess, uint32_t &threads)
	{
		DWORD dwProcessID = GetProcessId(hProcess);
		if (dwProcessID == NULL)
		{
			return false;
		}

		int threadnum = 0;
		HANDLE hProcessShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 info = {0};
		info.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hProcessShot, &info))
		{
			if (dwProcessID == info.th32ProcessID)
			{
				threadnum = info.cntThreads;
			}
			else
			{
				while (Process32Next(hProcessShot, &info))
				{
					if (dwProcessID == info.th32ProcessID)
					{
						threadnum = info.cntThreads;
						break;
					}
				}
			}
		}
		CloseHandle(hProcessShot);
		threads = threadnum;

		return true;
	}
	/// ʱ��ת��
	uint64_t file_time_2_utc(const FILETIME *ftime)
	{
		LARGE_INTEGER li;

		assert(ftime);
		li.LowPart = ftime->dwLowDateTime;
		li.HighPart = ftime->dwHighDateTime;
		return li.QuadPart;
	}

	/// ���CPU�ĺ���
	int get_processor_number()
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return (int)info.dwNumberOfProcessors;
	}

	int getcpuage(HANDLE hProcess, uint32_t timeout)
	{
		int64_t time1_sys, time1_now, time2_sys, time2_now;
		if (getcpuusageitem(hProcess, time1_now, time1_sys) != 0)
		{
			return -1;
		}

		Sleep(timeout);

		if (getcpuusageitem(hProcess, time2_now, time2_sys) != 0)
		{
			return -1;
		}

		int64_t system_time_delta = time2_sys - time1_sys;
		int64_t time_delta = time2_now - time1_now;

		if (time_delta == 0)
			return -1;

		// We add time_delta / 2 so the result is rounded.
		int cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);

		return cpu;
	}

	int getcpuusageitem(HANDLE hProcess, int64_t &nowtime, int64_t &systime)
	{
		//cpu����
		static int processor_count_ = -1;
		//��һ�ε�ʱ��
		static int64_t last_time_ = 0;
		static int64_t last_system_time_ = 0;

		FILETIME now;
		FILETIME creation_time;
		FILETIME exit_time;
		FILETIME kernel_time;
		FILETIME user_time;
		int64_t system_time;
		int64_t time;
		int64_t system_time_delta;
		int64_t time_delta;

		int cpu = -1;

		if (processor_count_ == -1)
		{
			processor_count_ = get_processor_number();
		}

		GetSystemTimeAsFileTime(&now);

		if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
		{
			// We don't assert here because in some cases (such as in the Task Manager)
			// we may call this function on a process that has just exited but   we have
			// not yet received the notification.
			return -1;
		}
		systime = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
		nowtime = file_time_2_utc(&now);

		return 0;

		if ((last_system_time_ == 0) || (last_time_ == 0))
		{
			// First call, just set the last values.
			last_system_time_ = system_time;
			last_time_ = time;
			return -1;
		}

		system_time_delta = system_time - last_system_time_;
		time_delta = time - last_time_;

		assert(time_delta != 0);

		if (time_delta == 0)
			return -1;

		// We add time_delta / 2 so the result is rounded.
		cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
		last_system_time_ = system_time;
		last_time_ = time;
		return cpu;
	}
#else
#define PROCESS_ITEM 14 //����CPUʱ�俪ʼ������

	typedef struct //����һ��occupy�Ľṹ��
	{
		unsigned int user;   //��ϵͳ������ʼ�ۼƵ���ǰʱ�̣������û�̬������ʱ�䣬������ niceֵΪ�����̡�
		unsigned int nice;   //��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�niceֵΪ���Ľ�����ռ�õ�CPUʱ��
		unsigned int system; //��ϵͳ������ʼ�ۼƵ���ǰʱ�̣����ں���̬������ʱ��
		unsigned int idle;   //��ϵͳ������ʼ�ۼƵ���ǰʱ�̣���IO�ȴ�ʱ������������ȴ�ʱ��iowait (12256) ��ϵͳ������ʼ�ۼƵ���ǰʱ�̣�IO�ȴ�ʱ��(since 2.5.41)
	} total_cpu_occupy_t;

	typedef struct
	{
		pid_t pid;			 //pid��
		unsigned int utime;  //���������û�̬���е�ʱ�䣬��λΪjiffies
		unsigned int stime;  //�������ں���̬���е�ʱ�䣬��λΪjiffies
		unsigned int cutime; //���������߳����û�̬���е�ʱ�䣬��λΪjiffies
		unsigned int cstime; //���������ں���̬���е�ʱ�䣬��λΪjiffies
	} process_cpu_occupy_t;

	unsigned int get_cpu_process_occupy(const pid_t p)
	{
		char file[64] = {0}; //�ļ���
		process_cpu_occupy_t t;

		FILE *fd;						   //�����ļ�ָ��fd
		char line_buff[1024] = {0};		   //��ȡ�еĻ�����
		sprintf(file, "/proc/%d/stat", p); //�ļ��е�11�а�����
		fd = fopen(file, "r");			   //��R���ķ�ʽ���ļ��ٸ���ָ��fd
		if (fd == NULL)
		{
			return 0;
		}
		if (fgets(line_buff, sizeof(line_buff), fd) == NULL)
		{
		} //��fd�ļ��ж�ȡ����Ϊbuff���ַ����ٴ浽��ʼ��ַΪbuff����ռ���

		sscanf(line_buff, "%u", &t.pid);									//ȡ�õ�һ��
		const char *q = get_items(line_buff, PROCESS_ITEM);					//ȡ�ôӵ�14�ʼ����ʼָ��
		sscanf(q, "%u %u %u %u", &t.utime, &t.stime, &t.cutime, &t.cstime); //��ʽ����14,15,16,17��

		fclose(fd); //�ر��ļ�fd
		return (t.utime + t.stime + t.cutime + t.cstime);
	}
	unsigned int get_cpu_total_occupy()
	{
		FILE *fd;			   //�����ļ�ָ��fd
		char buff[1024] = {0}; //����ֲ�����buff����Ϊchar���ʹ�СΪ1024
		total_cpu_occupy_t t;

		fd = fopen("/proc/stat", "r"); //��R���ķ�ʽ��stat�ļ��ٸ���ָ��fd
		if (fd == NULL)
		{
			return 0;
		}
		if (fgets(buff, sizeof(buff), fd) == NULL)
		{
		} //��fd�ļ��ж�ȡ����Ϊbuff���ַ����ٴ浽��ʼ��ַΪbuff����ռ���
		/*�����ǽ�buff���ַ������ݲ���format��ת��Ϊ���ݵĽ��������Ӧ�Ľṹ����� */
		char name[16]; //��ʱ��������ַ���
		sscanf(buff, "%s %u %u %u %u", name, &t.user, &t.nice, &t.system, &t.idle);

		fclose(fd); //�ر��ļ�fd
		return (t.user + t.nice + t.system + t.idle);
	}

	float get_pcpu(pid_t p, uint32_t timeout)
	{
		float pcpu = 0;
#if 0
	{
		float processnum = sysconf(_SC_NPROCESSORS_CONF);

		float cpu = 0;

		char buffer[256];
		sprintf(buffer,"top -b -n 1 -p %d |grep %d | awk '{print $9}'",p,p);
		FILE* fd = popen(buffer,"r");
		if(fd != NULL)
		{
			int readlen = fread(buffer,1,128,fd);
			if(readlen <= 0 || readlen > 128)
			{
				readlen = 0;
			}
			buffer[readlen] = 0;
			fclose(fd);
		}
		cpu = atof(buffer);
		pcpu =  cpu / processnum;
	}
#else
		{
			unsigned int total1, total2, idle1, idle2;
			total1 = get_cpu_total_occupy();
			idle1 = get_cpu_process_occupy(p);

			usleep(500000);

			total2 = get_cpu_total_occupy();
			idle2 = get_cpu_process_occupy(p);

			pcpu = 100.0 * (idle2 - idle1) / (total2 - total1);
		}
#endif

		return pcpu;
	}

	const char *get_items(const char *buffer, int ie)
	{
		assert(buffer);
		const char *p = buffer; //ָ�򻺳���
		int len = strlen(buffer);
		int count = 0; //ͳ�ƿո���
		if (1 == ie || ie < 1)
		{
			return p;
		}
		int i;

		for (i = 0; i < len; i++)
		{
			if (' ' == *p)
			{
				count++;
				if (count == ie - 1)
				{
					p++;
					break;
				}
			}
			p++;
		}

		return p;
	}

	bool getProcessInfo(pid_t pid, uint32_t timeout)
	{
		cpu = (uint32_t)get_pcpu(pid, timeout);

		char buffer[64];
		snprintf_x(buffer, 63, "/proc/%d/status", pid);

		if (access(buffer, R_OK) != 0)
		{
			return false;
		}
		FILE *fd = fopen(buffer, "rb");
		if (fd != NULL)
		{
			char buffer[128];
			while (fgets(buffer, 128, fd) != NULL)
			{
				if (getValue(buffer, "VmSize:", pagefileMem))
				{
					pagefileMem /= 1024;
				}
				else if (getValue(buffer, "VmRSS:", workmem))
				{
					workmem /= 1024;
				}
				else if (getValue(buffer, "Threads:", threads))
				{
				}
			}
			fclose(fd);
		}

		return true;
	}
	bool getValue(const char *buffer, const std::string &key, uint32_t &val)
	{
		char *tmp = (char *)strstr(buffer, key.c_str());
		if (tmp == NULL)
		{
			return false;
		}
		tmp += key.length();
		while (*tmp)
		{
			if (*tmp == ' ' || *tmp == '\t')
			{
				tmp++;
			}
			else
			{
				break;
			}
		}

		char *endtmp = (char *)strchr(tmp, ' ');
		if (endtmp != NULL)
		{
			*endtmp = 0;
		}
		else
		{
			char *endtmp = tmp + strlen(tmp) - 1;
			while (endtmp > tmp)
			{
				if (*endtmp == ' ' || *endtmp == '\t' || *endtmp == '\r' || *endtmp == '\n')
				{
					*endtmp = 0;
					endtmp--;
				}
				else
				{
					break;
				}
			}
		}

		val = atoi(tmp);

		return true;
	}
#endif
};

//��ȡָ�����̵Ľ�����Ϣ,process ���̶���
shared_ptr<ProcessInfo> ProcessInfo::getProcessInfo(const shared_ptr<Process> &process, uint32_t timeout)
{
	shared_ptr<ProcessInfo> info(new BaseProcessInfo(process, timeout));
	return info;
}

//���ݽ���ID��ȡ������Ϣ,pidΪ����ID��
shared_ptr<ProcessInfo> ProcessInfo::getProcessInfo(int pid, uint32_t timeout)
{
	shared_ptr<ProcessInfo> info(new BaseProcessInfo(pid, timeout));
	return info;
}

//��ȡ��ǰ���̵Ľ�����Ϣ
shared_ptr<ProcessInfo> ProcessInfo::getCurrProcessInfo(uint32_t timeout)
{
	return getProcessInfo(0, timeout);
}

} // namespace Base
} // namespace Public
