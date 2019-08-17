#pragma  once
#include "protocol/CmdDiscovery.h"
#include "Network/Network.h"
#include "OnvifClient/OnvifClient.h"
using namespace Public::Network;

#define DISCONVERYPORT		3702
#define DISCONVERADDR		"239.255.255.250"

#define IP_ADD_MEMBERSHIP_V2	12

class OnvifDisconvery
{
public:
	OnvifDisconvery(){}
	~OnvifDisconvery(){}
	bool start(const shared_ptr<IOWorker>& _worker, const OnvifClientManager::DisconveryCallback& _callback)
	{
		ioworker = _worker;
		callback = _callback;

		muludp = UDP::create(ioworker);
		muludp->bind(DISCONVERYPORT);
		
		{
			/* ����Ҫ�����鲥�ĵ�ַ */
			memset(&mreq,0, sizeof(struct ip_mreq));

			/* �������ַ */
			mreq.imr_multiaddr.s_addr = inet_addr(DISCONVERADDR);
			/* ���÷����鲥��Ϣ��Դ�����ĵ�ַ��Ϣ */
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);

			/* �ѱ��������鲥��ַ��������������Ϊ�鲥��Ա��ֻ�м���������յ��鲥��Ϣ */
			if (!muludp->setSocketOpt(IPPROTO_IP, IP_ADD_MEMBERSHIP_V2, (const char*)&mreq, sizeof(struct ip_mreq)))
			{
				return false;
			}
			int loop = 0;
			if (!muludp->setSocketOpt(IPPROTO_IP, IP_MULTICAST_LOOP, (const char*)&loop, sizeof(loop)))
			{
				return false;
			}
		}

		muludp->async_recvfrom(Socket::RecvFromCallback(&OnvifDisconvery::onSocketMULRecvCallback, this), 1024 * 10);


		return true;
	}
	bool stop()
	{
		muludp = NULL;

		callback(shared_ptr<OnvifClientDefs::DiscoveryInfo>());

		return true;
	}
	bool sendSearch()
	{
		NetAddr toaddr(DISCONVERADDR, DISCONVERYPORT);

		CmdDiscovery disconvery;
		disconvery.initGSopProtocol();

		std::string protocol = disconvery.build(URL());

		shared_ptr<Socket> sock = muludp;
		if (!sock) return false;

		sock->sendto(protocol.c_str(), protocol.length(), toaddr);

		return true;
	}
private:
	void onSocketMULRecvCallback(const weak_ptr<Socket>& , const char* buffer, int len,const NetAddr& addr)
	{
		if (len > 0)
		{
			shared_ptr<OnvifClientDefs::DiscoveryInfo> info = parseDiscoverMessage(buffer, len);

			if (info) callback(info);
		}

		muludp->async_recvfrom(Socket::RecvFromCallback(&OnvifDisconvery::onSocketMULRecvCallback, this), 1024 * 10);
	}
	shared_ptr<OnvifClientDefs::DiscoveryInfo> parseDiscoverMessage(const char* buffer, int len)
	{
		CmdDiscovery disconvery;
		if(!disconvery.parseGSopProtocol(std::string(buffer, len))) return shared_ptr<OnvifClientDefs::DiscoveryInfo>();

		if(!disconvery.parseProtocol()) return shared_ptr<OnvifClientDefs::DiscoveryInfo>();

		return disconvery.info;
	}
private:
	shared_ptr<IOWorker>	ioworker;
	OnvifClientManager::DisconveryCallback callback;
	shared_ptr<Socket>		muludp;
	
	struct ip_mreq mreq;
};