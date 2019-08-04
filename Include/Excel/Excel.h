#ifndef __Public_EXCEL_H__
#define __Public_EXCEL_H__
#include "Excel_Def.h"

namespace Public {
namespace Excel {

//excel �Ĺ�����
class EXCEL_API WorkBook
{
public:
	//excel�е���ɫ
	class EXCEL_API Color
	{
	public:
		Color();
		Color(const Color& color);
		Color(uint8_t _r, uint8_t _g, uint8_t _b);
		virtual ~Color();

		Color& operator = (const Color& val);
	public:
		struct ColorInternal;
		ColorInternal* internal;
	};

	//excel�е�����
	class EXCEL_API Font
	{
	public:
		Font();
		Font(const Font& font);
		virtual ~Font();

		Font& operator = (const Font& val);

		static shared_ptr<Font> create(const std::string& name);

		//���������С
		bool setSize(uint32_t size);
		//����Ӵ�
		bool setBold();
		//�����»���
		bool setUnderline();
		//������ɫ
		bool setColor(const Color& color);
		bool setColor(const shared_ptr<Color>& color);
	public:
		struct FontInternal;
		FontInternal* internal;
	};

	//excel�е���ʾ��ʽ
	class EXCEL_API Format
	{
	public:
		Format();
		Format(const Format& format);
		Format& operator = (const Format& val);
		virtual ~Format();

		//��ʾ��ʾ��ʽ
		bool setFormat(FomatType type);

		//���ö��뷽ʽ
		bool setAlign(ALIGN_X_Type xalign, ALIGN_Y_Type yalign);

		//������ʾ��ʽ
		bool setTxtori(TxtoriType type);
	public:
		struct FormatInternal;
		FormatInternal* internal;
	};



	//excel�еĵ�Ԫ����
	class EXCEL_API Side
	{
	public:
		//���ñ߿���ʾ����,�μ�SIDE_BOTTOM
		Side(int val, const shared_ptr<Color>& sideColor);
		Side(int val, const Color& sideColor);
		Side(const Side& side);
		Side& operator = (const Side& val);
		~Side();
		//���ñ߿���ʾ����,�μ�SIDE_BOTTOM
		//		bool setType(xlslib_core::border_side_t side, xlslib_core::border_style_t style);
		/*virtual bool borderstyle(xlslib_core::border_side_t side, xlslib_core::border_style_t style);
		virtual bool bordercolor(xlslib_core::border_side_t side, unsigned8_t color);
		*/
	public:
		struct LineInternal;
		LineInternal* internal;
	};

	//excel�е�ĳһ����Ԫ��
	class EXCEL_API Cell
	{
	public:
		Cell() {}
		virtual ~Cell() {}

		//--------------�������
		///��ȡ����,����
		virtual Value data() const { return Value(); };

		//--------------�������
		///�к�
		virtual unsigned int rowNum() = 0;
		///�к�
		virtual unsigned int colNum() = 0;

		//--------------�������		
		virtual bool font(const shared_ptr<Font>& font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color& color) { return false; }
		virtual bool fill(const shared_ptr<Color>& color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format& fmt) { return false; }
		virtual bool format(const shared_ptr<Format>& fmt) { return false; }

		//--------------��Ԫ��߿�
		virtual bool side(const Side& sd) { return false; }
		virtual bool side(const shared_ptr<Side>& sd) { return false; }
	};

	///excel�ж��ж���ѡ������
	class EXCEL_API Range
	{
	public:
		Range() {}
		virtual ~Range() {}

		//--------------�������		
		virtual bool font(const shared_ptr<Font>& font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color& color) { return false; }
		virtual bool fill(const shared_ptr<Color>& color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format& fmt) { return false; }
		virtual bool format(const shared_ptr<Format>& fmt) { return false; }

		//--------------��Ԫ��߿�
		virtual bool side(const Side& sd) { return false; }
		virtual bool side(const shared_ptr<Side>& sd) { return false; }

		////--------------��ʾ������,�������������ʾ
		//virtual void hidden(bool hidden_opt) {};

		//--------------�ϲ�,ָ����Ԫ��
		virtual bool merge() { return false; }
	public:
		uint32_t startRowNum;
		uint32_t startColNum;
		uint32_t stopRowNum;
		uint32_t stopColNum;
	};
	//excel�е���,���ڰ��д���
	class EXCEL_API Row
	{
	public:
		Row() {}
		virtual ~Row() {}

		//--------------�������
		///��ȡ����,����
		virtual Value data(uint32_t colNum) const { return Value(); };
		///�������ݣ�����
		virtual shared_ptr<Cell> setData(uint32_t colNum, const Value& val) { return shared_ptr<Cell>(); };

