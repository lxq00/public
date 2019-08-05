//
//  Copyright (c)1998-2012, Public Technology
//  All Rights Reserved.
//
//	Description:
//	$Id: File.h 252 2013-12-18 04:40:28Z  $
//

#ifndef __BASE_FILEFIND_H__
#define __BASE_FILEFIND_H__

#include <string>
#ifdef WIN32
#include <io.h>
#endif

#include "Base/Defs.h"
#include "Base/IntTypes.h"
#include "Base/Shared_ptr.h"

namespace Public {
namespace Base {

typedef enum
{
	RegExType_Normal = 0,		//����ģʽ
	RegExType_InCase,			//���Դ�Сдģʽ
}RegExType;

//������ʽ�������
class BASE_API RegEx
{
public:
	//����һ������ƥ���������ʽ����
	RegEx(const std::string& pattern, RegExType type = RegExType_Normal);
	~RegEx();

	static bool regex_match(const std::string& str, const RegEx& regex);
private:
	struct RegExInternal;
	RegExInternal* internal;
};


} // namespace Base
} // namespace Public

#endif //__BASE_FILE_H__

