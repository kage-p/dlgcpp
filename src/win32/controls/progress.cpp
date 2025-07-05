#include "../dlgmsg.h"
#include "progress_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Progress::Progress(const Position& p) :
    Control(std::string(), p),
    _props(new progress_props())
{
    this->border(BorderStyle::Thin);
}

Progress::~Progress()
{
    delete _props;
}

void Progress::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // apply the properties
    SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)_props->range.first, (LPARAM)_props->range.second);
    SendMessage(hwnd, PBM_SETPOS, (WPARAM)_props->value, FALSE);

    updateDisplayStyles();
}

std::string Progress::className() const
{
    return PROGRESS_CLASSA;
}

unsigned int Progress::styles() const
{
    auto styles = Control::styles();

    styles |= PBS_SMOOTH;

    if (_props->vertical)
        styles |= PBS_VERTICAL;
    return styles;
}

IEvent<ISharedControl>& Progress::ChangedEvent()
{
    return _props->changedEvent;
}

void Progress::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_HSCROLL)
    {
        value((int)msg.wParam);
        ChangedEvent().invoke(shared_from_this());
    }
    else if (msg.wMsg == WM_VSCROLL)
    {
        value((int)msg.wParam);
        ChangedEvent().invoke(shared_from_this());
    }
    Control::notify(msg);
}

bool Progress::vertical() const
{
    return _props->vertical;
}

void Progress::vertical(bool value)
{
    if (_props->vertical == value)
        return;
    _props->vertical = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // update style
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

int Progress::value() const
{
    return _props->value;
}

void Progress::value(int value)
{
    if (_props->value == value)
        return;
    _props->value = value;

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, PBM_SETPOS, (WPARAM)_props->value, 0);
}

std::pair<int, int> Progress::range() const
{
    return _props->range;
}

void Progress::range(int from, int to)
{
    if (_props->range == std::pair<int, int>(from, to))
        return;
    _props->range = std::pair<int, int>(from, to);

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, PBM_SETRANGE32, (WPARAM)_props->range.first, (LPARAM)_props->range.second);
}

void Progress::colors(Color fgColor, Color bgColor)
{
    Control::colors(fgColor, bgColor);

    if (handle() == nullptr)
        return;

    updateDisplayStyles();
}

void Progress::updateDisplayStyles()
{
    auto hwnd = reinterpret_cast<HWND>(handle());
    auto clrPair = Control::colors();

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

