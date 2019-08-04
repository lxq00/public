//
//  Copyright (c)1998-2012,  Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: host.h 216 2015-12-15 11:33:55Z  $
//
#ifndef __NETWORKHOST_H__
#define __NETWORKHOST_H__
#include "Defs.h"
#include "Base/IntTypes.h"

namespace Public{
namespace Base {

///������ص���
class BASE_API Host
{
public:
	struct DiskInfo
	{
		std::string		Id;
		std::string		Name;
		std::string		Alias; //����
		std::string		SerialNumber;//���к�
		enum{
			DiskType_Usb = 0,
			DiskType_CDRom,
			DiskType_Remove,
			DiskType_Disk,
			DiskType_Network,
		}Type;
		enum {
			FormatType_Unkown = 0,
			FormatType_FAT,
			FormatType_NTFS,
			FormatType_ext4,
		}FormatType;
		uint64_t		TotalSize;
		uint64_t		FreeSize;
	};
	typedef enum {
		SocketType_TCP,
		SocketType_UDP,
	}SocketType;

	struct IPInfo
	{
		std::string Ip;
		std::string Netmask;
		std::string Gateway;
	};

	struct NetworkInfo :public IPInfo
	{
		std::string AdapterName;
		std::string Description;
		std::string Mac;
	};
public:
	///��ȡһ�����õĶ˿ں� 10000��ʼ ʧ�ܷ���0
	///type ��ĳЭ�����ж϶˿��Ƿ����
	static uint16_t getAvailablePort(uint16_t startPort = 10000,SocketType type = SocketType_TCP);

	static bool getNowUsedPortMap(std::set<uint16_t>& portmap,SocketType type = SocketType_TCP);

	///���˿��Ƿ�ռ��
	///type ��ĳЭ�����ж϶˿��Ƿ����
	static bool checkPortIsNotUsed(uint16_t port,SocketType type = SocketType_TCP);

	///��ȡCPU ����
	static int getProcessorNum();

	///��ȡ�����ڴ�ʹ���ʡ������ڴ�ʹ�ô�С
//	static bool getMemoryUsage(uint64_t& totalPhys,uint64_t& totalVirual,int& physusage,int& virualUsage);

	///��ȡcpuʹ���� 0 ~ 100 
//	static uint16_t getCPUUsage();

	///��ȡ������Ϣ
	/// num ��������
	/// totalSize �����ܴ�С MΪ��λ
	/// freeSize  ʣ���С
	static bool getDiskInfo(int& num,uint64_t& totalSize,uint64_t& freeSize);

	static bool getDiskInfos(std::vector<DiskInfo>& infos);

	///��ȡ���縺��
//	static bool getNetwordLoad(uint64_t& inbps,uint64_t& outbps);

	static bool getNetworkInfos(std::map<std::string, NetworkInfo>& infos,std::string& defaultMac);

	static bool setIPInfo(const NetworkInfo& info,const std::string& adapterName = "");

	static std::string	guessMyIpaddr(const std::string& destip = "");
	static bool guessMyIpInfo(NetworkInfo& info);

	//��ȡϵͳλ��
	static uint32_t getSystemBits();
};

};
};


#endif //__BASEHOST_H__

