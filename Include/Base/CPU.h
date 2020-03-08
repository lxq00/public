#ifndef _LIMITCPU_H__
#define _LIMITCPU_H__
#include "Base/Defs.h"

namespace Public
{
namespace Base
{
class BASE_API CPU
{
	CPU();

public:
	~CPU();

	// ��ȡcpu������
	// @param logiccpu �߼�������
	// @param corecpu ���Ĵ�����
	static bool getCpuCount(unsigned int &logiccpu, unsigned int &corecpu);

	// ����������cpu����
	// scale �������������� 4/1��cpu
	static bool limit(unsigned int scale);
};
} // namespace Base
} // namespace Public

#endif