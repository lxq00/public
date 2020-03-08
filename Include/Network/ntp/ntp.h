#pragma once
#include "Network/Socket/Socket.h"

namespace Public {
namespace Network {
namespace NTP {

class NETWORK_API Server
{
public:
	typedef Function<Time()> GetTimeCallback;
public:
	Server();
	virtual ~Server();

	//���û�ȡʱ��ص�����������ã�����ȡ����ʱ��
	ErrorInfo start(const shared_ptr<IOWorker>& worker,uint32_t port = 123,const GetTimeCallback& getcallback = GetTimeCallback());
	ErrorInfo stop();
private:
	struct ServerInternal;
	ServerInternal* internal;
};

class NETWORK_API Client
{
public:
	typedef Function<void(const Time&)> SetTimeCallback;
	typedef Function<void(const ErrorInfo&)> FinishCallback;
public:
	Client();
	virtual ~Client();

	//���������SetTimeCallback�������ñ���ʱ��
	void asyncUpdate(const shared_ptr<IOWorker>& worker,const std::string& serverip, uint32_t port, const FinishCallback& finishcallback, uint32_t timeout = 5000, const SetTimeCallback& setcallback = SetTimeCallback());
	ErrorInfo update(const shared_ptr<IOWorker>& worker, const std::string& serverip, uint32_t port,uint32_t timeout = 5000, const SetTimeCallback& setcallback = SetTimeCallback());
private:
	struct ClientInternal;
	ClientInternal* internal;
};


}
}
}