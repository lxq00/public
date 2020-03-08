#include "Write_WorkBook.h"
#include "Write_Sheet.h"
#include "Write_Color.h"


//using namespace xlslib_core;
namespace Public {
namespace Base {

Write_WorkBook::Write_WorkBook(const shared_ptr<xlslib_core::workbook>& _pWorkbook, const std::string& _xlsfile):
	xlsfile(_xlsfile), pWorkbook(_pWorkbook),colorindex(0)
{
	
}
Write_WorkBook::~Write_WorkBook()
{
	if (pWorkbook != NULL && xlsfile != "")
	{
		pWorkbook->Dump(xlsfile);
	}
}

shared_ptr<WorkBook> Write_WorkBook::create(const std::string& xlsfile)
{
	shared_ptr<xlslib_core::workbook> pWorkbook = make_shared<xlslib_core::workbook>();
	if (pWorkbook == NULL)
	{
		return shared_ptr<WorkBook>();
	}

	shared_ptr<WorkBook> obj(new Write_WorkBook(pWorkbook,xlsfile));
	if (obj == NULL)
	{
		return shared_ptr<WorkBook>();
	}
	return obj;
}

shared_ptr<WorkBook::Sheet> Write_WorkBook::addSheet(const std::string& name)
{
	if (pWorkbook == NULL)
	{
		return shared_ptr<WorkBook::Sheet>();
	}

	xlslib_core::worksheet* writesheet = pWorkbook->sheet(Write_Sheet::c2w(name));
	if (writesheet == NULL)
	{
		return shared_ptr<WorkBook::Sheet>();
	}
	return shared_ptr<WorkBook::Sheet> (new Write_Sheet(shared_from_this(), writesheet, name));
}

uint8_t Write_WorkBook::getColorIndex(const WorkBook::Color& color)
{
	char buffer[128];
	snprintf(buffer, 127, "%d-%d-%d", color.internal->c_r, color.internal->c_g, color.internal->c_b);

	std::map<std::string, uint8_t>::iterator iter = colormap.find(buffer);
	if (iter != colormap.end())
	{
		return iter->second;
	}

	//xlslib index ��ΧΪ 0 - 56 +8
	uint8_t index = ((colorindex++) % 56) + 8;

	pWorkbook->setColor(color.internal->c_r, color.internal->c_g, color.internal->c_b, index);
	
	colormap[buffer] = index;
	
	return index;
}

}
}