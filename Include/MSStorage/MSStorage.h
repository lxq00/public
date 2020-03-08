#pragma  once
#include "Base/Base.h"
#include "MSMQ/MSMQ.h"
#include "MSProtocol/MSProtocol.h"
using namespace Public::Base;
using namespace Milesight::MQ;
using namespace Milesight::Protocol;

namespace Milesight {
namespace Storage {

#ifdef _MSC_VER

#ifdef MSSTORAGE_DLL_BUILD
#define  MSSTORAGE_API _declspec(dllexport)
#else
#define  MSSTORAGE_API _declspec(dllimport)
#endif
#else
#define MSSTORAGE_API
#endif

struct MSVolumeConfig
{
	bool		enable = false;	//�Ƿ����ã����ú����д
	uint32_t	reserved = 10;	//�ٷֱ�,Ԥ�������ռ�
};

struct MSVolumeInfo :public Host::DiskInfo,public MSVolumeConfig
{
	ErrorInfo  status;		//volume״̬
};

//����ֱ�Ӹ��ƣ��޿���
struct MSFrameInfo
{
	CodeID					codeid;			//��������
	FrameType				frametype;		//֡����
	uint32_t				timestmap;		//֡ʱ���
	String 					data;
};
struct MSBookMarkInfo
{
	DeviceId				devid;
	uint64_t				startTime;
	uint32_t				timeLong;

	std::string				bookmarkName;
	std::string				bookmarkDescrition;
	std::string				creator;
};

struct MSBookRecordInfo
{
	uint64_t					startTime;
	uint64_t					stopTime;
	uint32_t					mediaSize;
	uint32_t					fileid;
	uint32_t					indexpos;
	MSProtoStreamType			streamtype;
	std::string					volume;
};

//������ʽ
enum MSTrigger
{
	MSTrigger_Time = 1 << 1,		//��ʱ
	MSTrigger_Motion = 1 << 2,		//�ƶ���ⱨ��
	MSTrigger_AlarmIn = 1 << 3,		//�������봥������
};


struct MSCameraConfig
{
	DeviceId		devid;
	uint32_t		duetime;	//����ʱ�䣬��
};


enum ReadType {
	ReadType_Forward = 0,	//�����ȡ����������
	ReadType_Backward = 1,		//�����ȡ������
};

class FS_Manager;

//¼������������¼�񲥷ţ����ص�
class MSSTORAGE_API MSRecordStream
{
public:
	MSRecordStream() {}
	virtual ~MSRecordStream() {}

	virtual	ErrorInfo open(uint64_t starttime, uint64_t stoptime) = 0;
	virtual ErrorInfo close() = 0;

	//��λ
	virtual ErrorInfo seek(uint64_t time) = 0;
	
	//��ȡ֡����
	virtual std::vector<shared_ptr<MSFrameInfo>> readGop(ReadType type) = 0;

	//�Ƿ��Ѿ�����
	virtual bool isEndOfFile()const = 0;

	//¼����ʼ����ʱ��
	virtual uint64_t startTime() const = 0;
	virtual uint64_t stopTime() const = 0;
};

enum MSStorageEventType
{
	MSStorageEventType_Disk_Ready = 10000,			//׼������
	MSStorageEventType_Disk_Offline,				//������
	MSStorageEventType_Disk_AccessDenied,			//�ܾ�����
	MSStorageEventType_Disk_Full,					//������

	MSStorageEventType_File_CreateError = 20000,	//�����ļ�ʧ��
	MSStorageEventType_File_OpenError,				//���ļ�ʧ��
	MSStorageEventType_File_WriteError,				//д���ļ�ʧ��
	MSStorageEventType_File_ReadError,				//��ȡ�ļ�ʧ��

	
};

//���ﶨ����Ǵ洢��ص��¼�֪ͨ
class MSSTORAGE_API MSMSStorageEvent
{
public:
	MSMSStorageEvent() {}
	virtual ~MSMSStorageEvent() {}

	//�����ļ��¼�
	virtual void onEvent(MSStorageEventType event,const std::string& name, const std::string& filename,const ErrorInfo& err) = 0;
	
	//�洢�¼�
	virtual void onStorageEvent(const DeviceId& devid, MSStorageEventType event, const ErrorInfo& err) = 0;


};

//�洢���󣬴洢¼�񣬲�ѯ��
class MSSTORAGE_API MSStorage:public enable_shared_from_this<MSStorage>
{
public:
	MSStorage(const shared_ptr<MSMSStorageEvent>& event);
	virtual ~MSStorage();

	ErrorInfo start(uint32_t threadnum);
	ErrorInfo stop();

	//��ȡ���̾���Ϣ
	ErrorInfo getVoumeList(std::map<std::string, MSVolumeInfo>& volumeList);
	
	//���ɾ��nas��
	ErrorInfo addNasVolume(const Host::DiskInfo& diskinfo);
	ErrorInfo delNasVolume(const std::string& name);

	//���þ��ʹ��״̬
	ErrorInfo setVolumeConfig(const std::map<std::string, MSVolumeConfig>& config);
	ErrorInfo setVolumeConfig(const std::string& name, const MSVolumeConfig& config);

	//�������������
	ErrorInfo setCameraConfig(const std::vector<MSCameraConfig>& config);
	

	//����/ֹͣ �豸Ԥ¼
	ErrorInfo startPrerecord(const DeviceId& devid,MSProtoStreamType streamtype, uint32_t startprercordtime, uint32_t stopprevrecordtime);
	ErrorInfo stopPrerecord(const DeviceId& devid, MSProtoStreamType streamtype);

	//�������ݽ���Ԥ¼��¼��
	//triggerΪ�������ͣ�����ʹ�� MSTrigger ��򣬵���;�б仯��ֱ��inputStream����
	ErrorInfo inputStream(const DeviceId& devid, MSProtoStreamType streamtype, const shared_ptr<MSFrameInfo>& framinfo,uint32_t trigger);

	//�����豸¼��Ԥ¼��ʱ��,¼������
	//triggerΪ�������ͣ�����ʹ�� MSTrigger ��򣬵���;�б仯��ֱ��inputStream����
	ErrorInfo startRecord(const DeviceId& devid, MSProtoStreamType streamtype, uint32_t trigger);
	//ֹͣ�豸¼��ֹͣ¼���Ԥ,¼��ʱ��
	ErrorInfo stopRecord(const DeviceId& devid, MSProtoStreamType streamtype);
	//�ж��豸¼���Ƿ��Ѿ����
	bool checkRecordIsFinish(const DeviceId& devid, MSProtoStreamType streamtype);

	ErrorInfo searchRecord(const DeviceId& devid, uint64_t starttime, uint64_t stoptime, std::list<MSBookRecordInfo>& recordlist);

	//��������������
	shared_ptr<MSRecordStream> createRecordStream(const DeviceId& devid, MSProtoStreamType streamtype);
private:
	struct MSStorageInternal;
	MSStorageInternal* internal;
};

}
}

