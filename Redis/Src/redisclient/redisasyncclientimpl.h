
#ifndef REDISCLIENT_REDISCLIENTIMPL_H
#define REDISCLIENT_REDISCLIENTIMPL_H


#include "Base/Base.h"
#include "Network/Network.h"
using namespace Public::Base;
using namespace Public::Network;

#include "redisvalue.h"
#include "redisprotocol.h"

namespace redisclient{

typedef Function<void> ConnectCallback;
typedef Function<void> DisconnectCallback;
typedef Function<void, const RedisValue&> CmdCallback;

typedef CmdCallback SubcribeHandler;

class RedisAsyncClientImpl :public RedisParser
{
public:
	RedisAsyncClientImpl(const shared_ptr<IOWorker>& worker);
	virtual ~RedisAsyncClientImpl();

	void asyncConnect(const NetAddr& addr, const ConnectCallback& callback,const DisconnectCallback& discallback);

	void* subscribe(const std::string &command, const std::string &channel, const SubcribeHandler&  subhandler, const CmdCallback& callback);

	void unsubscribe(const std::string &command, const std::string &channel, void* subid);

	void close();

	void doAsyncCommand(const std::string& cmdstr, const CmdCallback&  handler);

	bool isConnected() const;
private:
	void _socketSendCmd();
	void socketConnectCallback(const weak_ptr<Socket>& sock, bool status, const std::string& errmsg);
	void socketDisconnectCallback(const weak_ptr<Socket>& sock, const std::string&);
	void socketSendCallback(const weak_ptr<Socket>&s, const char* tmp, int len);
	void socketRecvCallback(const weak_ptr<Socket>&s, const char* tmp, int len);
	void doProcessMessage(const shared_ptr<RedisValue>& v);
private:
	struct CmdInfo
	{
		std::string		cmd;
		CmdCallback		handler;
	};

	struct SubcribeInfo
	{
		SubcribeHandler		handler;
	};

	Mutex																mutex;
	shared_ptr<IOWorker>												worker;
	shared_ptr<Socket>													sock;
	ConnectCallback														connectCallback;
	DisconnectCallback													disconnectCallback;
	std::list<shared_ptr<CmdInfo> >										cmdSendList;
	std::list<shared_ptr<CmdInfo> >										cmdWaitRecvList;
	std::map<std::string, std::map<void*, shared_ptr<SubcribeInfo> > >  sublist;
};
}

#endif // REDISCLIENT_REDISCLIENTIMPL_H
