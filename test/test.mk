#
#  Copyright (c)1998-2012, Chongqing Public Technology
#  All Rights Reserved.
#
#	 Description:
#  $Id: Base.mk 2055 2014-07-31 03:58:39Z lixiaoqiang $  
#

#�����Լ�дһ��.platform �������� ƽ̨����̬�⿽����λ��, �����Լ�ָ������ѡ��μ�.platform
-include .platform

# ����
# ����·��
   SRCS_PATH = Src
	
# �������
   SHARDLIB_NAME = 
   STATICLIB_NAME = 


# Ӧ�ó�������
   APP_NAME = test

# ��Ŀ¼������Ҫ��Ŀ¼
   SUB_INCLUDE = 


# Ӧ�ó��������Ŀ�(���˱���ı���Ŀ�)
  LIBS = ${PRJ_OUTPUTDIR}/libBase.a ${PRJ_OUTPUTDIR}/libNetwork.a ${PRJ_OUTPUTDIR}/libRTSP.a  -lpthread
  LIBS_DBG = ${PRJ_OUTPUTDIR}/libBase_debug.a ${PRJ_OUTPUTDIR}/libNetwork_debug.a ${PRJ_OUTPUTDIR}/libRTSP_debug.a -lpthread

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



