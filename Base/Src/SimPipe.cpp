//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: SimPipe.cpp 141 2013-06-27 09:08:03Z  $

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include "Base/SimPipe.h"
#include "Base/Guard.h"
#include "Base/File.h"
#include "Base/BaseTemplate.h"

namespace Public {
namespace Base {

struct SimPipe::Internal
{
	Internal(const std::string &name_):status(false), file(NULL), readpos(0), writepos(0),isSave(false)
	{
		file = ::fopen(name_.c_str(),"rb");
		if(file == NULL)
		{
			file = ::fopen(name_.c_str(), "wb+");
		}		
		if (file != NULL)
		{
			status = true;
			name = name_;
			::fseek(file,0,SEEK_END);
			writepos = ::ftell(file);
			::fseek(file,0,SEEK_SET);
		}
	}
	~Internal()
	{
		if (file != NULL )
		{
			fclose(file);
		}
		if(!isSave)
		{
			File::remove(name.c_str());
		}
	}
	size_t write(const void *buf, size_t len)
	{
		Guard guard(mutex);
		::fseek(file, (long)writepos, SEEK_SET);
		size_t wlen = ::fwrite(buf,1,len, file);
		if (wlen > 0)
		{
			writepos += wlen;

			::fflush(file);

		}
		return wlen;
	}
	size_t read(void *buf, size_t len)
	{
		Guard guard(mutex);
		::fseek(file,  (long)readpos, SEEK_SET);
		size_t rlen = ::fread(buf,1,  len, file);

		if (rlen >= 0)
		{
			readpos += rlen;
		}
		return rlen;
	}
	
	bool readSeek(size_t offset, SimPipe::SeekPos pos)
	{
		Guard guard(mutex);
		if (pos == SimPipe::seekPos_Cur)
		{
			uint64_t off = (uint64_t)((int64_t)readpos + offset);
			if (off >= 0 && off <= writepos)
			{
				readpos = (size_t)off;
				return true;
			}
			else return false;
		}
		else if (offset >= 0 && offset <= writepos)
		{
			readpos = (uint64_t)offset;
			return true;
		}
		return false;
	}

	/// дλ��Seek
	/// \param offset [in] ƫ��
	/// \param pos [in]λ��
	/// \retval true �ɹ�
	///         false ʧ��
	bool writeSeek(size_t offset, SeekPos pos)
	{
		Guard guard(mutex);
		if (pos == SimPipe::seekPos_Cur)
		{
			uint64_t off = (uint64_t)((int64_t)writepos + offset);
			if (off >= readpos && off <= writepos)
			{
				writepos = (size_t)off;
				return true;
			}
			else return false;
		}
		else if (offset >= readpos && offset <= writepos)
		{
			writepos = (uint64_t)offset;
			return true;
		}
		return false;	
	}
	size_t getReadPos()
	{
		Guard gurad(mutex);
		return readpos;
	}

	/// ���д��λ��
	/// \retval д��λ��
	size_t getWritePos()
	{
		Guard gurad(mutex);
		return writepos;
	}
	const std::string &getname() const { return name;}

	bool status;
	std::string name;
	FILE *file;
	size_t readpos;
	size_t writepos;
	Mutex mutex;
	bool isSave;
};

SimPipe::SimPipe():internal(NULL)
{
}

/// ����Pipeͨ��
/// \param name [in] Pipe������
/// \return != NULL SimPipeʵ��ָ��
	///          == NULL ʧ��
bool SimPipe::creat(const std::string &name)
{
	if(internal != NULL)
	{
		SAFE_DELETE(internal);
	}
	internal = new Internal(name);

	return internal != NULL && internal->status;
}

bool SimPipe::destory(bool save)
{
	if(internal != NULL)
	{
		internal->isSave = save;
		SAFE_DELETE(internal);
	}

	return true;
}
	/// ��������

SimPipe::~SimPipe()
{
	SAFE_DELETE(internal);
}

/// д����
/// \param buf [in] ���ݵ�ַ
/// \param len [in] ���ݳ���
/// \retval  > 0 д�����ݳ���
///          = 0 δд��
///          < 0 ʧ��
size_t SimPipe::write(const void *buf, size_t len)
{
	if(internal == NULL)
	{
		return -1;
	}
	return internal->write(buf, len);
}

/// ������
/// \param buf [out] �����ݵ�ַ
/// \param len [in] ���ݳ���
/// \retval  > 0 �������ݳ���
///          = 0 û������
///          < 0 ʧ��
size_t SimPipe::read(void *buf, size_t len)
{
	if(internal == NULL)
	{
		return -1;
	}
	return internal->read(buf, len);
}


/// ��λ��Seek
/// \param offset [in] ƫ��
/// \param pos [in]λ��
/// \retval true �ɹ�
///         false ʧ��
bool SimPipe::readSeek(size_t offset, SeekPos pos)
{
	if(internal == NULL)
	{
		return false;
	}
	return internal->readSeek(offset, pos);
}

/// дλ��Seek
/// \param offset [in] ƫ��
/// \param pos [in]λ��
/// \retval true �ɹ�
///         false ʧ��
bool SimPipe::writeSeek(size_t offset, SeekPos pos)
{
	if(internal == NULL)
	{
		return false;
	}
	return internal->writeSeek(offset, pos);
}

/// ��ö���λ��
/// \retval ����λ��
uint64_t SimPipe::getReadPos()
{
	if(internal == NULL)
	{
		return -1;
	}
	return internal->getReadPos();
}

/// ���д��λ��
/// \retval д��λ��
uint64_t SimPipe::getWritePos()
{
	if(internal == NULL)
	{
		return -1;
	}
	return internal->getWritePos();
}

/// ���Pipe�ļ�������
/// \retval �����ļ����Ƶ�����
const std::string SimPipe::getname() const
{
	if(internal == NULL)
	{
		return "";
	}
	return internal->getname();
}


} // namespace Base
} // namespace Public

