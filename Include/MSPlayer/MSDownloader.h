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
	
class MSPLAYER_API MSDownloader
{
public:
	// url �����Ǳ���ý���ļ���rtsp��ַ
	// saveFileName ����ı����ļ�
	MSDownloader(const std::string& url, const std::string& saveFileName);

	MSDownloader(const std::shared_ptr<ISource> &source, const std::string& saveFileName);

	virtual ~MSDownloader();

	// ��ʼ��
	static ErrorInfo init();

	// ����ʼ��
	static ErrorInfo uninit();

	// ��ʼ����
	ErrorInfo start();

	// ֹͣ����
	ErrorInfo stop();

	// ��ȡ�Ѿ�����¼������ʱ��
	ErrorInfo getDownloadTime(uint32_t &downloadTime);

	// ���������ٶ�
	ErrorInfo setDownloadSpeed(Play_Speed speed);

	// ��ȡ��ǰ�����ٶ�
	ErrorInfo getDownloadSpeed(Play_Speed &speed);

	// �Ƿ�ʹ����Ƶ
	ErrorInfo enableVideo(bool enable);

	// �Ƿ�ʹ����Ƶ
	ErrorInfo enableAudio(bool enable);

	// ��������״̬�ص�
	ErrorInfo setDownloadStatusCallback(const StatusCallback &callback);

	// ��ȡ��Ƶ��������
	// @param codeid ��Ƶ��������
	ErrorInfo getVideoCode(CodeID &codeid);

	// ��ȡ��Ƶ��������
	// @param codeid ��Ƶ��������
	ErrorInfo getAudioCode(CodeID &codeid);

	// ��ȡ��λʱ���ڵĴ����С�������ļ�������
	// @param bitRate ��ȡ���յ�����������С����λkbps
	ErrorInfo getBandWidth(uint32_t& bitRate);

	// ��ȡ��λʱ���ڽ��յ���Ƶ֡���������ļ�������
	// @param frameRate ��λʱ���ڽ��յ���֡��
	ErrorInfo getRecviceFrameRate(uint32_t &frameRate);

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

	// ���ʱ��ˮӡ
	ErrorInfo addTimeMark(const std::string& format, const std::string& fontFile, uint32_t fontSize, uint32_t color, uint32_t x, uint32_t y);
private:
	struct MSDownloadInternal;
	MSDownloadInternal *internal;
};
}
}