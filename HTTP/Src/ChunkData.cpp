#include "HTTP/HTTPPublic.h"

namespace Public {
namespace HTTP {

struct ChunkData::ChunkDataInternal
{
	WriteCallback writecallback;
	ReadCallback readcallback;

	uint32_t		chunkbodysize;

	ChunkDataInternal() :chunkbodysize(0) {}
};

ChunkData::ChunkData()
{
	internal = new ChunkDataInternal();
}
ChunkData::ChunkData(const WriteCallback& writecallback)
{
	internal = new ChunkDataInternal();
	internal->writecallback = writecallback;
}
ChunkData::~ChunkData()
{
	SAFE_DELETE(internal);
}

void ChunkData::setReadCallback(const ReadCallback& readcallback)
{
	internal->readcallback = readcallback;
}

uint32_t ChunkData::append(const char* bufferptr, uint32_t len)
{
	const char* buffer = bufferptr;

	while (len >= (int)strlen(HTTPSEPERATOR))
	{
		//find 
		if (internal->chunkbodysize == 0)
		{
			int pos = (int)String::indexOf(buffer, len, HTTPSEPERATOR);
			if (pos == -1) return buffer - bufferptr;

			std::string chunksizestr = std::string(buffer, pos);
			sscanf(String::tolower(chunksizestr).c_str(), "%x", &internal->chunkbodysize);
			//chuned eof
			if (internal->chunkbodysize == 0)
			{
				if (len < (int)(pos + strlen(HTTPSEPERATOR) * 2)) return buffer - bufferptr;
				//�Ƿ������ʶ 2��HTTPHREADERLINEEND 
				if (memcmp(buffer + pos, HTTPSEPERATOR HTTPSEPERATOR, strlen(HTTPSEPERATOR) * 2) != 0) return buffer - bufferptr;

				len -= pos + (int)strlen(HTTPSEPERATOR) * 2;
				buffer += pos + (int)strlen(HTTPSEPERATOR) * 2;

				return buffer - bufferptr;
			}
			buffer += pos + (int)strlen(HTTPSEPERATOR);
			len -= pos + (int)strlen(HTTPSEPERATOR);

			//���ݳ��ȼ��Ͻ�����ʶ����
			internal->chunkbodysize += (int)strlen(HTTPSEPERATOR);
		}
		else
		{
			//�����ݻ�����ʱ���������ݳ��ȣ����������ˣ�����chunkȱ�ٳ���
			int datalen = min(len, (uint32_t)(internal->chunkbodysize > strlen(HTTPSEPERATOR) ?
				internal->chunkbodysize - strlen(HTTPSEPERATOR) : internal->chunkbodysize));

			//���ݲ���ʱ����������
			if (internal->chunkbodysize > strlen(HTTPSEPERATOR))
			{
				internal->readcallback(buffer, datalen);
			}
			buffer += datalen;
			len -= datalen;
			internal->chunkbodysize -= datalen;
		}
	}

	return buffer - bufferptr;
}
void ChunkData::write(const char* buffer, uint32_t len)
{
	//write chunk start
	{
		char buffer[32] = { 0 };
		sprintf(buffer, "%x" HTTPSEPERATOR, len);

		internal->writecallback(buffer, strlen(buffer));
	}
	internal->writecallback(buffer, len);

	//write chunk end
	{
		internal->writecallback(HTTPSEPERATOR, strlen(HTTPSEPERATOR));
	}
}


}
}
