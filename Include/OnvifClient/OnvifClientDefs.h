#ifndef __ONVIFCLIENT_DEFS_H__
#define __ONVIFCLIENT_DEFS_H__
#include "Base/Base.h"
#include "Network/Network.h"
using namespace Public::Base;
using namespace Public::Network;

namespace Public {
namespace Onvif {
namespace OnvifClientDefs {


//�豸��Ϣ
struct Info
{
	std::string Manufacturer;		//������Ϣ
	std::string Model;				//�豸��Ϣ
	std::string FirmwareVersion;	//�̼��汾
	std::string SerialNumber;		//���к�
	std::string HardwareId;			//Ӳ����ʶ
//	std::string Name;				//�豸����
};

//�豸������
struct Capabilities
{
	struct _Medis{
		URL xaddr;
		BOOL RTPMulticast;
		BOOL RTP_TCP;
		BOOL RTP_RTSP_TCP;
		BOOL Support;

		_Medis():RTPMulticast(false),RTP_TCP(false),RTP_RTSP_TCP(false),Support(false){}
	}Media;
	struct _PTZ{
		URL		xaddr;
		BOOL        Support;

		_PTZ():Support(false){}
	}PTZ;
	struct _Events {
		URL		xaddr;
		BOOL        Support;

		_Events() :Support(false) {}
	}Events;
	struct _Mmessage {
		URL		xaddr;
		BOOL        Support;

		_Mmessage():Support(false){}
	}Mmessage;
} ;

//��ʱ������GetScopes��̫���ӣ�Ŀǰû����
typedef struct {

}Scopes;

struct _VideoSource {
	int  width, height, x, y;

	int  use_count;
	std::string token;
	std::string stream_name;
	std::string source_token;

	_VideoSource():width(0),height(0),x(0),y(0),use_count(0){}
};

typedef enum {
	VIDEO_ENCODING_JPEG = 0,
	VIDEO_ENCODING_MPEG4 = 1,
	VIDEO_ENCODING_H264 = 2,
	VIDEO_ENCODING_UNKNOWN = 3,
}VIDEO_ENCODING;

typedef enum {
	H264_PROFILE_Baseline = 0,
	H264_PROFILE_Main = 1,
	H264_PROFILE_Extended = 2,
	H264_PROFILE_High = 3,
}H264_PROFILE;

struct _VideoEncoder
{
	float quality;
	int  width;
	int  height;
	int  use_count;
	int  session_timeout;

	std::string name;
	std::string token;

	VIDEO_ENCODING  encoding;

	int  framerate_limit;
	int  encoding_interval;
	int  bitrate_limit;

	/* H264Configuration */
	int  gov_len;
	H264_PROFILE  h264_profile;

	_VideoEncoder():quality(0),width(0),height(0),use_count(0),session_timeout(0),framerate_limit(0),encoding_interval(0),bitrate_limit(0),gov_len(0){}
} ;

struct _Range
{
	float min;
	float max;

	_Range() { min = max = 0; }
};

struct PTZConfig
{
	int  use_count;
	int  def_timeout;

	std::string name;
	std::string token;
	std::string nodeToken;

	struct _Speed
	{
		int pan_tilt_x;
		int pan_tilt_y;
		int zoom;

		_Speed():pan_tilt_x(0),pan_tilt_y(0),zoom(0){}
	} def_speed;

	_Range pantilt_x;
	_Range pantilt_y;
	_Range zoom;

	PTZConfig():use_count(0),def_timeout(0){}
} ;

struct ConfigurationOptions
{
	int used;

	_Range absolute_pantilt_x;
	_Range absolute_pantilt_y;
	_Range absolute_zoom;

	_Range relative_pantilt_x;
	_Range relative_pantilt_y;
	_Range relative_zoom;

	_Range continuous_pantilt_x;
	_Range continuous_pantilt_y;
	_Range continuous_zoom;

	_Range pantilt_speed;
	_Range zoom_speed;
	_Range timeout;

	ConfigurationOptions():used(0){}
};

//������Ϣ
struct ProfileInfo {
	shared_ptr<_VideoSource> VideoSource;
	shared_ptr<_VideoEncoder> VideoEncoder;
	shared_ptr<PTZConfig> PTZConfig;


	std::string name;
	std::string token;
	bool fixed;

	ProfileInfo():fixed(false){}
};

struct Profiles {
	std::vector<ProfileInfo> infos;
};

struct NetworkInterfaces {
	std::string		name;
	std::string		macaddr;
	std::string		ipaddr;
	bool			dhcp;

	NetworkInterfaces():dhcp(false){}
};
typedef struct {

}VideoEncoderConfigurations;

struct PTZCtrl
{
	enum {
		PTZ_CTRL_PAN  =  0,
		PTZ_CTRL_ZOOM =  1,
	}ctrlType;
	double           panTiltX;
	double           panTiltY;
	float           zoom;
	int				duration;

	PTZCtrl():panTiltX(0),panTiltY(0),zoom(0),duration(0){}
} ;


struct StreamUrl
{
	std::string url;
};

struct SnapUrl
{
	std::string url;
};

struct StartRecvAlarm
{
	URL				xaddr;
	Time			currentTime;
	Time			terminationTime;
};

struct AlarmInfo
{
	Time			arrivalTime;
	std::string		topic;
	std::string		operation;
	std::map<std::string, std::string> sources;
	std::map<std::string, std::string> datas;
};

struct RecvAlarmInfo
{
	std::vector<AlarmInfo> alarminfos;
};

struct DiscoveryInfo
{
	std::string		name;
	std::vector<std::string> addrs;
};

struct PresetInfo
{
	std::string token;
	std::string name;
};

struct PresetInfos
{
	std::vector<PresetInfo> infos;
};

}
}
}



#endif //__ONVIFCLIENT_H__
