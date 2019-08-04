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
#include "Network/NetAddr.h"

using namespace Public::Base;

namespace Public{
namespace Network{

#define INVALIDHANDLE -1

///socket������Ҫ��worker���ϣ���Ҫ���ⶨ�嵱ǰsocket������Ҫ���߳�����Ϣ
//�ڲ�Ϊboost::asio::io_work����Ϊboost::asio�ṩio_server
class NETWORK_API IOWorker
{
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
	IOWorker(const ThreadNum& num);
	~IOWorker();

	static shared_ptr<IOWorker> defaultWorker();

	//����ֵΪ boost::asio::io_service*
	void* getBoostASIOIOServerPtr() const;

	uint32_t	threadNum();
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
};

class NETWORK_API Socket
{
public:
	/// socket����ʱ��accept�¼��ص���������һ��������ʾ����socket�����ڶ�����ʾ�¹����socket.��Ҫ�ⲿ�ͷ�
	/// �ص�����ο���void acceptCallbackFunc(Socket* oldSock,Socket* newSock);
	typedef Function2<void, const weak_ptr<Socket>& /*oldSock*/, const shared_ptr<Socket>& /*newSock*/> AcceptedCallback;

	/// socket�첽�����¼��ص�����,��һ��������ʾ����socket����
	///�ص�����ο���void connectCallbackFunc(Socket* connectSock);
	typedef Function3<void, const weak_ptr<Socket>& /*connectSock*/,bool,const std::string&> ConnectedCallback;

	/// socket�첽�Ͽ��¼��ص�����,int ��һ��������ʾ����socket�����ڶ�����ʾ�����Ͽ������������ͻ��ˣ�����˾�ʹ�ø��¼�
	/// ��socket��һ�����ӳɹ������10�������ݣ��������Ͽ����ӣ����󣬷�ֹ tcp�������ӣ�ֻ���accept������socket
	///�ص�����ο���void disconnectCallbackFunc(Socket* connectionSock,const char* disconectErrorInfo);
	typedef Function2<void, const weak_ptr<Socket>& /*connectSock*/,const std::string&> DisconnectedCallback;

	// socket����received�¼��ص���������һ��������ʾ����socket�����ڶ�����ʾʵ�ʽ��յ�socket���ݳ���.
	///�ص�����ο���void recvCallbackFunc(Socket* sock,const char* recvBuffer,int recvlen);
	typedef Function3<void, const weak_ptr<Socket>& /*sock*/,const char*, int> ReceivedCallback;

	// socket����send�¼��ص���������һ��������ʾ����socket�����ڶ�����ʾʵ�ʷ��͵�socket���ݳ���.
	///�ص�����ο���void sendCallbackFunc(Socket* sock,const char* sendBuffer,int sendlen);
	typedef Function3<void, const weak_ptr<Socket>& /*sock*/,const char*, int> SendedCallback;
	
	/// socket����udp��recvfrom�¼��ص���������һ��������ʾsocket�����ڶ�����������ʾ���յ�������Ϣ��ַ�ͳ��ȣ����ĸ�������ʾ���ݷ��ͷ�ip
	///�ص�����ο���void recvfromCallbackFunc(Socket* sock,const char* recvBuffer, int recvlen ,const NetAddr& otheraddr);
	typedef Function4<void, const weak_ptr<Socket>& /*sock*/,const char*, int,const NetAddr&> RecvFromCallback;
public:
	Socket(){}
	virtual ~Socket(){}

	///�Ͽ�socket���ӣ�ֹͣsocket�ڲ��������ر�socket�����
	///UDP/TCP����ʹ�øýӿ��ͷ���Դ���ر�socket
	virtual bool disconnect(){return false;}

	///�󶨴�����Ϣ
	///param[in]		addr		��Ҫ�󶨵Ķ˿�
	///param[in]		reusedAddr	�˿��Ƿ�������Ҫ�ظ���
	///return		true �ɹ���false ʧ�� 
	///ע����������ʹ�øú������жϴ����Ƿ�ռ�ã��˿��ж��ƽ�ʹ��host::checkPortIsNotUsed�ӿ�
	virtual bool bind(const NetAddr& addr,bool reusedAddr = true){return false;}
	
	///��ȡsocket��������С
	///param[in]		readSize		���Ļ�������С
	///param[in]		sendSize		���ͻ�������С
	///retun		 true �ɹ���false ʧ�� 
	virtual bool getSocketBuffer(uint32_t& recvSize,uint32_t& sendSize) const{return false;}
	
