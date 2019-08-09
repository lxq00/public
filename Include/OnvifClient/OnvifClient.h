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
	~OnvifClient();

	shared_ptr<OnvifClientDefs::Info> getInfo(int timeoutms = 10000);			//��ȡ�豸��Ϣ��������Ϣʹ��XM_GetLastError����
	shared_ptr<OnvifClientDefs::Capabilities> getCapabities(int timeoutms = 10000);	//��ȡ�豸�������ϣ�������Ϣʹ��XM_GetLastError����
//	shared_ptr<OnvifClientDefs::Scopes> getScopes(int timeoutms = 10000); //��ȡ������Ϣ��������Ϣʹ��XM_GetLastError����
	
	//��ȡ����Ƶ/��̨����Ϥ
	shared_ptr<OnvifClientDefs::Profiles> getProfiles(int timeoutms = 10000); //��ȡ������Ϣ��������Ϣʹ��XM_GetLastError����
	
	//��ȡ��Ƶ���ŵ�ַ
	shared_ptr<OnvifClientDefs::StreamUrl> getStreamUrl(const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000); //��ȡ����Ϣ,������Ϣʹ��XM_GetLastError����
	
	//��ȡ��Ԥ����ͼ��ַ
	shared_ptr<OnvifClientDefs::SnapUrl> getSnapUrl(const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);	//��ȡ��ͼ��Ϣ��������Ϣʹ��XM_GetLastError����
	
	//��ȡ������Ϣ
	shared_ptr<OnvifClientDefs::NetworkInterfaces> getNetworkInterfaces(int timeoutms = 10000);//������Ϣ��������Ϣʹ��XM_GetLastError����
//	shared_ptr<OnvifClientDefs::VideoEncoderConfigurations> getVideoEncoderConfigurations(int timeoutms = 10000); //��ȡ��Ƶ������Ϣ��������Ϣʹ��XM_GetLastError����
	
	//��̨���
	bool continuousMove(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	bool absoluteMove(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	bool stopPTZ(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PTZCtrl& ptzctrl, int timeoutms = 10000); //ֹͣ��̨
	
	//Ԥ��λ���
	bool setPreset(const OnvifClientDefs::ProfileInfo& info, const std::string& presetname, int timeoutms = 10000);//����Ԥ��λ
	shared_ptr<OnvifClientDefs::PresetInfos> getPreset(const OnvifClientDefs::ProfileInfo& info, int timeoutms = 10000);//��ȡԤ��λ��Ϣ
	bool gotoPreset(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);//����Ԥ��λ
	bool removePreset(const OnvifClientDefs::ProfileInfo& info, const OnvifClientDefs::PresetInfo& presetinfo, int timeoutms = 10000);//ɾ��Ԥ��λ

	//��ȡ��̨������Ϣ
	shared_ptr<OnvifClientDefs::PTZConfig> getConfigurations(int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	shared_ptr<OnvifClientDefs::ConfigurationOptions> getConfigurationOptions(const shared_ptr<OnvifClientDefs::PTZConfig>& ptzcfg,int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	
	//��ȡϵͳʱ��
	shared_ptr<Time> GetSystemDatetime(int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	bool SetSystemDatetime(const Time& time, int timeoutms = 10000); //������Ϣʹ��XM_GetLastError����
	bool SystemReboot(int timeoutms = 10000);//������Ϣʹ��XM_GetLastError����

	//�����¼����
	shared_ptr<OnvifClientDefs::StartRecvAlarm> startRecvAlarm(const shared_ptr<OnvifClientDefs::Capabilities>& capabilities,int timeoutms = 10000);
	bool recvAlarm(const shared_ptr<OnvifClientDefs::StartRecvAlarm>& alarminfo,int timeoutms = 2*60000);
	bool stopRecvAlarm();
private:
	struct OnvifClientInternal;
	OnvifClientInternal* internal;
};

//OnvifClient������
class ONVIFCLIENT_API OnvifClientManager
{
	friend OnvifClient;
public:
	//������һ��OnvifClientDefs::DiscoveryInfo�յİ�����ʾ��������
	typedef Function1<void, const shared_ptr<OnvifClientDefs::DiscoveryInfo>&> DisconveryCallback;
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
