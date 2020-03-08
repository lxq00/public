#pragma once
#include "Defs.h"
#include "Base/Base.h"
#include "RTSPStructs.h"
using namespace Public::Base;

namespace Public
{
namespace RTSP
{

class RTSP_API RTPPackage
{
public:
	RTPPackage();
	RTPPackage(const String &data, uint32_t offset, uint32_t len);
	RTPPackage(const RTPPackage &package);
	~RTPPackage();

	//RTPͷ
	const RTPHEADER &rtpHeader() const;

	//RTP���ݵ�ַ��������ͷ����,������չ����
	const char *rtpDataAddr() const;
	//RTP���ݳ��ȣ�������ͷ����,������չ����
	uint32_t rtpDataLen() const;

	//��չ���ݳ���
	uint32_t rtpExternLen() const;

	const char *buffer() const;
	uint32_t bufferlen() const;

	RTPPackage &operator=(const RTPPackage &package);

private:
	String data;
	uint32_t offset = 0;
	uint32_t len = 0;
};

} // namespace RTSP
} // namespace Public