#pragma once

#ifndef _I_RTP_UNPACK_H_
#define _I_RTP_UNPACK_H_

#include "RTSP/Defs.h"
#include "RTSP/RTSPStructs.h"
#include "Base/Base.h"
using namespace Public::Base;


namespace Public {
namespace RTSP {

//֡���Ͷ���
typedef enum {
	FrameType_NONE = -1,
	FrameType_VIDEO_I_FRAME = 0 ,
	FrameType_VIDEO_P_FRAME = 1,
	FrameType_AUDIO_FRAME = 2,
	FrameType_H264_SPS_FRAME = 3,
	FrameType_H264_PPS_FRAME = 4,
	FrameType_MJPEG_FRAME = 5,
	FrameType_VIDEO_FRAME = 6,
}FrameType;


//��RTP�����ݽ��зֽ�Ϊ��Ӧ��������
class RTSP_API RTPAnalyzer
{
public:
	//���ݻص�
//const unsigned char *pBuf ֡����
//int nFreamSize			֡����
//int nFreamType			֡����
//long lTimestamp			ʱ���,����Ƿְ��Ե�һ��RTP��ʱ���Ϊ׼
	typedef Function4<void, FrameType, const char*, uint32_t, long> FrameCallback;
public:
	RTPAnalyzer(const shared_ptr<STREAM_TRANS_INFO>& transportinfo,const FrameCallback& framcallback);
	~RTPAnalyzer();

	///����RTP��
	///@[in] pBuf RTP ������
	///@[in] nBufSize RTP ������
	///@[in] nOffset ����buffer��ƫ����
	///@[return] 0:�������ݲ���һ֡�������ȴ�����, -1:���ִ���
	bool inputRtpPacket(const RTPHEADER& rtpheader, const char* bufferaddr, uint32_t bufferlen);

	///��ջ�����
	bool reset();
private:
	struct RTPAnalyzerInternal;
	RTPAnalyzerInternal* internal;

};

}
}


#endif