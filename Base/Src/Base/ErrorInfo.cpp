#include "Base/ErrorInfo.h"
namespace Public
{
namespace Base
{

struct ErrorItemInfo
{
	Error_Code code;
	const char *errmsg;
};
std::string ErrorInfo::getErrorMsg(Error_Code code)
{
	static ErrorItemInfo errorItemInfosMap[] =
		{
			{Error_Code_Success, "�ɹ�"},
			{Error_Code_Fail, "ʧ��"},
			{Error_Code_NotSupport, "��֧��"},
			{Error_Code_Param, "��������"},

			//100�࣬Ȩ����صĴ�����
			{Error_Code_Password, "�������"},
			{Error_Code_OldPassword, "���������"},
			{Error_Code_Username, "�û�������"},
			{Error_Code_Authen, "��֤ʧ��"},

			//200�࣬������صĴ�����
			{Error_Code_Network, "�������"},
			{Error_Code_NetworkTimeout, "���糬ʱ"},
			{Error_Code_ConnectTimeout, "���ӳ�ʱ"},
			{Error_Code_CommunicateTimeout, "ͨѶ��ʱ"},
			{Error_Code_Unreachable, "���ܵ���"},

			//300�࣬���ݽ�����ش�����
			{Error_Code_ParseParam, "������������"},
			{Error_Code_ParseObject, "�����������"},
			{Error_Code_ParseFile, "�����ļ�����"},
			{Error_Code_ParseUrl, "����url����"},
			{Error_Code_Request, "�������"},
			{Error_Code_Response, "�ظ�����"},

			//400�࣬�豸������ش�����
			{Error_Code_InitFail, "��ʼ��ʧ��"},
			{Error_Code_ResourceUninit, "��Դδ��ʼ��"},
			{Error_Code_ResourceCreateFail, "��Դ����ʧ��"},
			{Error_Code_ResourceNotExist, "��Դ������"},
			{Error_Code_ResourceExisted, "��Դ�Ѵ���"},
			{Error_Code_DeviceNotExist, "�豸������"},
			{Error_Code_DeviceExisted, "�豸�Ѵ���"},
			{Error_Code_DeviceOffOnline, "�豸����"},
			{Error_Code_ServiceNotExist, "���񲻴���"},
			{Error_Code_ServiceExisted, "�����Ѵ���"},
			{Error_Code_CameraNotExist, "ͨ��������"},
			{Error_Code_CameraExisted, "ͨ���Ѵ���"},
			{Error_Code_CameraOffOnline, "ͨ������"},
			{Error_Code_FileExisted, "�ļ��Ѵ���"},
			{Error_Code_FileNotExist, "�ļ�������"},
			{Error_Code_Busy, "��Դæ"},

			//500�࣬¼�����
			{Error_Code_InvalidRecord, "��Ч��¼��"},
			{Error_Code_InvalidService, "��Ч�ķ���"},
			{Error_Code_InvalidDevice, "��Ч���豸"},

			//600�࣬�ļ��������
			{Error_Code_Send, "��������ʧ��"},
			{Error_Code_Recive, "��������ʧ��"},
			{Error_Code_OpenFile, "���ļ�ʧ��"},
			{Error_Code_WriteFile, "д���ļ�ʧ��"},
			{Error_Code_ReadFile, "��ȡ�ļ�ʧ��"},

			//700�࣬������ز���
			{Error_Code_DiskErr, "�����쳣����"},
			{Error_Code_DiskFull, "������"},

			//800�࣬����ز���
			{Error_Code_StreamType, "�����ʹ���"},
			{Error_Code_StreamCmd, "���ſ����������"},

			//900�࣬���ݿ����
			{Error_Code_OpenDatabase, "�����ݿ�ʧ��"},
			{Error_Code_CreateDatabase, "�������ݿ�ʧ��"},
			{Error_Code_ExecDatabase, "ִ�����ݿ�����ʧ��"},
		};

	for (uint32_t i = 0; i < sizeof(errorItemInfosMap) / sizeof(ErrorItemInfo); i++)
	{
		if (errorItemInfosMap[i].code == code)
			return errorItemInfosMap[i].errmsg;
	}

	return "δ֪����";
}

ErrorInfo::ErrorInfo(const ErrorInfo &ret)
{
	errcode = ret.errcode;
	errmsgstr = ret.errmsgstr;
}
//ϵͳ������
ErrorInfo::ErrorInfo(Error_Code _errorCode, const std::string &_errmsg)
{
	errcode = _errorCode;
	errmsgstr = _errmsg;

	if (errmsgstr.length() <= 0)
		errmsgstr = getErrorMsg(errcode);
}
//���result == false�������getlasterror��ȡ

ErrorInfo ::~ErrorInfo() {}

ErrorInfo &ErrorInfo::operator=(const ErrorInfo &ret)
{
	errcode = ret.errcode;
	errmsgstr = ret.errmsgstr;

	return *this;
}

bool ErrorInfo::operator==(const ErrorInfo &ret) const
{
	return errcode == ret.errcode && errmsgstr == ret.errmsgstr;
}

bool ErrorInfo::operator!=(const ErrorInfo &ret) const
{
	return errcode != ret.errcode || errmsgstr != ret.errmsgstr;
}

bool ErrorInfo::error() const
{
	return errcode != Error_Code_Success;
}

////�Ƿ�ʧ��
ErrorInfo::operator bool() const
{
	return errcode != Error_Code_Success;
}

//�Ƿ�����
bool ErrorInfo::operator!() const
{
	return errcode == Error_Code_Success;
}
std::string ErrorInfo::errorMessage() const
{
	return errmsgstr;
}
int ErrorInfo::errorCode() const
{
	return errcode;
}

} // namespace Base
} // namespace Public