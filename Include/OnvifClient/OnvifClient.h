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

typedef enum {
	OnvifResult_OK,					//����
	OnvifResult_ConnectError,		//����ʧ��
	OnvifResult_AuthenError,		//��֤ʧ��
	OnvifResult_RequestError,		//����ʧ��
	OnvifResult_ResponseError,		//�ظ�ʧ��
	OnvifResult_ParseError,			//�����ظ�����ʧ��
	OnvifResult_NotSupport,			//��֧��
}OnvifResult;


//Onvif�ͻ���
class OnvifClientManager;
class ONVIFCLIENT_API OnvifClient
{
	friend class OnvifClientManager;
	OnvifClient(const shared_ptr<IOWorker>& worker, const URL& url,const std::string& useragent);
public:
	typedef Function<void(const std::string&)> AlarmCallback;
public:
	~OnvifClient();

	///--------------------------------�豸��Ϣ���---------------------------------//
	//��ȡ�豸��Ϣ
	OnvifResult getInfo(OnvifClientDefs::Info& info,int timeoutms = 10000);	

	//�첽��ȡ�豸��Ϣ
	typedef Function<void(OnvifResult,const OnvifClientDefs::Info&)> GetInfoCalblack;
	bool asyncGetInfo(const GetInfoCalblack& callback, int timeoutms = 10000);

	//��ȡϵͳʱ��
	OnvifResult getSystemDatetime(Time& time, int timeoutms = 10000);

	//�첽��ȡϵͳʱ��
	typedef Function<void(OnvifResult, const Time&) > GetSystemDateTimeCallback;
	bool asyncGetSystemDatetime(const GetSystemDateTimeCallback& callbck, int timeoutms = 10000);

	//����ϵͳʱ��
	OnvifResult SetSystemDatetime(const Time& time, int timeoutms = 10000);

	//�첽����ϵͳʱ��
	typedef Function<void(OnvifResult)> SetSystemDatetimeCallback;
	bool asyncSetSystemDatetime(const SetSystemDatetimeCallback& callback, const Time& time, int timeoutms = 10000);

	//��ȡ������Ϣ
	OnvifResult getNetworkInterfaces(OnvifClientDefs::NetworkInterfaces& network, int timeoutms = 10000);
	//�첽��ȡ������Ϣ
	typedef Function<void(OnvifResult, const OnvifClientDefs::NetworkInterfaces&)> GetNetworkInterfacesCallback;
	bool asyncGetNetworkInterfaces(const GetNetworkInterfacesCallback& callback, int timeoutms = 10000);

	//ϵͳ����
	OnvifResult systemReboot(int timeoutms = 10000);

	//�첽ϵͳ����
	typedef Function<void(OnvifResult)> SystemRebootCallback;
	bool asyncSystemReboot(const SystemRebootCallback& callback, int timeoutms = 10000);


	//��ȡ�豸��������Ϣ
	OnvifResult getCapabities(OnvifClientDefs::Capabilities& cap,int timeoutms = 10000);

	//�첽��ȡ�豸������Ϣ
	typedef Function<void(OnvifResult, const OnvifClientDefs::Capabilities&)> GetCapabilitiesCallback;
	bool asyncGetCapabities(const GetCapabilitiesCallback& callback, int timeoutms = 10000);
								
	
	//��ȡ����Ƶ/��̨��������Ϣ
	OnvifResult getProfiles(OnvifClientDefs::Profiles& profiles,int timeoutms = 10000);
	
	//�첽��ȡ������Ϣ
	typedef Function<void(OnvifResult,const OnvifClientDefs::Profiles&)> GetProfilesCallback;
	bool asyncGetProfiles(const GetProfilesCallback& callback, int timeoutms = 10000);


	///--------------------------------Media���---------------------------------//
	//��ȡ��Ƶ���ŵ�ַ
	OnvifResult getStreamUrl(const OnvifClientDefs::ProfileInfo& info, OnvifClientDefs::StreamUrl& streamurl,int timeoutms = 10000);
	
	//�첽��ȡ��Ƶ����ַ
	typedef Function<void(OnvifResult, const OnvifClientDefs::StreamUrl& )> GetStreamUrlCallback;
	bool asyncGetStreamUrl(const GetStreamUrlCallback& callback, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);

	//��ȡ��Ԥ����ͼ��ַ
	OnvifResult getSnapUrl(const OnvifClientDefs::ProfileInfo& info, OnvifClientDefs::SnapUrl& snapurl,int timeoutms = 10000);
	
