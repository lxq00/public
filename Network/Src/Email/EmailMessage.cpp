#include "Network/Email/Message.h"

namespace Public{
namespace Network{
namespace Email{
struct Message::MessageInternal
{
	Address				from;
	std::vector<Address>	to;
	std::vector<Address>	copy;
	std::vector<Address>	bcc;

	std::string					subject;
	std::string					content;

	std::vector<std::string>	attachments;
};

Message::Message()
{
	internal = new MessageInternal;
}
Message::Message(const Message& msg)
{
	internal = new MessageInternal;
	*internal = *msg.internal;
}
Message::~Message()
{
	SAFE_DELETE(internal);
}

//�����ߵ�ַ
Address& Message::from() { return internal->from; }
const Address& Message::from() const { return internal->from; }

//Ŀ�ĵص�ַ
std::vector<Address>& Message::to() { return internal->to; }
const std::vector<Address>& Message::to() const { return internal->to; }

//���͵�ַ
std::vector<Address>& Message::copyTo() { return internal->copy; }
const std::vector<Address>& Message::copyTo() const { return internal->copy; }

//���͵�ַ blind carbon copy
std::vector<Address>& Message::bccTo() { return internal->bcc; }
const std::vector<Address>& Message::bccTo() const { return internal->bcc; }

//����
std::string& Message::subject() { return internal->subject; }
const std::string& Message::subject() const { return internal->subject; }

//����
std::vector<std::string>& Message::attachments() { return internal->attachments; }
const std::vector<std::string>& Message::attachments() const { return internal->attachments; }

//����
std::string& Message::content() { return internal->content; }
const std::string& Message::content() const { return internal->content; }

}
};
};



