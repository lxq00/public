//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: Socket.h 216 2015-12-15 11:33:55Z  $
//

#ifndef __NETWORK_SOCKET_H__
#define __NETWORK_SOCKET_H__

#include "Base/Base.h"
#include "Network/Defs.h"
#include "NetAddr.h"

using namespace Public::Base;

namespace Public{
namespace Network{


#define INVALIDHANDLE -1

#ifndef INFINITE
#define INFINITE -1
#endif
#define SOCKETDEFAULTTIMEOUT 10000


///socket������Ҫ��worker���ϣ���Ҫ���ⶨ�嵱ǰsocket������Ҫ���߳�����Ϣ
//�ڲ�Ϊboost::asio::io_work����Ϊboost::asio�ṩio_server
class NETWORK_API IOWorker
{
public:
	typedef Function<void(void*)> EventCallback;
public:
	class NETWORK_API ThreadNum
	{
	public:
		///ָ�������̵߳ĵĸ��� ��num == 0 ʱ ʹ�� num = 1
		ThreadNum(uint32_t _num) :num(_num) {};
		///cpuCorePerThread Ϊһ��cpucore�����м����̣߳�����Ϊ cpuCorePerThread*cpucore
		///minThread ���ٵ��߳���,maxThread�����߳���
		///����������߳���Ϊ0��ʹ��1���߳�
		ThreadNum(uint32_t cpuCorePerThread, uint32_t minThread, uint32_t maxThread)
		{
			num = cpuCorePerThread * Host::getProcessorNum();
			num = num < minThread ? minThread : num;
			num = num > maxThread ? maxThread : num;
		}
		~ThreadNum() {}

		uint32_t getNum() const { return num; }
	public:
		uint32_t num;
	};
public:
	IOWorker(const ThreadNum& num,Thread::Priority pri = Thread::priorDefault);
	~IOWorker();
	bool postEvent(const EventCallback& callback,void* param);

	static shared_ptr<IOWorker> defaultWorker();
public:
	class IOWorkerInternal;
	IOWorkerInternal* internal;
};

/// Net ���ӵ�״̬
enum NetStatus{
	NetStatus_connected = 0, 		//	�Ѿ�������
	NetStatus_disconnected = -1, 	// 	δ����
	NetStatus_error = -2			//  ����
};

enum NetType{
	NetType_TcpServer = 0,			//Accept��TCP
	NetType_TcpClient,				//Connect��TCP
	NetType_TcpConnection,			//���ӳɹ���TCP��Accept������TCP���ӡ�����Connect�ɹ�����TCP����
	NetType_Udp,					//udp
	NetType_Raw,
};

class NETWORK_API Socket
{
public:
	struct NETWORK_API SBuf
	{
		const char*		bufaddr;
		uint32_t		buflen;

		SBuf(const char* _buf = NULL,uint32_t _len = 0):bufaddr(_buf),buflen(_len){}
	};
	struct NETWORK_API SendBuffer
	{
	public:
		SendBuffer() {}
		virtual ~SendBuffer() {}

		virtual const char* bufferaddr() = 0;
		virtual uint32_t bufferlen() = 0;
	};
	struct NETWORK_API RecvBuffer
	{
	public:
		RecvBuffer() {}
		virtual ~RecvBuffer() {}

		virtual char* bufferaddr() = 0;
		virtual uint32_t bufferSize() = 0;
	};

public:
	/// socket����ʱ��accept�¼��ص���������һ��������ʾ����socket�������ڶ�����ʾ�¹����socket.��Ҫ�ⲿ�ͷ�
	/// �ص�����ο���void acceptCallbackFunc(Socket* oldSock,Socket* newSock);
	typedef Function<void(const weak_ptr<Socket>& /*oldSock*/, const shared_ptr<Socket>& /*newSock*/)> AcceptedCallback;

	/// socket�첽�����¼��ص�����,��һ��������ʾ����socket����
	///�ص�����ο���void connectCallbackFunc(Socket* connectSock);
	typedef Function<void(const weak_ptr<Socket>& /*connectSock*/,bool,const std::string&)> ConnectedCallback;

	/// socket�첽�Ͽ��¼��ص�����,int ��һ��������ʾ����socket�������ڶ�����ʾ�����Ͽ������������ͻ��ˣ�����˾�ʹ�ø��¼�
	/// ��socket��һ�����ӳɹ������10�������ݣ��������Ͽ����ӣ����󣬷�ֹ tcp�������ӣ�ֻ���accept������socket
	///�ص�����ο���void disconnectCallbackFunc(Socket* connectionSock,const char* disconectErrorInfo);
	typedef Function<void(const weak_ptr<Socket>& /*connectSock*/,const std::string&)> DisconnectedCallback;

