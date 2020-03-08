#ifndef __ONVIFCLIENT_H__
#define __ONVIFCLIENT_H__
#include "Base/Base.h"
#include "Network/Network.h"
#include "OnvifClientDefs.h"
using namespace Public::Base;
using namespace Public::Network;

#ifdef WIN32
#ifdef ONVIFCLIENT_EXPORTS
#define ONVIFCLIENT_API __declspec(dllexport)
#else
#define ONVIFCLIENT_API __declspec(dllimport)
#endif
#else
#define ONVIFCLIENT_API
#endif


namespace Public {
namespace Onvif {

//Onvif�ͻ���
class OnvifClientManager;
class ONVIFCLIENT_API OnvifClient
{
	friend class OnvifClientManager;
	OnvifClient(const shared_ptr<IOWorker>& worker,const shared_ptr<HTTP::AsyncClient>& asyncclient, const URL& url,const std::string& useragent);
	void onPoolTimerProc();
public:
	typedef Function<void(const std::string&)> AlarmCallback;
public:
	~OnvifClient();

	const URL& OnvifUrl() const;

	///--------------------------------�豸��Ϣ���---------------------------------//
	//��ȡ�豸��Ϣ
	ErrorInfo getInfo(OnvifClientDefs::Info& info,int timeoutms = 10000);

	//�첽��ȡ�豸��Ϣ
	typedef Function<void(const ErrorInfo&,const OnvifClientDefs::Info&)> GetInfoCalblack;
	ErrorInfo asyncGetInfo(const GetInfoCalblack& callback, int timeoutms = 10000);

	//��ȡϵͳʱ��
	ErrorInfo getSystemDatetime(Time& time, int timeoutms = 10000);

	//�첽��ȡϵͳʱ��
	typedef Function<void(const ErrorInfo&, const Time&) > GetSystemDateTimeCallback;
	ErrorInfo asyncGetSystemDatetime(const GetSystemDateTimeCallback& callbck, int timeoutms = 10000);

	//����ϵͳʱ��
	ErrorInfo SetSystemDatetime(const Time& time, int timeoutms = 10000);

	//�첽����ϵͳʱ��
	typedef Function<void(const ErrorInfo&)> SetSystemDatetimeCallback;
	ErrorInfo asyncSetSystemDatetime(const SetSystemDatetimeCallback& callback, const Time& time, int timeoutms = 10000);

	//��ȡ������Ϣ
	ErrorInfo getNetworkInterfaces(OnvifClientDefs::NetworkInterfaces& network, int timeoutms = 10000);
	//�첽��ȡ������Ϣ
	typedef Function<void(const ErrorInfo&, const OnvifClientDefs::NetworkInterfaces&)> GetNetworkInterfacesCallback;
	ErrorInfo asyncGetNetworkInterfaces(const GetNetworkInterfacesCallback& callback, int timeoutms = 10000);

	//ϵͳ����
	ErrorInfo systemReboot(int timeoutms = 10000);

	//�첽ϵͳ����
	typedef Function<void(const ErrorInfo&)> SystemRebootCallback;
	ErrorInfo asyncSystemReboot(const SystemRebootCallback& callback, int timeoutms = 10000);


	//��ȡ�豸��������Ϣ
	ErrorInfo getCapabities(OnvifClientDefs::Capabilities& cap,int timeoutms = 10000);

	//�첽��ȡ�豸������Ϣ
	typedef Function<void(const ErrorInfo&, const OnvifClientDefs::Capabilities&)> GetCapabilitiesCallback;
	ErrorInfo asyncGetCapabities(const GetCapabilitiesCallback& callback, int timeoutms = 10000);
								
	
	//��ȡ����Ƶ/��̨��������Ϣ
	ErrorInfo getProfiles(OnvifClientDefs::Profiles& profiles,int timeoutms = 10000);
	
	//�첽��ȡ������Ϣ
	typedef Function<void(const ErrorInfo&,const OnvifClientDefs::Profiles&)> GetProfilesCallback;
	ErrorInfo asyncGetProfiles(const GetProfilesCallback& callback, int timeoutms = 10000);


	///--------------------------------Media���---------------------------------//
	//��ȡ��Ƶ���ŵ�ַ
	ErrorInfo getStreamUrl(const OnvifClientDefs::ProfileInfo& info, OnvifClientDefs::StreamUrl& streamurl,int timeoutms = 10000);
	
