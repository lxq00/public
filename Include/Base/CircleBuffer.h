#ifndef __CIRCLE_BUFFER_H__
#define __CIRCLE_BUFFER_H__
//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: ByteOrder.h 3 2013-01-21 06:57:38Z  $
//

#include "Base/IntTypes.h"
#include "Base/Defs.h"
#include "Base/String.h"
#include "Base/StringBuffer.h"
namespace Public {
namespace Base {

//����buffer,���Բ���ȫ
class BASE_API CircleBuffer
{
public:
	CircleBuffer(uint32_t buffersize);
	~CircleBuffer();


	//���ݳ���
	uint32_t dataLenght() const;
	
	//�ú������������
	//return false��ʾ������,��ȡָ������ ���������ݣ���length = -1 ��ʾ��������
	StringBuffer consumeBuffer(uint32_t pos, uint32_t length = -1);

	//�ú������������
	//��ȡһ�����ݣ�\r\n�� \r��\nΪ������ʾ��return false��ʾʧ�ܣ�
	bool consumeLine(std::string& str,const std::string& flag="\r\n");

	//��ȡ�����ߵ�ַ 
	const char* getConsumeAddr() const;
	//��ȥ�����ߵ�ַ���ݳ��� 
	uint32_t getConsumeLength() const;

	//�ú����������������
	//���������ѵ����ݳ��ȣ�return false ��ʾʧ��
	bool setConsumeLength(uint32_t length);


	//��ȡ���������ڵĵ�ַ
	char* getProductionAddr() const;
	//��ȡ�����������ɵĳ��� 
	uint32_t getProductionLength() const;

	//�����������ɵĳ���
	bool setProductionLength(uint32_t length);

	//��������
	bool production(const char* str);
	bool production(const char* str, uint32_t len);
	bool production(const std::string& str);
	bool production(const String& str);
	bool production(const StringBuffer& str);

	//һ��read���������������
	//pos����С��dataLenght�����򷵻�0���ӽ���޷��жϳɹ�ʧ��
	char readChar(uint32_t pos) const;
	//��ȡ���ݣ��������ģ�return flash ��ʾʧ��
	bool readBuffer(uint32_t pos,void* dst, uint32_t length) const;
	StringBuffer readBuffer(uint32_t pos, uint32_t length = -1);

	//ͬgetcharͬ����
	char operator[](uint32_t pos) const;
private:
	struct CircleBufferInternal;
	CircleBufferInternal* internal;
};

}
}


#endif
