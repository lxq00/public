#ifndef __READ_COL_H__
#define __READ_COL_H__
#include "Read_Cell.h"
#include "Read_Sheet.h"

namespace Public {
namespace Base {

class Read_Col :public WorkBook::Col
{
public:
	Read_Col(const shared_ptr<WorkBook::Sheet>& sheet, uint32_t m_colNum);
	~Read_Col();
	//��ȡĳһ����ĳһ�еı�� 
	shared_ptr<WorkBook::Cell> cell(uint32_t rowNum);

	//--------------�������
	///��ȡ����,����
	virtual Value data(uint32_t rowNum) const;
	//--------------��ǰ�е������к�
	uint32_t colNum();

	//--------------��ǰ�е���������
	uint32_t maxRowNum();
private:
	shared_ptr<WorkBook::Sheet> sheet;
	uint32_t m_colNum;
};

}
}

#endif //__READ_COL_H__