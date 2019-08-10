#
#  Copyright (c)1998-2012, Chongqing Public Technology
#  All Rights Reserved.
#
#	 Description:
#  $Id: Base.mk 2055 2014-07-31 03:58:39Z lixiaoqiang $  
#

#�����Լ�дһ��.platform �������� ƽ̨����̬�⿽����λ��, �����Լ�ָ������ѡ��μ�.platform
-include .platform

# ���ɿ������
# �������ļ�Ŀ¼
   SRCS_PATH = Src
	
# �������
   SHARDLIB_NAME = 
   STATICLIB_NAME = 

# Ӧ�ó���ѡ��
# Ӧ�ó���Ĵ���·��
   APP_SRCS_PATH = test

# Ӧ�ó�������
   APP_NAME = 

# ��Ŀ¼������Ҫ��Ŀ¼
   SUB_INCLUDE = 


# Ӧ�ó��������Ŀ�(���˱���ı���Ŀ�)
  LIBS = Base/Base.lib Network/Network.lib RTSP/RTSP.lib
  LIBS_DBG = Base/Base_debug.lib Network/Network_debug.lib RTSP/RTSP_debug.lib

#���Ϊ�������Զ����ɵĿ⣬�����޸�
#AutoAddOtherDefineStart
LDLIBS = 
LDLIBS_DBG = 
#AotoAddOtherDefineEnd


include ${PRJ_PATH}/mk/platform/x86
include ${PRJ_PATH}/mk/builddir_v2.mk


#UserDefined Option
 -include userdefinedOption.mk

include ${PRJ_PATH}/mk/checksvn.mk					
include ${PRJ_PATH}/mk/print.mk
include ${PRJ_PATH}/mk/option.mk
include ${PRJ_PATH}/mk/rules_v2.mk



