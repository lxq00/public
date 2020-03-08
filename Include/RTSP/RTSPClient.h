//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: File.h 252 2013-12-18 04:40:28Z  $
//


#pragma once
#include "Defs.h"
#include "Base/Base.h"
#include "RTSPStructs.h"
#include "RTSPUrl.h"
#include "Network/Network.h"
#include "RTSPHandler.h"
using namespace Public::Base;
using namespace Public::Network;

namespace Public {
namespace RTSP {

class RTSPClientManager;
class RTSPClientHandler;

class RTSP_API RTSPClient:public RTSPCommandSender,public enable_shared_from_this<RTSPCommandSender>
{
	friend class RTSPClientManager;
	struct RTSPClientInternal;
	RTSPClient(const std::shared_ptr<IOWorker>& work, const shared_ptr<RTSPClientHandler>& handler, const shared_ptr<UDPPortAlloc>& portalloc,const RTSPUrl& rtspUrl,const std::string& useragent);
	//��ʱ������
	void onPoolTimerProc();
public:
	~RTSPClient();

	/*����RTP���ݽ��շ�ʽ 0:TCP��1:UDP  Ĭ��UDP*/
	bool initRTPOverTcpType();
	bool initRTPOverUdpType();

	/*׼�����ݽ��գ������������ݽ����̣߳������߳�*/
	//timeout ���ӳ�ʱʱ�䣬
	//reconnect �Ƿ���������
	ErrorInfo start(uint32_t timeout = 10000);

	ErrorInfo stop();

	//�첽���ʹ��RTSPClientHandler->onPlayResponse���ս��
	shared_ptr<RTSPCommandInfo> sendPlayRequest(const PlayInfo& range);
	//ͬ�����ͬ������
	ErrorInfo sendPlayRequest(const PlayInfo& range, uint32_t timeout);

	//�첽���ʹ��RTSPClientHandler->onPauseResponse���ս��
	shared_ptr<RTSPCommandInfo> sendPauseRequest();
	//ͬ������,ͬ������
	ErrorInfo sendPauseRequest(uint32_t timeout);


	//�첽���ʹ��RTSPClientHandler->onGetparameterResponse���ս��
	shared_ptr<RTSPCommandInfo> sendGetparameterRequest(const std::string& body);
	//ͬ������,ͬ������
	ErrorInfo sendGetparameterRequest(const std::string& body, uint32_t timeout);


	//�첽���ʹ��RTSPClientHandler->onTeradownResponse���ս��
	shared_ptr<RTSPCommandInfo> sendTeradownRequest();
	//ͬ������,ͬ������
	ErrorInfo sendTeradownRequest(uint32_t timeout);

	ErrorInfo sendRTPFrame(const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTPFrame>& rtppackge);
	ErrorInfo sendRTCPPackage(const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTCPPackage>& rtcppackge);
private:
	RTSPClientInternal *internal;
};

class RTSP_API RTSPClientHandler:public RTSPHandler
{
public:
	RTSPClientHandler() {}
	virtual ~RTSPClientHandler() {}
	
	virtual void onConnectResponse(shared_ptr<RTSPCommandSender>& sender, const ErrorInfo& err) {}
	virtual void onErrorResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, int errcode, const std::string& errmsg) {}

	virtual void onOptionResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const ErrorInfo& err, const std::string& optstr) {}
	virtual void onDescribeResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const shared_ptr<RTSP_Media_Infos>& info) { info->cleanExStreamInfo(); }
	virtual void onSetupResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const shared_ptr<STREAM_TRANS_INFO>& transport) {}
	virtual void onPlayResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo) {}
	virtual void onPauseResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const ErrorInfo& err) {}
	virtual void onGetparameterResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const std::string& content) {}
	virtual void onTeradownResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo) {}
	
	virtual void onRTPFrameCallback(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTPFrame>& frame) {}
	//virtual void onRTPPackageCallback(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTPPackage>& rtppackge) {}
	virtual void onRTCPPackageCallback(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTCPPackage>& rtcppackge) {}

	virtual void onClose(shared_ptr<RTSPCommandSender>& sender, const ErrorInfo& err) {}
};

class RTSP_API RTSPClientManager
{
	struct RTSPClientManagerInternal;
public:
	//userContent �û�������Ϣ,threadNum �߳���������RTSP���û�������
	RTSPClientManager(const shared_ptr<IOWorker>& iowrker,const std::string& useragent);
	~RTSPClientManager();

	//rtp ģʽ��ַ
	bool initRTPOverUdpPort(uint32_t startport = 40000, uint32_t stopport = 41000);

	//����һ������
	shared_ptr<RTSPClient> create(const shared_ptr<RTSPClientHandler>& handler, const RTSPUrl& pRtspUrl);
private:
	RTSPClientManagerInternal * internal;
};


}
}
