#ifndef __WRITE_COL_H__
#define __WRITE_COL_H__
#include "Excel/Excel.h"
#include "xlslib.h"


namespace Public {
namespace Excel {
class Write_Col :public WorkBook::Col
{
public:
	Write_Col(const shared_ptr<WorkBook::Sheet>& _worksheet, xlslib_core::worksheet* writesheet, uint32_t m_colNum);
	~Write_Col();
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
	shared_ptr<WorkBook::Cell> setData(uint32_t rowNum, const Value& val);
	//--------------���򣬵��е�ָ���кϲ�, ��ʼ���кţ��������к�
	shared_ptr<WorkBook::Range> range(uint32_t startRowNum, uint32_t stopRowNum);

	//--------------��ʾ������,���е�������ʾ
	/*bool show(bool showflag);*/
	//--------------��ǰ�е��к�
	uint32_t colNum();
	//--------------��ǰ�е���������
	uint32_t maxRowNum();

	//�п�
	//uint32_t width();
	//bool setWidth(uint32_t height);
private:
	shared_ptr<WorkBook::Sheet> worksheet;
	xlslib_core::worksheet* writesheet;
	uint32_t m_colNum;
	uint32_t m_width;
};

}
}

#endif //__WRITE_COL_H__