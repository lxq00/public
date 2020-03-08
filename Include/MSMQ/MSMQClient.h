#pragma  once
#include "Message.h"
#include "Connection.h"
#include "MSDispatcher.h"
#include "MSSystem.h"
namespace Milesight {
namespace MQ {

//����ֻ�Ǹ��ͻ���ʹ�õ�
//��Ϣ����
class MSMQ_API MSMQClient :public MSDispatcher
{
public:
	typedef Function<void(const MSProtoVMSSystemNotify&)> QuerySytemInfoCallback;
public:
	//Ĭ����Ϣ�����߳���
	MSMQClient(const shared_ptr<IOWorker>& ioworker, const shared_ptr<MSSystem>&mssystem, ConnectionType type = ConnectionType_ConnectTCP);
	~MSMQClient();

	//����ϵͳ��Ϣ����
	ErrorInfo startBySystemId(const std::string& systemId);
	//���ݷ����ַ����
	ErrorInfo startByServerIp(const std::string& serverIp);

	void registerQuerySystemInfoCallback(const QuerySytemInfoCallback& callback);

	//--------ֹͣ����
	ErrorInfo stop();

	shared_ptr<Connection> connection();

	std::string systemId();
	std::string masterId();

	//��ʱ������
	void onPoolTimerProc();
private:
	struct MSMQClientInternal;
	MSMQClientInternal* internal;
};

}
}