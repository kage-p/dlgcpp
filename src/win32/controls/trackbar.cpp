#include "trackbar_p.h"
#include "utility/message.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TrackBar::TrackBar(const Position& p) :
    Control(std::string(), p),
    _props(new trackbar_props())
{
}

TrackBar::~TrackBar()
{
    delete _props;
}

void TrackBar::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // apply the properties
    SendMessage(hwnd, TBM_SETRANGEMIN, FALSE, (LPARAM)_props->range.first);
    SendMessage(hwnd, TBM_SETRANGEMAX, FALSE, (LPARAM)_props->range.second);
    SendMessage(hwnd, TBM_SETPOS, FALSE, (WPARAM)_props->value);
    SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)_props->pageSize);
}

std::string TrackBar::className() const
{
    return TRACKBAR_CLASSA;
}

unsigned int TrackBar::styles() const
{
    auto styles = Control::styles();

    if (_props->tickMarks != TrackBarTickMark::None)
    {
        styles |= TBS_AUTOTICKS;

        if (_props->tickMarks == TrackBarTickMark::Both)
        {
            styles |= TBS_BOTH;
        }
        else if (_props->tickMarks == TrackBarTickMark::Above)
        {
            styles |= (_props->vertical) ? TBS_LEFT : TBS_TOP;
        }
        else if (_props->tickMarks == TrackBarTickMark::Below)
        {
            styles |= (_props->vertical) ? TBS_RIGHT : TBS_BOTTOM;
        }
    }
    else
    {
        styles |= TBS_NOTICKS;
    }

    styles |= (_props->vertical ? TBS_VERT : TBS_HORZ);
    return styles;
}

IEvent<ISharedControl>& TrackBar::ChangedEvent()
{
    return _props->changedEvent;
}

void TrackBar::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_HSCROLL)
    {
        // just report the new position value
        auto hwnd = reinterpret_cast<HWND>(handle());
        auto actualValue = (int)SendMessage(hwnd, TBM_GETPOS, 0, 0);
        _props->value = actualValue;

        ChangedEvent().invoke(shared_from_this());
    }
    else if (msg.wMsg == WM_VSCROLL)
    {
        // just report the new position value
        auto hwnd = reinterpret_cast<HWND>(handle());
        auto actualValue = (int)SendMessage(hwnd, TBM_GETPOS, 0, 0);
        _props->value = actualValue;

        ChangedEvent().invoke(shared_from_this());
    }

    Control::notify(msg);
}

bool TrackBar::vertical() const
{
    return _props->vertical;
}

void TrackBar::vertical(bool value)
{
    if (_props->vertical == value)
        return;
    _props->vertical = value;

    if (handle() == nullptr)
        return;

    // update style
    auto hwnd = reinterpret_cast<HWND>(handle());
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

int TrackBar::value() const
{
    return _props->value;
}

void TrackBar::value(int value)
{
    if (_props->value == value)
        return;
    _props->value = value;

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, TBM_SETPOS, TRUE, (WPARAM)_props->value);
}

std::pair<int, int> TrackBar::range() const
{
    return _props->range;
}

void TrackBar::range(int from, int to)
{
    if (_props->range == std::pair<int, int>(from, to))
        return;
    _props->range = std::pair<int, int>(from, to);

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, TBM_SETRANGEMIN, TRUE, (LPARAM)_props->range.first);
    SendMessage(hwnd, TBM_SETRANGEMAX, TRUE, (LPARAM)_props->range.second);
}

int TrackBar::pageSize() const
{
    return _props->pageSize;
}

void TrackBar::pageSize(int value)
{
    if (_props->pageSize == value)
        return;
    _props->pageSize = value;

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)_props->pageSize);
}

TrackBarTickMark TrackBar::tickMarks() const
{
    return _props->tickMarks;
}

void TrackBar::tickMarks(TrackBarTickMark value)
{
    if (_props->tickMarks == value)
        return;
    _props->tickMarks = value;

    if (handle() != nullptr)
        rebuild();
}

void TrackBar::colors(Color, Color bgColor)
{
    // foreground color is not supported for track bar
    Control::colors(Color::Default, bgColor);
}
