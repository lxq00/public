#pragma once

#ifndef _I_UNPACK_H_
#define _I_UNPACK_H_

#include "rtpAnalyzerStructs.h"
#include "RTSP/RTPAnalyzer.h"
using namespace Public::Base;
using namespace Public::RTSP;

#define VIEOD_FRAME_LEN		1024*1024
#define AUDIO_FRAME_LEN		1024*10
#define SHORT_BUFFER_LEN    128
#define MIDDLE_BUFFER_LEN   512
#define BIG_BUFFER_LEN		1024
#define LARG_BUFFER_LEN		2048


typedef RTPAnalyzer::FrameCallback CBFreamCallBack;

class IRtpAnalyzer
{

public:
	IRtpAnalyzer() {}
	virtual ~IRtpAnalyzer() {}

	///����RTP��
	///@[in] pBuf RTP ������
	///@[in] nBufSize RTP ������
	///@[in] nOffset ����buffer��ƫ����
	///@[return] 0:�������ݲ���һ֡�������ȴ�����, -1:���ִ���
	virtual int InputData(const RTPHEADER& rtpheader,const char *pBuf, unsigned short nBufSize) = 0;
};

#endif