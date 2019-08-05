#include "HTTP/HTTPServer.h"
#include "HTTPDefine.h"
#include "HTTPCache.h"
#include "HTTPCommunication.h"
namespace Public {
namespace HTTP {

struct  HTTPServerRequest::HTTPServerRequestInternal
{
	DisconnectCallback		disconnectcallback;
	shared_ptr<HTTPHeader>	header;
	shared_ptr<ReadContent>	content;

	weak_ptr<Socket>		sock;
};

HTTPServerRequest::HTTPServerRequest(const shared_ptr<HTTPHeader>& header, const shared_ptr<ReadContent>& content, const shared_ptr<Socket>& sock)
{
	internal = new HTTPServerRequestInternal;
	internal->header = header;
	internal->content = content;
	internal->sock = sock;
}
HTTPServerRequest::~HTTPServerRequest()
{
	SAFE_DELETE(internal);
}

const std::map<std::string, Value>& HTTPServerRequest::headers() const
{
	return internal->header->headers;
}
Value HTTPServerRequest::header(const std::string& key) const
{
	return internal->header->header(key);
}

const std::string& HTTPServerRequest::method() const
{
	return internal->header->method;
}

URL HTTPServerRequest::url()const
{
	return internal->header->url;
}

const shared_ptr<ReadContent>& HTTPServerRequest::content() const
{
	return internal->content;
}

NetAddr HTTPServerRequest::remoteAddr() const
{
	shared_ptr<Socket> sock = internal->sock.lock();
	if (sock == NULL) return NetAddr();
		
	return sock->getOtherAddr();
}

NetAddr HTTPServerRequest::myAddr() const
{
	shared_ptr<Socket> sock = internal->sock.lock();
	if (sock == NULL) return NetAddr();

	return sock->getMyAddr();
}

HTTPServerRequest::DisconnectCallback&	HTTPServerRequest::discallback()
{
	return internal->disconnectcallback;
}




struct HTTPServerResponse::HTTPServerResponseInternal :public WriteContenNotify
{
	shared_ptr<HTTPHeader> header;
	shared_ptr<WriteContent> content;

	weak_ptr<HTTPCommunication> commu;

	virtual void ReadReady() {}
	virtual void WriteNotify()
	{
		shared_ptr<HTTPCommunication> commutmp = commu.lock();
		if (commutmp) commutmp->setSendHeaderContentAndPostSend(header, content);
	}
};

HTTPServerResponse::HTTPServerResponse(const shared_ptr<HTTPCommunication>& commu, HTTPCacheType type)
{
	internal = new HTTPServerResponseInternal;

	internal->header = make_shared<HTTPHeader>();

	internal->content = make_shared<WriteContent>(commu->recvHeader,internal, type);

	internal->header->statuscode = 200;
	internal->header->statusmsg = "OK";
}
HTTPServerResponse::~HTTPServerResponse()
{
	internal->WriteNotify();
	SAFE_DELETE(internal);
}

int& HTTPServerResponse::statusCode()
{
	return internal->header->statuscode;
}
std::string& HTTPServerResponse::errorMessage()
{
	return internal->header->statusmsg;
}

std::map<std::string, Value>& HTTPServerResponse::headers()
{
	return internal->header->headers;
}
Value HTTPServerResponse::header(const std::string& key)
{
	return internal->header->header(key);
}

shared_ptr<WriteContent>& HTTPServerResponse::content()
{
	return internal->content;
}



}
}