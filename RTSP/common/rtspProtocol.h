#pragma once
#include "HTTP/HTTPParse.h"
#include "rtp/rtpSession.h"
using namespace Public::HTTP;
using namespace Public::RTSP;

#define MAXPARSERTSPBUFFERLEN	1024*1024
#define RTPOVERTCPMAGIC		'$'
#define RTSPCMDFLAG			"RTSP/1.0"

#define RTSPCONENTTYPESDP	"application/sdp"

class RTSPProtocol:public HTTPParse
{
public:
	struct TCPInterleaved
	{
		int dataChannel;
		int contrlChannel;
	};
public:
	typedef Function1<void, const shared_ptr<RTSPCommandInfo>&> CommandCallback;
	typedef Function0<void> DisconnectCallback;

public:
	RTSPProtocol(const shared_ptr<Socket>& sock, const CommandCallback& cmdcallback, const DisconnectCallback& disconnectCallback,bool server)
		:HTTPParse(server),m_recvBuffer(MAXPARSERTSPBUFFERLEN), m_sendBuffer(MAXPARSERTSPBUFFERLEN),m_isSending(false)
	{
		m_sock = sock;
		m_cmdcallback = cmdcallback;
		
		m_disconnect = disconnectCallback;
		m_prevalivetime = Time::getCurrentMilliSecond();
		m_bodylen = 0;
		m_haveFindHeaderStart = false;

		m_sock->setSocketBuffer(1024 * 1024 * 4, 1024 * 1024 * 4);
		m_sock->setDisconnectCallback(Socket::DisconnectedCallback(&RTSPProtocol::onSocketDisconnectCallback, this));
		m_sock->async_recv(m_recvBuffer.getProductionAddr(), m_recvBuffer.getProductionLength(), Socket::ReceivedCallback(&RTSPProtocol::onSocketRecvCallback, this));
	}
	~RTSPProtocol()
	{
		m_sock->disconnect();
		m_sock = NULL;
	}

	uint64_t prevalivetime() const { return m_prevalivetime; }
	uint32_t sendListSize()
	{
		return m_sendBuffer.dataLenght();
	}

