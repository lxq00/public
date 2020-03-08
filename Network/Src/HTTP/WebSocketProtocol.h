#pragma once

#include "Base/Base.h"
#include "Network/Network.h"
using namespace Public::Base;
using namespace Public::Network;

namespace Public {
namespace Network {
namespace HTTP {

#define WEBSOCKETMASK "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"


	/*
	0                   1                   2                   3
		0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
		+-+-+-+-+-------+-+-------------+-------------------------------+
		|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
		|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
		|N|V|V|V|       |S|             |   (if payload len==126/127)   |
		| |1|2|3|       |K|             |                               |
		+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
		|     Extended payload length continued, if payload len == 127  |
		+ - - - - - - - - - - - - - - - +-------------------------------+
		|                               |Masking-key, if MASK set to 1  |
		+-------------------------------+-------------------------------+
		| Masking-key (continued)       |          Payload Data         |
		+-------------------------------- - - - - - - - - - - - - - - - +
		:                     Payload Data continued ...                :
		+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
		|                     Payload Data continued ...                |
		+---------------------------------------------------------------+
	*/

	struct netframe
	{

#define TYP_INIT 0
#define TYP_SMLE 1
#define TYP_BIGE 2

		static uint64_t _htonll(uint64_t src) {
			static int typ = TYP_INIT;
			unsigned char c;
			union {
				uint64_t ull;
				unsigned char c[8];
			} x;
			if (typ == TYP_INIT) {
				x.ull = 0x01;
				typ = (x.c[7] == 0x01ULL) ? TYP_BIGE : TYP_SMLE;
			}
			if (typ == TYP_BIGE)
				return src;
			x.ull = src;
			c = x.c[0]; x.c[0] = x.c[7]; x.c[7] = c;
			c = x.c[1]; x.c[1] = x.c[6]; x.c[6] = c;
			c = x.c[2]; x.c[2] = x.c[5]; x.c[5] = c;
			c = x.c[3]; x.c[3] = x.c[4]; x.c[4] = c;
			return x.ull;
		}

		static uint64_t _ntohll(uint64_t src) {
			return _htonll(src);
		}
	};

	class WebSocketProtocol
	{
		struct FrameInfo
		{
			uint64_t	playloadlen;
			bool		mask;
			char		maskkey[4];
			std::string	data;
			int			datatype;
			/*
			%x0����ʾһ������֡����OpcodeΪ0ʱ����ʾ�������ݴ�����������ݷ�Ƭ����ǰ�յ�������֡Ϊ����һ�����ݷ�Ƭ��
			%x1����ʾ����һ���ı�֡��frame��
			%x2����ʾ����һ��������֡��frame��
			%x3-7�������Ĳ������룬���ں�������ķǿ���֡��
			%x8����ʾ���ӶϿ���
			%x9����ʾ����һ��ping������
			%xA����ʾ����һ��pong������
			%xB-F�������Ĳ������룬���ں�������Ŀ���֡��
			*/

			FrameInfo() :playloadlen(0), mask(false), datatype(0)
			{
				maskkey[0] = maskkey[1] = maskkey[2] = maskkey[3] = 0;
			}
			~FrameInfo()
			{
			}
		};
		struct SendItemInfo
		{
			std::string		needsenddata;
			uint32_t		havesendlen;

			SendItemInfo() :havesendlen(0) {}
		};
	public:
		typedef Function<void(const std::string&, WebSocketDataType)> ParseDataCallback;
	public:
		WebSocketProtocol(bool client, const ParseDataCallback& callback) :isClient(client), datacallback(callback)
		{
		}
		virtual ~WebSocketProtocol()
		{
		}
		std::string buildProtocol(const std::string& data, WebSocketDataType type)
		{
			std::string protocolstr;
			char maskkey[4] = { 0 };
			char header[32] = { 0 };
			int headerlen = 0;

			//build websocket header
			{
				header[headerlen] |= 1 << 7;	//fin
				header[headerlen] |= (type == WebSocketDataType_Txt ? 1 : 2) & 0x0f;
				headerlen += 1;

				header[headerlen] |= (isClient ? 1 : 0) << 7;	//mask

				//ext playload len
				if (data.length() <= 125)
				{
					header[headerlen] |= data.length() & 0x7f;
					headerlen += 1;
				}
				else if (data.length() > 125 && data.length() <= 0xffff)
				{
					header[headerlen] |= 126 & 0x7f;
					headerlen += 1;

					*(unsigned short*)(header + headerlen) = htons((unsigned short)data.length());
					headerlen += 2;
				}
				else if (data.length() > 0xffff)
				{
					header[headerlen] |= 127 & 0x7f;
					headerlen += 1;

					*(unsigned long long*)(header + headerlen) = netframe::_htonll(data.length());
					headerlen += 8;
				}
				//client mask key
				if (isClient)
				{
					for (uint32_t i = 0; i < 4; i++)
					{
						maskkey[i] = (((size_t)this) >> (i * 8)) & 0xff;
						while (maskkey[i] == 0)
						{
							maskkey[i] = ((char)Time::getCurrentMilliSecond()) & 0xff;
						}
					}

					header[headerlen + 0] = maskkey[0];
					header[headerlen + 1] = maskkey[1];
					header[headerlen + 2] = maskkey[2];
					header[headerlen + 3] = maskkey[3];
					headerlen += 4;
				}
			}

			protocolstr = std::string(header, headerlen) + data;

			//mask data
			if (isClient)
			{
				char* datatmp = (char*)protocolstr.c_str();
				datatmp += headerlen;

				for (uint32_t i = 0; i < data.length(); i++)
				{
					uint32_t j = i % 4;
					datatmp[i] = datatmp[i] ^ maskkey[j];
				}
			}

			return protocolstr;
		}

