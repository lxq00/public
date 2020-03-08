#pragma  once
#include "Base/Base.h"
#include "MSProtocol/MSProtocol.h"
#include "Connection.h"
using namespace Public::Base;
using namespace Milesight::Protocol;

namespace Milesight {
namespace MQ {


//system ������Э��master��
//�����system��������Э�̣��ȵ�
class MSMQ_API MSSystem
{
public:
	struct ServerInfo
	{
		MSProtoVMSSystemNotify		notify;
		std::string					recvaddr;	//ֱ�ӿ��Խ��յ��ĵ�ַ
	};
public:
	MSSystem(const shared_ptr<IOWorker>& ioworker, bool isserver);
	~MSSystem();


	//----------------֪ͨ����Լ�ϵͳ����Ϣ
	void notifySystemNotify(const MSProtoVMSSystemNotify& msg);
	
	//----------------�����н��ܵ���֪ͨ��Ϣ
	void inputRecvNotify(const NetAddr& addr,const MSProtoVMSSystemNotify& msg);

	//---------------��ȡ����ϵͳ��Ϣ
	ErrorInfo getAllSystemList(std::map<std::string, std::map<std::string, ServerInfo> >& systemlist);

	//---------------����ϵͳ��ȡ������Ϣ
	ErrorInfo getSystemServerList(const std::string& systemid, std::map<std::string, ServerInfo>& serverlist);

	//---------------��ȡ����ϵͳ��master����Ϣ
	std::string getSystemMasterServerId(const std::string& systemid);

	static bool systemNotifyCompare(const MSProtoVMSSystemNotify& notify1, const MSProtoVMSSystemNotify& notify2);

	//��ʱ������
	void onPoolTimerProc();
private:
	struct MSSystemInternal;
	MSSystemInternal* internal;
};


}
}