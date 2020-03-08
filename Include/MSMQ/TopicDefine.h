#pragma  once
#include "Base/Base.h"
using namespace Public::Base;

namespace Milesight {
namespace MQ {



//--------------------------����ΪϵͳͨѶ��Topic����
//������ص�topic
//
////------------------------------ģ���ڲ���ͨѶ

#define Topic_Module_SYSTEMINFO_NOTIFY		"Topic_Module_systeminfo_notify"


////����ͨѶ
//�����matser֪ͨ
#define Topic_Module_System_CalcMaster				"topic_mod_sytmem_calcMaster"			//?/?

////�豸��ص�ͨѶ
#define Topic_Module_Device_Reload			"topic_mod_device_reload"			//?/?
#define Topic_Module_Device_Failover		"topic_mod_device_Failover"
//
////-----------------------------���Ȩ��
#define Topic_Module_Authen_Check			"topic_mod_authen_check"			//?/MSProtoUserInfo
//
////-------------------------------����������ص�
//
//
////-----------------------------�¼�����
//#define Topic_Module_EventRules_Load		"topic_mod_eventrules_load"			//?/MSProtoEventRules
#define Topic_Module_EventRules_Action_NotifyUser	"topic_mod_eventrules_action_notifyuser"	//MSProtoNotifyToUser/?
#define Topic_Module_EventRules_Action_NotifyDevice "topic_mod_eventrules_action_notifydevice" //MSProtoNotifyToDevice/?
#define Topic_Module_EventRules_Action_SystemEvent "topic_mod_eventrules_action_systemevent"	//MSProtoLogInfo/?

#define Topic_Module_EventRules_Source_CameraAlarm	"topic_mod_eventrules_source_cameraalarm"	//MSProtoCamerAlarmInfo/?
#define Topic_Module_EventRules_Source_DeviceStatus "topic_mod_eventrules_source_devicestatus" //MSProtoDeviceStatusInfos/?
#define Topic_Module_EventRules_Source_ServerNotify	"topic_mod_eventrules_source_servernotify"	//MSProtoServerNotify/?

////-----------------------������־
//#define Topic_Module_Operation				"topic_mod_operation"				//
//
////------------------------ϵͳ
#define Topic_Module_QueryServerDataBaseLog		"topic_mod_queryserverdatabaselog"		//MSProtoQueryDatabaseLogRequest/MSProtoQueryDatabaseLogResponse

//-----------------------------------ϵͳͨѶ��صĶ���
#define Topic_System_NewSystemInfo			"topic_system_newsysteminfo"		//MSProtoSystemInfo/?
#define Topic_System_ModSystemInfo			"topic_system_modsysteminfo"		//MSProtoSystemInfo/?
#define Topic_System_ModServerInfo			"topic_system_modserverinfo"		//MSProtoServerInfo/?

//��topicת��http reqeust pathʱ��ʽ����:
// topic_device_getdisconvery ת�� request pathΪ /device/getdisconvery
//�豸���
#define Topic_Device_GetDisconvery			"topic_device_getdisconvery"		//?/MSProtoDisconveryDeviceInfos
#define Topic_Device_TryLogin				"topic_device_trylogin"				//MSProtoDisconveryDeviceInfos/MSProtoDeviceOptResults
#define Topic_Device_UpdateDevices			"topic_device_updatedevices"		//MSProtoDisconveryDeviceInfos/MSProtoDeviceOptResults
#define Topic_Device_DelDevices				"topic_device_deldevices"			//MSProtoDeleteDeviceIps/MSProtoDeviceOptResults
#define Topic_Device_GetDevicesStatus		"topic_device_getdevicesstatus"		//?/MSProtoDeviceStatusInfos
#define Topic_Device_GetDevicesList			"topic_device_getdeviceslist"		//?/MSProtoDeviceStatusInfos
//#define Topic_Device_ScanDC					"topic_device_scandc"				//?/?
#define Topic_Device_CMD_PTZ				"topic_device_cmd_ptz"				//MSProtoPtzInfo/?
#define Topic_Device_CMD_SNAP				"topic_device_cmd_snap"				//MSProtoSnapRequest/MSProtoSnapResponse
#define Topic_Device_CMD_FishEye			"topic_device_cmd_fisheye"			//MSProtoSnapRequest/MSProtoSnapResponse

//������������
#define Topic_Config_Camera                 "topic_config_camera"               //MSProtoConfigInfo/?

//�豸����صĲ���
#define Topic_Group_GetAllInfos				"topic_group_getallinfos"			//?/MSProtoDeviceGroupAllInfos
#define Topic_Group_UpdateInfos				"topic_group_updateinfos"			//MSProtoDeviceGroupInfos/?
#define Topic_Group_UpdateAllInfos			"topic_group_updateallinfos"		//MSProtoDeviceGroupAllInfos/?
#define Topic_Group_DeleteInfos				"topic_group_deleteinfos"			//MSProtoDeleteGroupIds/?
#define Topic_Group_ModiDeviceBind			"topic_group_updatedevbind"			//MSProtoDeviceGroupBindInfo/?
#define Topic_Group_Notify					"topic_group_notify"				//MSProtoDeviceGroupAllInfo/?
//��ͼ���
#define Topic_View_GetViews				    "topic_view_getviews"			    //?/MSProtoViewInfos
#define Topic_View_UpdateViews				"topic_view_updateview"			    //MSProtoViewInfo/?
#define Topic_View_DeleteViews			    "topic_view_delview"		        //MSProtoViewIds/?
#define Topic_View_Notify					"topic_View_notify"				    //MSProtoViewInfo/?

//ϵͳ���
#define Topic_System_Info					"topic_system_info"					//?/MSProtoSystemInfo
#define Topic_System_GroupNotify			"topic_system_groupnotify"			//MSProtoVMSSystemNotify
//#define Topic_System_QuerySystemNotify		"topic_system_querysystemnotify"	//?/MSProtoVMSSystemNotify

//�û����
#define Topic_User_Login					"topic_user_login"					//MSProtoLoginRequest/MSProtoLoginResponse
#define Topic_User_Logout					"topic_user_logout"					//MSProtoLogoutRequest/?
#define Topic_User_UpdateUser				"topic_user_updateuser"				//MSProtoUserInfo/?
#define Topic_User_GetUser				    "topic_user_getuser"				//MSProtoUserInfo/?

//�û���ɫ���
#define Topic_UserRoles_Update				"topic_user_update"				    //MSProtoUserRolesInfo/?
#define Topic_UserRoles_Delete				"topic_user_detele"				    //MSProtoUserRolesInfo/?
#define Topic_UserRoles_Query				"topic_user_query"				    //?/MSProtoUserRolesDetailInfos

//-------------------------------EventRule���
#define Topic_Eventrule_Device_Status		"topic_eventrule_device_status"		//MSProtoDeviceStatusInfo/?
#define Topic_Eventrule_Device_Alarm		"topic_eventrule_device_alarm"		//MSProtoCamerAlarmInfo/?

//����Ϊ¼����ص�

#define Topic_Module_RecordStatusNotify	    "topic_module_recordstatusnotify"	//MSProtoRecordStatusNotify/

#define Topic_Module_Stroage_SchdulesReload	    "topic_module_storage_schedules_reload"			//?/?

#define Topic_Storage_QueryDisk				"topic_storage_querydisk"			//?/MSProtoDiskInfos
#define Topic_Storage_SetDisk				"topic_storage_setdisk"				//MSProtoDiskSetting/?
#define Topic_Storage_QuerySchedule			"topic_storage_queryschedule"		//MSProtoDeviceId/MSProtoStorageSchedule
#define Topic_Storage_UpdateSchedule		"topic_storage_updateschedule"		//MSProtoStorageSchedule/?
#define Topic_Storage_QueryDeviceSetting	"topic_storage_querydevset"			//MSProtoDeviceId/MSProtoStorageSetting
#define Topic_Storage_UpdateDeviceSetting	"topic_storage_updatedevset"		//MSProtoStorageSetting/?

#define Topic_Storage_AddExtendDisk	        "topic_storage_addextenddisk"	    //MSProtoDiskExtendInfo/
#define Topic_Storage_DelExtendDisk	        "topic_storage_delextenddisk"		//MSProtoDiskExtendInfo/?


#define Topic_Storage_QueryRecord			"topic_storage_queryrecord"			//MSProtoRecordQueryRequest/MSProtoRecordInfos

#define Topic_Storage_QueryTag				"topic_storage_querytag"			//MSProtoTagQueryRequest/MSProtoTagInfos
#define Topic_Storage_UpdateTag				"topic_storage_updatetag"			//MSProtoTagInfo/?
#define Topic_Storage_DelTag				"topic_storage_deltag"				//MSProtoTagInfo?

#define Topic_Storage_QueryLock				"topic_storage_querylock"			//MSProtoRecLockQueryRequest/MSProtoRecLockInfos
#define Topic_Storage_UpdateLock			"topic_storage_updatelock"			//MSProtoRecLockInfos/?
#define Topic_Storage_DelLock				"topic_storage_dellock"				//MSProtoRecLockInfos?

#define Topic_Storage_DeviceRecordInfo		"topic_storage_devrecordinfo"		//MSProtoCameraRecordInfos

#define Topic_Storage_CheckAddress          "topic_storage_checkaddress"        
//---------------------------------����Ϊϵͳȫ������-------------


}
}