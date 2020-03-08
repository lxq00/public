#include "Write_Font.h"

namespace Public
{
namespace Base
{

shared_ptr<WorkBook::Font> WorkBook::Font::create(const std::string &name)
{
	if (name == "" || !FontInternal::IsASCII(name))
	{
		return shared_ptr<WorkBook::Font>();
	}
	shared_ptr<WorkBook::Font> Ffont(new WorkBook::Font);
	Ffont->internal->font_name = name;
	return Ffont;
}
WorkBook::Font::Font()
{
	internal = new FontInternal;
}
WorkBook::Font::Font(const Font &font)
{
	internal = new FontInternal;
	*internal = *font.internal;
}
WorkBook::Font &WorkBook::Font::operator=(const Font &val)
{
	*internal = *val.internal;
	return *this;
}
WorkBook::Font::~Font()
{
	SAFE_DELETE(internal);
}

//���������С
bool WorkBook::Font::setSize(uint32_t size)
{
	if (size < 0)
	{
		return false;
	}
	//��Ҫ������size����20������������ʾ
	internal->font_size = size * 20;
	return true;
}

//����Ӵ�
bool WorkBook::Font::setBold()
{
	internal->font_bold = true;
	return true;
}

//�����»���
bool WorkBook::Font::setUnderline()
{
	internal->font_underline = true;
	return true;
}

bool WorkBook::Font::setColor(const Color &color)
{
	internal->font_color = make_shared<Color>(color);
	return true;
}
bool WorkBook::Font::setColor(const shared_ptr<Color> &color)
{
	if (color == NULL)
	{
		return false;
	}

	return setColor(*color.get());
}

std::string WorkBook::Font::name() const
{
	return internal->font_name;
}
} // namespace Base
} // namespace Public