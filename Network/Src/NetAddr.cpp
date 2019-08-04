//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//  Description:
//  $Id: NetAddr.cpp 17 2013-01-22 09:26:19Z  $
//

#include <string.h>

#ifdef WIN32
	#include <Winsock2.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
 	#include <netdb.h>
#endif
#include "Base/String.h"
#include "Network/NetAddr.h"
namespace Public{
namespace Network{


//һ��IP��ַ��һ���˿ںŹ�����һ��IPV4��ַ.
struct NetAddr::NetAddrInternal
{
	NetAddrType 	type;
	SockAddrIPv4 	ipv4;
#ifdef SUPPORT_IPV6
	SockAddrIPv6 	ipv6;
#endif	
};

/// ȱʡ���캯������Ч��ַ
NetAddr::NetAddr()
{
	internal = new struct NetAddrInternal;
	::memset( internal, 0, sizeof(struct NetAddrInternal));
}

NetAddr::NetAddr(const NetAddr& addr)
{
	internal = new struct NetAddrInternal;

	*internal = *addr.internal;
}
	
/// ���캯�� ���� ��ַΪ INADDR_ANY
/// \param [in] port �˿ں�
NetAddr::NetAddr(uint16_t port )
{
	internal = new struct NetAddrInternal;
	::memset( internal, 0, sizeof(struct NetAddrInternal));
	
	setPort(port);
}	
/// ���캯��
/// \param [in] ip ���ʮ����IPv4�ַ���
/// \param [in] port �˿ں�	
NetAddr::NetAddr(const std::string& addr,int port)
{
	internal = new struct NetAddrInternal;

	::memset( internal, 0, sizeof(struct NetAddrInternal));

	setAddr(addr, port);
}

/// ���캯��
/// \param [in] addr SockAddrIPv4 ����
NetAddr::NetAddr(const SockAddrIPv4& ipv4)
{
	internal = new struct NetAddrInternal;
	::memset( internal, 0, sizeof(struct NetAddrInternal));

	setAddr(ipv4);
}

NetAddr::NetAddr(const SockAddr& addr)
{
	internal = new struct NetAddrInternal;
	::memset( internal, 0, sizeof(struct NetAddrInternal));

	setAddr(addr);
}


/// ��������
NetAddr::~NetAddr()
{
	delete internal;
}

/// ��ֵ����
NetAddr& NetAddr::operator=( NetAddr const & other )
{
	*internal = *other.internal;
	
	return *this;
}

/// ��ϵ����:����
bool NetAddr::operator==( NetAddr & other) const
{
	if(internal->type == netaddr_ipv4)
	{
		if ( (internal->ipv4.sin_port == other.internal->ipv4.sin_port)
		&& (internal->ipv4.sin_addr.s_addr == other.internal->ipv4.sin_addr.s_addr) ) 
			return true;
	}
#ifdef SUPPORT_IPV6
	else if(internal->type == netaddr_ipv6)
	{
		if ( (internal->addrin6.sin6_port == other.internal->addrin6.sin6_port)
		&& !memcmp( (char*)internal->addrin6.sin6_addr.s6_addr,(char*)other.internal->addrin6.sin6_addr.s6_addr, sizeof(other.internal->addrin6.sin6_addr.s6_addr) )) 
			return true;
	}
#endif

	return false;
}



// ��ǰ�������ַ�Ƿ�Ϊ��Ч��
bool NetAddr::isValid() const
{
	return internal->type != netaddr_unknown;
}

NetAddr::NetAddrType NetAddr::getType() const
{
	return internal->type;
}

/// ���ʵ�����SockAddrָ��
/// \param [in/out] addr ָ��
/// \param [in]  addr�ĳ���
/// \retval !=NULL ��Ч�ĵ�ַ
/// \retval ==NULL ��Ч��ַ
SockAddr * NetAddr::getAddr() const
{
#ifdef SUPPORT_IPV6
	if(internal->type == netaddr_ipv6)
	{
		return (SockAddr *)&internal->ipv6;
	}
#endif
	return (SockAddr *)&internal->ipv4;
}

SockAddrIPv4* NetAddr::getAddrV4() const
{
#ifdef SUPPORT_IPV6
		if(internal->type == netaddr_ipv6)
		{
			NULL;
		}
#endif
	return &internal->ipv4;
}
	
/// ���ʵ�����SockAddr��ַ�ĳ���
/// \retval  >  0 ��ַ����
/// \reval   == 0 ��ַ��Ч
uint32_t NetAddr::getAddrLen() const
{
#ifdef SUPPORT_IPV6
	if(internal->type == netaddr_ipv6)
	{
		return sizeof(SockAddrIPv6);
	}
#endif
	return sizeof(SockAddrIPv4);
}

std::string NetAddr::getIP() const
{
#ifdef SUPPORT_IPV6
	if(internal->type == netaddr_ipv6)
	{
		return "";
	}
#endif

	return ::inet_ntoa(internal->ipv4.sin_addr);
}

uint16_t NetAddr::getPort() const
{
#ifdef SUPPORT_IPV6
		if(internal->type == netaddr_ipv6)
		{
			return 0;
		}
#endif
	return ntohs( internal->ipv4.sin_port);
}

/// ����IP��ַ
/// \param [in] ip ���ʮ����IPv4�ַ���
/// \param [in] port �˿ں�	
/// \reval  >=0 �ɹ�
/// \reval  < 0 ʧ��
int NetAddr::setAddr( const SockAddrIPv4 & addr )
{
	internal->ipv4 = addr;

	internal->type = netaddr_ipv4;

	return 0;
}
/// ����IP��ַ
/// \param [in] addr SockAddrIPv4
/// \reval  >=0 �ɹ�
/// \reval  < 0 ʧ��	
int NetAddr::setAddr( const SockAddr & addr )
{
	return setAddr(*(SockAddrIPv4*)&addr);
}

/// ֻ����IP��ַ,���ı�˿�
/// \param [in] ip ���ʮ����IPv4�ַ���
/// \reval  >=0 �ɹ�
/// \reval  < 0 ʧ��		
int NetAddr::setAddr( const std::string& ip, uint16_t port)
{
	setPort(port);
	
#ifdef SUPPORT_IPV6	
	struct in_addr ia;
#endif
	if( ip == "" ) 	
	{
	//	internal->addrin.sin_addr.s_addr = (uint32_t)(-1);
	}
	else if(inet_addr(ip.c_str()) != INADDR_NONE)
	{
		internal->ipv4.sin_addr.s_addr = inet_addr(ip.c_str());
		internal->ipv4.sin_family = AF_INET;
	}
	else if(inet_addr(ip.c_str()) == INADDR_NONE)
	{
		struct hostent * hent = NULL;
		if( (hent = gethostbyname( ip.c_str() )) == NULL ) 
			return -1;
		::memcpy( &internal->ipv4.sin_addr.s_addr, *(hent->h_addr_list), sizeof(internal->ipv4.sin_addr.s_addr) );
		internal->ipv4.sin_family = AF_INET;
	}
#ifdef SUPPORT_IPV6	
	else
	{
		internal->ipv6.sin6_family = AF_INET6;
#ifdef WIN32
		struct addrinfo hints;
		struct addrinfo* result = NULL;
		memset( &hints, 0, sizeof(hints) );
		hints.ai_flags = AI_CANONNAME;
		hints.ai_family = AF_INET6;
		hints.ai_socktype = SOCK_RAW;
		int ret = getaddrinfo(ip, NULL, &hints, &result);
		if( ret != 0 )
		{
			struct hostent * hent = NULL;
			if(inet_addr(ip) != INADDR_NONE){ //����ת����ipv4��ַ����ipv4 mapped��ַ����IPv6��ַ
				*((int*)&(internal->ipv6.sin6_addr.s6_addr[12])) = inet_addr(ip);
				internal->ipv6.sin6_addr.s6_addr[10] = 0xff;
				internal->ipv6.sin6_addr.s6_addr[11] = 0xff;		
				return 0;
			}else if( (hent = gethostbyname( ip )) != NULL ){
				*((int*)&(internal->ipv6.sin6_addr.s6_addr[12])) = *((int*)(*(hent->h_addr_list)));
				internal->ipv6.sin6_addr.s6_addr[10] = 0xff;
				internal->ipv6.sin6_addr.s6_addr[11] = 0xff;		
				return 0;
			}
			return -1;
		}
		for( struct addrinfo* tmp = result; tmp != NULL; tmp = tmp->ai_next ){
			if( tmp->ai_family == AF_INET6 && tmp->ai_addr != NULL ){
				memcpy( &internal->ipv6, tmp->ai_addr, sizeof(internal->ipv6) );
				break;
			}
		}
		freeaddrinfo( result );	
#else
		if( inet_pton( AF_INET6, ip, internal->ipv6.sin6_addr.s6_addr ) <= 0 )
		{
			struct in_addr ia;
			if( inet_aton( ip, &ia ) != 0 ) 
			{
				internal->ipv6.sin6_addr.s6_addr[10] = 0xff;
				internal->ipv6.sin6_addr.s6_addr[11] = 0xff;
				memcpy(internal->ipv6.sin6_addr.s6_addr+12,(char*)&ia.s_addr,sizeof(ia));
				internal->ipv6.sin6_family = AF_INET6;
			}
			else
				return -1;
		}
#endif
	}
#endif

	return 0;
}

/// ֻ���ö˿�
/// \param [in] port �˿�
/// \reval  >=0 �ɹ�
/// \reval  < 0 ʧ��	
int NetAddr::setPort( uint16_t port )
{
	internal->ipv4.sin_port = htons( port);
	internal->ipv4.sin_family = AF_INET;
#ifdef SUPPORT_IPV6
	internal->ipv6.sin6_family = AF_INET6;
	internal->ipv6.sin6_port = htons( port );
#endif

	if(internal->type == netaddr_unknown)
	{
		internal->type = netaddr_ipv4;
	}	
	
	return 0;
}

#ifdef SUPPORT_IPV6
NetAddr::NetAddr(const SockAddrIPv6& ipv6)
{
	internal = new struct NetAddrInternal;
	::memset( &internal, 0, sizeof(internal));

	
}
SockAddrIPv6* NetAddr::getAddrV6() const
{
	if(internal->type == netaddr_ipv4)
	{
		return NULL;
	}
	return (SockAddr *)&internal->ipv6;
}
}
int NetAddr::setAddr( const SockAddrIPv6 & addr )
{
	internal->ipv6 = addr;

	internal->type = netaddr_ipv6;

	return 0;
}
NetAddr& NetAddr::operator=( NetAddrIPv6 const & other )
{
	setAddr(other);
}
#endif
} // namespace Net
} // namespace Public



	
