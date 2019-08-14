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
	OnvifClient(const shared_ptr<IOWorker>& worker, const URL& url,const std::string& useragent);
public:
	typedef Function<void, const std::string&> AlarmCallback;
public:
	~OnvifClient();

	shared_ptr<OnvifClientDefs::Info> getInfo(int timeoutms = 10000);			//��ȡ�豸��Ϣ��������Ϣʹ��XM_GetLastError����

	typedef Function<void, shared_ptr<OnvifClientDefs::Info> > GetInfoCalblack;
	bool asyncGetInfo(const GetInfoCalblack& callback, int timeoutms = 10000);

	shared_ptr<OnvifClientDefs::Capabilities> getCapabities(int timeoutms = 10000);	//��ȡ�豸�������ϣ�������Ϣʹ��XM_GetLastError����

	typedef Function<void, shared_ptr<OnvifClientDefs::Capabilities> > GetCapabilitiesCallback;
	bool asyncGetCapabities(const GetCapabilitiesCallback& callbac, int timeoutms = 10000);
																					
//	shared_ptr<OnvifClientDefs::Scopes> getScopes(int timeoutms = 10000); //��ȡ������Ϣ��������Ϣʹ��XM_GetLastError����
	
	//��ȡ����Ƶ/��̨����Ϥ
	shared_ptr<OnvifClientDefs::Profiles> getProfiles(int timeoutms = 10000); //��ȡ������Ϣ��������Ϣʹ��XM_GetLastError����
	
	typedef Function<void, shared_ptr<OnvifClientDefs::Profiles> > GetProfilesCallback;
	bool asyncGetProfiles(const GetProfilesCallback& callback, int timeoutms = 10000);


	//��ȡ��Ƶ���ŵ�ַ
	shared_ptr<OnvifClientDefs::StreamUrl> getStreamUrl(const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000); //��ȡ����Ϣ,������Ϣʹ��XM_GetLastError����
	
	typedef Function<void, shared_ptr<OnvifClientDefs::StreamUrl> > GetStreamUrlCallback;
	bool asyncGetStreamUrl(const GetStreamUrlCallback& callbac, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);

	//��ȡ��Ԥ����ͼ��ַ
	shared_ptr<OnvifClientDefs::SnapUrl> getSnapUrl(const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);	//��ȡ��ͼ��Ϣ��������Ϣʹ��XM_GetLastError����
	
	typedef Function<void, shared_ptr<OnvifClientDefs::SnapUrl> > GetSnapUrlCallback;
	bool asyncGetSnapUrl(const GetSnapUrlCallback& callbac, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);

	//��ȡ������Ϣ
	shared_ptr<OnvifClientDefs::NetworkInterfaces> getNetworkInterfaces(int timeoutms = 10000);//������Ϣ��������Ϣʹ��XM_GetLastError����

	typedef Function<void, shared_ptr<OnvifClientDefs::NetworkInterfaces> > GetNetworkInterfacesCallback;
	bool asyncGetNetworkInterfaces(const GetNetworkInterfacesCallback& callbac, int timeoutms = 10000);
																							   
//	shared_ptr<OnvifClientDefs::VideoEncoderConfigurations> getVideoEncoderConfigurations(int timeoutms = 10000); //��ȡ��Ƶ������Ϣ��������Ϣʹ��XM_GetLastError����
	
	//��̨���
	bool continuousMove(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	
	typedef Function<void, bool> ContinuousMoveCallback;
	bool asyncContinuousMoveCallback(const ContinuousMoveCallback& callback, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);
	
	//bool absoluteMove(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	bool stopPTZ(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000); //ֹͣ��̨
	
	typedef Function<void, bool> StopPTRCallback;
	bool asyncStopPtz(const StopPTRCallback& callbac, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000);

	//Ԥ��λ���
	bool setPreset(const OnvifClientDefs::ProfileInfo& info, const std::string& presetname, int timeoutms = 10000);//����Ԥ��λ
	
	typedef Function<void, bool> SetPresetCallback;
	bool asyncSetPresest(const SetPresetCallback& callbac, const OnvifClientDefs::ProfileInfo& info, const std::string& presetname, int timeoutms = 10000);

	shared_ptr<OnvifClientDefs::PresetInfos> getPreset(const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);//��ȡԤ��λ��Ϣ
	
	typedef Function<void, shared_ptr<OnvifClientDefs::PresetInfos> > GetPresetCallback;
	bool asyncGetPreset(const GetPresetCallback& callback, const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);

	bool gotoPreset(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);//����Ԥ��λ
	
	typedef Function<void, bool> GotoPresetCallback;
	bool asyncGotoPreset(const GotoPresetCallback& callback, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);
	
	bool removePreset(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);//ɾ��Ԥ��λ

	typedef Function<void, bool> RemovePresetCallback;
	bool asyncRemovePreset(const RemovePresetCallback& callback, const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);

	//��ȡ��̨������Ϣ
//	shared_ptr<OnvifClientDefs::PTZConfig> getConfigurations(int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
//	shared_ptr<OnvifClientDefs::ConfigurationOptions> getConfigurationOptions(const shared_ptr<OnvifClientDefs::PTZConfig>& ptzcfg,int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	
	//��ȡϵͳʱ��
	shared_ptr<Time> getSystemDatetime(int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	
	typedef Function<void, shared_ptr<Time> > GetSystemDateTimeCallback;
	bool asyncGetSystemDatetime(const GetSystemDateTimeCallback& callbck, int timeoutms = 10000);

	bool SetSystemDatetime(const Time& time, int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	
	typedef Function<void, bool> SetSystemDatetimeCallback;
	bool asyncSetSystemDatetime(const SetSystemDatetimeCallback& callback, const Time& time, int timeoutms = 10000);
	
	bool systemReboot(int timeoutms = 10000);//������Ϣʹ��XM_GetLastError����

	typedef Function<void, bool> SystemRebootCallback;
	bool asyncSystemReboot(const SystemRebootCallback& callback, int timeoutms = 10000);

	//�����¼����
	shared_ptr<OnvifClientDefs::StartRecvAlarm> startRecvAlarm(const shared_ptr<OnvifClientDefs::Capabilities>& capabilities,int timeoutms = 10000);
	
	typedef Function<void, shared_ptr<OnvifClientDefs::StartRecvAlarm> > StartRecvAlarmCallback;
	bool asyncStartRecvAlarm(const StartRecvAlarmCallback& callback, const shared_ptr<OnvifClientDefs::Capabilities>& capabilities, int timeoutms = 10000);

	shared_ptr<OnvifClientDefs::RecvAlarmInfo> recvAlarm(const shared_ptr<OnvifClientDefs::StartRecvAlarm>& alarminfo,int timeoutms = 2*60000);

	typedef Function<void, shared_ptr<OnvifClientDefs::RecvAlarmInfo> > RecvAlarmCallback;
	bool asyncRecvAlarm(const RecvAlarmCallback& callback, const shared_ptr<OnvifClientDefs::StartRecvAlarm>& alarminfo, int timeoutms = 2 * 60000);

	bool stopRecvAlarm();
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
