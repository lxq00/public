#ifndef __ONVIFPROTOCOL_PROFILES_H__StartRecvAlarm
#define __ONVIFPROTOCOL_PROFILES_H__StartRecvAlarm
#include "CmdObject.h"


class CMDStartRecvAlarm :public CmdObject
{
public:
	CMDStartRecvAlarm()
	{
		action = "http://www.onvif.org/ver10/events/wsdl/EventPortType/CreatePullPointSubscriptionRequest";
	}
	virtual ~CMDStartRecvAlarm() {}

	virtual std::string build(const URL& URL)
	{
		stringstream stream;

		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
			<< "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://www.w3.org/2005/08/addressing\">"
			<< buildHeader(URL)
			<< "<s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
			<< "<CreatePullPointSubscription xmlns=\"http://www.onvif.org/ver10/events/wsdl\">"
			<< "<InitialTerminationTime>PT60S</InitialTerminationTime>"
			<< "</CreatePullPointSubscription>"
			<< "</s:Body></s:Envelope>";

		return stream.str();
	}

	shared_ptr<OnvifClientDefs::StartRecvAlarm>	startrecvalarm;
	virtual bool parse(const XMLObject::Child& body)
	{
		startrecvalarm = make_shared<OnvifClientDefs::StartRecvAlarm>();

		const XMLObject::Child& resp = body.getChild("tev:CreatePullPointSubscriptionResponse");
		if (!resp) return false;

		startrecvalarm->xaddr = resp.getChild("tev:SubscriptionReference").getChild("wsa:Address").data();
		startrecvalarm->currentTime = onvif_parse_datetime(resp.getChild("wsnt:CurrentTime").data());
		startrecvalarm->terminationTime = onvif_parse_datetime(resp.getChild("wsnt:TerminationTime").data());

		return true;
	}
};



#endif //__ONVIFPROTOCOL_H__
