#pragma once
#include "Base/Base.h"
#include "Defs.h"
#include "RTSP/RTSP.h"

using namespace Public::Base;
using namespace Public::RTSP;

#ifndef WIN32
typedef void* HWND;
#endif


namespace Milesight {
namespace Player {

struct FrameInfo
{
	CodeID codeid;					// ��������
	FrameType frameType;			// ֡����
	uint32_t frameNo;				// ֡���
	uint64_t timestamp;				// ʱ���
	uint64_t utctime;				// utcʱ��
	String data;					// ֡����

	int channels;					// ����
	int samplerate;					// ������
	int samplebits;					// ����λ��
};

struct DataInfo
{
	const char*		data = NULL;
	uint32_t		size = 0;
};

#define MAXVIDEODATASIZE	5

typedef struct {
	int				width = 0;				//��
	int				height = 0;				//��
	int				frameRate;				//֡��
	union
	{
		struct{
			uint8_t *pY;
			uint8_t *pU;
			uint8_t *pV;

			uint32_t linesizeY;			// Y �����Ŀ��
			uint32_t linesizeUV;		// UV �����Ŀ��
		} yuv;

		struct {
			uint8_t *pRgb;
			uint32_t linesize;			// rgbÿ�еĿ��
		} rgb;

		struct {
			uint8_t* lpSurface;			// �������
		} dxva;

	};
	FrameDataType	dataType;						//֡��������
}FrameVideoData;

typedef struct {
	int				sampleRate = 0;		// ������
	int				sampleBits = 0;		// ����λ��
	int				channels = 0;		// ����
	DataInfo		frameData;
	FrameDataType	dataType;		//��������
}FrameAudioData;

typedef enum
{
	ImageType_Jpeg = 0,		//JPEG
	ImageType_Bmp,			//BMP
} ImageType;

//�ö��󲻴������ݿ���
class MSPLAYER_API Frame
{
public:
	Frame() {}
	virtual ~Frame() {}

	virtual FrameType frametype()const = 0;
	virtual uint64_t timestamp() const = 0;
	virtual uint64_t utctime() const = 0;
	virtual shared_ptr<FrameVideoData> videodata() const = 0;
	virtual shared_ptr<FrameAudioData> audiodata() const = 0;
	virtual shared_ptr<Frame> converTo(FrameDataType type) { return shared_ptr<Frame>(); }
	virtual ErrorInfo snap(const std::string& filename, ImageType imgType) { return ErrorInfo(Error_Code_NotSupport); }
};

//����������
class MSPLAYER_API IDecoder
{
public:
	struct AudioInfo
	{
		int channels;
		int sampleRate;
		int sampleBits;
	};

	IDecoder() {}
	virtual ~IDecoder() {}

	virtual ErrorInfo init(CodeID code, const set<FrameDataType>& supportlist, const AudioInfo &info) = 0;		//��ʼ��
	virtual ErrorInfo uninit() = 0;					//����ʼ��

	virtual CodeID codeid() const = 0;				//��ȡ��������

	virtual std::vector<shared_ptr<Frame> > decoder(const std::shared_ptr<FrameInfo> &frameinfo) = 0;	//����
	
	virtual ErrorInfo clean() = 0;				//��ս��뻺����
};

//������Ⱦ
class MSPLAYER_API IRender
{
public:
	IRender(){}
	virtual ~IRender() {}

	virtual ErrorInfo init() = 0;				//��ʼ��

	virtual ErrorInfo uninit() = 0;				//����ʼ��

	virtual ErrorInfo setAlpha(float alpha) { return {}; };
	//��Ⱦ����/��Ƶ/��Ƶ
	virtual ErrorInfo display(const shared_ptr<Frame>& frame) = 0;

	virtual ErrorInfo setViewSize(int w, int h) { return {}; };

	virtual ErrorInfo setViewPort(int x, int y, int w, int h) { return {}; };

	virtual ErrorInfo draw(int left, int top, int right, int bottom) { return {}; };

	virtual ErrorInfo drawRectangle(int left, int top, int right, int bottom, uint32_t color, bool full) { return {}; };

	virtual ErrorInfo refresh() { return {}; };

	virtual FrameType type() = 0;		//��ȡ����

	virtual ErrorInfo clean() = 0;		//�����Ⱦ������

	virtual set<FrameDataType> supportDataType() = 0;

	//------------����Ϊ��Ƶrenderר��
	//0-100֮��
	virtual uint32_t getVolume() { return 0; }		//��ȡ����
	virtual ErrorInfo setVolume(uint32_t volume) {	return ErrorInfo(Error_Code_NotSupport);}	//��������

	virtual bool mute() {return false;}	//�Ƿ���
	virtual ErrorInfo setMute(bool mute) { return ErrorInfo(Error_Code_NotSupport); }	//���þ���

