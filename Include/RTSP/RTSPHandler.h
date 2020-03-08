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
#include "RTSP/RTCPPackage.h"
#include "RTPFrame.h"

using namespace Public::Base;
using namespace Public::Network;

namespace Public {
namespace RTSP {
//RTSP COMMAND HANDLER

class RTSP_API RTSPCommandSender
{
public:
	RTSPCommandSender() {}
	virtual ~RTSPCommandSender() {}

	// send error response
	virtual void sendErrorResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo, int errcode, const std::string& errmsg) {}
	
	//command ---------------Option
	virtual void sendOptionResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo, const std::string& cmdstr = "") {}

	//command ---------Describe
	virtual void sendDescribeResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo, const RTSP_Media_Infos& mediainfo) {}
	
	//command ---------SETUP
	virtual void sendSetupResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo, const shared_ptr<STREAM_TRANS_INFO>& transport) {}
	
	//command ---------PLAY
	//�첽���ʹ��RTSPClientHandler->onPlayResponse���ս��
	virtual shared_ptr<RTSPCommandInfo> sendPlayRequest(const PlayInfo& range) { return shared_ptr<RTSPCommandInfo>(); }
	//ͬ�����ͬ������
	virtual ErrorInfo sendPlayRequest(const PlayInfo& range, uint32_t timeout) { return ErrorInfo(Error_Code_NotSupport); }
	virtual void sendPlayResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo) {}
	
	//command ----------PAUSE
	//�첽���ʹ��RTSPClientHandler->onPauseResponse���ս��
	virtual shared_ptr<RTSPCommandInfo> sendPauseRequest() { return shared_ptr<RTSPCommandInfo>(); }
	//ͬ������,ͬ������
	virtual ErrorInfo sendPauseRequest(uint32_t timeout) { return ErrorInfo(Error_Code_NotSupport); }
	virtual void sendPauseResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo) {}
	
	//command ----------Getparameter
	//�첽���ʹ��RTSPClientHandler->onGetparameterResponse���ս��
	virtual shared_ptr<RTSPCommandInfo> sendGetparameterRequest(const std::string& body) { return shared_ptr<RTSPCommandInfo>(); }
	//ͬ������,ͬ������
	virtual ErrorInfo sendGetparameterRequest(const std::string& body, uint32_t timeout) { return ErrorInfo(Error_Code_NotSupport); }
	virtual void sendGetparameterResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo, const std::string& content) {}

	//command------------Teradown
	//�첽���ʹ��RTSPClientHandler->onTeradownResponse���ս��
	virtual shared_ptr<RTSPCommandInfo> sendTeradownRequest() { return shared_ptr<RTSPCommandInfo>(); }
	//ͬ������,ͬ������
	virtual ErrorInfo sendTeradownRequest(uint32_t timeout) { return ErrorInfo(Error_Code_NotSupport); }
	virtual void sendTeardownResponse(const shared_ptr<RTSPCommandInfo>& cmdinfo) {};

	//rtp and rtcp
	virtual ErrorInfo sendRTPFrame(const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTPFrame>& rtpframe) { return ErrorInfo(Error_Code_NotSupport); }
//	virtual ErrorInfo sendRTPPackage(const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTPPackage>& rtppackge) { return ErrorInfo(Error_Code_NotSupport); }
	virtual ErrorInfo sendRTCPPackage(const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTCPPackage>& rtcppackge) { return ErrorInfo(Error_Code_NotSupport); }
};

//RTSP COMMAND HANDLER
class RTSP_API RTSPHandler
{
public:
	RTSPHandler() {}
	virtual ~RTSPHandler() {}
	
	//��ѯ�û������뺯�������ض�Ӧ�û�������
	virtual ErrorInfo queryUserPassword(const shared_ptr<RTSPCommandSender>& sender,const std::string& username, std::string& passwd){ return ErrorInfo(Error_Code_NotSupport); }

	//���ӻص�
	virtual void onConnectResponse(shared_ptr<RTSPCommandSender>& sender, const ErrorInfo& err) {}

	// send error response
	virtual void onErrorResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo,int errcode, const std::string& errmsg) {}

	//command ---------------Option
	virtual void onOptionRequest(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo){ sender->sendOptionResponse(cmdinfo);}
	virtual void onOptionResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const ErrorInfo& err, const std::string& optstr) {}


	//command ---------Describe
	virtual void onDescribeRequest(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo) { sender->sendErrorResponse(cmdinfo, 500, "NOT SUPPORT"); }
	virtual void onDescribeResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const shared_ptr<RTSP_Media_Infos>& info) { info->cleanExStreamInfo(); }

	//command ---------SETUP
	virtual void onSetupRequest(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const shared_ptr<STREAM_TRANS_INFO>& transport) { sender->sendErrorResponse(cmdinfo, 500, "NOT SUPPORT"); }
	virtual void onSetupResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const shared_ptr<STREAM_TRANS_INFO>& transport) {}

	//command ---------PLAY
	virtual void onPlayRequest(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const shared_ptr<RTSP_Media_Infos>& mediainfo, const PlayInfo& range) { sender->sendErrorResponse(cmdinfo, 500, "NOT SUPPORT"); }
	virtual void onPlayResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo) {}

	//command ----------PAUSE
	virtual void onPauseRequest(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo) { sender->sendErrorResponse(cmdinfo, 500, "NOT SUPPORT"); }
	virtual void onPauseResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const ErrorInfo& err) {}

	//command ----------Getparameter
	virtual void onGetparameterRequest(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const std::string& content) { sender->sendErrorResponse(cmdinfo, 500, "NOT SUPPORT"); }
	virtual void onGetparameterResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo, const std::string& content) {}
	
	//command------------Teradown
	virtual void onTeardownRequest(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo) { sender->sendErrorResponse(cmdinfo, 500, "NOT SUPPORT"); }
	virtual void onTeradownResponse(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<RTSPCommandInfo>& cmdinfo) {}


	//rtcp
	virtual void onRTCPPackageCallback(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTCPPackage>& rtcppackge) {}
	//rtp frame

	virtual void onRTPFrameCallback(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTPFrame>& frame) {}
	//virtual void onRTPPackageCallback(shared_ptr<RTSPCommandSender>& sender, const shared_ptr<STREAM_TRANS_INFO>& transport, const shared_ptr<RTPPackage>& rtppackge) {}

	//socket disconnect or session timeout or teradown
	virtual void onClose(shared_ptr<RTSPCommandSender>& sender, const ErrorInfo& err) {}
};

}
}
