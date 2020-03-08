#pragma once
#include "Base/Base.h"
#include "Defs.h"
#include "iPlayer.h"
using namespace Public::Base;

#ifdef WIN32
#ifdef MSPLAYER_EXPORTS
#define MSPLAYER_API __declspec(dllexport)
#else
#define MSPLAYER_API __declspec(dllimport)
#endif
#else
#define MSPLAYER_API
#endif

namespace Milesight {
namespace Player {

class MSPLAYER_API MSPlayer
{
public:
	// url �����Ǳ���ý���ļ���rtsp��
	// window ���ھ�������ΪQTOPenglES��������NULL
	MSPlayer(const std::string& url, uint64_t window);

	MSPlayer(const std::shared_ptr<ISource> &source, uint64_t window);

	virtual ~MSPlayer();

	// ��ʼ��
	static ErrorInfo init();

	// ����ʼ��
	static ErrorInfo uninit();

	// ��ʼ����
	ErrorInfo play();

	// ֹͣ����
	ErrorInfo stop();

	ErrorInfo clear(int x, int y, int w, int h, uint32_t color);

	// ���QT���øÿⲢ��ʹ��opengles��Ⱦ������Ҫ��QT�̵߳��øýӿڣ���QT�������
	ErrorInfo refresh(int x = 0, int y = 0, int w = 0, int h = 0);

	// ��λ���ţ����ʱ�䣬��λ��
	ErrorInfo seek(uint64_t playTime);

	// ��λ���ţ�����ʱ�䣬��λ��
	ErrorInfo seekUTCTime(uint64_t playTime);

	// ��ͣ����
	ErrorInfo pause();

	// �ָ����ţ�ͬʱҲ�ָ���1����
	ErrorInfo resume();

	// ��֡����
	ErrorInfo step();

	// ץͼ,���浱ǰͼƬ
	ErrorInfo snap(const std::string& filename, ImageType imgType);

	// ��ȡÿ֡��Ⱦ��ʱ��
	ErrorInfo getFrameRendeTime(uint32_t &frameTime);

	// ��ȡ�Ѳ���ʱ��
	ErrorInfo getPlayedTime(uint64_t &playedTime);

	// ���ò����ٶ�
	ErrorInfo setPlaySpeed(Play_Speed speed);

	// ��ȡ��ǰ�����ٶ�
	ErrorInfo getPlaySpeed(Play_Speed &speed);

	// ���ò��ŷ��򣬵��š�����
	ErrorInfo setPlayDirect(PlayDirect eDirect);

	// ��ȡ��ǰ���ŷ���
	ErrorInfo getPlayDirect(PlayDirect &eDirect);

	// ��ȡ��ǰ����״̬������״̬��ͬʱ����
	ErrorInfo getPlayStatus(std::set<PlayStatus> &status);

	// ע�Ქ��״̬�ص�
	ErrorInfo registerPlayStatusCallback(const StatusCallback &callback);

	// ��ע�Ქ��״̬�ص�
	ErrorInfo unRegisterPlayStatusCallback(const StatusCallback &callback);

	// ��������¼��
	ErrorInfo startLocalRecord(const std::string& filename);

	// ֹͣ����¼��
	ErrorInfo stopLocalRecord();

	// ��ȡ��¼���ʱ�䣬��λ��
	ErrorInfo getRecordTime(uint32_t &recordTime);

	// ��ȡ�����ļ�����ʱ�䣬��λ��
	ErrorInfo getTotalTime(uint32_t &time);

	// ��ȡ������С��ȡֵ��Χ0 - 100
	ErrorInfo getVolume(uint32_t &volume);

	// ����������С��ȡֵ��Χ0 - 100
	ErrorInfo setVolume(uint32_t volume);

	// ʹ����Ƶ
	ErrorInfo enableVolume(bool enable);

	// ��ʼ���ӷŴ�
	ErrorInfo startDigital();

	// ���õ��ӷŴ�����ű���
	// @param zoomRate ���ű���ȡֵ��Χ(0.1~1.0); 1.0Ϊԭʼ����
	ErrorInfo setDigitalPTZZoom(float zoomRate);

	// ���õ��ӷŴ������ԭ��
	// @param xPos ����ڴ���(�ӿ�)�ߴ�ĺ�����
	// @param yPos ����ڴ���(�ӿ�)�ߴ��������
	ErrorInfo setDigitalPTZOrigin(uint32_t xPos, uint32_t yPos);

	// ��ȡ���ӷŴ�����ű��ʣ�ȡֵ��Χ 0.1 - 1.0
	// @param zoomRate ���ű��ʻ�ȡ
	ErrorInfo getDigitalPTZZoom(float &zoomRate);

	// ��ȡ���ӷŴ������ԭ��
	// @param xPos ��ȡ��ǰ���ӷŴ�ĺ�����
	// @param yPos ��ȡ��ǰ���ӷŴ��������
	ErrorInfo getDigitalPTZOrigin(uint32_t &xPos, uint32_t &yPos);

	// ֹͣ���ӷŴ�
	ErrorInfo stopDigital();

	// ���ͼƬˮӡ
	// @param filename ͼƬ�ļ�����֧��PNG͸��
	// @param x �����ͼ��ĺ�����
	// @param y �����ͼ���������
	ErrorInfo addPictureMark(const std::string& filename, int x, int y);

	// �������ˮӡ
	// @param txt ��������
	// @param fontFile �����ļ������ƣ�����Ϊ�գ�Ĭ��ʹ��΢���ź�
	// @param fontSize �����С
	// @param color ������ɫ��RGBA��ʽ
	// @param x �����ͼ��ĺ�����
	// @param y �����ͼ���������
	ErrorInfo addTxtMark(const std::string& txt, const std::string& fontFile, uint32_t fontSize, uint32_t color, uint32_t x, uint32_t y);

	// ��Ӿ��ο�ˮӡ
	// @param x �����ͼ��ĺ�����
	// @param y �����ͼ���������
	// @param width �����ͼ��Ŀ��
	// @param height �����ͼ��ĸ߶�
	// @param borderSize -1 ��ʾ������
	// @param color ���ο���ɫ��RGBA��ʽ
	ErrorInfo addRectangleMark(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t borderSize, uint32_t color);

	// ��ȡ��Ƶ֡��
	ErrorInfo getFrameRate(uint32_t &frameRate);

	// ��ȡ��ǰ�������ͼ���
	ErrorInfo getStreamTypes(std::set<StreamType>& streamTypes);

	// ��ȡ��Ƶ��������
	// @param codeid ��Ƶ��������
	ErrorInfo getVideoCode(CodeID &codeid);

	// ��ȡ��Ƶ��������
	// @param codeid ��Ƶ��������
	ErrorInfo getAudioCode(CodeID &codeid);

	// ��ȡ��Ƶ�ֱ���
	// @param width ���
	// @param height �߶�
	ErrorInfo getResolution(uint32_t &width, uint32_t &height);

	// ��ȡ��Ƶ����
	// @param bitRate ��Ƶ���ʴ�С
	ErrorInfo getVideoBitRate(uint32_t &bitRate);

	// ��ȡ��λʱ���ڵĴ����С�������ļ�����������
	// @param bitRate ��ȡ���յ�����������С����λkbps
	ErrorInfo getBandWidth(uint32_t& bitRate);

	// ��ȡ��λʱ���ڽ��յ���Ƶ֡���������ļ�����������
	// @param frameRate ��λʱ���ڽ��յ���֡��
	ErrorInfo getRecviceFrameRate(uint32_t &frameRate);
private:
	struct MSPlayerInternal;
	MSPlayerInternal *internal;

};
}
}