		//--------------�������		
		virtual bool font(const shared_ptr<Font>& font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color& color) { return false; }
		virtual bool fill(const shared_ptr<Color>& color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format& fmt) { return false; }
		virtual bool format(const shared_ptr<Format>& fmt) { return false; }

		//--------------���򣬵��е�ָ���кϲ�, ��ʼ���кţ��������к�
		virtual shared_ptr<Range> range(uint32_t startColNum, uint32_t stopColNum) { return shared_ptr<Range>(); }

		//--------------��ʾ������,���е�������ʾ
		//	virtual bool show(bool showflag) { return false; }

		//--------------��ǰ����������
		virtual uint32_t maxColNum() = 0;

		//--------------��ǰ�е��к�
		virtual uint32_t rowNum() = 0;

		//�и�
		virtual uint32_t height() { return 0; }
		virtual bool setHeight(uint32_t height) { return false; }
	};

	//excel�е��У����ڰ��д���
	class EXCEL_API Col
	{
	public:
		Col() {}
		virtual ~Col() {}

		//--------------�������
		///��ȡ����,����
		virtual Value data(uint32_t rowNum) const { return Value(); };
		///�������ݣ�����
		virtual shared_ptr<Cell> setData(uint32_t rowNum, const Value& val) { return shared_ptr<Cell>(); };

		//--------------�������		
		virtual bool font(const shared_ptr<Font>& font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color& color) { return false; }
		virtual bool fill(const shared_ptr<Color>& color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format& fmt) { return false; }
		virtual bool format(const shared_ptr<Format>& fmt) { return false; }

		//--------------���򣬵��е�ָ���кϲ�, ��ʼ���кţ��������к�
		virtual shared_ptr<Range> range(uint32_t startRowNum, uint32_t stopRowNum) { return shared_ptr<Range>(); }

		//--------------��ʾ������,���е�������ʾ
		//	virtual bool show(bool showflag) { return false; }

		//--------------��ǰ�е��к�
		virtual uint32_t colNum() = 0;

		//--------------��ǰ�е���������
		virtual uint32_t maxRowNum() = 0;

		//�п�
		//virtual uint32_t width() { return 0; }
		//virtual bool setWidth(uint32_t height) { return false; }
	};

	//excel�е�sheet
	class EXCEL_API Sheet
	{
	public:
		Sheet() {}
		virtual ~Sheet() {}

		//sheet����
		virtual std::string name() { return ""; }

		//--------------��������
		virtual uint32_t maxRowNum() = 0;
		//--------------��������
		virtual uint32_t maxColNum() = 0;

		//��ȡĳһ��
		virtual shared_ptr<Row> row(uint32_t rowNum) = 0;
		//��ȡĳһ��
		virtual shared_ptr<Col> col(uint32_t colNum) = 0;

		virtual shared_ptr<Cell> cell(uint32_t rowNum, uint32_t colNum) = 0;

		//--------------�������
		///��ȡ����,����
		virtual Value data(uint32_t rowNum, uint32_t colNum) const { return Value(); };
		///�������ݣ�����
		virtual shared_ptr<Cell> setData(uint32_t rowNum, uint32_t colNum, const Value& val) { return shared_ptr<Cell>(); };

		//--------------�������		
		virtual bool font(const shared_ptr<Font>& font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color& color) { return false; }
		virtual bool fill(const shared_ptr<Color>& color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format& fmt) { return false; }
		virtual bool format(const shared_ptr<Format>& fmt) { return false; }

		//--------------�ϲ�
		virtual shared_ptr<Range> range(uint32_t startRowNum, uint32_t startColNum, uint32_t stopRowNum, uint32_t stopColNum) { return shared_ptr<Range>(); }

		//--------------Ĭ�Ͽ���
		virtual bool defaultRowHeight(uint16_t height) { return false; } // sets column widths to 1/256 x width of "0"
		virtual bool defaultColwidth(uint16_t width) { return false; }  // in points (Excel uses twips, 1/20th of a point, but xlslib didn't)

	};

public:
	WorkBook() {}
	virtual ~WorkBook() {}

	//����һ��sheet
	virtual shared_ptr<Sheet> addSheet(const std::string& name) {return shared_ptr<Sheet>();}

	//��ȡһ��sheet,num��0��ʼ
	virtual shared_ptr<Sheet> getSheet(uint32_t num) { return shared_ptr<Sheet>(); }

	virtual uint32_t sheetCount() { return 0; }


	///��ȡһ��xls�ļ�
	static shared_ptr<WorkBook> read(const std::string& xlsfile);

	//����һ��xls�ļ�
	static shared_ptr<WorkBook> create(const std::string& xlsfile);
};

	

}
}


#endif //__Public_EXCEL_H__
