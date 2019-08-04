#ifndef __Public_EXCEL_DEFINE_H__
#define __Public_EXCEL_DEFINE_H__
#include "Base/Base.h"

using namespace Public::Base;

// WIN32 Dynamic Link Library
#ifdef _MSC_VER

#ifdef EXCEL_DLL_BUILD
#define  EXCEL_API _declspec(dllexport)
#else
#define  EXCEL_API _declspec(dllimport)
#endif

#else
#define EXCEL_API
#endif

namespace Public {
namespace Excel {

//������ʾ��ʽ
typedef enum
{
	FomatType_GENERAL = 0,
	FomatType_NUMBER1,            // 0
	FomatType_NUMBER2,            // 0.00
	FomatType_NUMBER3,            // #,##0
	FomatType_NUMBER4,            // #,##0.00
	FomatType_CURRENCY1,          // "$"#,##0_);("$"#,##0)
	FomatType_CURRENCY2,          // "$"#,##0_);[Red]("$"#,##0)
	FomatType_CURRENCY3,          // "$"#,##0.00_);("$"#,##0.00)
	FomatType_CURRENCY4,          // "$"#,##0.00_);[Red]("$"#,##0.00)
	FomatType_PERCENT1,           // 0%
	FomatType_PERCENT2,           // 0.00%
	FomatType_SCIENTIFIC1,        // 0.00E+00
	FomatType_FRACTION1,          // # ?/?
	FomatType_FRACTION2,          // # ??/??
	FomatType_DATE1,              // M/D/YY
	FomatType_DATE2,              // D-MMM-YY
	FomatType_DATE3,              // D-MMM
	FomatType_DATE4,              // MMM-YY
	FomatType_TIME1,              // h:mm AM/PM
	FomatType_TIME2,              // h:mm:ss AM/PM
	FomatType_TIME3,              // h:mm
	FomatType_TIME4,              // h:mm:ss
	FomatType_DATETIME,           // M/D/YY h:mm
	FomatType_ACCOUNTING1,        // _(#,##0_);(#,##0)
	FomatType_ACCOUNTING2,        // _(#,##0_);[Red](#,##0)
	FomatType_ACCOUNTING3,        // _(#,##0.00_);(#,##0.00)
	FomatType_ACCOUNTING4,        // _(#,##0.00_);[Red](#,##0.00)
	FomatType_CURRENCY5,          // _("$"* #,##0_);_("$"* (#,##0);_("$"* "-"_);_(@_)
	FomatType_CURRENCY6,          // _(* #,##0_);_(* (#,##0);_(* "-"_);_(@_)
	FomatType_CURRENCY7,          // _("$"* #,##0.00_);_("$"* (#,##0.00);_("$"* "-"??_);_(@_)
	FomatType_CURRENCY8,          // _(* #,##0.00_);_(* (#,##0.00);_(* "-"??_);_(@_)
	FomatType_TIME5,              // mm:ss
	FomatType_TIME6,              // [h]:mm:ss
	FomatType_TIME7,              // mm:ss.0
	FomatType_SCIENTIFIC2,        // ##0.0E+0
	FomatType_TEXT                // @
} FomatType;


//������뷽ʽ,X��Y��������|
typedef enum 
{
	ALIGN_X_NONE = 0,//��ʼ��
	ALIGN_X_LEFT,//�����
	ALIGN_X_CENTER ,//���ж���
	ALIGN_X_RIGHT ,//�Ҷ���
}ALIGN_X_Type;

typedef enum {
	ALIGN_Y_TOP,	//���϶���
	ALIGN_Y_CENTER,		//���ж���
	ALIGN_Y_BOTTOM,		//���¶���	
}ALIGN_Y_Type;


//������ʾ��ʽ
typedef enum
{
	TxtoriType_NONE = 0,			//����ģʽ
	TxtoriType_TOPBOTTOMTXT,		//���У����ϵ�����ʾ
	TxtoriType_90NOCLOCKTXT,		//���У�������ʾ
	TxtoriType_90CLOCKTXT			//���У�������ʾ
} TxtoriType;


//�߿���ʾ��ʽ������|�ķ�ʽ
#define SIDE_BOTTOM		1			//�ײ��߿�
#define SIDE_TOP		2			//�ϲ��߿� 
#define SIDE_LEFT		4			//�󲿱߿�
#define SIDE_RIGHT		8			//�Ҳ��߿�


}
}

#ifdef WIN32
#pragma warning(disable:4200)
#endif

#endif //__Public_EXCEL_H__
