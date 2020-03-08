#ifndef __WRITE_ROW_H__
#define __WRITE_ROW_H__
#include "Base/Excel.h"
#include "xlslib/xlslib.h"

namespace Public {
namespace Base {

class Write_Row :public WorkBook::Row
{
public:
	Write_Row(const shared_ptr<WorkBook::Sheet>& _worksheet, xlslib_core::worksheet* writesheet, uint32_t m_rowNum);
	~Write_Row();

	//--------------�������		
	bool font(const shared_ptr<WorkBook::Font>& font);

	//--------------�����ɫ
	bool fill(const WorkBook::Color& color);
	bool fill(const shared_ptr<WorkBook::Color>& color);
	//--------------��ʾ��ʽ
	bool format(const WorkBook::Format& _fmt);
	bool format(const shared_ptr<WorkBook::Format>& _fmt);
	//--------------��Ԫ��߿�
	bool side(const WorkBook::Side& _sd);
	bool side(const shared_ptr<WorkBook::Side>& _sd);
	///�������ݣ�����
	shared_ptr<WorkBook::Cell> setData(uint32_t colNum, const Value& val);
	//--------------���򣬵��е�ָ���кϲ�, ��ʼ���кţ��������к�
	shared_ptr<WorkBook::Range> range(uint32_t startColNum, uint32_t stopColNum);

	//--------------��ʾ������,���е�������ʾ
	bool show(bool showflag);
	//--------------��ǰ����������
	uint32_t maxColNum();
	//--------------��ǰ�е��к�
	uint32_t rowNum();

	uint32_t height();
	bool setHeight(uint32_t height);
private:
	shared_ptr<WorkBook::Sheet> worksheet;
	xlslib_core::worksheet* writesheet;
	uint32_t m_rowNum;
	uint32_t m_height;
};

}
}

#endif //__WRITE_ROW_H__