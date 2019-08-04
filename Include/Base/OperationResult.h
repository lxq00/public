#pragma once
#include "Base/IntTypes.h"
#include "Base/Defs.h"

namespace Public {
namespace Base {

enum Operation_ErrorCode
{
	Operation_Fail = -1,
	Operation_Success = 0,
	Operation_Error_UnSupport,					//��֧��

	Operation_Error_Password = 100,				//�˺��������
	Operation_Error_OldPassword,				//���������
	Operation_Error_Port,						//�˿ڴ���
	Operation_Error_Authen,						//��֤ʧ��

	Operation_Error_NetworkErr = 200,			//�������
	Operation_Error_NetworkTimeOut,				//���糬ʱ

	Operation_Error_ParseParam = 300,			//������������
	Operation_Error_ParseObject,				//�����������
	Operation_Error_ParseFile,					//�����ļ�����
	Operation_Error_ParseUrl,					//����url����
	Operation_Error_Request,					//�������
	Operation_Error_Param,						//��������
	Operation_Error_DBEroor,					//���ݿ����
	Operation_Error_SetCfg,						//�������ô���
	Operation_Error_GetCfg,						//��ȡ���ô���
	Operation_Error_QueryFail,					//��ѯʧ��
	Operation_Error_QueryTimeOut,				//��ѯ��ʱ
	Operation_Error_ControlFail,				//����ʧ��
	Operation_Error_ControlTimeOut,				//���Ƴ�ʱ
	Operation_Error_RepeatOPeration,			//�ظ�����

	Operation_Error_InitFail = 400,				//��ʼ��ʧ��
	Operation_Error_ResourceUninit,				//��Դδ��ʼ��
	Operation_Error_ResourceCreateFail,			//��Դ����ʧ��
	Operation_Error_ResourceNotExist,			//��Դ������
	Operation_Error_ResourceExisted,			//��Դ�Ѵ���
	Operation_Error_DeviceNotExist,				//�豸������
	Operation_Error_DeviceExisted,				//�豸�Ѵ���
	Operation_Error_DeviceOffOnline,			//�豸����
	Operation_Error_ChannelNotExist,			//ͨ��������
	Operation_Error_ChannelExisted,				//ͨ���Ѵ���
	Operation_Error_ChannedOffOnline,			//ͨ������
	Operation_Error_ServiceNotExist,			//���񲻴���
	Operation_Error_ServiceExisted,				//�����Ѵ���
	Operation_Error_ServiceOffOnline,			//��������
	Operation_Error_FileExisted,				//�ļ��Ѵ���
	Operation_Error_FileNotExist,				//�ļ�������
	Operation_Error_EnterpriseExisted,			//��ҵ�Ѵ���
	Operation_Error_EnterpriseNotExist,			//��ҵ������
	Operation_Error_PointExisted,				//��λ�Ѵ���
	Operation_Error_PointNotExist,				//��λ������

	Operation_Error_InvalidRecord = 500,		//��Ч��¼��
	Operation_Error_InvalidService,				//��Ч�ķ���
	Operation_Error_InvalidDevice,				//��Ч���豸

	Operation_Error_Send = 600,					//��������ʧ��
	Operation_Error_Recive,						//��������ʧ��
	Operation_Error_OpenFile,					//���ļ�ʧ��
	Operation_Error_WriteFile,					//д���ļ�ʧ��
	Operation_Error_ReadFile,					//��ȡ�ļ�ʧ��

	Operation_Error_DiskErr = 700,				//�����쳣����
	Operation_Error_DiskFull,					//������


	Operation_Error_StreamType	= 1000,			//�����ʹ���
	Operation_Error_StreamCmd,					//���ſ����������

};

class BASE_API OperationResult
{
public:
	OperationResult(const OperationResult& result);
	OperationResult();
	//ϵͳ������
	OperationResult(Operation_ErrorCode errorCode, const std::string& errmsg = "");
	//���result == false�������getlasterror��ȡ

	~OperationResult();

	OperationResult& operator=(const OperationResult& result);
	operator bool() const;
	bool operator !() const;
	operator std::string() const;

	std::string errorMessage() const;
	Operation_ErrorCode errorCode() const;
private:
	struct OperationResultInternal;
	OperationResultInternal* internal;
};

}
}