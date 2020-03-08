#ifndef __Public_EXCEL_H__
#define __Public_EXCEL_H__
#include "Base/Defs.h"
#include "Base/IntTypes.h"
#include "Base/Shared_ptr.h"
#include "Base/Value.h"
#include "Base/BaseTemplate.h"

namespace Public
{
namespace Base
{

//������ʾ��ʽ
typedef enum
{
	FomatType_GENERAL = 0,
	FomatType_NUMBER1,	 // 0
	FomatType_NUMBER2,	 // 0.00
	FomatType_NUMBER3,	 // #,##0
	FomatType_NUMBER4,	 // #,##0.00
	FomatType_CURRENCY1,   // "$"#,##0_);("$"#,##0)
	FomatType_CURRENCY2,   // "$"#,##0_);[Red]("$"#,##0)
	FomatType_CURRENCY3,   // "$"#,##0.00_);("$"#,##0.00)
	FomatType_CURRENCY4,   // "$"#,##0.00_);[Red]("$"#,##0.00)
	FomatType_PERCENT1,	// 0%
	FomatType_PERCENT2,	// 0.00%
	FomatType_SCIENTIFIC1, // 0.00E+00
	FomatType_FRACTION1,   // # ?/?
	FomatType_FRACTION2,   // # ??/??
	FomatType_DATE1,	   // M/D/YY
	FomatType_DATE2,	   // D-MMM-YY
	FomatType_DATE3,	   // D-MMM
	FomatType_DATE4,	   // MMM-YY
	FomatType_TIME1,	   // h:mm AM/PM
	FomatType_TIME2,	   // h:mm:ss AM/PM
	FomatType_TIME3,	   // h:mm
	FomatType_TIME4,	   // h:mm:ss
	FomatType_DATETIME,	// M/D/YY h:mm
	FomatType_ACCOUNTING1, // _(#,##0_);(#,##0)
	FomatType_ACCOUNTING2, // _(#,##0_);[Red](#,##0)
	FomatType_ACCOUNTING3, // _(#,##0.00_);(#,##0.00)
	FomatType_ACCOUNTING4, // _(#,##0.00_);[Red](#,##0.00)
	FomatType_CURRENCY5,   // _("$"* #,##0_);_("$"* (#,##0);_("$"* "-"_);_(@_)
	FomatType_CURRENCY6,   // _(* #,##0_);_(* (#,##0);_(* "-"_);_(@_)
	FomatType_CURRENCY7,   // _("$"* #,##0.00_);_("$"* (#,##0.00);_("$"* "-"??_);_(@_)
	FomatType_CURRENCY8,   // _(* #,##0.00_);_(* (#,##0.00);_(* "-"??_);_(@_)
	FomatType_TIME5,	   // mm:ss
	FomatType_TIME6,	   // [h]:mm:ss
	FomatType_TIME7,	   // mm:ss.0
	FomatType_SCIENTIFIC2, // ##0.0E+0
	FomatType_TEXT		   // @
} FomatType;

//������뷽ʽ,X��Y��������|
typedef enum
{
	ALIGN_X_NONE = 0, //��ʼ��
	ALIGN_X_LEFT,	 //�����
	ALIGN_X_CENTER,   //���ж���
	ALIGN_X_RIGHT,	//�Ҷ���
} ALIGN_X_Type;

typedef enum
{
	ALIGN_Y_TOP,	//���϶���
	ALIGN_Y_CENTER, //���ж���
	ALIGN_Y_BOTTOM, //���¶���
} ALIGN_Y_Type;

//������ʾ��ʽ
typedef enum
{
	TxtoriType_NONE = 0,	 //����ģʽ
	TxtoriType_TOPBOTTOMTXT, //���У����ϵ�����ʾ
	TxtoriType_90NOCLOCKTXT, //���У�������ʾ
	TxtoriType_90CLOCKTXT	//���У�������ʾ
} TxtoriType;

//�߿���ʾ��ʽ������|�ķ�ʽ
enum
{
	SIDE_BOTTOM = 1, //�ײ��߿�
	SIDE_TOP = 2,	//�ϲ��߿�
	SIDE_LEFT = 4,   //�󲿱߿�
	SIDE_RIGHT = 8,  //�Ҳ��߿�
};

//excel �Ĺ�����
class BASE_API WorkBook
{
public:
	//excel�е���ɫ
	class BASE_API Color
	{
	public:
		Color();
		Color(const Color &color);
		Color(uint8_t _r, uint8_t _g, uint8_t _b);
		virtual ~Color();

		Color &operator=(const Color &val);

	public:
		struct ColorInternal;
		ColorInternal *internal;
	};

	//excel�е�����
	class BASE_API Font
	{
	public:
		Font();
		Font(const Font &font);
		virtual ~Font();

		Font &operator=(const Font &val);

		static shared_ptr<Font> create(const std::string &name);

		//���������С
		bool setSize(uint32_t size);
		//����Ӵ�
		bool setBold();
		//�����»���
		bool setUnderline();
		//������ɫ
		bool setColor(const Color &color);
		bool setColor(const shared_ptr<Color> &color);

		std::string name() const;

	public:
		struct FontInternal;
		FontInternal *internal;
	};

	//excel�е���ʾ��ʽ
	class BASE_API Format
	{
	public:
		Format();
		Format(const Format &format);
		Format &operator=(const Format &val);
		virtual ~Format();