	// socket����received�¼��ص���������һ��������ʾ����socket�������ڶ�����ʾʵ�ʽ��յ�socket���ݳ���.
	///�ص�����ο���void recvCallbackFunc(Socket* sock,const char* recvBuffer,int recvlen);
	typedef Function<void(const weak_ptr<Socket>& /*sock*/,const char*, int)> ReceivedCallback;
	typedef Function<void(const weak_ptr<Socket>& /*sock*/,const shared_ptr<RecvBuffer>&, int)> ReceivedCallback1;

	// socket����send�¼��ص���������һ��������ʾ����socket�������ڶ�����ʾʵ�ʷ��͵�socket���ݳ���.
	///�ص�����ο���void sendCallbackFunc(Socket* sock,const char* sendBuffer,int sendlen);
	typedef Function<void(const weak_ptr<Socket>& /*sock*/,const char*, int)> SendedCallback;
	typedef Function<void(const weak_ptr<Socket>& /*sock*/, const shared_ptr<SendBuffer>&)> SendedCallback1;
	typedef Function<void(const weak_ptr<Socket>& /*sock*/, const std::vector<SBuf>&)> SendedCallback2;
	typedef Function<void(const weak_ptr<Socket>& /*sock*/, const std::vector<shared_ptr<SendBuffer>>&)> SendedCallback3;
		
	/// socket����udp��recvfrom�¼��ص���������һ��������ʾsocket�������ڶ�����������ʾ���յ�������Ϣ��ַ�ͳ��ȣ����ĸ�������ʾ���ݷ��ͷ�ip
	///�ص�����ο���void recvfromCallbackFunc(Socket* sock,const char* recvBuffer, int recvlen ,const NetAddr& otheraddr);
	typedef Function<void(const weak_ptr<Socket>& /*sock*/,const char*, int,const NetAddr&)> RecvFromCallback;
	typedef Function<void(const weak_ptr<Socket>& /*sock*/,const shared_ptr<RecvBuffer>&, int, const NetAddr&)> RecvFromCallback1;
public:
	Socket(const shared_ptr<IOWorker>& _worker):worker(_worker){}
	virtual ~Socket(){}

	///�Ͽ�socket���ӣ�ֹͣsocket�ڲ��������ر�socket�����
	///UDP/TCP����ʹ�øýӿ��ͷ���Դ���ر�socket
	virtual bool disconnect() = 0;

	///�󶨴�����Ϣ
	///param[in]		addr		��Ҫ�󶨵Ķ˿�
	///param[in]		reusedAddr	�˿��Ƿ�������Ҫ�ظ���
	///return		true �ɹ���false ʧ�� 
	///ע����������ʹ�øú������жϴ����Ƿ�ռ�ã��˿��ж��ƽ�ʹ��host::checkPortIsNotUsed�ӿ�
	virtual bool bind(const NetAddr& addr,bool reusedAddr = true) = 0;
	
	///��ȡsocket��������С
	///param[in]		readSize		���Ļ�������С
	///param[in]		sendSize		���ͻ�������С
	///retun		 true �ɹ���false ʧ�� 
	virtual bool getSocketBuffer(uint32_t& recvSize,uint32_t& sendSize) const = 0;
	
	///����socket��������С
	///param[in]		readSize		���Ļ�������С
	///param[in]		sendSize		���ͻ�������С
	///retun		 true �ɹ���false ʧ�� 
	virtual bool setSocketBuffer(uint32_t recvSize,uint32_t sendSize) = 0;


	///���첽��������������
	///param[in]		callback		accept������socket֪ͨ�ص�������ΪNULL
	///retun		 true �ɹ���false ʧ�� 
	///ע��
	/// 1:ֻ��tcpserver��֧��
	///	2:�첽accept��accept�����Ľ��ͨ��callback����
	/// 3:��accept��������ͬʱʹ��
	virtual bool async_accept(const AcceptedCallback& callback,uint32_t timeoutms = INFINITE) = 0;


