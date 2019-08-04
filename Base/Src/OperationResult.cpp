#ifdef WIN32
#include <Windows.h>
#else
#include <errno.h> 
#endif

#include "Base/OperationResult.h"
#include "Base/String.h"
#include "Base/BaseTemplate.h"
namespace Public {
namespace Base {

//std::string toErrorString(int errCode)
//{
//	char buffer[128] = { 0 };
//#ifdef WIN32
//	{
//		LPVOID lpMsgBuf;
//		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);
//		snprintf_x(buffer, 127, "������:%d:%s", errCode, lpMsgBuf == NULL ? "error" : lpMsgBuf);
//
//		if (lpMsgBuf != NULL) {
//			LocalFree(lpMsgBuf);
//		}
//	}
//#else
//	{
//		char * mesg = strerror(errCode);
//		snprintf_x(buffer, 127, "������:%d:%s", errCode, mesg == NULL ? "error" : mesg);
//	}
//#endif
//
//	return buffer;
//}
typedef struct {
	Operation_ErrorCode errcode;
	const char *errmsg;
} errorString;

static const errorString faults[] = {
	{Operation_Fail,					"ʧ��"},
	{Operation_Success,					"�ɹ�"},
	{Operation_Error_UnSupport,			"��֧��"},
	{Operation_Error_Password,			"�˺��������"},
	{Operation_Error_OldPassword,		"���������"},
	{Operation_Error_Port,				"�˿ڴ���"},
	{Operation_Error_Authen,			"��֤ʧ��"},
	{Operation_Error_NetworkErr,		"�������"},
	{Operation_Error_NetworkTimeOut,	"���糬ʱ"},
	{Operation_Error_ParseParam,		"������������"},
	{Operation_Error_ParseObject,		"�����������"},
	{Operation_Error_ParseFile,			"�����ļ�����"},
	{Operation_Error_ParseUrl,			"����url����"},
	{Operation_Error_Request,			"�������"},
	{Operation_Error_Param,				"��������"},
	{Operation_Error_DBEroor,			"���ݿ����"},
	{Operation_Error_SetCfg,			"�������ô���"},
	{Operation_Error_GetCfg,			"��ȡ���ô���"},
	{Operation_Error_QueryFail,			"��ѯʧ��"},
	{Operation_Error_QueryTimeOut,		"��ѯ��ʱ"},
	{Operation_Error_ControlFail,		"����ʧ��"},
	{Operation_Error_ControlTimeOut,	"���Ƴ�ʱ"},
	{Operation_Error_RepeatOPeration,	"�ظ�����"},
	{Operation_Error_InitFail,			"��ʼ��ʧ��"},
	{Operation_Error_ResourceUninit,	"��Դδ��ʼ��"},
	{Operation_Error_ResourceCreateFail,"��Դ����ʧ��"},
	{Operation_Error_ResourceNotExist,	"��Դ������"},
	{Operation_Error_ResourceExisted,	"��Դ�Ѵ���"},
	{Operation_Error_DeviceNotExist,	"�豸������"},
	{Operation_Error_DeviceExisted,		"�豸�Ѵ���"},
	{Operation_Error_DeviceOffOnline,	"�豸����"},
	{Operation_Error_ChannelNotExist,	"ͨ��������"},
	{Operation_Error_ChannelExisted,	"ͨ���Ѵ���"},
	{Operation_Error_ChannedOffOnline,  "ͨ������"},
	{Operation_Error_ServiceNotExist,	"���񲻴���"},
	{Operation_Error_ServiceExisted,	"�����Ѵ���"},
	{Operation_Error_ServiceOffOnline,  "��������"},
	{Operation_Error_FileExisted,		"�ļ��Ѵ���"},
	{Operation_Error_FileNotExist,		"�ļ�������"},
	{Operation_Error_EnterpriseExisted, "��ҵ�Ѵ���"},
	{Operation_Error_EnterpriseNotExist,"��ҵ������"},
	{Operation_Error_PointExisted,		"��λ�Ѵ���"},
	{Operation_Error_PointNotExist,		"��λ������"},
	{Operation_Error_InvalidRecord,		"��Ч��¼��"},
	{Operation_Error_InvalidService,	"��Ч�ķ���"},
	{Operation_Error_InvalidDevice,		"��Ч���豸"},
	{Operation_Error_Send,				"��������ʧ��"},
	{Operation_Error_Recive,			"��������ʧ��"},
	{Operation_Error_OpenFile,			"���ļ�ʧ��"},
	{Operation_Error_WriteFile,			"д���ļ�ʧ��"},
	{Operation_Error_ReadFile,			"��ȡ�ļ�ʧ��"},
	{Operation_Error_DiskErr,			"�����쳣����"},
	{Operation_Error_DiskFull,			"������"},
	{Operation_Error_StreamType,		"�����ʹ���"},
	{Operation_Error_StreamCmd,			"���ſ����������"}

};

std::string toErrorString(Operation_ErrorCode errCode)
{
	for (unsigned int i = 0; i < sizeof(faults) / sizeof(faults[0]); i++)
	{
		if (faults[i].errcode == errCode)
		{
			return faults[i].errmsg;
		}
	}

	return "δ֪����";
}

struct OperationResult::OperationResultInternal
{
	bool result;
	std::string errmsg;
	Operation_ErrorCode errCode;
};
OperationResult::OperationResult(const OperationResult& result)
{
	internal = new OperationResultInternal;
	internal->result = result.internal->result;
	internal->errmsg = result.internal->errmsg;
	internal->errCode = result.internal->errCode;
}
OperationResult::OperationResult()
{
	internal = new OperationResultInternal;
	internal->result = true;
	internal->result = "success";
	internal->errCode = Operation_Success;
}

OperationResult::OperationResult(Operation_ErrorCode errCode, const std::string& errmsg)
{
	internal = new OperationResultInternal;
	internal->result = errCode == Operation_Success ? true : false;
	internal->errCode = errCode;
	internal->errmsg = errmsg == "" ? toErrorString(errCode) : errmsg;
}

OperationResult::~OperationResult()
{
	SAFE_DELETE(internal);
}

OperationResult& OperationResult::operator=(const OperationResult& result)
{
	internal->result = result.internal->result;
	internal->errmsg = result.internal->errmsg;
	internal->errCode = result.internal->errCode;

	return *this;
}
OperationResult::operator bool() const
{
	return internal->result;
}
bool OperationResult::operator !() const
{
	return !internal->result;
}
OperationResult::operator std::string() const
{
	return internal->errmsg;
}

std::string OperationResult::errorMessage() const
{
	return internal->errmsg;
}

Operation_ErrorCode OperationResult::errorCode() const
{
	return internal->errCode;
}

}
}