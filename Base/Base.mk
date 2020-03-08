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
   SRCS_PATH =  Src Src/Base Src/Expression Src/uuid Src/regex Src/Excel Src/Excel/libxls Src/Excel/xlslib Src/Excel/sqlite3 Src/zip Src/zip/miniZip Src/XML Src/XML/tinyxml
	
# �������
   SHARDLIB_NAME = 
   STATICLIB_NAME = Base


# Ӧ�ó�������
   APP_NAME = 

# ��Ŀ¼������Ҫ��Ŀ¼
   SUB_INCLUDE = 


# Ӧ�ó��������Ŀ�(���˱���ı���Ŀ�)
#  LIBS = /usr/lib/libiconv.dylib
#  LIBS_DBG = /usr/lib/libiconv.dylib
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



