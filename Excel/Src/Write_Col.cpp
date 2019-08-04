#include "Write_Col.h"
#include "Write_Sheet.h"

using namespace Public;
using namespace Excel;

Write_Col::Write_Col(const shared_ptr<WorkBook::Sheet>& _worksheet, xlslib_core::worksheet* _writesheet, uint32_t _colNum)
	: worksheet(_worksheet), writesheet(_writesheet), m_colNum(_colNum)
{

}
Write_Col::~Write_Col()
{

}
//�������ݣ�����
shared_ptr<WorkBook::Cell> Write_Col::setData(uint32_t rowNum, const Value& val)
{
	if (worksheet == NULL)
		return shared_ptr<WorkBook::Cell>();
	return worksheet->setData(rowNum, m_colNum, val);
}
//--------------�������		
bool Write_Col::font(const shared_ptr<WorkBook::Font>& font)
{
	if (worksheet == NULL)
	{
		return false;
	}
	return worksheet->font(font);
}
//--------------�����ɫ
bool Write_Col::fill(const WorkBook::Color& color)
{
	if (worksheet == NULL)
	{
		return false;
	}
	return worksheet->fill(color);
}
bool Write_Col::fill(const shared_ptr<WorkBook::Color>& color)
{
	return fill(*color.get());
}
//--------------��ʾ��ʽ
bool Write_Col::format(const WorkBook::Format& _fmt)
{
	if (worksheet == NULL)
	{
		return false;
	}
	return worksheet->format(_fmt);
}
bool Write_Col::format(const shared_ptr<WorkBook::Format>& _fmt)
{
	return format(*_fmt.get());
}
//--------------��Ԫ��߿�
bool Write_Col::side(const WorkBook::Side& _sd)
{
	if (worksheet == NULL)
	{
		return false;
	}
	shared_ptr<WorkBook::Range> ran = range(0, maxRowNum());
	if (ran == NULL)
	{
		return false;
	}
	
	return ran->side(_sd);
}
bool Write_Col::side(const shared_ptr<WorkBook::Side>& _sd)
{
	return side(*_sd.get());
}

//--------------���򣬵��е�ָ���кϲ�, ��ʼ���кţ��������к�
shared_ptr<WorkBook::Range>  Write_Col::range(uint32_t startRowNum, uint32_t stopRowNum)
{ 
	return worksheet->range(startRowNum, m_colNum, stopRowNum, m_colNum);
}

//--------------��ʾ������,���е�������ʾ
//bool  Write_Col::show(bool showflag)
//{
//	shared_ptr<WorkBook::Col> c_col = worksheet->col(m_colNum);
//
//	shared_ptr<xlslib_core::range> c_range = worksheet->range(m_colNum, 0, m_colNum, c_col->maxRowNum());
//	c_range->hidden(showflag);
//
//	return true;
//}

//--------------��ǰ�е��к�
uint32_t Write_Col::colNum()
{
	return m_colNum;
}
//--------------��ǰ�е���������
uint32_t Write_Col::maxRowNum()
{
	Write_Sheet* _writesheet = (Write_Sheet*)worksheet.get();

	return _writesheet->getColMaxRowNum(m_colNum);
}
//�п�
//uint32_t Write_Col::width()
//{ 
//	return m_width;
//}
//bool Write_Col::setWidth(uint32_t width)
//{ 
//	if (width < 0)
//	{
//		return false;
//	}
//	m_width = width;
//	writesheet->label(m_colNum, 1, " ");
//	writesheet->colwidth(m_colNum, m_width);
//	return true;
//}
