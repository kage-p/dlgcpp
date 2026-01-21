#include "progress_impl.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ProgressImpl::ProgressImpl() :
    ControlImpl()
{

}

ProgressImpl::~ProgressImpl()
{
}

void ProgressImpl::owner(IProgress* progress)
{
    ControlImpl::owner(progress);
    _progress = progress;

    _progress->colors().event() += [&](auto) { updateDisplayStyles(); };
    _progress->range().event() += [&](auto) { onRangeChanged(); };
    _progress->value().event() += [&](auto) { onValueChanged(); };
    _progress->vertical().event() += [&](auto) { onVerticalChanged(); };
}

void ProgressImpl::build()
{
    ControlImpl::build();

    if (hwnd() == 0)
        return;
    auto hwnd = this->hwnd();

    // apply the properties
    auto range = _progress->range().value();
    auto value = _progress->value().value();
    SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)range.first, (LPARAM)range.second);
    SendMessage(hwnd, PBM_SETPOS, (WPARAM)value, FALSE);

    updateDisplayStyles();
}

std::string ProgressImpl::className() const
{
    return PROGRESS_CLASSA;
}

unsigned int ProgressImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |= PBS_SMOOTH;

    if (_progress->vertical())
        styles |= PBS_VERTICAL;
    return styles;
}

void ProgressImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_HSCROLL || msg.wMsg == WM_VSCROLL)
    {
        _progress->value() = (int)msg.wParam;
    }

    // default notify handler
    switch (msg.wMsg)
    {
    case WM_NOTIFY:
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);
        switch (nmhdr.code)
        {
        case NM_CLICK:
            _progress->ClickEvent().invoke();
            break;

        case NM_DBLCLK:
            _progress->DoubleClickEvent().invoke();
            break;

        case NM_RCLICK:
            _progress->RightClickEvent().invoke();
            break;

        case NM_RDBLCLK:
            _progress->DoubleRightClickEvent().invoke();
            break;

        case NM_SETFOCUS:
            _progress->focus() = true;
            break;

        case NM_KILLFOCUS:
            _progress->focus() = false;
            break;
        }
    }
    }
}

void ProgressImpl::onVerticalChanged()
{
    if (hwnd() == 0)
        return;

    // update style
    SetWindowLong(hwnd(), GWL_STYLE, styles());
}

void ProgressImpl::onValueChanged()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), PBM_SETPOS, (WPARAM)_progress->value(), 0);
}

void ProgressImpl::onRangeChanged()
{
    if (hwnd() == 0)
        return;

    auto range = _progress->range().value();
    SendMessage(hwnd(), PBM_SETRANGE32, (WPARAM)range.first, (LPARAM)range.second);
}


void ProgressImpl::updateDisplayStyles()
{
    if (hwnd() == 0)
        return;

    auto clrPair = _progress->colors().value();

    COLORREF backColor =
        clrPair.second != Color::Default
        ? (COLORREF)clrPair.second
        : GetSysColor(COLOR_3DFACE);
    COLORREF barColor =
        clrPair.first != Color::Default
        ? (COLORREF)clrPair.first
        : GetSysColor(COLOR_HIGHLIGHT);

    SendMessage(hwnd(), PBM_SETBARCOLOR, 0, (LPARAM)barColor);
    SendMessage(hwnd(), PBM_SETBKCOLOR, 0, (LPARAM)backColor);
}