	///��ͬ����ͬ��accept����socket
	///param[in]		��
	///return		�����²�����socket���󣬵�NULLʱ��ʾʧ�ܣ�����ֵ��Ҫ�ⲿ�ͷ���Դ
	///ע��
	/// 1:ֻ��tcpserver��֧��
	///	2:��startListen����ͬʱʹ��
	///	3:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual shared_ptr<Socket> accept(uint32_t timeoutms = INFINITE) = 0;


	///���첽������TCP����
	///param[in]		addr			�������ĵ�ַ
	///param[in]		callback		connect�ɹ���Ļص�������Ϊ��
	///retun		 true �ɹ���false ʧ��  ����ֵΪ�첽Ͷ����Ϣ���
	///ע��
	/// 1:ֻ��tcpclient��֧��
	virtual bool async_connect(const NetAddr& addr,const ConnectedCallback& callback, uint32_t timeoutms = SOCKETDEFAULTTIMEOUT) = 0;

	///��ͬ����TCP����
	///param[in]		addr			�������ĵ�ַ
	///retun		 true �ɹ���false ʧ��  ����ֵΪ���ӵĽ��
	///ע��
	/// 1:ֻ��tcpclient��֧��
	virtual bool connect(const NetAddr& addr, uint32_t timeoutms = SOCKETDEFAULTTIMEOUT) = 0;

	///����TCP�Ͽ��ص�֪ͨ
	///param[in]		disconnected	�Ͽ��ص�֪ͨ
	///retun		 true �ɹ���false ʧ�� 
	///ע��
	/// 1:ֻ��TCP��֧��
	///ע�����첽IO֧��
	virtual bool setDisconnectCallback(const DisconnectedCallback& disconnected) = 0;