	//�첽��ȡ��Ԥ����ͼ��ַ
	typedef Function<void(OnvifResult, const OnvifClientDefs::SnapUrl&)> GetSnapUrlCallback;
	bool asyncGetSnapUrl(const GetSnapUrlCallback& callback, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);

	
	//��ȡ��Ƶ����������Ϣ
	OnvifResult getVideoEncoderConfigurations(OnvifClientDefs::VideoEncoderConfigurations& configs,int timeoutms = 10000);
	
	typedef Function<void(OnvifResult, const OnvifClientDefs::VideoEncoderConfigurations&) > GetVideoEncoderConfigurationsCallback;
	bool asyncGetVideoEncoderConfigurations(const GetVideoEncoderConfigurationsCallback& callback, int timeoutms = 10000);


	///----------------------------------------��̨���--------------------------//
	//��̨���
	OnvifResult continuousMove(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);
	
	//�첽��̨����
	typedef Function<void(OnvifResult)> ContinuousMoveCallback;
	bool asyncContinuousMoveCallback(const ContinuousMoveCallback& callback, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);
	
	//ֹͣ��̨����
	OnvifResult stopPTZ(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);
	
	//�첽ֹͣ��̨����
	typedef Function<void(OnvifResult)> StopPTRCallback;
	bool asyncStopPtz(const StopPTRCallback& callback, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);

	//Ԥ��λ���
	//������̨Ԥ��λ
	OnvifResult setPreset(const OnvifClientDefs::ProfileInfo& info, const std::string& presetname, int timeoutms = 10000);
	
	//�첽������̨Ԥ��λ
	typedef Function<void(OnvifResult)> SetPresetCallback;
	bool asyncSetPresest(const SetPresetCallback& callback, const OnvifClientDefs::ProfileInfo& info, const std::string& presetname, int timeoutms = 10000);

	//��ȡ��̨Ԥ��λ
	OnvifResult getPreset(const OnvifClientDefs::ProfileInfo& info, OnvifClientDefs::PresetInfos& presetinfo,int timeoutms = 10000);
	
	//�첽��ȡ��̨Ԥ��λ
	typedef Function<void(OnvifResult,const OnvifClientDefs::PresetInfos&)> GetPresetCallback;
	bool asyncGetPreset(const GetPresetCallback& callback, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);

	//ִ��Ԥ��λ
	OnvifResult gotoPreset(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);
	
	//�첽ִ��Ԥ��λ
	typedef Function<void(OnvifResult)> GotoPresetCallback;
	bool asyncGotoPreset(const GotoPresetCallback& callback, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);
	

	//ɾ��Ԥ��λ
	OnvifResult removePreset(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);

	//�첽ɾ����̨Ԥ��λ
	typedef Function<void(OnvifResult)> RemovePresetCallback;
	bool asyncRemovePreset(const RemovePresetCallback& callback, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);

	//---------------------------------------------EVENT���----------------------------------------//

	//�����¼����
	OnvifResult subscribeEvent(const OnvifClientDefs::Capabilities& capabilities, OnvifClientDefs::SubEventResponse& subeventresp,int timeoutms = 10000);
	
	typedef Function<void(OnvifResult, const OnvifClientDefs::SubEventResponse&) > SubscribeEventCallback;
	bool asyncSubscribeEvent(const SubscribeEventCallback& callback, const OnvifClientDefs::Capabilities& capabilities, int timeoutms = 10000);

	OnvifResult getEvent(const OnvifClientDefs::SubEventResponse& subeventresp,OnvifClientDefs::EventInfos& eventinfos,int timeoutms = 2*60000);

	typedef Function<void(OnvifResult,const OnvifClientDefs::EventInfos&)> GetEventCallback;
	bool asyncGetEvent(const GetEventCallback& callback, const OnvifClientDefs::SubEventResponse& subeventresp, int timeoutms = 2 * 60000);

	bool stopSubEvent();
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
	//������һ��OnvifClientDefs::DiscoveryInfo�յİ�����ʾ��������
	typedef Function<void, const shared_ptr<OnvifClientDefs::DiscoveryInfo>&> DisconveryCallback;
public:
	//userContent �û�������Ϣ,threadNum �߳���������RTSP���û�������
	OnvifClientManager(const shared_ptr<IOWorker>& worker,const std::string& userContent);
	~OnvifClientManager();

	//����onvif�豸��ַ����һ���豸,url Ϊ�豸��ַ������IP���˿ڣ��û������������Ϣ,onvifĬ������·��Ϊ "/onvif/device_service"
	//��:admin:admin@192.168.13.33
	shared_ptr<OnvifClient> create(const URL& url);

	//�����豸����ʱ���Զ�ֹͣ����
	bool disconvery(const DisconveryCallback& callback, uint32_t timeout = 10000);
private:
	struct OnvifClientManagerInternal;
	OnvifClientManagerInternal* internal;
};


}
}



#endif //__ONVIFCLIENT_H__
