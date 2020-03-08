#pragma once 
#include "Defs.h"
#include "Base/Base.h"
using namespace Public::Base;


namespace Milesight {
namespace Protocol {

struct FiledDesInfo;
struct MSPacket
{
public:	
	virtual const char* getTypeName()const = 0;
//	std::vector<shared_ptr<FiledDesInfo>>	unknown_fields;
public:
	//���ڴ��н���������
	static bool MSProtocol_Export parseFromString(const std::string& str, MSPacket& pkg);
	//��json�ַ����н���������
	static bool MSProtocol_Export parseFromJson(const std::string& str, MSPacket& pkg);
	//���������л�Ϊ�ַ���
	static std::string MSProtocol_Export serializeAsString(const MSPacket& pkg);
	//���������л�Ϊjson
	static std::string MSProtocol_Export serializeAsJson(const MSPacket& pkg);
};


}
}