	///���첽��Ͷ��TCP���������¼�
	///param[in]		buf				���յ������ݴ洢��ַ
	///param[in]		len				���յ������ݴ洢�ռ����ֵ
	///param[in]		received		�ɹ����ܵ����ݺ�Ļص�������ΪNULL
	///retun		 true �ɹ���false ʧ�� ������Ͷ����Ϣ���
	///ע��
	///	1:ֻ�����ӳɹ����TCP��֧��
	///ע�����첽IO֧��
	virtual bool async_recv(char *buf , uint32_t len,const ReceivedCallback& received, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_recv(const shared_ptr<RecvBuffer>& buffer, const ReceivedCallback1& received, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_recv(const ReceivedCallback& received,int maxlen = 1024, uint32_t timeoutms = INFINITE) = 0;

	///��ͬ����TCP����
	///param[in]		buf				���յ������ݴ洢��ַ
	///param[in]		len				���յ������ݴ洢�ռ����ֵ
	///retun		 ����ֱ�ӽ��ܵ���Ϣ�ĳ���
	///ע��
	///	1:ֻ�����ӳɹ����TCP��֧��
	/// 2:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual int recv(char *buf , uint32_t len, uint32_t timeoutms = SOCKETDEFAULTTIMEOUT) = 0;

	///���첽��Ͷ��TCP���ݷ����¼�
	///param[in]		buf				�������ݻ����ַ���õ�ַ�ռ����ݷ��͹����в��ܱ��޸�ɾ����ֱ��sended���ú���ܲ���
	///param[in]		len				���������������ֵ
	///param[in]		sended			���ݷ��ͳɺ���첽֪ͨ������ΪNULL
	///retun		 true �ɹ���false ʧ�� ������Ͷ����Ϣ���
	///ע��
	///  1:ֻ�����ӳɹ����TCP��֧��
	///ע�����첽IO֧��
	virtual bool async_send(const char * buf, uint32_t len,const SendedCallback& sended, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_send(const shared_ptr<SendBuffer>& buffer, const SendedCallback1& sended, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_send(const std::vector<SBuf>& sendbuf, const SendedCallback2& sended, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_send(const std::vector<shared_ptr<SendBuffer>>& sendbuf, const SendedCallback3& sended, uint32_t timeoutms = INFINITE) = 0;

	///��ͬ����TCP����
	///param[in]		buf				�������ݻ����ַ
	///param[in]		len				���������������ֵ
	///retun		��������ֱ�ӷ��͵ĳ���
	///ע��
	///  1:ֻ�����ӳɹ����TCP��֧��
	///  2:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual int send(const char * buf, uint32_t len, uint32_t timeoutms = SOCKETDEFAULTTIMEOUT) = 0;


	///���첽��Ͷ��UDP���ݽ����¼� 
	///param[in]		buf				���յ������ݴ洢��ַ
	///param[in]		len				���յ������ݴ洢�ռ����ֵ
	///param[in]		received		�ɹ����ܵ����ݺ�Ļص�������ΪNULL
	///retun		 true �ɹ���false ʧ�� ������Ͷ���첽��Ϣ������
	///ע��
	///	1:ֻ��UDP������Bind���֧��/
	///	2:received����Ϊ��
	///ע�����첽IO֧��
	virtual bool async_recvfrom(char *buf , uint32_t len,const RecvFromCallback& received, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_recvfrom(const shared_ptr<RecvBuffer>& buffer, const RecvFromCallback1& received, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_recvfrom(const RecvFromCallback& received, int maxlen = 1024, uint32_t timeoutms = INFINITE) = 0;

	///��ͬ����UDP����
	///param[in]		buf				���յ������ݴ洢��ַ
	///param[in]		len				���յ������ݴ洢�ռ����ֵ
	///param[in]		other			���ݷ��͵�Ŀ��
	///retun		����ʵ�ʽ������ݵĳ���
	///ע��
	///	1:ֻ��UDP������Bind���֧��/
	///	2:received����Ϊ��
	/// 3:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual int recvfrom(char *buf , uint32_t len,NetAddr& other, uint32_t timeoutms = SOCKETDEFAULTTIMEOUT) = 0;

	///���첽��Ͷ��UDP���ݷ����¼�
	///param[in]		buf				�������ݻ����ַ���õ�ַ�ռ����ݷ��͹����в��ܱ��޸�ɾ����ֱ��sended���ú���ܲ���
	///param[in]		len				���������������ֵ
	///param[in]		other			���ݷ��͵�Ŀ��
	///param[in]		sended			���ݷ��ͳɺ���첽֪ͨ������ΪNULL
	///retun		����Ͷ�ݷ�������������
	///ע��
	///  1:ֻ��UDP��֧��
	///ע�����첽IO֧��
	virtual bool async_sendto(const char * buf, uint32_t len,const NetAddr& other,const SendedCallback& sended, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_sendto(const shared_ptr<SendBuffer>& buffer, const NetAddr& other, const SendedCallback1& sended, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_sendto(const std::vector<SBuf>& sendbuf, const NetAddr& other, const SendedCallback2& sended, uint32_t timeoutms = INFINITE) = 0;
	virtual bool async_sendto(const std::vector<shared_ptr<SendBuffer>>& sendbuf, const NetAddr& other, const SendedCallback3& sended, uint32_t timeoutms = INFINITE) = 0;

	///��ͬ����UDP����
	///param[in]		buf				�������ݻ����ַ
	///param[in]		len				���������������ֵ
	///param[in]		other			���ݷ��͵�Ŀ��
	///retun		����ʵ�ʷ������ݵĳ���
	///ע��
	/// 1:ֻ��UDP��֧��
	///	2:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual int sendto(const char * buf, uint32_t len,const NetAddr& other, uint32_t timeoutms = SOCKETDEFAULTTIMEOUT) = 0;

	
	//��ȥsocket����
	virtual InetType inet()const = 0;

	///��ȡSocket���
	///return ���	����socket����ʧ�� -1
	virtual SOCKET getHandle() const = 0;

	///��ȡSocket����״̬
	///param in		
	///return ����״̬��TCPServerĬ��NetStatus_notconnected��UDPĬ��NetStatus_connected
	virtual NetStatus getStatus() const = 0;

	///��ȡSocket��������
	///param in		
	///return ��������
	virtual NetType getNetType() const = 0;

	///��ȡSocket�����ĵ�ַ
	///param in		
	///return ����bind�ĵ�ַ��δbindΪ��
	virtual NetAddr getMyAddr() const = 0;
	
	///��ȡSocket�Է��ĵ�ַ
	///param in		
	///return TCPConnectionʹ��
	virtual NetAddr getOtherAddr() const = 0;


	//����socket����
	virtual bool setSocketOpt(int level, int optname, const void *optval, int optlen) = 0;

	//��ȡ����
	virtual bool getSocketOpt(int level, int optname, void *optval, int *optlen)const = 0;

	//socket׼������
	virtual void socketReady() = 0;

	//socket��������
	virtual void socketError(const std::string &errmsg) = 0;

	const shared_ptr<IOWorker>& ioWorker()const { return worker; }
protected:
	shared_ptr<IOWorker>	worker;
};

};
};




#endif //__NETWORK_SOCKET_H__
