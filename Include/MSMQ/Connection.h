#pragma  once
#include "Base/Base.h"
#include "Network/Network.h"
#include "MSProtocol/MSProtocol.h"
#include "Defs.h"
#include "Message.h"

using namespace Public::Base;
using namespace Public::Network;
using namespace Milesight::Protocol;

#define MSMQWEBSOCKERTREQUEST		"/msmq"

namespace Milesight {
namespace MQ {
	
//���ﶨ���Ϊ��Ϣ�ص�������ֵΪ�ظ�����
typedef Function<shared_ptr<Message>(const shared_ptr<Message>&)> RequestMessageCallback;
typedef Function<void(const shared_ptr<Message>&)> ResponseMessageCallback;


//�Ը�connection����ֻ֧�ֶ����ӵ��û�
//�����connect connectionģʽ���Ͽ�����Զ�����
//�����listen connectionģʽ���Ͽ��󲻻��Զ�����
//�����direct connectionģʽ������Ͽ�

typedef enum
{
	ConnectionType_ConnectHTTP = 0,
	ConnectionType_ConnectWebSocket,
	ConnectionType_ConnectTCP,
	ConnectionType_ListenHTTP,
	ConnectionType_ListenWebsocket,
	ConnectionType_ListenTCP,
	ConnectionType_Direct,
}ConnectionType;


//���ﶨ���ΪMQͨѶ������,��������Ӷ������һ������
struct SendMessageInfo;

class MSMQ_API Connection
{
public:
	Connection();
	virtual ~Connection();

	virtual void disconnect();

	virtual ConnectionType type() const = 0;

	//��ȡconnection�Է��ĵ�ַ
	virtual NetAddr srcAddr() const = 0;

	virtual NetStatus status() = 0;

	void setSession(const std::string& session);

	virtual void onPoolTimerProc();

	//*****************************����Ϊ�첽������Ϣ�ӿ�
	//ֱ�Ӹ�toserver������Ϣ
	//-----------------------------��Щ��ֱ�Ӹ�toserver������Ϣ
	uint32_t pushRequest(const std::string& topic, const MSPacket* pkt, const ResponseMessageCallback& ackcallback = ResponseMessageCallback(), uint32_t timeout = 10000);
	uint32_t pushRequest(const std::string& toserver, const std::string& topic, const MSPacket* pkt, const ResponseMessageCallback& ackcallback = ResponseMessageCallback(), uint32_t timeout = 10000);
	uint32_t pushRequest(const std::string& topic, const std::string& body, const ResponseMessageCallback& ackcallback = ResponseMessageCallback(), uint32_t timeout = 10000);
	uint32_t pushRequest(const std::string& toserver, const std::string& topic, const std::string& body, const ResponseMessageCallback& ackcallback = ResponseMessageCallback(), uint32_t timeout = 10000);
	ErrorInfo pushRequest(const shared_ptr<Message>& msg, const ResponseMessageCallback& ackcallback = ResponseMessageCallback(), uint32_t timeout = 10000);
	ErrorInfo pushResponse(const shared_ptr<Message>& msg);
	ErrorInfo forwardMessage(const std::string& myserver, const std::string& toserver, const shared_ptr<Message>& msg, const MSPacket* extpkt = NULL);
	//*****************************����Ϊͬ��������Ϣ�ӿ�
	//ֱ�Ӹ�toserver������Ϣ
	virtual shared_ptr<Message> command(const std::string& toserver, const std::string& topic, const MSPacket* pkt, uint32_t timeout = 10000);
	virtual shared_ptr<Message> command(const shared_ptr<Message>& msg, uint32_t timeout = 10000);
private:
	virtual shared_ptr<SendMessageInfo> sendMessage(const shared_ptr<Message>& msg, const ResponseMessageCallback& ackcallback, uint32_t timeout) = 0;
private:
	struct ConnectionInternal;
	ConnectionInternal* internal;
};

}
}

