#pragma  once
#include "Message.h"
#include "Connection.h"
#include "MSDefine.h"
#include "MSDispatcher.h"
#include "MSSystem.h"

namespace Milesight {
namespace MQ {

/*

	Milesight Message Queue ����

	//��Ϣ���з���ģʽ/��topicΪ����ý��/topic�����ִ�Сд

	[connection 1]	\                                                     / [connection 3]
					  --------publish------- [topic] -------subscribe-----
	[connection 2]  /                                                     \ [connection 4]


*/


//���ﶨ�����milesight Message Queue
//��MQ֧��HTTP/Websocket/TCP/direct��ͨѶ
//�����Ǹ�������õģ���Ϣ��ת��
class MSMQ_API MSMQServer :public MSDispatcher
{
public:
	typedef Function<void(const shared_ptr<Socket>&, const char* buffer, uint32_t bufferlen)> RTSPSocketCallblack;
public:
	//Ĭ����Ϣ�����߳���
	MSMQServer(uint32_t threadnum);
	~MSMQServer();

	//---------����listen connectionģʽ����Ϊ����ˣ�����ͬʱ��������˿�
	//server����������server�����˿�,listen
	ErrorInfo start();
	//--------ֹͣ����
	ErrorInfo stop();

	shared_ptr<IOWorker> ioWorker();
	void registeRTSPSocketCallback(const RTSPSocketCallblack& callback);

	shared_ptr<MSSystem> system();

	//----------------֪ͨ����Լ�ϵͳ����Ϣ
	void notifySystemNotify(const MSProtoVMSSystemNotify& msg);


	//----------����connect connectionģʽ
	shared_ptr<Connection> getServerConnection(const std::string& serverid);	


	//---------------------�������direct connectionģʽ
	//����Ϊͬ������,������������Ļظ���
	ErrorInfo invok(const std::string& topic, const MSPacket* pkt, MSPacket* response = NULL);
	ErrorInfo invok(const std::string& topic, MSPacket* response = NULL);
	virtual shared_ptr<Message> invok(const std::string& topic, const shared_ptr<Message>& msg);

	virtual ErrorInfo directNotify(const std::string& topic, const MSPacket* msg);
	shared_ptr<Connection> directNotifyConnection();
private:
	struct MSMQServerInternal;
	MSMQServerInternal* internal;
};

}
}