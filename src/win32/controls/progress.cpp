#include "progress_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ProgressImpl::ProgressImpl(
    const Position& p) :
    ControlImpl(std::string(), p)
{
    this->border(BorderStyle::Thin);
}

ProgressImpl::~ProgressImpl()
{
}

void ProgressImpl::rebuild()
{
    ControlImpl::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // apply the properties
    SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)_range.first, (LPARAM)_range.second);
    SendMessage(hwnd, PBM_SETPOS, (WPARAM)_value, FALSE);

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

    if (_vertical)
        styles |= PBS_VERTICAL;
    return styles;
}

IEvent<ISharedControl>& ProgressImpl::ChangedEvent()
{
    return _changedEvent;
}

void ProgressImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_HSCROLL)
    {
        value((int)msg.wParam);
        ChangedEvent().invoke(control());
    }
    else if (msg.wMsg == WM_VSCROLL)
    {
        value((int)msg.wParam);
        ChangedEvent().invoke(control());
    }
    ControlImpl::notify(msg);
}

bool ProgressImpl::vertical() const
{
    return _vertical;
}

void ProgressImpl::vertical(bool value)
{
    if (_vertical == value)
        return;
    _vertical = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // update style
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

int ProgressImpl::value() const
{
    return _value;
}

void ProgressImpl::value(int value)
{
    if (_value == value)
        return;
    _value = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, PBM_SETPOS, (WPARAM)_value, 0);
}

std::pair<int, int> ProgressImpl::range() const
{
    return _range;
}

void ProgressImpl::range(int from, int to)
{
    if (_range == std::pair<int, int>(from, to))
        return;
    _range = std::pair<int, int>(from, to);

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)_range.first, (LPARAM)_range.second);
}

void ProgressImpl::colors(Color fgColor, Color bgColor)
{
    ControlImpl::colors(fgColor, bgColor);

    if (handle() == nullptr)
        return;

    updateDisplayStyles();
}

void ProgressImpl::updateDisplayStyles()
{
    auto hwnd = reinterpret_cast<HWND>(handle());
    auto clrPair = ControlImpl::colors();

    COLORREF backColor =
        clrPair.second != Color::Default
        ? (COLORREF)clrPair.second
        : GetSysColor(COLOR_3DFACE);
    COLORREF barColor =
        clrPair.first != Color::Default
        ? (COLORREF)clrPair.first
        : GetSysColor(COLOR_HIGHLIGHT);

    SendMessage(hwnd, PBM_SETBARCOLOR, 0, (LPARAM)barColor);
    SendMessage(hwnd, PBM_SETBKCOLOR, 0, (LPARAM)backColor);
}

