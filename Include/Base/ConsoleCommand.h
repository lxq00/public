#ifndef _BASE_CONSOLE_COMMAND_H
#define _BASE_CONSOLE_COMMAND_H

#include <iostream>
#include <list>
using namespace std;

#include "Base/Defs.h"
#include "Base/Thread.h"

namespace Public {
namespace Base {

//����ص�����
typedef Function<void(int,const char**)> ConsoleCmdCallback;

class BASE_API ConsoleCommand : public Thread
{
public:
	ConsoleCommand(void);
	~ConsoleCommand(void);

	//����Consoleģ����⣬��������ǰ���Զ���ʾ�ñ���
	//����Ĭ��Ϊ��
	void setModuleTitle(const std::string& title);

	//�ж�Console�����ȡ�߳��Ƿ���ִ��
	//true��ʾ��������
	//false��ʾֹͣ����
	bool getValid(void);

	//����Console�����ȡ�߳�
	//standalone:
	//			false��ʾ����ʹ�ö����߳�
	//			true��ʾ����ʹ�õ������߳�
	void run(const std::string& title);

	//����û��Զ�������
	//name : ��������
	//desc : ��������
	//func : ����ص�����
	bool addUserCmd(const std::string& name, const std::string& desc,const ConsoleCmdCallback& func);

	static void runCmd(const std::string& title);

protected:
	/// �߳�ִ����,��һ���麯��,�������߳��������ش˺���,ʵ�ָ��Ե���Ϊ.
	virtual void threadProc();

	void runing();
	//�������
	virtual bool parseCmd(const char* command);

private:
	//�ַ�����
	//?,help,exit�������ڲ�������������ʹ�ûص��������û�����
	bool dispatchCmd(const int argc,const char** argv, ConsoleCmdCallback func);

	//����������������
	bool separatePara(const char* command, int& argc, char** argv);

private:
	//���ݼ���
	struct tagConsoleInternal_t;
	tagConsoleInternal_t* consoleInternal;
	bool finished;
};//class ConsoleCommand
}//namespace Base
}//namespace Public

#endif//_BASE_CONSOLE_COMMAND_H