	//�첽��ȡ��Ƶ����ַ
	typedef Function<void(const ErrorInfo&, const OnvifClientDefs::StreamUrl& )> GetStreamUrlCallback;
	ErrorInfo asyncGetStreamUrl(const GetStreamUrlCallback& callback, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);

	//��ȡ��Ԥ����ͼ��ַ
	ErrorInfo getSnapUrl(const OnvifClientDefs::ProfileInfo& info, OnvifClientDefs::SnapUrl& snapurl,int timeoutms = 10000);

	//�첽��ȡ��Ԥ����ͼ��ַ
	typedef Function<void(const ErrorInfo&, const OnvifClientDefs::SnapUrl&)> GetSnapUrlCallback;
	ErrorInfo asyncGetSnapUrl(const GetSnapUrlCallback& callback, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);


	//-----------����Ƶ��������
	//��ȡ��Ƶ����������Ϣ
	ErrorInfo getVideoEncoderConfigurations(OnvifClientDefs::VideoEncoderConfigurations& configs,int timeoutms = 10000);
	
	typedef Function<void(const ErrorInfo&, const OnvifClientDefs::VideoEncoderConfigurations&) > GetVideoEncoderConfigurationsCallback;
	ErrorInfo asyncGetVideoEncoderConfigurations(const GetVideoEncoderConfigurationsCallback& callback, int timeoutms = 10000);

    //ͼ��������
    typedef Function<void(const ErrorInfo&, const OnvifClientDefs::ImageSettingInfo&) > GetImageSettingCallback;
    ErrorInfo asyncGetImageSetting(const OnvifClientDefs::ProfileInfo& info, const GetImageSettingCallback& callback, int timeoutms = 10000);

    typedef Function<void(const ErrorInfo&)> SetImageSettingCallback;
    ErrorInfo asyncSetImageSetting(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::ImageSettingInfo&, const SetImageSettingCallback& callback, int timeoutms = 10000);

	typedef Function<void(const ErrorInfo&, const OnvifClientDefs::ImageOptions)> GetImageOptionsCallback;
	ErrorInfo asyncGetImageOptions(const OnvifClientDefs::ProfileInfo& info, const GetImageOptionsCallback& callback, int timeoutms = 10000);

	///----------------------------------------��̨���--------------------------//
	//��̨���
	ErrorInfo continuousMove(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);
	
	//�첽��̨����
	typedef Function<void(const ErrorInfo&)> ContinuousMoveCallback;
	ErrorInfo asyncContinuousMove(const ContinuousMoveCallback& callback, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);
	
	//ֹͣ��̨����
	ErrorInfo stopPTZ(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);
	
	//�첽ֹͣ��̨����
	typedef Function<void(const ErrorInfo&)> StopPTRCallback;
	ErrorInfo asyncStopPtz(const StopPTRCallback& callback, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);

	//Ԥ��λ���
	//������̨Ԥ��λ
	ErrorInfo setPreset(uint32_t index, const std::string& presetname, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);
	
	//�첽������̨Ԥ��λ
	typedef Function<void(const ErrorInfo&)> SetPresetCallback;
	ErrorInfo asyncSetPresest(uint32_t index, const std::string& presetname, const OnvifClientDefs::ProfileInfo& info, const SetPresetCallback& callback, int timeoutms = 10000);

	//��ȡ��̨Ԥ��λ
	ErrorInfo getPreset(const OnvifClientDefs::ProfileInfo& info, OnvifClientDefs::PresetInfos& presetinfo, int timeoutms = 10000);
	
	//�첽��ȡ��̨Ԥ��λ
	typedef Function<void(const ErrorInfo&,const OnvifClientDefs::PresetInfos&)> GetPresetCallback;
	ErrorInfo asyncGetPreset(const OnvifClientDefs::ProfileInfo& info, const GetPresetCallback& callback, int timeoutms = 10000);

	//ִ��Ԥ��λ
	ErrorInfo gotoPreset(uint32_t index, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);
	
	//�첽ִ��Ԥ��λ
	typedef Function<void(const ErrorInfo&)> GotoPresetCallback;
	ErrorInfo asyncGotoPreset(uint32_t index, const OnvifClientDefs::ProfileInfo&  presetinfo, const GotoPresetCallback& callback, int timeoutms = 10000);
	

