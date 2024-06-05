#include "label_p.h"
#include "../utility.h"
#include "../dlgmsg.h"

using namespace dlgcpp;
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

    switch (_props->horzAlign)
    {
    case HorizontalAlign::Left:
        styles |= SS_LEFT;
        break;
    case HorizontalAlign::Center:
        styles |= SS_CENTER;
        break;
    case HorizontalAlign::Right:
        styles |= SS_RIGHT;
        break;
    }

    switch (_props->vertAlign)
    {
    case VerticalAlign::Top:
        // default
        break;
    case VerticalAlign::Center:
        styles |= SS_CENTERIMAGE;
        break;
    case VerticalAlign::Bottom:
        // not supported
        break;
    }

    if (_props->elipsis)
        styles |= SS_ENDELLIPSIS;

    return styles | SS_NOPREFIX | SS_NOTIFY;
}

void Label::p(const Position& p)
{
    Control::p(p);
    updateAutoSize();
}

void Label::notify(struct dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == STN_CLICKED)
        {
            ClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == STN_DBLCLK)
        {
            DoubleClickEvent().invoke();
        }
    }
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
    if (_props->autoSize == value)
        return;
    _props->autoSize = value;
    updateAutoSize();
}

void Label::updateAutoSize()
{
    if (handle() == nullptr)
        return;
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

    auto hwndParent = reinterpret_cast<HWND>(parent()->handle());
    Position pos = Control::p();
    Position newPos = toUnits(hwndParent, Position{0,0,szl.cx,szl.cy});
    pos._cx = newPos._cx;
    pos._cy = newPos._cy;
    Control::p(pos);
}

bool Label::elipsis() const
{
    return _props->elipsis;
}

void Label::elipsis(bool value)
{
    if (_props->elipsis == value)
        return;
    _props->elipsis = value;
    rebuild();
}

HorizontalAlign Label::horizontalAlignment() const
{
    return _props->horzAlign;
}

void Label::horizontalAlignment(HorizontalAlign value)
{
    if (_props->horzAlign == value)
        return;
    _props->horzAlign = value;
    rebuild();
}

VerticalAlign Label::verticalAlignment() const
{
    return _props->vertAlign;
}

void Label::verticalAlignment(VerticalAlign value)
{
    if (_props->vertAlign == value)
        return;

    // bottom alignment is not supported
    if (value == VerticalAlign::Bottom)
        return;

    _props->vertAlign = value;
    rebuild();
}
