//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: tcpclient.h 216 2015-12-15 11:33:55Z  $
//
#ifndef __NETWORK_Message_H__
#define __NETWORK_Message_H__

#include "Network/Socket/Socket.h"
#include "Address.h"
namespace Public{
namespace Network{
namespace Email{
	
//�ʼ���Ϣ
class NETWORK_API Message
{
public:
	Message();
	Message(const Message& msg);
	~Message();

	//�����ߵ�ַ
	Address& from();
	const Address& from() const;

	//Ŀ�ĵص�ַ
	std::vector<Address>& to();
	const std::vector<Address>& to() const;

	//���͵�ַ
	std::vector<Address>& copyTo();
	const std::vector<Address>& copyTo() const;

	//���͵�ַ blind carbon copy
	std::vector<Address>& bccTo();
	const std::vector<Address>& bccTo() const;

	//����
	std::string& subject();
	const std::string& subject() const;

	//����
	std::vector<std::string>& attachments();
	const std::vector<std::string>& attachments() const;

	//����
	std::string& content();
	const std::string& content() const;
private:
	struct MessageInternal;
	MessageInternal* internal;
};


};
};
};




#endif //__NETWORK_SOCKET_H__

