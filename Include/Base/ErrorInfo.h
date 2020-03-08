#pragma once
#include "Base/IntTypes.h"
#include "Base/Defs.h"
#include "Base/String.h"
namespace Public
{
namespace Base
{

typedef enum
{
	//�����붨��
	Error_Code_Success = 0,
	Error_Code_Fail = -1,
	Error_Code_NotSupport = -2,
	Error_Code_Param = -3,

	//100�࣬Ȩ����صĴ�����
	Error_Code_Password = -100,
	Error_Code_OldPassword = -101,
	Error_Code_Username = -102,
	Error_Code_Authen = -103,

	//200�࣬������صĴ�����
	Error_Code_Network = -200,
	Error_Code_NetworkTimeout = -201,
	Error_Code_ConnectTimeout = -202,
	Error_Code_CommunicateTimeout = -203,
	Error_Code_Unreachable = -204,

	//300�࣬���ݽ�����ش�����
	Error_Code_ParseParam = -300,
	Error_Code_ParseObject = -301,
	Error_Code_ParseFile = -302,
	Error_Code_ParseUrl = -303,
	Error_Code_Request = -304,
	Error_Code_Response = -305,

	//400�࣬�豸������ش�����
	Error_Code_InitFail = -400,
	Error_Code_ResourceUninit = -401,
	Error_Code_ResourceCreateFail = -402,
	Error_Code_ResourceNotExist = -403,
	Error_Code_ResourceExisted = -404,
	Error_Code_DeviceNotExist = -405,
	Error_Code_DeviceExisted = -406,
	Error_Code_DeviceOffOnline = -407,
	Error_Code_ServiceNotExist = -408,
	Error_Code_ServiceExisted = -409,
	Error_Code_CameraNotExist = -410,
	Error_Code_CameraExisted = -411,
	Error_Code_CameraOffOnline = -412,
	Error_Code_FileExisted = -413,
	Error_Code_FileNotExist = -414,
	Error_Code_Busy = -415,

	//500�࣬¼�����
	Error_Code_InvalidRecord = -500,
	Error_Code_InvalidService = -501,
	Error_Code_InvalidDevice = -502,

	//600�࣬�ļ��������
	Error_Code_Send = -600,
	Error_Code_Recive = -601,
	Error_Code_OpenFile = -602,
	Error_Code_WriteFile = -603,
	Error_Code_ReadFile = -604,

	//700�࣬������ز���
	Error_Code_DiskErr = -700,
	Error_Code_DiskFull = -701,

	//800�࣬����ز���
	Error_Code_StreamType = -800,
	Error_Code_StreamCmd = -801,

	//900�࣬���ݿ����
	Error_Code_OpenDatabase = -900,
	Error_Code_CreateDatabase = -901,
	Error_Code_ExecDatabase = -902,
} Error_Code;

class BASE_API ErrorInfo
{
	std::string getErrorMsg(Error_Code code);

public:
	ErrorInfo(const ErrorInfo &ret);
	//ϵͳ������
	ErrorInfo(Error_Code _errorCode = Error_Code_Success, const std::string &_errmsg = "");
	//���result == false�������getlasterror��ȡ

	~ErrorInfo();

	ErrorInfo &operator=(const ErrorInfo &ret);

	bool operator==(const ErrorInfo &ret) const;
	bool operator!=(const ErrorInfo &ret) const;

	bool error() const;

	////�Ƿ�ʧ��
	operator bool() const;

	//�Ƿ�����
	bool operator!() const;

	std::string errorMessage() const;
	int errorCode() const;

public:
	Error_Code errcode;
	std::string errmsgstr;
};

} // namespace Base
} // namespace Public