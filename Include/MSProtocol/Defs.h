//
//  Copyright (c)1998-2012, Technology
//  All Rights Reserved.
//
//  Description:
//  $Id: Defs.h 84 2013-11-28 07:12:20Z  $
//


#ifndef __PROTOCOL_DEFS_H__
#define __PROTOCOL_DEFS_H__


// �Ƿ�֧��IPv6 �ȹر�
//#define SUPPORT_IPV6


// WIN32 Dynamic Link Library
#ifdef _MSC_VER

#ifdef MSPROTOCOL_DLL_BUILD
#define  MSProtocol_Export _declspec(dllexport)
#else
#define MSProtocol_Export _declspec(dllimport)
#endif

#else

#define MSProtocol_Export

#endif

/// ��ӡ Protocol �汾��Ϣ
#ifdef WIN32
#pragma warning (disable : 4251)  //���õ�����ӿ���ʹ����STL��C4251�ľ���
#endif

#endif //__PROTOCOL_DEFS_H__