	//ɾ��Ԥ��λ
	ErrorInfo removePreset(uint32_t index, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);

	//�첽ɾ����̨Ԥ��λ
	typedef Function<void(const ErrorInfo&)> RemovePresetCallback;
    ErrorInfo asyncRemovePreset(uint32_t index, const OnvifClientDefs::ProfileInfo& info, const RemovePresetCallback& callback, int timeoutms = 10000);

	//---------------------------------------------EVENT���----------------------------------------//

	//�����¼����
	ErrorInfo subscribeEvent(const OnvifClientDefs::Capabilities& capabilities, OnvifClientDefs::SubEventResponse& subeventresp,int timeoutms = 10000);
	
	typedef Function<void(const ErrorInfo&, const OnvifClientDefs::SubEventResponse&) > SubscribeEventCallback;
	ErrorInfo asyncSubscribeEvent(const SubscribeEventCallback& callback, const OnvifClientDefs::Capabilities& capabilities, int timeoutms = 10000);

	ErrorInfo getEvent(const OnvifClientDefs::SubEventResponse& subeventresp,OnvifClientDefs::EventInfos& eventinfos,int timeoutms = 2*60000);

	typedef Function<void(const ErrorInfo&,const OnvifClientDefs::EventInfos&)> GetEventCallback;
	ErrorInfo asyncGetEvent(const GetEventCallback& callback, const OnvifClientDefs::SubEventResponse& subeventresp, int timeoutms = 2 * 60000);

	ErrorInfo stopSubEvent();

	//---------------------------------------------HTTP���------------------------------//
	//imgtype == 0 ? jpg, == 1 ? png, == 2 ? bmp
	ErrorInfo getSnapImage(const OnvifClientDefs::SnapUrl& snapurl, std::string& imgdata, int& imgtype, int timeoutms = 10000);

	typedef Function<void(const ErrorInfo&, const std::string& imgdata, int imgtype)> GetSnapImageCallback;
	ErrorInfo asyncGetSnapImage(const GetSnapImageCallback& callback, const OnvifClientDefs::SnapUrl& snapurl, int timeoutms = 10000);



	//ͬ������HTTP����
	//method ����ʽ��get/post
	//url �����ַ
	//ackdata �ظ�����
	//reqdata ���󸽴�������
	//reqcontenttype��������������
	//timeoutms ��ʱʱ��
	shared_ptr<HTTP::ClientResponse> httpRequest(const std::string& method,const std::string& url,const std::string& reqdata = "",const std::string& reqcontenttype = "", int timeoutms = 10000);

	typedef Function<void(const shared_ptr<HTTP::ClientResponse>&)> HttpRequestCallback;
	ErrorInfo asyncHttpRequest(const HttpRequestCallback& callback,const std::string& method, const std::string& url, const std::string& reqdata = "", const std::string& reqcontenttype = "", int timeoutms = 10000);

	//����豸ʱ��͵�ǰ�Ա��Ƿ����
	bool checkDeviceTimeIsOverdue(const Time& devtime);
	Time getLocalCurrDeviceTime();
public:
	struct OnvifClientInternal;
private:
	OnvifClientInternal* internal;
};

//OnvifClient������
class ONVIFCLIENT_API OnvifClientManager
{
	friend OnvifClient;
public:
	struct ONVIFCLIENT_API Disconvery
	{
		friend OnvifClientManager;
	public:
		Disconvery();
		~Disconvery();

		void sendDisconvery();

		void getDeviceList(std::list<OnvifClientDefs::DiscoveryInfo>& list);
	private:
		struct DisconveryInternal;
		DisconveryInternal* internal;
	};
public:
	//userContent �û�������Ϣ,threadNum �߳���������RTSP���û�������
	OnvifClientManager(const shared_ptr<IOWorker>& worker,const std::string& userContent);
	~OnvifClientManager();

	//����onvif�豸��ַ����һ���豸,url Ϊ�豸��ַ������IP���˿ڣ��û������������Ϣ,onvifĬ������·��Ϊ "/onvif/device_service"
	//��:admin:admin@192.168.13.33
	shared_ptr<OnvifClient> create(const URL& url);

	//�����豸����ʱ���Զ�ֹͣ����
	shared_ptr<Disconvery> disconvery();
private:
	struct OnvifClientManagerInternal;
	OnvifClientManagerInternal* internal;
};


}
}



#endif //__ONVIFCLIENT_H__
