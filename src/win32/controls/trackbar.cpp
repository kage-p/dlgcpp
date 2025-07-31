#include "trackbar_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TrackBarImpl::TrackBarImpl(
    const Position& p) :
    ControlImpl(std::string(), p)
{
}

TrackBarImpl::~TrackBarImpl()
{
}

void TrackBarImpl::rebuild()
{
    ControlImpl::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    // apply the properties
    SendMessage(hwnd, TBM_SETRANGEMIN, FALSE, (LPARAM)_range.first);
    SendMessage(hwnd, TBM_SETRANGEMAX, FALSE, (LPARAM)_range.second);
    SendMessage(hwnd, TBM_SETPOS, FALSE, (WPARAM)_value);
    SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)_pageSize);
}

std::string TrackBarImpl::className() const
{
    return TRACKBAR_CLASSA;
}

unsigned int TrackBarImpl::styles() const
{
    auto styles = ControlImpl::styles();

    if (_tickMarks != TrackBarTickMark::None)
    {
        styles |= TBS_AUTOTICKS;

        if (_tickMarks == TrackBarTickMark::Both)
        {
            styles |= TBS_BOTH;
        }
        else if (_tickMarks == TrackBarTickMark::Above)
        {
            styles |= (_vertical) ? TBS_LEFT : TBS_TOP;
        }
        else if (_tickMarks == TrackBarTickMark::Below)
        {
            styles |= (_vertical) ? TBS_RIGHT : TBS_BOTTOM;
        }
    }
    else
    {
        styles |= TBS_NOTICKS;
    }

    styles |= (_vertical ? TBS_VERT : TBS_HORZ);
    return styles;
}

IEvent<ISharedControl>& TrackBarImpl::ChangedEvent()
{
    return _changedEvent;
}

void TrackBarImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_NOTIFY)
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);
        if (nmhdr.code == NM_CUSTOMDRAW)
        {
            auto& nmcd = *((NMCUSTOMDRAW*)msg.lParam);

            switch (nmcd.dwDrawStage)
            {
            case CDDS_PREPAINT:
                msg.msgResult = CDRF_NOTIFYITEMDRAW;
                msg.dlgResult = TRUE;
                break;

            case CDDS_ITEMPREPAINT:
            {
                switch (nmcd.dwItemSpec)
                {
                case TBCD_CHANNEL:
                    if (_barColor != Color::Default &&
                        _barColor != Color::None)
                    {
                        HDC hdc = nmcd.hdc;
                        RECT rc = nmcd.rc;
                        SetBkMode(hdc, TRANSPARENT);
                        SetDCBrushColor(hdc, (COLORREF)_barColor);
                        FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
                        msg.msgResult = CDRF_SKIPDEFAULT;
                    }
                    else
                    {
                        msg.msgResult = CDRF_DODEFAULT;
                    }
                    break;

                case TBCD_THUMB:
                    if (_thumbColor != Color::Default &&
                        _thumbColor != Color::None)
                    {
                        HDC hdc = nmcd.hdc;
                        RECT rc = nmcd.rc;
                        SetBkMode(hdc, TRANSPARENT);
                        SetDCBrushColor(hdc, (COLORREF)_thumbColor);
                        FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
                        msg.msgResult = CDRF_SKIPDEFAULT;
                    }
                    else
                    {
                        msg.msgResult = CDRF_DODEFAULT;
                    }
                    break;

                case TBCD_TICS:
                {
                    auto fgColor = colors().first;
                    if (_tickMarks != TrackBarTickMark::None &&
                        fgColor != Color::Default &&
                        fgColor != Color::None)
                    {
                        HWND hwndTrack = nmhdr.hwndFrom;
                        HDC hdc = nmcd.hdc;
                        RECT rc = nmcd.rc;

                        RECT thumbRect = {};
                        SendMessage(hwndTrack, TBM_GETTHUMBRECT, 0, (LPARAM)&thumbRect);

                        int numTics = (int)SendMessage(hwndTrack, TBM_GETNUMTICS, 0, 0);
                        int ticLength = 4; // Length of tic in pixels
                        int tickSpace = 2; // Space between thumb and tic

                        HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF)fgColor);
                        HGDIOBJ oldPen = SelectObject(hdc, hPen);

                        for (int i = 0; i < numTics; ++i)
                        {
                            int pos = (int)SendMessage(hwndTrack, TBM_GETTICPOS, i, 0);

                            if (_vertical)
                            {
                                // Vertical
                                if (_tickMarks == TrackBarTickMark::Above ||
                                    _tickMarks == TrackBarTickMark::Both)
                                {
                                    // Left (Above)
                                    MoveToEx(hdc, (thumbRect.left - tickSpace) - ticLength, pos, nullptr);
                                    LineTo(hdc, (thumbRect.left - tickSpace), pos);
                                }

                                if (_tickMarks == TrackBarTickMark::Below ||
                                    _tickMarks == TrackBarTickMark::Both)
                                {
                                    // Right (Below)
                                    MoveToEx(hdc, (thumbRect.right + tickSpace), pos, nullptr);
                                    LineTo(hdc, (thumbRect.right + tickSpace) + ticLength, pos);
                                }
                            }
                            else
                            {
                                // Horizontal                               
                                if (_tickMarks == TrackBarTickMark::Above ||
                                    _tickMarks == TrackBarTickMark::Both)
                                {
                                    MoveToEx(hdc, pos, (thumbRect.top - tickSpace) - ticLength, nullptr);
                                    LineTo(hdc, pos, (thumbRect.top - tickSpace));
                                }

                                if (_tickMarks == TrackBarTickMark::Below ||
                                    _tickMarks == TrackBarTickMark::Both)
                                {
                                    MoveToEx(hdc, pos, (thumbRect.bottom + tickSpace), nullptr);
                                    LineTo(hdc, pos, (thumbRect.bottom + tickSpace) + ticLength);
                                }
                            }
                        }

                        SelectObject(hdc, oldPen);
                        DeleteObject(hPen);
                        msg.msgResult = CDRF_SKIPDEFAULT;
                    }
                    else
                    {
                        msg.msgResult = CDRF_DODEFAULT;
                    }
                    break;
                }

                default:
                    msg.msgResult = CDRF_DODEFAULT;
                    break;
                }
                msg.dlgResult = TRUE;
            }
            }
        }
    }
    else if (msg.wMsg == WM_HSCROLL)
    {
        // just report the new position value
        auto hwnd = reinterpret_cast<HWND>(handle());
        auto actualValue = (int)SendMessage(hwnd, TBM_GETPOS, 0, 0);
        _value = actualValue;

        ChangedEvent().invoke(control());
    }
    else if (msg.wMsg == WM_VSCROLL)
    {
        // just report the new position value
        auto hwnd = reinterpret_cast<HWND>(handle());
        auto actualValue = (int)SendMessage(hwnd, TBM_GETPOS, 0, 0);
        _value = actualValue;

        ChangedEvent().invoke(control());
    }

    ControlImpl::notify(msg);
}