	///����socket��������С
	///param[in]		readSize		���Ļ�������С
	///param[in]		sendSize		���ͻ�������С
	///retun		 true �ɹ���false ʧ�� 
	virtual bool setSocketBuffer(uint32_t recvSize,uint32_t sendSize){return false;}

	///��ȡsocket���ͽ��ܳ�ʱʱ��
	///param[in]		recvTimeout		���ճ�ʱ ��λ������
	///param[in]		sendTimeout		���ͳ�ʱ ��λ������
	///retun		 true �ɹ���false ʧ�� 
	virtual bool getSocketTimeout(uint32_t& recvTimeout,uint32_t& sendTimeout) const{return false;}

	///����socket���ͽ��ܳ�ʱʱ��
	///param[in]		recvTimeout		���ճ�ʱ ��λ������
	///param[in]		sendTimeout		���ͳ�ʱ ��λ������
	///retun		 true �ɹ���false ʧ�� 
	virtual bool setSocketTimeout(uint32_t recvTimeout,uint32_t sendTimeout) {return false;}


	///����socket�������Ƕ���ģʽ
	///param[in]		nonblock		true ����ģʽ  false �Ƕ���ģʽ
	///return		true �ɹ���false ʧ�� 
	virtual bool nonBlocking(bool nonblock){return false;}


	///���첽��������������
	///param[in]		callback		accept������socket֪ͨ�ص�������ΪNULL
	///retun		 true �ɹ���false ʧ�� 
	///ע��
	/// 1:ֻ��tcpserver��֧��
	///	2:�첽accept��accept�����Ľ��ͨ��callback����
	/// 3:��accept��������ͬʱʹ��
	virtual bool async_accept(const AcceptedCallback& callback){return false;}


	///��ͬ����ͬ��accept����socket
	///param[in]		��
	///return		�����²�����socket���󣬵�NULLʱ��ʾʧ�ܣ�����ֵ��Ҫ�ⲿ�ͷ���Դ
	///ע��
	/// 1:ֻ��tcpserver��֧��
	///	2:��startListen����ͬʱʹ��
	///	3:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual shared_ptr<Socket> accept(){return NULL;}


	///���첽������TCP����
	///param[in]		addr			�������ĵ�ַ
	///param[in]		callback		connect�ɹ���Ļص�������Ϊ��
	///retun		 true �ɹ���false ʧ��  ����ֵΪ�첽Ͷ����Ϣ���
	///ע��
	/// 1:ֻ��tcpclient��֧��
	virtual bool async_connect(const NetAddr& addr,const ConnectedCallback& callback){return false;}

	///��ͬ����TCP����
	///param[in]		addr			�������ĵ�ַ
	///retun		 true �ɹ���false ʧ��  ����ֵΪ���ӵĽ��
	///ע��
	/// 1:ֻ��tcpclient��֧��
	virtual bool connect(const NetAddr& addr){return false;}

	///����TCP�Ͽ��ص�֪ͨ
	///param[in]		disconnected	�Ͽ��ص�֪ͨ
	///retun		 true �ɹ���false ʧ�� 
	///ע��
	/// 1:ֻ��TCP��֧��
	///ע�����첽IO֧��
	virtual bool setDisconnectCallback(const DisconnectedCallback& disconnected){return false;}

	///���첽��Ͷ��TCP���������¼�
	///param[in]		buf				���յ������ݴ洢��ַ
	///param[in]		len				���յ������ݴ洢�ռ����ֵ
	///param[in]		received		�ɹ����ܵ����ݺ�Ļص�������ΪNULL
	///retun		 true �ɹ���false ʧ�� ������Ͷ����Ϣ���
	///ע��
	///	1:ֻ�����ӳɹ����TCP��֧��
	///ע�����첽IO֧��
	virtual bool async_recv(char *buf , uint32_t len,const ReceivedCallback& received){return false;}
	virtual bool async_recv(const ReceivedCallback& received,int maxlen = 1024) { return false; }

	///��ͬ����TCP����
	///param[in]		buf				���յ������ݴ洢��ַ
	///param[in]		len				���յ������ݴ洢�ռ����ֵ
	///retun		 ����ֱ�ӽ��ܵ���Ϣ�ĳ���
	///ע��
	///	1:ֻ�����ӳɹ����TCP��֧��
	/// 2:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual int recv(char *buf , uint32_t len){return false;}