	virtual bool enable(bool en) { return false; }
};

typedef enum
{
	Play_Speed_Normal = 0,		//�����ٶ�,1����
	Play_Speed_Fast_2x,			//2���٣����
	Play_Speed_Fast_4x,			//4���٣����
	Play_Speed_Fast_8x,			//8���٣����
	Play_Speed_Fast_16x,		//16���٣����
	Play_Speed_Fast_32x,		//32���٣����

	Play_Speed_Slow_2x,			//1/2���٣�����
	Play_Speed_Slow_4x,			//1/4���٣�����
	Play_Speed_Slow_8x,			//1/8���٣�����
	Play_Speed_Slow_16x,		//1/16���٣�����
	Play_Speed_Slow_32x,		//1/32���٣�����
} Play_Speed;

typedef enum
{
	PlayStatus_Init = 0,		//��ʼ�� 
	PlayStatus_ConnectSuccess,	//���ӳɹ�
	PlayStatus_Play,			//����״̬
	PlayStatus_Pause,			//��ͣ 
	PlayStatus_FastPlay,		//���
	PlayStatus_SlowPlay,		//����
	PlayStatus_ForwardPlay,		//����
	PlayStatus_BackPlay,		//����
	PlayStatus_Error,			//����
	PlayStatus_EOF,				//����

	PlayStatus_Decode_Begin,		// ��Ƶÿ֡���뿪ʼ
	PlayStatus_Decode_End,			// ��Ƶÿ֡�������
	PlayStatus_Rendering_Begin,		// ��Ƶÿ֡��Ⱦ��ʼ
	PlayStatus_Rendering_End,		// ��Ƶÿ֡��Ⱦ����

	PlayStatus_Resolution,			// �ֱ��ʱ仯
} PlayStatus;

typedef enum
{
	direction_forward = 0,			// ����
	direction_backward,				// ����
} PlayDirect;

typedef enum 
{
	SourceType_Live = 0,		//ʵʱ����
	SourceType_NVRPlayback,		//NVR�ط�
	SourceType_VOD,				//���д洢�ط�
	SourceType_File,			//���ػط�
	SourceType_Talkback,		//�Խ�
}SourceType;

typedef enum
{
	StreamType_Video = 0,		// ��Ƶ
	StreamType_Audio,			// ��Ƶ
} StreamType;

//����״̬�ص�
typedef Function<void(PlayStatus status, const ErrorInfo& errinfo)> StatusCallback;

//��ƵԴ����,������ȡ��ʽ
class MSPLAYER_API ISource
{
public:
	struct AudioInfo
	{
		int nChannels;
		int nSampleRate;
		int nSampleBit;
	};

	struct VideoInfo
	{

	};
public:
	ISource(){}
	virtual ~ISource() {}

	virtual ErrorInfo startTime(Time& st) = 0;
	virtual ErrorInfo endTime(Time& et) = 0;
	virtual ErrorInfo start() = 0;		//��ʼ
	virtual ErrorInfo stop() = 0;		//ֹͣ
	virtual ErrorInfo registerStatusCallback(const StatusCallback& statuscallback) = 0;

	virtual ErrorInfo pause() = 0;		//��ͣ
	virtual ErrorInfo resume() = 0;		//�ָ�

	virtual Play_Speed playSpeed() = 0;	//��ȡ�����ٶ�
	virtual ErrorInfo setPlaySpeed(Play_Speed speed) = 0;	//���ò����ٶ�

	virtual std::set<StreamType> streamTypes() = 0;

	virtual ErrorInfo seek(uint64_t seekTime) = 0;				// ���ʱ��
	virtual ErrorInfo seekUTCTime(uint64_t utctime) { return {}; };		// utcʱ���

	virtual PlayStatus status() = 0;	//��ȡ����״̬

	virtual PlayDirect getDirection() = 0;

	virtual void setDirection(PlayDirect direction) = 0;

	virtual ErrorInfo read(FrameType& frametype, CodeID& codeid, shared_ptr<RTPFrame>& frame, uint64_t& timestmap, uint64_t &utctime, uint32_t timeout_ms) = 0; //��ȡ����

	virtual ErrorInfo write(FrameType frametype, CodeID codeid, std::shared_ptr<RTPFrame>& frame, uint64_t timestamp, uint32_t timeout_ms) = 0; // д������

	virtual AudioInfo getAuidoInfo() = 0;

	virtual ErrorInfo getVideoCode(CodeID &code) = 0;	// ��ȡ��Ƶ����

	virtual ErrorInfo getAudioCode(CodeID &code) = 0;	// ��ȡ��Ƶ����

	virtual ErrorInfo getResolution(uint32_t &width, uint32_t &height) = 0;	// ��ȡ��Ƶ�ֱ���

	virtual int32_t getVideoDelayTime() = 0;		// ��ȡ��Ƶ�ӳ�ʱ��

	// ��ȡ����
	virtual ErrorInfo getVideoBitRate(uint32_t &bitRate) = 0;

	// ��ȡ����
	virtual ErrorInfo getAudioBitRate(uint32_t &bitRate) = 0;

	// ��ȡ����
	virtual ErrorInfo getBandWidth(uint32_t &bitRate) = 0;

	// ��ȡ���յ���֡��
	virtual ErrorInfo getRecviceFrameRate(uint32_t &frameRate) = 0;

	//�������
	virtual ErrorInfo clean() = 0;

	virtual SourceType type() = 0;

	virtual uint32_t getCacheSize() = 0;

	virtual ErrorInfo getTotalTime(uint32_t &totalTime) = 0;
};

}
}