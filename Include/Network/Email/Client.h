//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: tcpclient.h 216 2015-12-15 11:33:55Z  $
//
#ifndef __NETWORK_EMAILCLIENT_H__
#define __NETWORK_EMAILCLIENT_H__

#include "Network/Socket/Socket.h"
#include "Address.h"
#include "Message.h"

namespace Public{
namespace Network{
namespace Email{


class NETWORK_API Client
{
public:
	enum ProtocolType
	{
		ProtocolType_IMAP = 0,	//���ʼ����������ļ��е�
		ProtocolType_SMTP,		//�����ʼ�
		ProtocolType_POP3,		//���ʼ�

		ProtocolType_IMAP_SSL,	//���ʼ����������ļ��е�
		ProtocolType_SMTP_SSL,	//�����ʼ�
		ProtocolType_POP3_SSL,	//���ʼ�
	};
public:
	Client(const shared_ptr<IOWorker>& worker, const std::string& emailserveraddr, uint32_t emailserverport, ProtocolType type = ProtocolType_SMTP);
	virtual ~Client();

	//��¼
	virtual ErrorInfo login(const std::string& email, const std::string& password, uint32_t timeout = 5000);
	virtual void loginout();

	//������Ϣ
	virtual ErrorInfo submit(const shared_ptr<Message>& msg);
	
	//������Ϣ
	virtual ErrorInfo fetch(uint32_t index, shared_ptr<Message>& msg);
private:
	struct ClientInternal;
	ClientInternal* internal;
};
}
}
}




#endif //__NETWORK_SOCKET_H__

