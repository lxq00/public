#include "Write_Format.h"

using namespace Public;
using namespace Base;

WorkBook::Format::Format()
{
	internal = new FormatInternal();
}
WorkBook::Format::Format(const Format& format)
{
	internal = new FormatInternal();
	*internal = *format.internal;
}
WorkBook::Format& WorkBook::Format::operator = (const Format& val)
{
	*internal = *val.internal;

	return *this;
}

WorkBook::Format::~Format()
{
	SAFE_DELETE(internal);
}

//��ʾ��ʾ��ʽ
bool WorkBook::Format::setFormat(FomatType type)
{
	internal->fomat = type;
	return true;
}

//���ö��뷽ʽ
bool WorkBook::Format::setAlign(ALIGN_X_Type xalign, ALIGN_Y_Type yalign)
{ 
	internal->xalign = xalign;
	internal->yalign = yalign;
	
	return true;
}

//������ʾ��ʽ
bool WorkBook::Format::setTxtori(TxtoriType type)
{ 
	internal->orient = type;
	return false; 
}