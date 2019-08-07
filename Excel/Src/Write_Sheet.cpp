#include "Write_Sheet.h"
#include "Write_Row.h"
#include "Write_Col.h"
#include "Write_Font.h"
#include "Write_Cell.h"
#include "Write_Range.h"
#include "Write_WorkBook.h"

using namespace Public;
using namespace Excel;


wstring Write_Sheet::c2w(const string& str)
{
	wstring wstr;
	{
		size_t maxlen = str.length() + 100;

		wchar_t* ptr = new wchar_t[maxlen];

		if (setlocale(LC_CTYPE, "chs") == NULL)
		{
			setlocale(LC_CTYPE, "");
		}
		size_t len = mbstowcs(ptr, str.c_str(), maxlen);

		wstr = wstring(ptr, len);
		SAFE_DELETEARRAY(ptr);
	
	}

	return wstr;
}

Write_Sheet::Write_Sheet(const shared_ptr<WorkBook>& _workbook, xlslib_core::worksheet* _writesheet, const std::string& _sheetname)
	:sheetname(_sheetname), writesheet(_writesheet), workbook(_workbook)
{

}
Write_Sheet::~Write_Sheet()
{

}

std::string Write_Sheet::name()
{
	 return sheetname; 
}
///�������ݣ�����
shared_ptr<WorkBook::Cell> Write_Sheet::setData(uint32_t rowNum, uint32_t colNum, const Value& val)
{
	//cell_t* m_cell;
	if (writesheet == NULL)
	{
		return shared_ptr<WorkBook::Cell>();
	}

	cell_t* m_cell = NULL;
	if (val.type() == Value::Type_Bool)
	{
		m_cell = writesheet->boolean(rowNum, colNum, val.readBool());
	}
	else if (val.type() == Value::Type_Int32)
	{
		m_cell = writesheet->number(rowNum, colNum, val.readUint32());
	}
	else if (val.type() == Value::Type_Double)
	{
		m_cell = writesheet->number(rowNum, colNum, val.readFloat());
	}
	else
	{
		m_cell = writesheet->label(rowNum, colNum, c2w(val.readString()));
	}
	return shared_ptr<WorkBook::Cell>(new Write_Cell(shared_from_this(), rowNum, colNum, m_cell));
}

shared_ptr<WorkBook::Cell> Write_Sheet::cell(uint32_t rowNum, uint32_t colNum)
{
	cell_t* m_cell = writesheet->label(rowNum, colNum, (workbook->addSheet(sheetname)->data(rowNum, colNum)).readString());
	if (writesheet == NULL)
	{
		return shared_ptr<WorkBook::Cell>();
	}

	return shared_ptr<WorkBook::Cell>(new Write_Cell(shared_from_this(), rowNum, colNum, m_cell));
}

bool Write_Sheet::font(const shared_ptr<WorkBook::Font>& font, const std::string& name)
{ 
	if (name == "")
	{
		return false;
	}
	//shared_ptr<WorkBook::Font> font(new Write_Font(name));
	font->create(name);

	return true;
}
//--------------�����ɫ
bool Write_Sheet::fill(const WorkBook::Color& color)
{ 
	WorkBook::Color _color = color;
	return true;
}
bool Write_Sheet::fill(const shared_ptr<WorkBook::Color>& color)
{ 
	fill(*color.get());
	return true; 
}

//--------------��ʾ��ʽ
bool Write_Sheet::format(const WorkBook::Format& fmt)
{ 
	WorkBook::Format _fmt = fmt;
	return true;
}
bool Write_Sheet::format(const shared_ptr<WorkBook::Format>& fmt)
{ 
	format(*fmt.get());
	return true;
}
//--------------�ϲ�
shared_ptr<WorkBook::Range> Write_Sheet::range(uint32_t startRowNum, uint32_t startColNum, uint32_t stopRowNum, uint32_t stopColNum)
{ 
	xlslib_core::range* _range;	//�������������
	_range = writesheet->rangegroup(startRowNum, startColNum, stopRowNum, stopColNum);
	if (_range == NULL)
	{
		return shared_ptr<WorkBook::Range>();
	}

	return shared_ptr<WorkBook::Range>(new Write_Range(shared_from_this(), startRowNum, startColNum, stopRowNum, stopColNum, _range));
}

//--------------Ĭ�Ͽ���
bool Write_Sheet::defaultRowHeight(uint16_t height)
{
	if (height < 0)
	{
		return false;
	}
	m_height = height;
	writesheet->defaultRowHeight(height);
	return true;
}

bool Write_Sheet::defaultColwidth(uint16_t width)
{
	if (width < 0)
	{
		return false;
	}
	m_width = width;
	writesheet->defaultColwidth(width);
	return true;
}

//--------------��������
uint32_t Write_Sheet::maxRowNum()
{ 
	if (writesheet == NULL)
	{
		return 0;
	}
	//ʵ��excel������Ϊlastrow+1
	return writesheet->maxRow;
}
//--------------��������
uint32_t Write_Sheet::maxColNum()
{ 
	if (writesheet == NULL)
	{
		return 0;
	}
	//ʵ��excel������Ϊlastrow+1
	return writesheet->maxCol;
}
//��ȡĳһ��
shared_ptr<WorkBook::Row> Write_Sheet::row(uint32_t rowNum) 
{
	if (writesheet == NULL)
	{
		return shared_ptr<WorkBook::Row>();
	}
	
	//todo ��ǰֻ������str��������Ҫ����ʵ�ʵ����ʹ���value
	return shared_ptr<WorkBook::Row>(new Write_Row(shared_from_this(), writesheet, rowNum));
}
//��ȡĳһ��
shared_ptr<WorkBook::Col> Write_Sheet::col(uint32_t colNum) 
{
	if (writesheet == NULL)
	{
		return shared_ptr<WorkBook::Col>();
	}
	
	//todo ��ǰֻ������str��������Ҫ����ʵ�ʵ����ʹ���value
	return shared_ptr<WorkBook::Col>(new Write_Col(shared_from_this(), writesheet, colNum));
}
uint32_t Write_Sheet::getRowMaxColNum(uint32_t rownum)
{
	if (writesheet == NULL)
	{
		return 0;
	}
	if (rownum >  writesheet->maxCol)
	{
		return 0;
	}
	return (uint32_t)writesheet->NumCells();
}

uint32_t Write_Sheet::getColMaxRowNum(uint32_t colnum)
{
	if (writesheet == NULL)
	{
		return 0;
	}
	return  writesheet->minCol;
}


