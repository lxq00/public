#ifndef __HTTPPUBLIC_H__
#define __HTTPPUBLIC_H__

#include "../Defs.h"
#include "Base/Base.h"
#include "Network/Socket/Socket.h"
#include "Serialization.h"
using namespace Public::Base;

namespace Public {
namespace Network {
namespace HTTP {

//
////HTMLģ���滻
////{{name}}	��������Ϊname
////{{#starttmp}} {{/starttmp}}		ѭ������ʼ�ͽ��� ѭ������Ϊstarttmp
//class HTTP_API HTTPTemplate
//{
//public:
//	struct TemplateObject
//	{
//		TemplateObject() {}
//		virtual ~TemplateObject() {}
//
//		//�����������ģ������ֵ std::map<��������,����ֵ> 
//		virtual bool toTemplateData(std::map<std::string, Value>& datamap) = 0;
//	};
//	//ѭ�����������ֵ�
//	struct TemplateDirtionary
//	{
//		TemplateDirtionary() {}
//		virtual ~TemplateDirtionary() {}
//		virtual TemplateDirtionary* setValue(const std::string& key, const Value&  value) = 0;
//	};
//public:
//	HTTPTemplate(const std::string& tmpfilename);
//	virtual ~HTTPTemplate();
//	//����������ֵ
//	HTTPTemplate& setValue(const std::string& key, const Value&  value);
//	//ѭ������������ѭ�� HTTPTemplate
//	HTTPTemplate& setValue(const std::string& key, const std::vector<TemplateObject*>&  valuelist);
//	//���ѭ������
//	shared_ptr<TemplateDirtionary> addSectionDirtinary(const std::string& starttmpkey);
//
//	std::string toValue() const;
//private:
//	struct HTTPTemplateInternal;
//	HTTPTemplateInternal* internal;
//};


typedef enum {
	WebSocketDataType_Txt,
	WebSocketDataType_Bin,
}WebSocketDataType;

typedef enum {
	CacheType_Mem = 0,
	CacheType_File,
}CacheType;

class Communication;

class NETWORK_API IContent
{
public:
	IContent() {}
	virtual ~IContent() {}

	virtual uint32_t size() = 0;
	virtual uint32_t append(const char* buffer, uint32_t len, bool & endoffile) = 0;
	virtual std::string read() = 0;
};

class NETWORK_API WriteContenNotify
{
public:
	WriteContenNotify() {}
	virtual ~WriteContenNotify() {}

	virtual void WriteNotify() = 0;
	virtual void ReadReady() = 0;
};

class NETWORK_API ChunkData
{
public:
	typedef Function<uint32_t(const char*, uint32_t)> ReadCallback;
	typedef Function<uint32_t(const char*, uint32_t)> WriteCallback;

	ChunkData();
	ChunkData(const WriteCallback& writecallback);
	~ChunkData();

	void setReadCallback(const ReadCallback& readcallback);

	uint32_t append(const char* buffer, uint32_t len, bool & endoffile);
	void write(const char* buffer, uint32_t len);
private:
	struct ChunkDataInternal;
	ChunkDataInternal* internal;
};

class NETWORK_API ReadContent :public IContent
{
public:
	typedef Function<void(const char*, uint32_t)> DataCalback;
public:
	ReadContent(const shared_ptr<Header>& header, WriteContenNotify* notify, CacheType type, const std::string& filename = "");
	~ReadContent();

	uint32_t size();

	std::string cacheFileName() const;
	uint32_t read(char* buffer, uint32_t maxlen, uint32_t pos) const;
	std::string read();
	bool readToFile(const std::string& filename) const;

	void setDataCallback(const DataCalback& callback);
private:
	virtual uint32_t append(const char* buffer, uint32_t len, bool & endoffile);
private:
	struct ReadContentInternal;
	ReadContentInternal* internal;
};

class NETWORK_API WriteContent :public IContent
{
public:
	WriteContent(const shared_ptr<Header>& header, WriteContenNotify* notify, CacheType type);
	~WriteContent();

	uint32_t write(const char* buffer, uint32_t len);
	uint32_t write(const std::string& buffer);
	bool writeFromFile(const std::string& filename, bool needdeletefile = false);

	void writeChunk(const char* buffer, uint32_t len);
	void writeChunkEnd();
private:
	virtual uint32_t size();
	virtual uint32_t append(const char* buffer, uint32_t len, bool & endoffile);
	virtual std::string read();
private:
	struct WriteContentInternal;
	WriteContentInternal* internal;
};


}
}
}
#endif