		const char* parseProtocol(const char* buffer, int len)
		{
			int nTry = 100;
			while (len > 0 && --nTry >= 0)
			{
				if (frame == NULL)
				{
					frame = make_shared<FrameInfo>();

					const char* tmp = parseWebSocketHeard(buffer, len, frame);
					if (tmp == NULL) break;

					len = (int)(buffer + len - tmp);
					buffer = tmp;
				}

				int needdatalen = min(len, (int)(frame->playloadlen - frame->data.length()));
				if (needdatalen > 0)
				{
					frame->data += std::string(buffer, needdatalen);
					buffer += needdatalen;
					len -= needdatalen;
				}

				if (frame->playloadlen == frame->data.length())
				{
					if (frame->datatype == 1 || frame->datatype == 2)
					{
						if (frame->mask)
						{
							char* buffertmp = (char*)frame->data.c_str();
							uint32_t datalen = (uint32_t)(frame->data.length());
							for (uint32_t i = 0; i < datalen; i++)
							{
								uint32_t j = i % 4;
								buffertmp[i] = buffertmp[i] ^ frame->maskkey[j];
							}
						}

						datacallback(frame->data, frame->datatype == 1 ? WebSocketDataType_Txt : WebSocketDataType_Bin);
					}

					frame = NULL;
				}
			}

			return buffer;
		}
	private:
		bool parseAndCheckHeader(const char* buffer, int len, shared_ptr<FrameInfo>& tmpframe)
		{
			int fine = (buffer[0] >> 7) & 1;
			int rsv1 = (buffer[0] >> 6) & 1;
			int rsv2 = (buffer[0] >> 5) & 1;
			int rsv3 = (buffer[0] >> 5) & 1;
			tmpframe->datatype = buffer[0] & 0xf;

			tmpframe->mask = ((uint8_t)buffer[1] >> 7) != 0;
			tmpframe->playloadlen = buffer[1] & 0x7f;

			if (fine != 1 || rsv1 != 0 || rsv2 != 0 || rsv3 != 0 || tmpframe->playloadlen < 0 || tmpframe->playloadlen > 127)
			{
				assert(0);
				return false;
			}
			return true;
		}
		const char* parseWebSocketHeard(const char* buffer, int len, shared_ptr<FrameInfo>& tmpframe)
		{
			while (len > 0)
			{
				if (parseAndCheckHeader(buffer, len, tmpframe))
				{
					break;
				}
				buffer++;
				len--;
			}
			if (len <= 0)
			{
				return NULL;
			}
			int needlen = 2;
			if (len < needlen) return NULL;

			//	int fine = buffer[0] >> 7;
			tmpframe->datatype = buffer[0] & 0xf;
			tmpframe->mask = ((uint8_t)buffer[1] >> 7) != 0;
			tmpframe->playloadlen = buffer[1] & 0x7f;
			if (tmpframe->playloadlen == 126)
			{
				if (len < needlen + 2) return NULL;

				tmpframe->playloadlen = ntohs(*(unsigned short*)(buffer + needlen));
				needlen += 2;
			}
			else if (tmpframe->playloadlen == 127)
			{
				if (len < needlen + 8) return NULL;

				tmpframe->playloadlen = netframe::_ntohll(*(unsigned long long*)(buffer + needlen));
				needlen += 8;
			}

			if (tmpframe->playloadlen < 0)
			{
				assert(0);
				return NULL;
			}
			//���Գ���
			if (tmpframe->playloadlen >= 1024 * 256)
			{
				assert(0);
			}
			if (tmpframe->mask)
			{
				if (len < needlen + 4) return NULL;

				tmpframe->maskkey[0] = buffer[needlen + 0];
				tmpframe->maskkey[1] = buffer[needlen + 1];
				tmpframe->maskkey[2] = buffer[needlen + 2];
				tmpframe->maskkey[3] = buffer[needlen + 3];
				needlen += 4;
			}

			return buffer += needlen;
		}
	private:
		shared_ptr<FrameInfo>			frame;
		bool							isClient;
		ParseDataCallback				datacallback;
	};


}
}
}
