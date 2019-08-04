
#pragma once

#ifndef _RTSP_CLIENT_ERROR_CODE_H_
#define _RTSP_CLIENT_ERROR_CODE_H_

//error code*******************************************************
#define RTSP_ERR_NO_ERROR					(0)   //û�д���
#define RTSP_ERR_UNKOWN						(-1)  //δ֪����
#define RTSP_ERR_CONNECT_FAILED				(-2)  //���ӷ�����ʧ��
#define RTSP_ERR_MAX_TASK					(-3)  //�������������
#define RTSP_ERR_NOT_CONNECTED				(-4)  //��δ���ӷ�����
#define RTSP_ERR_INVLID_HANDLE				(-5)  //��������Ч
#define RTSP_ERR_INVLID_PARAM				(-6)  //�Ƿ�����
#define RTSP_ERR_URL_CHECK_FAILED			(-7)  //URL����޷�ͨ��
#define RTSP_ERR_OPTION_FAILED				(-8)  //OPTION ����ʧ��
#define RTSP_ERR_DISCRIBE_FAILED			(-9)  //DISCRIBE ����ʧ��
#define RTSP_ERR_SETUP_FAILED				(-10) //SETUP ����ʧ��
#define RTSP_ERR_PLAY_FAILED				(-11) //PLAY ����ʧ��
#define RTSP_ERR_GET_PARAMETER_FAILED		(-12) //GET_PARAMETER �������
#define RTSP_ERR_STARTRECV_FAILED			(-13) //�������ݽ���ʧ��
#define RTSP_ERR_RECV_TYPE_ERROR			(-14) //���ݽ��շ�ʽ����
#define RTSP_ERR_USER_INFO_ERROR			(-15) //�û���֤����Ϊ��

#define RTSP_ERR_UNINIT						(-100) //SDKδ��ʼ��
#define RTSP_ERR_UNSUPPORT					(-101) //Ŀǰ��֧��
#define RTSP_ERR_CREATECLIENT_FAILED		(-102) //����RTSP�ͻ���ʵ��ʧ��

typedef enum {
	RTSPStatus_CMDError,
	RTSPStatus_Timeout,
	RTSPStatus_DisConnect,
	RTSPStatus_ConnectSuccess, 
	RTSPStatus_Eof
}RTSPStatus_t;


#endif
