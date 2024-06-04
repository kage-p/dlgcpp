#include "label_p.h"
#include "../utility.h"

using namespace dlgcpp::controls;

Label::Label(std::shared_ptr<IDialog> parent,
             const std::string& text,
             const Position& p) :
    Control(parent),
    _props(new lbl_props())
{
    this->text(text);
    this->p(p);
}

Label::~Label()
{
    delete _props;
}

unsigned int Label::styles() const
{
    auto styles = Control::styles();
    styles = styles & ~WS_TABSTOP;
    //SS_LEFT | SS_CENTERIMAGE
    return styles | SS_NOPREFIX | SS_NOTIFY;
}

void Label::p(const Position& p)
{
    Control::p(p);
    updateAutoSize();
}

void Label::text(const std::string& value)
{
    Control::text(value);
    updateAutoSize();
}

void Label::font(const Font& value)
{
    Control::font(value);
    updateAutoSize();
}

bool Label::autoSize() const
{
    return _props->autoSize;
}

void Label::autoSize(bool value)
{
    _props->autoSize = value;
    updateAutoSize();
}

void Label::updateAutoSize()
{
    if (!_props->autoSize)
        return;

    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    auto f = Control::font();
    if (!f.faceName.empty())
        hFont = (HFONT)makeFont(f);

    auto hdc = CreateCompatibleDC(NULL);
    auto szl = SIZE();
    SelectObject(hdc, hFont);
    std::string text = Control::text();
    GetTextExtentPoint32W(hdc, toWide(text).c_str(), (int)text.size(), &szl);
    DeleteDC(hdc);
    DeleteObject(hFont);

    Position pos = Control::p();
    Position newPos = toUnits(reinterpret_cast<HWND>(parent()->handle()), Position{0,0,szl.cx,szl.cy});
    pos._cx = newPos._cx;
    pos._cy = newPos._cy;
    Control::p(pos);
}
