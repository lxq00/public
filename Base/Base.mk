#
#  Copyright (c)1998-2012, Chongqing Xunmei Technology
#  All Rights Reserved.
#
#	 Description:
#  $Id: Base.mk 2055 2014-07-31 03:58:39Z lixiaoqiang $  
#

#�����Լ�дһ��.platform �������� ƽ̨����̬�⿽����λ��, �����Լ�ָ������ѡ��μ�.platform
-include .platform

# ���ɿ������
# �������ļ�Ŀ¼
   SRCS_PATH =  Src Src/Expression Src/uuid
	
# �������
   SHARDLIB_NAME = 
   STATICLIB_NAME = Base

# Ӧ�ó���ѡ��
# Ӧ�ó���Ĵ���·��
   APP_SRCS_PATH = 

# Ӧ�ó�������
   APP_NAME = 

# ��Ŀ¼������Ҫ��Ŀ¼
   SUB_INCLUDE = 


# Ӧ�ó��������Ŀ�(���˱���ı���Ŀ�)
  LIBS = 
  LIBS_DBG = 

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



