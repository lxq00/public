#pragma once
#include "Base/Base.h"
#include "RTSP/Defs.h"
#include "Network/Network.h"
using namespace Public::Base;
using namespace Public::Network;

namespace Public {
namespace RTSP {
// Transport: RTP/AVP/TCP;interleaved=0-1
// Transport: RTP/AVP;unicast;client_port=4588-4589;server_port=6256-6257
// Transport: RTP/AVP;multicast;destination=224.2.0.1;port=3456-3457;ttl=16
struct RTSP_API TRANSPORT_INFO
{
	TRANSPORT_INFO() :transport(TRANSPORT_NONE), /*multicast(TRANSPORT_INFO::MULTICAST_UNICAST),*/ ssrc(0)
	{
		rtp.t.dataChannel = 0;
		rtp.t.contorlChannel = 0;
	}

	enum {
		TRANSPORT_NONE = 0,
		TRANSPORT_RTP_UDP = 1,
		TRANSPORT_RTP_TCP,
		TRANSPORT_RAW,
	} transport; // RTSP_TRANSPORT_xxx

	//enum {
	//	MULTICAST_NONE = 0,
	//	MULTICAST_UNICAST = 1,
	//	MULTICAST_MULTICAST,
	//} multicast; // 0-unicast/1-multicast, default multicast

	int ssrc; // RTP only(synchronization source (SSRC) identifier) 4-bytes
	
	union rtsp_header_transport_rtp_u
	{
		//struct rtsp_header_transport_multicast_t
		//{
		//	int ttl; // multicast only
		//	unsigned short port1, port2; // multicast only
		//} m;

		struct rtsp_header_transport_unicast_t
		{
			unsigned short client_port1, client_port2; // unicast RTP/RTCP port pair, RTP only
			unsigned short server_port1, server_port2; // unicast RTP/RTCP port pair, RTP only
		} u;

		struct rtsp_header_transport_tcp_t
		{
			int		dataChannel;
			int		contorlChannel;
		}t;
	} rtp;
};


typedef struct {
	int				frameRate;				//֡��
	int				width = 0;				//��
	int				height = 0;				//��
}RTSP_SDKVideo;

typedef struct {
	int				sampleBits = 0;		/// ����λ��	8/16
	int				channels = 0;		/// ����		1/2
}RTSP_SDKAudio;


//����Ϣ�ṹ����
struct RTSP_API SDPMediaInfo
{
	int  payLoad;				//��������

	RTSP_SDKVideo	video;
	RTSP_SDKAudio	audio;


	int  sampRate;				//������
	int  bandwidth;			//����(�е�ý����Ϣ����û������)


	std::string mediaName;	//ý������
	std::string trackID;		//track id ������������
	std::string codecName;		//���뷽ʽ

	FrameType	frametype;
	CodeID		codeId;

	int			profile_level_id;
	std::string sprop_parameter_sets;		//sps��Ϣ(һ��ΪBase64�ı��봮,���ڳ�ʼ��������,һ��ֻ��������Ƶ������Ϣ)

	std::string media_header;

	SDPMediaInfo();
	void parseMediaCodeInfo();
	void buildMediaCodeInfo();
};
class MediaSession;
class RTSPStatistics;
class UDPPortAlloc;

struct STREAM_TRANS_INFO
{
	SDPMediaInfo				streaminfo;
	TRANSPORT_INFO				transportinfo;
	weak_ptr<MediaSession>		mediasession;
	weak_ptr<RTSPStatistics>	rtspstatistics;
};

//ý����Ϣ�ṹ����
struct RTSP_API RTSP_Media_Infos
{
	std::string		ssrc;				//ssrc

	uint32_t startRange;
	uint32_t stopRange;

	std::list< shared_ptr<STREAM_TRANS_INFO> >	infos;

	RTSP_Media_Infos();
	RTSP_Media_Infos cloneStreamInfo() const;
	const shared_ptr<STREAM_TRANS_INFO> streamInfo(FrameType frametype)const;
	const shared_ptr<STREAM_TRANS_INFO> videoStreamInfo()const;
	const shared_ptr<STREAM_TRANS_INFO> audioStreamInfo()const;
    const shared_ptr<STREAM_TRANS_INFO> talkbackStreamInfo()const;

	shared_ptr<STREAM_TRANS_INFO> addVideoStreamInfo();
	shared_ptr<STREAM_TRANS_INFO> addAudioStreamInfo();
    shared_ptr<STREAM_TRANS_INFO> addTalkbackStreamInfo(CodeID codeid, int sampRate, int sampBit, int channels);
	shared_ptr<STREAM_TRANS_INFO> addStreamInfo(const std::string& flag);
	//�Ƿ�����Ƶ��
	bool hasVideo() const;
	//�Ƿ�����Ƶ��
	bool hasAudio() const;

