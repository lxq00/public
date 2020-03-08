#pragma once

#include "RTSP/Defs.h"
#include "RTSP/RTSPStructs.h"
#include "Base/Base.h"
#include "RTSP/RTPFrame.h"
#include "RTSP/RTSPStructs.h"
using namespace Public::Base;


namespace Public {
namespace RTSP {

//��RTP�����ݽ��зֽ�Ϊ��Ӧ��������
class RTSP_API RTPAnalyzer
{
public:
	//���ݻص�
//const unsigned char *pBuf ֡����
//int nFreamSize			֡����
//int nFreamType			֡����
//long lTimestamp			ʱ���,����Ƿְ��Ե�һ��RTP��ʱ���Ϊ׼
	typedef Function<void(const shared_ptr<RTPFrame>&)> FrameCallback;
public:
	RTPAnalyzer(const FrameCallback& framcallback);
	~RTPAnalyzer();

	///����RTP��
	///@[in] pBuf RTP ������
	///@[in] nBufSize RTP ������
	///@[in] nOffset ����buffer��ƫ����
	///@[return] 0:�������ݲ���һ֡�������ȴ�����, -1:���ִ���
	bool inputRtpPacket(const shared_ptr<STREAM_TRANS_INFO>& transportinfo, const shared_ptr<RTPPackage>& rtppackage);

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
	RTPBuilder(const shared_ptr<STREAM_TRANS_INFO>& rttransinfo);

	~RTPBuilder();

	//�������ݱ���Ϊһ������֡
	std::vector<shared_ptr<RTPPackage>> inputFrame(const shared_ptr<RTPFrame>& frame);

	void setAutoTimeStampEnable(bool enable);
private:
	struct RTPBuilderInternal;
	RTPBuilderInternal* internal;
};

}
}