		//��ʾ��ʾ��ʽ
		bool setFormat(FomatType type);

		//���ö��뷽ʽ
		bool setAlign(ALIGN_X_Type xalign, ALIGN_Y_Type yalign);

		//������ʾ��ʽ
		bool setTxtori(TxtoriType type);

	public:
		struct FormatInternal;
		FormatInternal *internal;
	};

	//excel�еĵ�Ԫ����
	class BASE_API Side
	{
	public:
		//���ñ߿���ʾ����,�μ�SIDE_BOTTOM
		Side(int val, const shared_ptr<Color> &sideColor);
		Side(int val, const Color &sideColor);
		Side(const Side &side);
		Side &operator=(const Side &val);
		~Side();
		//���ñ߿���ʾ����,�μ�SIDE_BOTTOM
		//		bool setType(xlslib_core::border_side_t side, xlslib_core::border_style_t style);
		/*virtual bool borderstyle(xlslib_core::border_side_t side, xlslib_core::border_style_t style);
		virtual bool bordercolor(xlslib_core::border_side_t side, unsigned8_t color);
		*/
	public:
		struct LineInternal;
		LineInternal *internal;
	};

	//excel�е�ĳһ����Ԫ��
	class BASE_API Cell
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
		virtual bool font(const shared_ptr<Font> &font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color &color) { return false; }
		virtual bool fill(const shared_ptr<Color> &color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format &fmt) { return false; }
		virtual bool format(const shared_ptr<Format> &fmt) { return false; }

		//--------------��Ԫ��߿�
		virtual bool side(const Side &sd) { return false; }
		virtual bool side(const shared_ptr<Side> &sd) { return false; }
	};

	///excel�ж��ж���ѡ������
	class BASE_API Range
	{
	public:
		Range() {}
		virtual ~Range() {}

		//--------------�������
		virtual bool font(const shared_ptr<Font> &font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color &color) { return false; }
		virtual bool fill(const shared_ptr<Color> &color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format &fmt) { return false; }
		virtual bool format(const shared_ptr<Format> &fmt) { return false; }

		//--------------��Ԫ��߿�
		virtual bool side(const Side &sd) { return false; }
		virtual bool side(const shared_ptr<Side> &sd) { return false; }

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
	class BASE_API Row
	{
	public:
		Row() {}
		virtual ~Row() {}

		//--------------�������
		///��ȡ����,����
		virtual Value data(uint32_t colNum) const { return Value(); };
		///�������ݣ�����
		virtual shared_ptr<Cell> setData(uint32_t colNum, const Value &val) { return shared_ptr<Cell>(); };

		//--------------�������
		virtual bool font(const shared_ptr<Font> &font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color &color) { return false; }
		virtual bool fill(const shared_ptr<Color> &color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format &fmt) { return false; }
		virtual bool format(const shared_ptr<Format> &fmt) { return false; }

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
	class BASE_API Col
	{
	public:
		Col() {}
		virtual ~Col() {}

		//--------------�������
		///��ȡ����,����
		virtual Value data(uint32_t rowNum) const { return Value(); };
		///�������ݣ�����
		virtual shared_ptr<Cell> setData(uint32_t rowNum, const Value &val) { return shared_ptr<Cell>(); };

		//--------------�������
		virtual bool font(const shared_ptr<Font> &font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color &color) { return false; }
		virtual bool fill(const shared_ptr<Color> &color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format &fmt) { return false; }
		virtual bool format(const shared_ptr<Format> &fmt) { return false; }

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
	class BASE_API Sheet
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
		virtual shared_ptr<Cell> setData(uint32_t rowNum, uint32_t colNum, const Value &val) { return shared_ptr<Cell>(); };

		//--------------�������
		virtual bool font(const shared_ptr<Font> &font) { return false; }

		//--------------�����ɫ
		virtual bool fill(const Color &color) { return false; }
		virtual bool fill(const shared_ptr<Color> &color) { return false; }

		//--------------��ʾ��ʽ
		virtual bool format(const Format &fmt) { return false; }
		virtual bool format(const shared_ptr<Format> &fmt) { return false; }

		//--------------�ϲ�
		virtual shared_ptr<Range> range(uint32_t startRowNum, uint32_t startColNum, uint32_t stopRowNum, uint32_t stopColNum) { return shared_ptr<Range>(); }

		//--------------Ĭ�Ͽ���
		virtual bool defaultRowHeight(uint16_t height) { return false; } // sets column widths to 1/256 x width of "0"
		virtual bool defaultColwidth(uint16_t width) { return false; }   // in points (Excel uses twips, 1/20th of a point, but xlslib didn't)
	};

public:
	WorkBook() {}
	virtual ~WorkBook() {}

	//����һ��sheet
	virtual shared_ptr<Sheet> addSheet(const std::string &name) { return shared_ptr<Sheet>(); }

	//��ȡһ��sheet,num��0��ʼ
	virtual shared_ptr<Sheet> getSheet(uint32_t num) { return shared_ptr<Sheet>(); }

	virtual uint32_t sheetCount() { return 0; }
};

class BASE_API Excel
{
public:
	///��ȡһ��xls�ļ�
	static shared_ptr<WorkBook> read(const std::string &xlsfile);

	//����һ��xls�ļ�
	static shared_ptr<WorkBook> create(const std::string &xlsfile);
};

} // namespace Base
} // namespace Public

#endif //__Public_EXCEL_H__