bool TrackBarImpl::vertical() const
{
    return _vertical;
}

void TrackBarImpl::vertical(bool value)
{
    if (_vertical == value)
        return;
    _vertical = value;

    if (handle() == nullptr)
        return;

    // update style
    auto hwnd = reinterpret_cast<HWND>(handle());
    SetWindowLong(hwnd, GWL_STYLE, styles());
}

int TrackBarImpl::value() const
{
    return _value;
}

void TrackBarImpl::value(int value)
{
    if (_value == value)
        return;
    _value = value;

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, TBM_SETPOS, TRUE, (WPARAM)_value);
}

std::pair<int, int> TrackBarImpl::range() const
{
    return _range;
}

void TrackBarImpl::range(int from, int to)
{
    if (_range == std::pair<int, int>(from, to))
        return;
    _range = std::pair<int, int>(from, to);

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, TBM_SETRANGEMIN, TRUE, (LPARAM)_range.first);
    SendMessage(hwnd, TBM_SETRANGEMAX, TRUE, (LPARAM)_range.second);
}

int TrackBarImpl::pageSize() const
{
    return _pageSize;
}

void TrackBarImpl::pageSize(int value)
{
    if (_pageSize == value)
        return;
    _pageSize = value;

    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)_pageSize);
}

TrackBarTickMark TrackBarImpl::tickMarks() const
{
    return _tickMarks;
}

void TrackBarImpl::tickMarks(TrackBarTickMark value)
{
    if (_tickMarks == value)
        return;
    _tickMarks = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

std::pair<Color, Color> TrackBarImpl::barColors() const
{
    return std::make_pair(_thumbColor, _barColor);
}

void TrackBarImpl::barColors(Color thumbColor, Color barColor)
{
    bool changed = false;
    if (_thumbColor != thumbColor)
    {
        _thumbColor = thumbColor;
        changed = true;
    }
    if (_barColor != barColor)
    {
        _barColor = barColor;
        changed = true;
    }

    if (!changed)
        return;

    if (handle() == nullptr)
        return;

    redraw();
}