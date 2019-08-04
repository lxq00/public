#ifndef __READ_ROW_H__
#define __READ_ROW_H__
#include "Read_Cell.h"
#include "Read_Sheet.h"

namespace Public {
namespace Excel {

class Read_Row :public WorkBook::Row
{
public:
	Read_Row(const shared_ptr<WorkBook::Sheet>& sheet, uint32_t m_rowNum);
	~Read_Row();
	//��ȡĳһ����ĳһ�еı�� 
	shared_ptr<WorkBook::Cell> cell(uint32_t colNum);

	//--------------�������
	///��ȡ����,����
	Value data(uint32_t colNum) const ;
	//--------------��ǰ����������
	uint32_t maxColNum();

	//--------------��ǰ�е��к�
	uint32_t rowNum();
private:
	shared_ptr<WorkBook::Sheet> sheet;
	uint32_t m_rowNum;
};

}
}

#endif //__READ_ROW_H__