	///���첽��Ͷ��TCP���ݷ����¼�
	///param[in]		buf				�������ݻ����ַ���õ�ַ�ռ����ݷ��͹����в��ܱ��޸�ɾ����ֱ��sended���ú���ܲ���
	///param[in]		len				���������������ֵ
	///param[in]		sended			���ݷ��ͳɺ���첽֪ͨ������ΪNULL
	///retun		 true �ɹ���false ʧ�� ������Ͷ����Ϣ���
	///ע��
	///  1:ֻ�����ӳɹ����TCP��֧��
	///ע�����첽IO֧��
	virtual bool async_send(const char * buf, uint32_t len,const SendedCallback& sended){return false;}

	///��ͬ����TCP����
	///param[in]		buf				�������ݻ����ַ
	///param[in]		len				���������������ֵ
	///retun		��������ֱ�ӷ��͵ĳ���
	///ע��
	///  1:ֻ�����ӳɹ����TCP��֧��
	///  2:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual int send(const char * buf, uint32_t len){return -1;}


	///���첽��Ͷ��UDP���ݽ����¼� 
	///param[in]		buf				���յ������ݴ洢��ַ
	///param[in]		len				���յ������ݴ洢�ռ����ֵ
	///param[in]		received		�ɹ����ܵ����ݺ�Ļص�������ΪNULL
	///retun		 true �ɹ���false ʧ�� ������Ͷ���첽��Ϣ������
	///ע��
	///	1:ֻ��UDP������Bind���֧��/
	///	2:received����Ϊ��
	///ע�����첽IO֧��
	virtual bool async_recvfrom(char *buf , uint32_t len,const RecvFromCallback& received){return false;}
	virtual bool async_recvfrom(const RecvFromCallback& received, int maxlen = 1024) { return false; }

	///��ͬ����UDP����
	///param[in]		buf				���յ������ݴ洢��ַ
	///param[in]		len				���յ������ݴ洢�ռ����ֵ
	///param[in]		other			���ݷ��͵�Ŀ��
	///retun		����ʵ�ʽ������ݵĳ���
	///ע��
	///	1:ֻ��UDP������Bind���֧��/
	///	2:received����Ϊ��
	/// 3:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual int recvfrom(char *buf , uint32_t len,NetAddr& other){return false;}

	///���첽��Ͷ��UDP���ݷ����¼�
	///param[in]		buf				�������ݻ����ַ���õ�ַ�ռ����ݷ��͹����в��ܱ��޸�ɾ����ֱ��sended���ú���ܲ���
	///param[in]		len				���������������ֵ
	///param[in]		other			���ݷ��͵�Ŀ��
	///param[in]		sended			���ݷ��ͳɺ���첽֪ͨ������ΪNULL
	///retun		����Ͷ�ݷ�������������
	///ע��
	///  1:ֻ��UDP��֧��
	///ע�����첽IO֧��
	virtual bool async_sendto(const char * buf, uint32_t len,const NetAddr& other,const SendedCallback& sended){return false;}

	///��ͬ����UDP����
	///param[in]		buf				�������ݻ����ַ
	///param[in]		len				���������������ֵ
	///param[in]		other			���ݷ��͵�Ŀ��
	///retun		����ʵ�ʷ������ݵĳ���
	///ע��
	/// 1:ֻ��UDP��֧��
	///	2:�ýӿڵĵ���ʱ���setSocketTimeout/nonBlocking������������
	virtual int sendto(const char * buf, uint32_t len,const NetAddr& other){return -1;}

	
	///��ȡSocket���
	///return ���	����socket����ʧ�� -1
	virtual SOCKET getHandle() const {return INVALIDHANDLE;}

	///��ȡSocket����״̬
	///param in		
	///return ����״̬��TCPServerĬ��NetStatus_notconnected��UDPĬ��NetStatus_connected
	virtual NetStatus getStatus() const{return NetStatus_error;}

	///��ȡSocket��������
	///param in		
	///return ��������
	virtual NetType getNetType() const{return NetType_Udp;}

	///��ȡSocket����ĵ�ַ
	///param in		
	///return ����bind�ĵ�ַ��δbindΪ��
	virtual NetAddr getMyAddr() const{return NetAddr();}
	
	///��ȡSocket�Է��ĵ�ַ
	///param in		
	///return TCPConnectionʹ��
	virtual NetAddr getOtherAddr() const{return NetAddr();}


	//����socket����
	virtual bool setSocketOpt(int level, int optname, const void *optval, int optlen) = 0;

	//socket׼������
	virtual void socketReady() {}

	//socket��������
	virtual void socketError(const std::string &errmsg) {}
};

};
};




#endif //__NETWORK_SOCKET_H__

