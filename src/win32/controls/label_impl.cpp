#include "dlgcpp/dialogs/dialog.h"
#include "label_impl.h"
#include "utility/convert.h"
#include "utility/font_loader.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

LabelImpl::LabelImpl() :
    ControlImpl()
{
}

LabelImpl::~LabelImpl()
{
}

void LabelImpl::owner(ILabel* label)
{
    ControlImpl::owner(label);

    _label = label;
    _label->autoSize().event() += [&](auto) { updateAutoSize(); };
    _label->p().event() += [&](auto) { updateAutoSize(); };
    _label->font().event() += [&](auto) { updateAutoSize(); };
    _label->text().event() += [&](auto) { setText(_label->text()); updateAutoSize(); };

    _label->clickable().event() += [&](auto) { onClickableChanged(); };
    _label->elipsis().event() += [&](auto) { rebuild(); };
    _label->horizontalAlignment().event() += [&](auto) { rebuild(); };

    _label->verticalAlignment().event() += [&](auto)
        {
            if (_label->verticalAlignment() == VerticalAlign::Bottom)
                // Windows: bottom alignment is not supported    
                _label->verticalAlignment() = VerticalAlign::Top;
            else
                rebuild();
        };
}

void LabelImpl::build()
{
    ControlImpl::build();

    setText(_label->text());
    updateAutoSize();
}

unsigned int LabelImpl::styles() const
{
    auto styles = ControlImpl::styles();
    styles = styles & ~WS_TABSTOP;
    styles |= SS_NOPREFIX;

    switch (_label->horizontalAlignment())
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

    switch (_label->verticalAlignment())
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

    if (_label->clickable())
        styles |= SS_NOTIFY;

    if (_label->elipsis())
        styles |= SS_ENDELLIPSIS;

    return styles;
}

void LabelImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == STN_CLICKED)
        {
            _label->ClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == STN_DBLCLK)
        {
            _label->DoubleClickEvent().invoke();
        }
    }
}

void LabelImpl::onClickableChanged()
{
    if (hwnd() == 0)
        return;

    // update style
    SetWindowLong(hwnd(), GWL_STYLE, styles());
}

void LabelImpl::updateAutoSize()
{
    if (_label->parent().expired())
        return;
    if (!_label->autoSize())
        return;

    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    auto f = _label->font().value();
    if (!f.family().empty())
        hFont = FontLoader::toGdiFont(f);

    auto hdc = CreateCompatibleDC(NULL);
    auto szl = SIZE();
    SelectObject(hdc, hFont);
    std::string text = _label->text().value();
    GetTextExtentPoint32W(hdc, StringEncoder::toWide(text).c_str(), (int)text.size(), &szl);
    DeleteDC(hdc);
    DeleteObject(hFont);

    auto hwndParent = GetParent(hwnd());
    Size size = Convert(hwndParent).toUnits(Size(szl.cx + 1, szl.cy + 1));
    _label->resize(size);
}
