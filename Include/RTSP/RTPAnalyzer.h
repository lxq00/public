#pragma once

#ifndef _I_RTP_UNPACK_H_
#define _I_RTP_UNPACK_H_

#include "RTSP/Defs.h"
#include "RTSP/RTSPStructs.h"
#include "Base/Base.h"
#include "RTSP/RTPPackage.h"
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
	typedef Function<void(FrameType, const char*, uint32_t, long)> FrameCallback;
public:
	RTPAnalyzer(const shared_ptr<STREAM_TRANS_INFO>& transportinfo,const FrameCallback& framcallback);
	~RTPAnalyzer();

	///����RTP��
	///@[in] pBuf RTP ������
	///@[in] nBufSize RTP ������
	///@[in] nOffset ����buffer��ƫ����
	///@[return] 0:�������ݲ���һ֡�������ȴ�����, -1:���ִ���
	bool inputRtpPacket(const RTPPackage& rtppackage);

	///��ջ�����
	bool reset();
private:
	struct RTPAnalyzerInternal;
	RTPAnalyzerInternal* internal;

};

//������������Ƶ���ݰ������RTP��
class RTSP_API RTPBuilder
{
public:
	typedef Function<void(const RTPPackage&)> RTPPackageCallback;
public:
	RTPBuilder(const shared_ptr<MEDIA_INFO>& rtspmedia,const RTPPackageCallback &callback);

	~RTPBuilder();

	//����֡��,Ĭ��25֡
	void setFrameRate(int framerate);

	//��ȡ֡��
	int getFrameRate();

	//���ò����ʣ�Ĭ��8000
	void setSamplingRate(int samplingRate);

	//��ȡ������
	int getSamplingRate();

	/// �������ݱ���Ϊһ������֡
	bool inputVideoData(unsigned char* data, int size);
	bool inputAudioData(unsigned char* data, int size);
private:
	struct RTPBuilderInternal;
	RTPBuilderInternal* internal;
};

}
}


#endif