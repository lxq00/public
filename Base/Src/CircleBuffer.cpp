//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: ByteOrder.h 3 2013-01-21 06:57:38Z  $
//

#include "Base/CircleBuffer.h"
namespace Public {
namespace Base {

struct CircleBuffer::CircleBufferInternal
{
	char*		bufferAddr;	//buffer��ַ
	uint32_t	bufferLen;	//buffer����

	uint32_t	dataLen;	//���ݳ��� 
	uint32_t	consumePos;	//������λ��
	uint32_t	productionPos;	//������λ��
};

CircleBuffer::CircleBuffer(uint32_t buffersize)
{
	internal = new CircleBufferInternal;
	internal->bufferLen = buffersize;
	internal->bufferAddr = new char[buffersize];
	internal->dataLen = 0;
	internal->consumePos = 0;
	internal->productionPos = 0;
}
CircleBuffer::~CircleBuffer()
{
	SAFE_DELETEARRAY(internal->bufferAddr);
	SAFE_DELETE(internal);
}

uint32_t CircleBuffer::dataLenght() const
{
	return internal->dataLen;
}
	
StringBuffer CircleBuffer::consumeBuffer(uint32_t pos, uint32_t length)
{
	StringBuffer buffer = readBuffer(pos, length);
	
	setConsumeLength(buffer.length());

	return buffer;
}

bool CircleBuffer::consumeLine(std::string& str, const std::string& flag)
{
	const char* flagaddr = flag.c_str();
	uint32_t flaglen = flag.length();
	uint32_t datalen = dataLenght();

	str.reserve(datalen + 10);

	uint32_t readpos = 0;
	bool havefindend = false;
	while (readpos < datalen)
	{
		str.append(1, readChar(readpos));
		readpos++;

		if (readpos >= flaglen && memcmp(str.c_str() + readpos - flaglen, flagaddr, flaglen) == 0)
		{
			havefindend = true;
			break;
		}
	}
	
	if (havefindend)
	{
		str.resize(readpos - flaglen);

		setConsumeLength(readpos);
	}
	else
	{
		str.resize(0);
	}

	return havefindend;
}

const char* CircleBuffer::getConsumeAddr() const
{
	return internal->bufferAddr + internal->consumePos;
}

uint32_t CircleBuffer::getConsumeLength() const
{
	if (internal->dataLen == 0) return 0;

	if (internal->productionPos <= internal->consumePos) return internal->bufferLen - internal->consumePos;
	else return internal->productionPos - internal->consumePos;
}
bool CircleBuffer::setConsumeLength(uint32_t length)
{
	if (length > internal->dataLen) return false;

	internal->dataLen -= length;
	internal->consumePos += length;

	if (internal->consumePos >= internal->bufferLen)
		internal->consumePos = internal->consumePos - internal->bufferLen;

	return true;
}

char* CircleBuffer::getProductionAddr() const
{
	return internal->bufferAddr + internal->productionPos;
}

uint32_t CircleBuffer::getProductionLength() const
{
	if (internal->dataLen == internal->bufferLen) return 0;

	if (internal->productionPos >= internal->consumePos) return internal->bufferLen - internal->productionPos;
	else return internal->consumePos - internal->productionPos;
}

bool CircleBuffer::setProductionLength(uint32_t length)
{
	if (length > internal->bufferLen - internal->dataLen) return false;

	internal->dataLen += length;
	internal->productionPos += length;

	if (internal->productionPos >= internal->bufferLen)
		internal->productionPos = internal->productionPos - internal->bufferLen;

	return true;
}

bool CircleBuffer::production(const char* bufferaddr)
{
	if (bufferaddr == NULL) return false;

	return production(bufferaddr, strlen(bufferaddr));
}
bool CircleBuffer::production(const char* bufferaddr, uint32_t bufferlen)
{
	if (bufferaddr == NULL || bufferlen <= 0) return false;

	if (bufferlen > internal->bufferLen - internal->dataLen) return false;

	while (bufferlen > 0)
	{
		uint32_t usedlen = 0;
		if (internal->productionPos >= internal->consumePos)
		{
			usedlen = min(internal->bufferLen - internal->productionPos, bufferlen);
		}
		else
		{
			usedlen = min(internal->consumePos - internal->productionPos, bufferlen);
		}

		memcpy(internal->bufferAddr + internal->productionPos, bufferaddr, usedlen);

		setProductionLength(usedlen);
		bufferlen -= usedlen;
		bufferaddr += usedlen;
	}

	return true;
}
bool CircleBuffer::production(const std::string& str)
{
	return production(str.c_str(), str.length());
}
bool CircleBuffer::production(const String& str)
{
	return production(str.c_str(), str.length());
}
bool CircleBuffer::production(const StringBuffer& str)
{
	if (str.length() > internal->bufferLen - internal->dataLen) return false;

	uint32_t offset = 0;
	uint32_t readlen = 0;

	while (readlen < str.length())
	{
		char* productionAddr = getProductionAddr();
		uint32_t producationLen = getProductionLength();

		uint32_t canreadlen = min(producationLen, str.length() - readlen);

		str.read(offset, productionAddr, canreadlen);

		setProductionLength(canreadlen);

		offset += canreadlen;
		readlen += canreadlen;
	}

	return true;
}
char CircleBuffer::readChar(uint32_t pos) const
{
	if (pos >= internal->dataLen) return 0;

	uint32_t readpos = internal->consumePos + pos;

	if (readpos >= internal->bufferLen)
		readpos = readpos - internal->bufferLen;


	return internal->bufferAddr[readpos];
}
bool CircleBuffer::readBuffer(uint32_t pos, void* dstptr, uint32_t length) const
{
	if (pos < 0 || length + pos > internal->dataLen || dstptr == NULL || length <= 0) return false;

	char* buffer = (char*)dstptr;

	uint32_t readpos = internal->consumePos + pos;

	if (readpos >= internal->bufferLen)
		readpos = readpos - internal->bufferLen;

	while (length > 0)
	{
		uint32_t usedlen = 0;
		if (readpos >= internal->productionPos)
		{
			usedlen = min(internal->bufferLen - readpos, length);
		}
		else
		{
			usedlen = min(internal->productionPos - readpos, length);
		}

		memcpy(buffer, internal->bufferAddr + readpos, usedlen);

		readpos += usedlen;
		if (readpos >= internal->bufferLen) 
			readpos -= internal->bufferLen;
		
		length -= usedlen;
		buffer += usedlen;
	}

	return true;
}

StringBuffer CircleBuffer::readBuffer(uint32_t pos,  uint32_t length)
{
	StringBuffer buffer;

	if (length == -1) length = internal->dataLen;

	if (internal->dataLen == 0 || pos < 0 || length + pos > internal->dataLen) return buffer;

	uint32_t readpos = internal->consumePos + pos;

	if (readpos >= internal->bufferLen)
		readpos = readpos - internal->bufferLen;


	while (length > 0)
	{
		const char* bufferAddr = internal->bufferAddr + readpos;
		uint32_t bufferLen = 0;

		//�������������ߺ��棬��ô����ֻ�� ������-������ ����
		if (readpos <= internal->productionPos)
		{
			bufferLen = min(length, internal->productionPos - readpos);
		}
		//��������������ǰ�棬��ô������ buffer-������ + ������ ����
		else
		{
			bufferLen = min(internal->bufferLen - readpos, length);
		}

		buffer.push_back(bufferAddr, bufferLen);

		readpos += bufferLen;
		if (readpos >= internal->bufferLen)
			readpos = readpos - internal->bufferLen;

		length -= bufferLen;
	}

	return buffer;
}
char  CircleBuffer::operator[](uint32_t pos) const
{
	return readChar(pos);
}

}
}

