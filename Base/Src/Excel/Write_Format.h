#ifndef __WRITE_FORMAT_H__
#define __WRITE_FORMAT_H__
#include "Base/Excel.h"

namespace Public {
namespace Base {

	struct WorkBook::Format::FormatInternal
	{
		FormatInternal() :xalign(ALIGN_X_NONE), yalign(ALIGN_Y_TOP), orient(TxtoriType_NONE), fomat(FomatType_GENERAL) {}

		ALIGN_X_Type xalign;
		ALIGN_Y_Type yalign;
		TxtoriType   orient;
		FomatType	 fomat;
	};

}
}

#endif //__WRITE_FORMAT_H__