//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: Base.h 3 2013-01-21 06:57:38Z  $
//   ������ͳһͷ�ļ�
#ifndef _BASE_CLASS_H_
#define _BASE_CLASS_H_

#include "Base/Defs.h"
#include "Base/CircleBuffer.h"
#include "Base/ErrorInfo.h"
#include "Base/Excel.h"
#include "Base/AtomicCount.h"
#include "Base/Base64.h"
#include "Base/BPlusTreeIO.h"
#include "Base/ConsoleCommand.h"
#include "Base/BaseTemplate.h"
#include "Base/ByteOrder.h"
#include "Base/Crc.h"
#include "Base/CPU.h"
#include "Base/DynamicLib.h"
#include "Base/DynamicMemPool.h"
#include "Base/File.h"
#include "Base/FileFind.h"
#include "Base/Directory.h"
#include "Base/Function.h"
#include "Base/Guard.h"
#include "Base/JSON.h"
#include "Base/IntTypes.h"
#include "Base/LockFreeList.h"
#include "Base/Math.h"
#include "Base/Md5.h"
#include "Base/Mutex.h"
#include "Base/MediaCode.h"
#include "Base/PrintLog.h"
#include "Base/Process.h"
#include "Base/ReadWriteMutex.h"
#include "Base/RecursiveMutex.h"
#include "Base/RecycleBin.h"
#include "Base/RegEx.h"
#include "Base/Semaphore.h"
#include "Base/Sha1.h"
#include "Base/ShareMem.h"
#include "Base/IPC.h"
#include "Base/SqlDataBase.h"
#include "Base/SimPipe.h"
#include "Base/StaticMemPool.h"
#include "Base/String.h"
#include "Base/StringBuffer.h"
#include "Base/System.h"
#include "Base/Shared_ptr.h"
#include "Base/Thread.h"
#include "Base/ThreadEx.h"
#include "Base/ThreadPool.h"
#include "Base/Time.h"
#include "Base/Timer.h"
#include "Base/Unicode.h"
#include "Base/URLEncoding.h"
#include "Base/Version.h"
#include "Base/Expression.h"
#include "Base/CoreDump.h"
#include "Base/Value.h"
#include "Base/Variant.h"
#include "Base/URL.h"
#include "Base/Guid.h"
#include "Base/Hash.h"
#include "Base/Host.h"
#include "Base/wwwAuthenticate.h"
#include "Base/XML.h"
#include "Base/zip.h"

/*
			Public ���������
һ���̡߳��̳߳ء����⡢ͬ��
	Thread.h			�̶߳��󣬼̳�ʹ�ã�����sleep��   
	ThreadEx.h			���������̣߳�   
	ThreadPool			�̳߳�
	Muetx.h				�̻߳�����
	Guard.h				�Զ��Ի�������������
	ReadWriteMutex.h	�̶߳�д��
	RecursiveMutex.h	�̵߳ݹ���
	Semaphore.h			�߳�ͬ���ź���

�����ڴ����
	DynamicMemPool.h	�ڴ��С�Զ��������ڴ��
	StaticMemPool.h		�ڴ��С�޶����ڴ�ء������ڴ��ַ�����ⲿ����
	ByteOrder.h			��С���жϼ�����
	TempCache.h			��ʱ����

�ģ�ϵͳ���
	AtomicCount.h		ԭ�Ӽ���
	BaseTemplate.h		��ȫ�ͷ�
	Shared_ptr.h		����ָ��
	DynamicLib.h		��̬�⶯̬����
	ConsoleCommand.h	�ն�����
	File.h				�ļ�����
	FileBrowser.h		�ļ���������ļ��в���
	PrintLog.h			��ӡ�ӿ�
	String.h			�ַ�������
	SimPipe.h			����ܵ���Ŀǰ�������ڽ���ͨѶ
	Function.h				�ص�����
	Callback.h			�ص�����
	InstanceObjectDefine.h		�������弰��ʼ��

�壺���̼�����ͨѶ
	Process.h			���̴������
	ShareMem.h			�����ڴ����
	ShareMemBuffer.h	���̼以���������̼��ź���������ͨѶ

����ʱ�䡢��ʱ��
	Time.h				ʱ�䶨��
	Timer.h				��ʱ������
	TimeRecord.h		ʱ��ʹ�ü�¼

�ߣ��������
	Base64.h			base64���롢����
	Crc.h				CRC����
	Sha1.h				sha1�����
	Md5.h				MD5����
	URLEncoding.h		URL�����
	Excel.h				excel�ļ���д����
	xml.h				XML�ļ���д����
	JSON.h				json���ݸ�ʽ������

�ˣ�����
	URL.h				URLͨѶ����
*/

namespace Public {
namespace Base {

///Base��Ľӿ���
class BASE_API BaseSystem
{
public:
	//�ر��¼�����
	enum CloseEvent
	{
		CloseEvent_INPUT,	//�û��ն��˳�����Ҫ��gConsoleCommand
		CloseEvent_CTRLC,	//�����յ�ctrl+Cɱ������
		CloseEvent_CTRLBREAK,//�����յ�ctrl+BREAKɱ������
		CloseEvent_ONCLOSE,	//�����յ����Ͻǹر�����
		CloseEvent_LOGOFF,	//�û�ע��
	};

	//�ر��¼��ص�֪ͨ����
	typedef Function<void(void*,CloseEvent)> 	closeEventCallback;
public:
	/// ��ӡ Base�� �汾��Ϣ
	static void  printVersion();

	//��ʼ��Base����ע��һ���ر��¼�֪ͨ�ص�
	static void init(const closeEventCallback& closeEvent = closeEventCallback(),void* userdata = NULL);
	//����ʼ��Base��
	static void uninit();

	//������־�ļ��洢
	static void startSaveLog(const std::string& appName, const std::string& logpath = "", LOG_Level logLevel = LOG_Level_DEBUG);

	//ֹͣ��־�ļ��洢
	static void stopSaveLog();

	//�����˳�
	static void consoleCommandClose(CloseEvent cmd);

	//�ȴ�base�˳��ӿڡ���ʹ�������ն�ʱ�����øýӿڡ��ýӿ�δ����ģʽ
	static void waitSystemClose();

	//�Զ���ʱǿ���˳�
	static void autoExitDelayer(uint64_t delaySecond = 30);
};

} // namespace Component
} // namespace Public

#endif // _BASE_CLASS_H_