	//���������Ƶ������Ϣ
	void cleanExStreamInfo();
};

enum PlayCmd {
	PlayCmd_RAND_NPT = 0,	// relative to the beginning of the presentation
	PlayCmd_RAND_CLOCK,		// absolute time, ISO 8601 timestamps, UTC(GMT)
	PlayCmd_SPEED,
};

struct PlayParam
{
	PlayCmd			cmd = PlayCmd_RAND_NPT;
	union {
		double		speed = 0.0;
		struct {
			uint64_t	from;
			uint64_t	to;
		};
	}data;
};

struct RTSP_API PlayInfo
{
	std::map<PlayCmd,PlayParam>		params;


	bool haveSpeed() const;
	bool haveRang() const;
	bool haveRangNpt() const;
	bool haveRangClock() const;

	void addSpeed(double speed);
	void addRangNpt(uint64_t start, uint64_t stop = 0);
	void addRangClock(uint64_t start, uint64_t stop = 0);

	PlayParam speed() const;
	PlayParam range() const;

	std::string buildSpeed() const;
	bool parseSpeed(const std::string& str);

	std::string buildRang() const;
	bool parseRang(const std::string& str);
};

#define Parameter_EndofFile "endoffile"

struct RTSP_API RtspParameter
{
	bool addParameter(const std::string& key, const Value& value);

	bool parseParameter(const std::string body);

	std::string toString();

	std::map<std::string, std::string> parameterMap;
};
//RTSP������Ϣ
struct RTSP_API RTSPCommandInfo :public HTTP::Header
{
	std::string body;

	uint32_t		cseq;

	RTSPCommandInfo() :cseq(0) {}
};

/*
+---------------+---------------+---------------+---------------+
|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|V=2|P|X|   CC  |M|	    PT      |     sequence number 			|
+---------------+---------------+---------------+---------------+
|		                    timestamp	        			    |
+---------------+---------------+---------------+---------------+
|		                       SSRC		        			    |
+---------------+---------------+---------------+---------------+
|		                       CSRC		        			    |
|							  ......							|
+---------------+---------------+---------------+---------------+
*/
typedef struct
{
	unsigned short   cc : 4;
	unsigned short   x : 1;
	unsigned short   p : 1;
	unsigned short   v : 2;
	unsigned short   pt : 7;
	unsigned short   m : 1;

	unsigned short   seq;
	unsigned int    ts;
	unsigned int    ssrc;

}RTPHEADER;


/*
+---------------+---------------+---------------+---------------+
|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   defined of profile          |     length        			|
+---------------+---------------+---------------+---------------+
|		             extern    data		        			    |
|							  ......							|
+---------------+---------------+---------------+---------------+
*/

typedef struct  
{
	unsigned short   dop;	//define of ptofile
	unsigned short   len;	//����
}RTPEXTERNHEADER;


/*
+---------------+---------------+---------------+---------------+
|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|0|1|2|3|4|5|6|7|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  flag |version|     type      |     length        			|
+---------------+---------------+---------------+---------------+
|		             extern    data		        			    |
|							  ......							|
+---------------+---------------+---------------+---------------+
*/

typedef enum
{
	MSRTPEXTERNTYPE_Time = 1,			//ʱ��,len = 64
	MSRTPEXTERNTYPE_Endoffile = 2,		//���ţ����Ž���
	MSRTPEXTERNTYPE_Beginoffile = 3,	//���ţ����ŵ�ͷ
}MSRTPEXTERNTYPE;

//����Ϊ4��������
typedef struct
{
	unsigned short		ver : 4;	//�汾��Ĭ��1
	unsigned short		flag : 4;	//��־��Ĭ�϶�����1010
	unsigned short		type : 8;	//���ͣ���� MSRTPEXTERNTYPE
	unsigned short		len;	//���� = sizeof(MSRTPEXTERNHEADER)
	unsigned long long	extendData; //��չ����
}MSRTPEXTERNHEADER;



#define RTP_VERSION 2

#define MAXRTPPACKETLEN		1440


#define RTPOVERTCPMAGIC		'$'

typedef struct 
{
	unsigned int magic : 8;// $
	unsigned int channel : 8; //0-1
	unsigned int rtp_len : 16;
}INTERLEAVEDFRAME;


}
}