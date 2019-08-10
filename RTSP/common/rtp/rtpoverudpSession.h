#pragma once
#include "rtpSession.h"
#include "../RTSPProtocol.h"
#include "rtpsort.h"
#include "RTSP/RTSPStructs.h"


class rtpOverUdpSession:public RTPSession
{
public:
	rtpOverUdpSession(bool _isserver, const shared_ptr<IOWorker>& ioworker,const std::string& _dstaddr, const shared_ptr<STREAM_TRANS_INFO>& _transport, const RTPSession::MediaDataCallback& _datacallback, const RTPSession::ContorlDataCallback& _contorlcallback)
		:RTPSession(_transport,_datacallback,_contorlcallback),isserver(_isserver), rtp_sendrtpsn(0), dstaddr(_dstaddr)
	{
		rtpsort = make_shared<RTPSort>(_transport,_datacallback);

		rtp_sock = UDP::create(ioworker);
		rtp_sock->bind(isserver ? transportinfo->transportinfo.rtp.u.server_port1 : transportinfo->transportinfo.rtp.u.client_port1);
		rtp_sock->setSocketBuffer(1024 * 1024 * 8, 0);
		rtp_sock->setSocketTimeout(1000, 1000);
		rtp_sock->nonBlocking(false);
		
		char* rtprecvbuffertmp = rtp_RecvBuffer.alloc(MAXRTPPACKGESIZE);
		rtp_sock->async_recvfrom(rtprecvbuffertmp, MAXRTPPACKGESIZE, Socket::RecvFromCallback(&rtpOverUdpSession::RTP_RecvCallback, this));

		rtcp_sock = UDP::create(ioworker);
		rtcp_sock->bind(isserver ? transportinfo->transportinfo.rtp.u.server_port2 : transportinfo->transportinfo.rtp.u.client_port2);
		rtcp_sock->setSocketBuffer(1024 * 1024, 0);
		rtcp_sock->setSocketTimeout(1000, 1000);
		rtcp_sock->nonBlocking(false);

		char* rtcprecvbuffertmp = rtcp_RecvBuffer.alloc(MAXRTPPACKGESIZE);
		rtcp_sock->async_recvfrom(rtcprecvbuffertmp, MAXRTPPACKGESIZE, Socket::RecvFromCallback(&rtpOverUdpSession::RTCP_RecvCallback, this));
	}
	~rtpOverUdpSession()
	{
		if(rtp_sock) rtp_sock->disconnect();
		if(rtcp_sock) rtcp_sock->disconnect();

		rtp_sock = NULL;
		rtcp_sock = NULL;
	}
	
