#include "Read_Row.h"
#include "Read_Sheet.h"
namespace Public {
namespace Base {

Read_Row::Read_Row(const shared_ptr<WorkBook::Sheet>& _sheet,uint32_t _rowNum)
	: sheet(_sheet), m_rowNum(_rowNum)
{

}
Read_Row::~Read_Row()
{

}
//��ȡĳһ����ĳһ�еı�� 
shared_ptr<WorkBook::Cell> Read_Row::cell(uint32_t colNum)
{ 
	return sheet->setData(m_rowNum, colNum, sheet->data(m_rowNum, colNum));
}

//--------------�������
///��ȡ����,����
Value Read_Row::data(uint32_t colNum) const
{
	return sheet->data(m_rowNum, colNum);
}
//--------------��ǰ����������
uint32_t Read_Row::maxColNum()
{
	Read_Sheet* readsheet = (Read_Sheet*)sheet.get();

	return readsheet->getRowMaxColNum(m_rowNum);
}

//--------------��ǰ�е��к�
uint32_t Read_Row::rowNum()
{
	return m_rowNum;
}

}
}