#pragma once

#include "Base/Base.h"
#include "RTSP/Defs.h"
#include "RTSP/RTSPStructs.h"
#include "RTSP/RTCPPackage.h"
using namespace Public::Base;

namespace Public {
namespace RTSP {

class RTPPackage;

//RTSP�����շ�ͳ��
class RTSP_API RTSPStatistics
{
public:
	struct RecvStatistics
	{
		uint64_t		needCountPackage = 0;		//�ý��ܵ���������
		uint64_t		realRecvPackage = 0;		//ʵ�ʽ��ܵ���������

		uint32_t		prevTimesmap = 0;			//��һ֡�¼���
		uint32_t		prevSeq = 0;				//��һ֡�İ����

		uint32_t		wrapAroundCount = 0;		//��ŷ�ת����

		uint32_t		rtpSsrc = 0;				//recv rtp ssrc
	};
	struct SendStatustics
	{
		uint32_t		sendPacketCount = 0;		//���Ͱ���
		uint32_t		sendOctetCount = 0;			//���ݴ�С������RTPͷ

		uint32_t		prevTimesmap = 0;			//��һ֡�¼���
	};
public:
	//intervalΪͳ�Ƶ�ʱ����
	RTSPStatistics(uint32_t interval = 10*1000);
	virtual ~RTSPStatistics();

	void inputRecvPackage(const shared_ptr<RTPPackage>& rtp);
	void inputRecvPackage(const shared_ptr<RTCPPackage>& rtcppackage);

	void inputSendPackage(const shared_ptr<RTPPackage>& rtp);
	void inputSendPackage(const shared_ptr<RTCPPackage>& rtcppackage);

	void getRecvStatistics(RecvStatistics& statistics);
	void getSendStatistics(SendStatustics& statistics);

	void onPoolTimerProc();
private:
	struct RTSPStatisticsInternal;
	RTSPStatisticsInternal* internal;
};


}
}