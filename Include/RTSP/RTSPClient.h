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
#include "RTSP/RTPPackage.h"
using namespace Public::Base;
using namespace Public::Network;

namespace Public {
namespace RTSP {

class RTSPClientManager;
class RTSPClientHandler;

class RTSP_API RTSPClient
{
	friend class RTSPClientManager;
	struct RTSPClientInternal;
	RTSPClient(const std::shared_ptr<IOWorker>& work, const shared_ptr<RTSPClientHandler>& handler, const AllockUdpPortCallback& allockport,const RTSPUrl& rtspUrl,const std::string& useragent);
public:
	~RTSPClient();

	/*����RTP���ݽ��շ�ʽ 0:TCP��1:UDP  Ĭ��UDP*/
	bool initRTPOverTcpType();
	bool initRTPOverUdpType();

	/*׼�����ݽ��գ������������ݽ����̣߳������߳�*/
	//timeout ���ӳ�ʱʱ�䣬
	//reconnect �Ƿ���������
	bool start(uint32_t timeout = 10000);

	bool stop();

	//�첽���ʹ��RTSPClientHandler->onPlayResponse���ս��
	shared_ptr<RTSPCommandInfo> sendPlayRequest(const RANGE_INFO& range);
	//ͬ�����ͬ������
	bool sendPlayRequest(const RANGE_INFO& range, uint32_t timeout);

	//�첽���ʹ��RTSPClientHandler->onPauseResponse���ս��
	shared_ptr<RTSPCommandInfo> sendPauseRequest();
	//ͬ������,ͬ������
	bool sendPauseRequest(uint32_t timeout);


	//�첽���ʹ��RTSPClientHandler->onGetparameterResponse���ս��
	shared_ptr<RTSPCommandInfo> sendGetparameterRequest(const std::string& body);
	//ͬ������,ͬ������
	bool sendGetparameterRequest(const std::string& body, uint32_t timeout);


	//�첽���ʹ��RTSPClientHandler->onTeradownResponse���ս��
	shared_ptr<RTSPCommandInfo> sendTeradownRequest();
	//ͬ������,ͬ������
	bool sendTeradownRequest(uint32_t timeout);

	bool sendMediaPackage(const shared_ptr<STREAM_TRANS_INFO> mediainfo, const RTPPackage& rtppackge);
	bool sendContorlPackage(const shared_ptr<STREAM_TRANS_INFO> mediainfo, const char* buffer,uint32_t bufferlen);
private:
	RTSPClientInternal *internal;
};

class RTSP_API RTSPClientHandler
{
public:
	RTSPClientHandler() {}
	virtual ~RTSPClientHandler() {}
	
	virtual void onConnectResponse(bool success, const std::string& errmsg) {}

	virtual void onDescribeResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo, const shared_ptr<MEDIA_INFO>& info) { info->cleanExStreamInfo(); }
	virtual void onSetupResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo, const shared_ptr<STREAM_TRANS_INFO>& transport) {}
	virtual void onPlayResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo) {}
	virtual void onPauseResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo) {}
	virtual void onGetparameterResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo, const std::string& content) {}
	virtual void onTeradownResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo) {}

	virtual void onErrorResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo,int statuscode,const std::string& errmsg) {}

	virtual void onClose(const std::string& errmsg) = 0;

	virtual void onMediaPackageCallback(const shared_ptr<STREAM_TRANS_INFO> mediainfo, const RTPPackage& rtppackge) {};
	virtual void onContorlPackageCallback(const shared_ptr<STREAM_TRANS_INFO> mediainfo, const char* buffer, uint32_t bufferlen) {}
};

class RTSP_API RTSPClientManager
{
	struct RTSPClientManagerInternal;
public:
	//userContent �û�������Ϣ,threadNum �߳���������RTSP���û�������
	RTSPClientManager(const shared_ptr<IOWorker>& iowrker,const std::string& useragent);
	~RTSPClientManager();

	//rtp ģʽ��ַ
	bool initRTPOverUdpType(uint32_t startport = 40000, uint32_t stopport = 41000);

	//����һ������
	shared_ptr<RTSPClient> create(const shared_ptr<RTSPClientHandler>& handler, const RTSPUrl& pRtspUrl);
private:
	RTSPClientManagerInternal * internal;
};


}
}
