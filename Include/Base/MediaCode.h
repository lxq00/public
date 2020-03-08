//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: Md5.h 3 2013-01-21 06:57:38Z  $
//

#ifndef __BASE_MEDIACODE_H__
#define __BASE_MEDIACODE_H__

#include <stddef.h>
#include "Base/IntTypes.h"
#include "Defs.h"

//���ⶨ������Ƶ��ص���ý����Ϣ

namespace Public {
namespace Base {


//֡��������,ֵ��ҪС��255
typedef enum
{
	FrameDataType_Unknown = 0,
	FrameDataType_YUV420 = 1,	//YUV 420
	FrameDataType_RGB24,
	FrameDataType_RGB32,
	FrameDataType_YV12 ,		//YV12
	FrameDataType_NV12,			//NV12
	FrameDataType_DXVA2_VLD,	//DAVA2
	FrameDataType_CUDA,
	FrameDataType_PCM,			//PCM
}FrameDataType;

//֡����/I/P/B/VUDIO/AUDIO,ֵ��ҪС��255
typedef enum
{
	FrameType_Unknown = 0,
	FrameType_Video_IFrame = 1,		// ����SPS��PPS��SEI
	FrameType_Video_PFrame,
	FrameType_Video_BFrame,
	FrameType_Video_SPS,
	FrameType_Video_PPS,
	FrameType_Video_SEI,
	FrameType_Video_IDR,
	FrameType_Video,
	FrameType_Audio,
}FrameType;

//����Ƶ���뷽ʽ,ֵ��ҪС��255
typedef enum {
	CodeID_Unknown = 0,
	CodeID_Video_H264 = 1,			//H264����
	CodeID_Video_H265,
	CodeID_Video_MPEG4,
	CodeID_Video_JPEG,
	CodeID_Video_H263,

	CodeID_Audio_AAC = 100,
	CodeID_Audio_G726,
	CodeID_Audio_G722,
	CodeID_Audio_G711A,
	CodeID_Audio_G711Mu,
	CodeID_Audio_MP3,
	CodeID_Audio_ADPCM,
	CodeID_Audio_PCM,
}CodeID; //��������

}
}

#endif