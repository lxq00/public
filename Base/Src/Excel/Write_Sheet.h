#ifndef __WRITE_SHEET_H__
#define __WRITE_SHEET_H__
#include "Base/Excel.h"
#include "xlslib/xlslib.h"

namespace Public
{
namespace Base
{

class Write_Sheet : public WorkBook::Sheet, public enable_shared_from_this<WorkBook::Sheet>
{
public:
	static wstring c2w(const string &str);

public:
	Write_Sheet(const shared_ptr<WorkBook> &_workbook, xlslib_core::worksheet *_writesheet, const std::string &_sheetname);
	~Write_Sheet();

	//��ȡĳһ��
	shared_ptr<WorkBook::Row> row(uint32_t rowNum);
	//��ȡĳһ��
	shared_ptr<WorkBook::Col> col(uint32_t colNum);
	std::string name();

	///�������ݣ�����
	shared_ptr<WorkBook::Cell> setData(uint32_t rowNum, uint32_t colNum, const Value &val) /*{ return shared_ptr<WorkBook::Cell>(); }*/;

	//--------------��������
	uint32_t maxRowNum();
	//--------------��������
	uint32_t maxColNum();

	//��ȡĳһ����ĳһ�еı��
	shared_ptr<WorkBook::Cell> cell(uint32_t rowNum, uint32_t colNum);
	uint32_t getRowMaxColNum(uint32_t rownum);
	uint32_t getColMaxRowNum(uint32_t colnum);

	bool font(const shared_ptr<WorkBook::Font> &font);

	//--------------�����ɫ
	bool fill(const WorkBook::Color &color);
	//bool fill(uint8_t _r, uint8_t _g, uint8_t _b);
	bool fill(const shared_ptr<WorkBook::Color> &color);

	//--------------��ʾ��ʽ
	bool format(const WorkBook::Format &fmt);
	bool format(const shared_ptr<WorkBook::Format> &fmt);

	//--------------�ϲ�
	shared_ptr<WorkBook::Range> range(uint32_t startRowNum, uint32_t startColNum, uint32_t stopRowNum, uint32_t stopColNum);

	//--------------Ĭ�Ͽ���
	bool defaultRowHeight(uint16_t height); // sets column widths to 1/256 x width of "0"
	bool defaultColwidth(uint16_t width);   // in points (Excel uses twips, 1/20th of a point, but xlslib didn't)

public:
	uint8_t s_r;
	uint8_t s_g;
	uint8_t s_b;
	uint16_t m_height;
	uint16_t m_width;

	std::string sheetname;
	xlslib_core::worksheet *writesheet;
	shared_ptr<WorkBook> workbook;
};

} // namespace Base
} // namespace Public

#endif //__WRITE_SHEET_H__