	void sendProtocolData(const std::string& cmdstr)
	{
		if (cmdstr.length() == 0) return;

		logdebug("\r\n%s",cmdstr.c_str());

		Guard locker(m_mutex);
		if (MAXPARSERTSPBUFFERLEN - m_sendBuffer.dataLenght() < cmdstr.length())
		{
			assert(0);
		}

		m_sendBuffer.production(cmdstr);

		_checkSendData();
	}
	void sendMedia(const shared_ptr<STREAM_TRANS_INFO>& mediainfo, const RTPHEADER& rtpheader, const StringBuffer& buffer)
	{
		INTERLEAVEDFRAME frame;
		frame.magic = RTPOVERTCPMAGIC;
		frame.channel = mediainfo->transportinfo.rtp.t.dataChannel;
		frame.rtp_len = htons((uint16_t)(buffer.length() + sizeof(RTPHEADER)));


		Guard locker(m_mutex);
		if (MAXPARSERTSPBUFFERLEN - m_sendBuffer.dataLenght() < sizeof(INTERLEAVEDFRAME) + sizeof(RTPHEADER) + buffer.length())
		{
			assert(0);
		}
		m_sendBuffer.production((const char*)& frame, sizeof(INTERLEAVEDFRAME));
		m_sendBuffer.production((const char*)& rtpheader, sizeof(RTPHEADER));
		m_sendBuffer.production(buffer);

		_checkSendData();
	}
	void sendContrlData(const shared_ptr<STREAM_TRANS_INFO>& mediainfo, const char* buffer, uint32_t bufferlen)
	{
		INTERLEAVEDFRAME frame;
		frame.magic = RTPOVERTCPMAGIC;
		frame.channel = mediainfo->transportinfo.rtp.t.contorlChannel;
		frame.rtp_len = htons(bufferlen);


		Guard locker(m_mutex);
		if (MAXPARSERTSPBUFFERLEN - m_sendBuffer.dataLenght() < sizeof(INTERLEAVEDFRAME)  + bufferlen)
		{
			assert(0);
		}
		m_sendBuffer.production((const char*)& frame, sizeof(INTERLEAVEDFRAME));
		m_sendBuffer.production(buffer, bufferlen);

		_checkSendData();
	}
	void setMediaTransportInfo(const shared_ptr<MEDIA_INFO>& _rtspmedia)
	{
		m_rtspmedia = _rtspmedia;
	}
private:
	void _checkSendData()
	{
		if (m_isSending || m_sendBuffer.dataLenght() == 0) return;


		const char* bufferaddr = m_sendBuffer.getConsumeAddr();
		uint32_t bufferlen = m_sendBuffer.getConsumeLength();


		m_isSending = true;

		m_prevalivetime = Time::getCurrentMilliSecond();

		m_sock->async_send(bufferaddr, bufferlen, Socket::SendedCallback(&RTSPProtocol::onSocketSendCallback, this));
	}
	void onSocketSendCallback(const weak_ptr<Socket>& sock, const char* buffer, int len)
	{
		if (len <= 0) return;

		Guard locker(m_mutex);

		if (buffer != m_sendBuffer.getConsumeAddr() || !m_sendBuffer.setConsumeLength(len))
		{
			assert(0);
		}
		m_isSending = false;

		_checkSendData();
	}
	void onSocketDisconnectCallback(const weak_ptr<Socket>&, const std::string&)
	{
		m_disconnect();
	}
	void onSocketRecvCallback(const weak_ptr<Socket>& sock, const char* buffer, int len)
	{
		m_prevalivetime = Time::getCurrentMilliSecond();
		if (len <= 0)
		{
			assert(0);
			return;
		}
		
		if (buffer != m_recvBuffer.getProductionAddr())
		{
			assert(0);
		}

		if ((uint32_t)len > m_recvBuffer.getProductionLength())
		{
			assert(0);
		}

		m_recvBuffer.setProductionLength(len);

		while (m_recvBuffer.dataLenght() > 0)
		{
			if (m_cmdinfo != NULL)
			{
				if (m_bodylen > m_cmdinfo->body.length())
				{
					//���ݲ���
					if (m_recvBuffer.dataLenght() < m_bodylen - m_cmdinfo->body.length()) break;

					uint32_t needlen = m_bodylen - (uint32_t)m_cmdinfo->body.length();

					StringBuffer buffer = m_recvBuffer.consumeBuffer(0, needlen);

					m_cmdinfo->body += buffer.read();
				}
				
				{
					m_cmdcallback(m_cmdinfo);
					m_cmdinfo = NULL;
					m_bodylen = 0;
					m_haveFindHeaderStart = false;
				}
				
			}
			else if (m_cmdinfo == NULL && m_haveFindHeaderStart)
			{
				std::string usedstr;
				shared_ptr<HTTPHeader> header = HTTPParse::parse(m_recvBuffer,&usedstr);
								
				if (header != NULL)
				{
					logdebug("\r\n%s", usedstr.c_str());

					m_cmdinfo = make_shared<RTSPCommandInfo>();
					m_cmdinfo->method = header->method;
					m_cmdinfo->url = header->url;
					m_cmdinfo->verinfo.protocol = header->verinfo.protocol;
					m_cmdinfo->verinfo.version = header->verinfo.version;
					m_cmdinfo->statuscode = header->statuscode;
					m_cmdinfo->statusmsg = header->statusmsg;
					m_cmdinfo->headers = std::move(header->headers);

					m_cmdinfo->cseq = m_cmdinfo->header("CSeq").readInt();

					m_bodylen = m_cmdinfo->header(Content_Length).readInt();

					if (m_bodylen == 0)
					{
						m_cmdcallback(m_cmdinfo);
						m_cmdinfo = NULL;
						m_bodylen = 0;
						m_haveFindHeaderStart = false;
					}
				}
				//û������˵�����ݲ���
				else
				{
					break;
				}
			}
			else if (m_recvBuffer.readChar(0) == RTPOVERTCPMAGIC)
			{
				if (m_recvBuffer.dataLenght() < sizeof(INTERLEAVEDFRAME)) break;

				INTERLEAVEDFRAME frame;
				if(!m_recvBuffer.readBuffer(0,&frame,sizeof(INTERLEAVEDFRAME))) break;

				uint32_t datalen = ntohs(frame.rtp_len);
				
				if (datalen <= 0 || datalen >= 0xffff)
				{
					//��������
					m_recvBuffer.setConsumeLength(1);
					continue;
				}

				if (datalen + sizeof(INTERLEAVEDFRAME) > m_recvBuffer.dataLenght())
				{
					break;
				}

				if (m_rtspmedia)
				{
					for (std::list<shared_ptr<STREAM_TRANS_INFO> >::iterator iter = m_rtspmedia->infos.begin(); iter != m_rtspmedia->infos.end(); iter++)
					{
						shared_ptr<STREAM_TRANS_INFO> transportinfo = *iter;
						if(transportinfo->transportinfo.transport != TRANSPORT_INFO::TRANSPORT_RTP_TCP) continue;
						shared_ptr<RTPSession> rtpsession = transportinfo->rtpsession;
						if(rtpsession == NULL) continue;

						if (frame.channel == transportinfo->transportinfo.rtp.t.dataChannel)
						{
							RTPHEADER rtpheader;
							if (!m_recvBuffer.readBuffer(sizeof(INTERLEAVEDFRAME), &rtpheader, sizeof(RTPHEADER))) break;
							
							if (rtpheader.v != RTP_VERSION)
							{
								assert(0);
								break;
							}
							StringBuffer buffer = m_recvBuffer.readBuffer(sizeof(INTERLEAVEDFRAME) + sizeof(RTPHEADER), datalen - sizeof(RTPHEADER));

							rtpsession->rtpovertcpMediaCallback(transportinfo,rtpheader, buffer);
							break;
						}
						else if (frame.channel == transportinfo->transportinfo.rtp.t.contorlChannel)
						{
							StringBuffer buffer = m_recvBuffer.readBuffer(sizeof(INTERLEAVEDFRAME), datalen);
							
							std::string bufferstr = buffer.read();

							rtpsession->rtpovertcpContorlCallback(transportinfo, bufferstr.c_str(), (uint32_t)bufferstr.length());
							break;
						}
					}
				}
				m_recvBuffer.setConsumeLength(datalen + sizeof(INTERLEAVEDFRAME));
			}
			else if(!m_haveFindHeaderStart)
			{
				uint32_t ret = checkIsRTSPCommandStart(m_recvBuffer);
				if (ret == 0)
				{
					m_recvBuffer.setConsumeLength(1);
					continue;
				}
				else if (ret == 1)
				{
					break;
				}
				else
				{
					m_haveFindHeaderStart = true;
				}
			}
			else
			{
				assert(0);
			}
		}
				
		shared_ptr<Socket> socktmp = sock.lock();
		if (socktmp != NULL)
		{
			socktmp->async_recv(m_recvBuffer.getProductionAddr(), m_recvBuffer.getProductionLength(),Socket::ReceivedCallback(&RTSPProtocol::onSocketRecvCallback, this));
		}
	}
	// return 0 not cmonnand,return 1 not sure need wait,return 2 is command
	uint32_t checkIsRTSPCommandStart(CircleBuffer& buffer)
	{
		static std::string rtspcmdflag = "rtsp";

		uint32_t datalen = buffer.dataLenght();
		uint32_t readpos = 0;

		char readbufferarray[5] = { 0, };
		while (readpos < datalen)
		{
			char ch = buffer.readChar(readpos);

			//b���ǿ���ʵ�ַ�������
			if (!isCanShowChar(ch)) return 0;

			readbufferarray[0] = readbufferarray[1];
			readbufferarray[1] = readbufferarray[2];
			readbufferarray[2] = readbufferarray[3];
			readbufferarray[3] = ch;
			readbufferarray[4] = 0;

			readpos++;

			if (readpos >= 4)
			{
				if (strncasecmp(readbufferarray, rtspcmdflag.c_str(), 4) == 0) return 2;
			}			
		}
		return 1;
	}
	//�ж��Ƿ�����ʾ�ַ�
	bool isCanShowChar(char ch)
	{
		return ((ch >= 0x20 && ch < 0x7f) || ch == '\r' || ch == '\n');
	}
public:
	shared_ptr<Socket>			m_sock;
private:
	CommandCallback				m_cmdcallback;
	DisconnectCallback			m_disconnect;

	CircleBuffer				m_recvBuffer;
	CircleBuffer				m_sendBuffer;
	bool						m_isSending;
	Mutex						m_mutex;

	uint64_t					m_prevalivetime;


	bool						m_haveFindHeaderStart;
	shared_ptr<RTSPCommandInfo>	m_cmdinfo;
	uint32_t					m_bodylen;
	
	shared_ptr<MEDIA_INFO>		m_rtspmedia;
};


