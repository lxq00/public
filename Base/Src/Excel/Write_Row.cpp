#include "Write_Row.h"
#include "Write_Cell.h"
#include "Write_Sheet.h"
#include "Write_Range.h"

using namespace Public;
using namespace Base;

Write_Row::Write_Row(const shared_ptr<WorkBook::Sheet>& _worksheet, xlslib_core::worksheet* _writesheet, uint32_t _rowNum)
	: worksheet(_worksheet), writesheet(_writesheet), m_rowNum(_rowNum)
{

}
Write_Row::~Write_Row()
{
	
}
//�������ݣ�����
shared_ptr<WorkBook::Cell> Write_Row::setData(uint32_t colNum, const Value& val)
{
	if (worksheet == NULL)
	{
		return shared_ptr<WorkBook::Cell>();
	}
	return worksheet->setData(m_rowNum, colNum, val);
}
//--------------�������		
bool Write_Row::font(const shared_ptr<WorkBook::Font>& font)
{
	if (worksheet == NULL)
	{
		return false;
	}
	worksheet->font(font);
	
	return true;
}
//--------------�����ɫ
bool Write_Row::fill(const WorkBook::Color& color)
{
	if (worksheet == NULL)
	{
		return false;
	}
	worksheet->fill(color);
	return true;
}
bool Write_Row::fill(const shared_ptr<WorkBook::Color>& color)
{
	fill(*color.get());
	return true;
}
//--------------��ʾ��ʽ
bool Write_Row::format(const WorkBook::Format& _fmt)
{
	if (worksheet == NULL)
	{
		return false;
	}
	worksheet->format(_fmt);
	return true;
}
bool Write_Row::format(const shared_ptr<WorkBook::Format>& _fmt)
{
	format(*_fmt.get());
	return true;
}
//--------------��Ԫ��߿�
bool Write_Row::side(const WorkBook::Side& _sd)
{
	if (worksheet == NULL)
	{
		return false;
	}
	shared_ptr<WorkBook::Range> ran = range(0, maxColNum());
	if (ran == NULL)
	{
		return false;
	}
	return ran->side(_sd);
}
bool Write_Row::side(const shared_ptr<WorkBook::Side>& _sd)
{
	return side(*_sd.get());
}

//--------------���򣬵��е�ָ���кϲ�, ��ʼ���кţ��������к�
shared_ptr<WorkBook::Range>  Write_Row::range(uint32_t startColNum, uint32_t stopColNum)
{
	return worksheet->range(m_rowNum, startColNum, m_rowNum, stopColNum);
}

//--------------��ʾ������,���е�������ʾ
bool  Write_Row::show(bool showflag)
{
	shared_ptr<WorkBook::Row> r_row = worksheet->row(m_rowNum);
	int r_col = 0;
	while (r_col++)
	{
		shared_ptr<WorkBook::Cell> r_cell = worksheet->cell(m_rowNum, r_col);
		((Write_Cell*)r_cell.get())->m_cell->hidden(showflag);
		if(r_cell != NULL)
		{
			break;
		}
	}
	return true;
}

//--------------��ǰ����������
uint32_t Write_Row::maxColNum()
{
	Write_Sheet* _writesheet = (Write_Sheet*)worksheet.get();

	return _writesheet->getRowMaxColNum(m_rowNum);
}
//--------------��ǰ�е��к�
uint32_t Write_Row::rowNum()
{
	return m_rowNum;
}

uint32_t Write_Row::height() 
{ 
	return m_height;
}

bool Write_Row::setHeight(uint32_t height)
{
	if (height < 0)
	{
		return false;
	}
	m_height = height;
	writesheet->label(m_rowNum, 1, " ");
	writesheet->rowheight(m_rowNum, m_height);
	return true;
}
