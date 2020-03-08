#pragma  once
#include "Message.h"
#include "Connection.h"

namespace Milesight {
namespace MQ {

class Connection_Interface;
//������MSMQ��Ϣ�ַ�����
class MSMQ_API MSDispatcher:public enable_shared_from_this<MSDispatcher>
{
public:
	MSDispatcher();
	~MSDispatcher();
	//---------------------����Ϊ��Ϣ���ķ���,������Ϣ���ĵ�Ϊȫ����Ϣ����������connection��������Ϣ
	//--------------------��Ϣ������Ҫͨ��topic��������Ϣ

	//���������
	template<typename O>
	void subscribe(const std::string& topic, shared_ptr<Message>(O::*callback)(const shared_ptr<Message>&), O* o)
	{
		subscribe(topic, o, RequestMessageCallback(callback, o), ResponseMessageCallback());
	}
	//���������
	template<typename O>
	void subscribe(const std::string& topic, shared_ptr<Message>(O::*callback)(const shared_ptr<Message>&), const shared_ptr<O>& o)
	{
		subscribe(topic, o.get(), RequestMessageCallback(callback, o), ResponseMessageCallback());
	}
	//���Ļظ���
	template<typename O>
	void subscribe(const std::string& topic, void(O::*callback)(const shared_ptr<Message>&), O* o)
	{
		subscribe(topic, o, RequestMessageCallback(), ResponseMessageCallback(callback, o));
	}
	//���������
	template<typename O>
	void subscribe(const std::string& topic, void(O::*callback)(const shared_ptr<Message>&), const shared_ptr<O>& o)
	{
		subscribe(topic, o.get(), RequestMessageCallback(), ResponseMessageCallback(callback, o));
	}

	//������Ϣ
	void subscribe(const std::string& topic, void* flag, const RequestMessageCallback& callback, const ResponseMessageCallback& ackcallback);
	void unsubcribe(void* flag);

	void connectionRecvCallback(const shared_ptr<Connection_Interface>& connection, const shared_ptr<Message>& msg);
private:
	struct MSDispatcherInternal;
	MSDispatcherInternal* internal;
};

}
}