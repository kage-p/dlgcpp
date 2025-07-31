#include "dlgcpp/dialogs/dialog.h"
#include "label_p.h"
#include "utility/convert.h"
#include "utility/font_loader.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

LabelImpl::LabelImpl(
    const std::string& text,
    const Position& p) :
    ControlImpl(text, p)
{
}

LabelImpl::~LabelImpl()
{
}

void LabelImpl::rebuild()
{
    ControlImpl::rebuild();
    updateAutoSize();
}

unsigned int LabelImpl::styles() const
{
    auto styles = ControlImpl::styles();
    styles = styles & ~WS_TABSTOP;
    styles |= SS_NOPREFIX;

    switch (_horzAlign)
    {
    case HorizontalAlign::Left:
        styles |= SS_LEFT;
        //styles |= SS_LEFTNOWORDWRAP;
        break;
    case HorizontalAlign::Center:
        styles |= SS_CENTER;
        break;
    case HorizontalAlign::Right:
        styles |= SS_RIGHT;
        break;
    }

    switch (_vertAlign)
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

    if (_clickable)
        styles |= SS_NOTIFY;

    if (_elipsis)
        styles |= SS_ENDELLIPSIS;

    return styles;
}

void LabelImpl::p(const Position& p)
{
    ControlImpl::p(p);
    updateAutoSize();
}

void LabelImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == STN_CLICKED)
        {
            ClickEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == STN_DBLCLK)
        {
            DoubleClickEvent().invoke(control());
        }
    }
    ControlImpl::notify(msg);
}

void LabelImpl::text(const std::string& value)
{
    ControlImpl::text(value);
    updateAutoSize();
}

void LabelImpl::font(const Font& value)
{
    ControlImpl::font(value);
    updateAutoSize();
}

bool LabelImpl::autoSize() const
{
    return _autoSize;
}

void LabelImpl::autoSize(bool value)
{
    if (_autoSize == value)
        return;
    _autoSize = value;
    updateAutoSize();
}

bool LabelImpl::clickable() const
{
    return _clickable;
}

void LabelImpl::clickable(bool value)
{
    if (_clickable == value)
        return;
    _clickable = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // update style
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

void LabelImpl::updateAutoSize()
{
    if (parent() == nullptr)
        return;
    if (!_autoSize)
        return;

    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    auto f = ControlImpl::font();
    if (!f.family.empty())
        hFont = FontLoader::toGdiFont(f);

    auto hdc = CreateCompatibleDC(NULL);
    auto szl = SIZE();
    SelectObject(hdc, hFont);
    std::string text = ControlImpl::text();
    GetTextExtentPoint32W(hdc, StringEncoder::toWide(text).c_str(), (int)text.size(), &szl);
    DeleteDC(hdc);
    DeleteObject(hFont);

    auto hwndParent = reinterpret_cast<HWND>(parent()->handle());
    Size size = Convert(hwndParent).toUnits(Size(szl.cx + 1, szl.cy + 1));
    ControlImpl::resize(size);
}

bool LabelImpl::elipsis() const
{
    return _elipsis;
}

void LabelImpl::elipsis(bool value)
{
    if (_elipsis == value)
        return;
    _elipsis = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

HorizontalAlign LabelImpl::horizontalAlignment() const
{
    return _horzAlign;
}

void LabelImpl::horizontalAlignment(HorizontalAlign value)
{
    if (_horzAlign == value)
        return;
    _horzAlign = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

VerticalAlign LabelImpl::verticalAlignment() const
{
    return _vertAlign;
}

void LabelImpl::verticalAlignment(VerticalAlign value)
{
    if (_vertAlign == value)
        return;

    // bottom alignment is not supported
    if (value == VerticalAlign::Bottom)
        return;

    _vertAlign = value;

    if (handle() == nullptr)
        return;
    rebuild();
}