	void sendContorlData(const shared_ptr<STREAM_TRANS_INFO>& transportinfo, const char*  buffer, uint32_t bufferlen)
	{
		Guard locker(mutex);

		String senddata = std::string(buffer, bufferlen);

		rtcp_sendlist.push_back(shared_ptr<SendPackgeInfo>(new SendPackgeInfo(senddata)));

		_sendAndCheckSend(false);
	}
	void sendMediaData(const shared_ptr<STREAM_TRANS_INFO>& transportinfo, const RTPPackage& rtppackge)
	{
		Guard locker(mutex);

		rtp_sendlist.push_back(shared_ptr<SendPackgeInfo>(new SendPackgeInfo(rtppackge)));

		_sendAndCheckSend(true);
	}	
	void _sendAndCheckSend(bool isdata, const char* buffer = NULL,size_t len = 0)
	{
		std::list<shared_ptr<SendPackgeInfo> >& sendlist = isdata ? rtp_sendlist : rtcp_sendlist;
		
		bool needSendData = false;
		//第一次需要发送数据
		if (buffer == NULL && sendlist.size() == 1)
		{
			needSendData = true;
		}
		else if (buffer)
		{
			if (len < 0) return;

			if (sendlist.size() <= 0) return;

			{
				shared_ptr<SendPackgeInfo> & item = sendlist.front();
				if (buffer != item->buffer)
				{
					assert(0);
				}
				sendlist.pop_front();
			}

			needSendData = true;
		}

		if (!needSendData) return;

		if (sendlist.size() <= 0) return;

		shared_ptr<SendPackgeInfo> & item = sendlist.front();

		//处理数据的零拷贝问题，添加前置数据		
		const char* sendbuffer = item->buffer;
		uint32_t sendbufferlen = (uint32_t)item->len;

		if (isdata)
		{
			shared_ptr<Socket> socktmp = rtp_sock;
			if (socktmp)
			{
				socktmp->async_sendto(sendbuffer, sendbufferlen,
					NetAddr(dstaddr, isserver ? transportinfo->transportinfo.rtp.u.client_port1 : transportinfo->transportinfo.rtp.u.server_port1),
					Socket::SendedCallback(&rtpOverUdpSession::RTP_SendCallback, this));
			}	
		}
		else
		{
			shared_ptr<Socket> socktmp = rtcp_sock;
			if (socktmp)
			{
				socktmp->async_sendto(sendbuffer, sendbufferlen,
					NetAddr(dstaddr, isserver ? transportinfo->transportinfo.rtp.u.client_port2 : transportinfo->transportinfo.rtp.u.server_port2),
					Socket::SendedCallback(&rtpOverUdpSession::RTCP_SendCallback, this));
			}	
		}
	}
	void RTP_SendCallback(const weak_ptr<Socket>& sock, const char* buffer, int len)
	{
		Guard locker(mutex);

		_sendAndCheckSend(true, buffer, len);
	}
	void RTCP_SendCallback(const weak_ptr<Socket>& sock, const char* buffer, int len)
	{
		Guard locker(mutex);

		_sendAndCheckSend(false, buffer, len);
	}
	void RTP_RecvCallback(const weak_ptr<Socket>& sock, const char* buffer, int len,const NetAddr& otearaddr)
	{
		if (buffer != rtp_RecvBuffer.c_str() || len <= 0 || len > MAXRTPPACKGESIZE) return;

		if ((size_t)len > sizeof(RTPHEADER))
		{
			rtp_RecvBuffer.resize(len);

			if (otearaddr.getPort() != (isserver ? transportinfo->transportinfo.rtp.u.client_port1 : transportinfo->transportinfo.rtp.u.server_port1))
			{
				//assert(0);
			}

			rtpsort->inputRtpData(rtp_RecvBuffer);
		}
		
		shared_ptr<Socket> socktmp = sock.lock();
		if (socktmp)
		{
			char* buffertmp = rtp_RecvBuffer.alloc(MAXRTPPACKGESIZE);
			socktmp->async_recvfrom(buffertmp, MAXRTPPACKGESIZE, Socket::RecvFromCallback(&rtpOverUdpSession::RTP_RecvCallback, this));
		}
	}
	void RTCP_RecvCallback(const weak_ptr<Socket>& sock, const char* buffer, int len, const NetAddr& otearaddr)
	{
		if (buffer != rtcp_RecvBuffer.c_str() || len <= 0 || len > MAXRTPPACKGESIZE) return;

		contorlcallback(transportinfo, buffer, len);

		shared_ptr<Socket> socktmp = sock.lock();
		if (socktmp)
		{
			socktmp->async_recvfrom((char*)rtcp_RecvBuffer.c_str(), MAXRTPPACKGESIZE, Socket::RecvFromCallback(&rtpOverUdpSession::RTCP_RecvCallback, this));
		}
	}
private:
	bool					 isserver;

	Mutex					 mutex;
	
	uint16_t				 rtp_sendrtpsn;

	shared_ptr<Socket>		 rtp_sock;
	shared_ptr<Socket>		 rtcp_sock;

	String					 rtp_RecvBuffer;
	String 					 rtcp_RecvBuffer;

	shared_ptr<RTPSort>		 rtpsort;

	std::list<shared_ptr<SendPackgeInfo> > rtp_sendlist;
	std::list<shared_ptr<SendPackgeInfo> > rtcp_sendlist;

	std::string				 dstaddr;
};