#include "trackbar_impl.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TrackBarImpl::TrackBarImpl() :
    ControlImpl()
{
}

TrackBarImpl::~TrackBarImpl()
{
}

void TrackBarImpl::owner(ITrackBar* trackBar)
{
    ControlImpl::owner(trackBar);

    _trackBar = trackBar;

    _trackBar->vertical().event() += [&](auto) { onVerticalChanged(); };
    _trackBar->range().event() += [&](auto) { onRangeChanged(); };
    _trackBar->pageSize().event() += [&](auto) { onPageSizeChanged(); };
    _trackBar->value().event() += [&](auto) { onValueChanged(); };
    _trackBar->tickMarks().event() += [&](auto) { rebuild(); };
    _trackBar->barColor().event() += [&](auto) { redraw(); };
    _trackBar->thumbColor().event() += [&](auto) { redraw(); };
}

void TrackBarImpl::build()
{
    ControlImpl::build();

    if (hwnd() == 0)
        return;
    auto hwnd = this->hwnd();

    // apply the properties
    const auto& range = _trackBar->range().value();
    SendMessage(hwnd, TBM_SETRANGEMIN, FALSE, (LPARAM)range.first);
    SendMessage(hwnd, TBM_SETRANGEMAX, FALSE, (LPARAM)range.second);
    SendMessage(hwnd, TBM_SETPOS, FALSE, (WPARAM)_trackBar->value().value());
    SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)_trackBar->pageSize().value());
}

std::string TrackBarImpl::className() const
{
    return TRACKBAR_CLASSA;
}

unsigned int TrackBarImpl::styles() const
{
    auto styles = ControlImpl::styles();

    if (_trackBar->tickMarks() != TrackBarTickMark::None)
    {
        styles |= TBS_AUTOTICKS;

        if (_trackBar->tickMarks() == TrackBarTickMark::Both)
        {
            styles |= TBS_BOTH;
        }
        else if (_trackBar->tickMarks() == TrackBarTickMark::Above)
        {
            styles |= (_trackBar->vertical()) ? TBS_LEFT : TBS_TOP;
        }
        else if (_trackBar->tickMarks() == TrackBarTickMark::Below)
        {
            styles |= (_trackBar->vertical()) ? TBS_RIGHT : TBS_BOTTOM;
        }
    }
    else
    {
        styles |= TBS_NOTICKS;
    }

    styles |= (_trackBar->vertical() ? TBS_VERT : TBS_HORZ);
    return styles;
}

void TrackBarImpl::notify(DialogMessage& msg)
{
    switch (msg.wMsg)
    {
    case WM_NOTIFY:
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
                    if (_trackBar->barColor() != Color::Default &&
                        _trackBar->barColor() != Color::None)
                    {
                        HDC hdc = nmcd.hdc;
                        RECT rc = nmcd.rc;
                        SetBkMode(hdc, TRANSPARENT);
                        SetDCBrushColor(hdc, (COLORREF)_trackBar->barColor().value());
                        FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
                        msg.msgResult = CDRF_SKIPDEFAULT;
                    }
                    else
                    {
                        msg.msgResult = CDRF_DODEFAULT;
                    }
                    break;

                case TBCD_THUMB:
                    if (_trackBar->thumbColor() != Color::Default &&
                        _trackBar->thumbColor() != Color::None)
                    {
                        HDC hdc = nmcd.hdc;
                        RECT rc = nmcd.rc;
                        SetBkMode(hdc, TRANSPARENT);
                        SetDCBrushColor(hdc, (COLORREF)_trackBar->thumbColor().value());
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
                    auto fgColor = _trackBar->colors().value().first;
                    auto tickMarks = _trackBar->tickMarks().value();

                    if (tickMarks != TrackBarTickMark::None &&
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

                            if (_trackBar->vertical())
                            {
                                // Vertical
                                if (tickMarks == TrackBarTickMark::Above ||
                                    tickMarks == TrackBarTickMark::Both)
                                {
                                    // Left (Above)
                                    MoveToEx(hdc, (thumbRect.left - tickSpace) - ticLength, pos, nullptr);
                                    LineTo(hdc, (thumbRect.left - tickSpace), pos);
                                }

                                if (tickMarks == TrackBarTickMark::Below ||
                                    tickMarks == TrackBarTickMark::Both)
                                {
                                    // Right (Below)
                                    MoveToEx(hdc, (thumbRect.right + tickSpace), pos, nullptr);
                                    LineTo(hdc, (thumbRect.right + tickSpace) + ticLength, pos);
                                }
                            }
                            else
                            {
                                // Horizontal                               
                                if (tickMarks == TrackBarTickMark::Above ||
                                    tickMarks == TrackBarTickMark::Both)
                                {
                                    MoveToEx(hdc, pos, (thumbRect.top - tickSpace) - ticLength, nullptr);
                                    LineTo(hdc, pos, (thumbRect.top - tickSpace));
                                }

                                if (tickMarks == TrackBarTickMark::Below ||
                                    tickMarks == TrackBarTickMark::Both)
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
        break;
    }
    case WM_HSCROLL:
    {
        // just report the new position value
        auto actualValue = (int)SendMessage(hwnd(), TBM_GETPOS, 0, 0);
        _trackBar->value() = actualValue;
        break;
    }
    case WM_VSCROLL:
    {
        // just report the new position value
        auto actualValue = (int)SendMessage(hwnd(), TBM_GETPOS, 0, 0);
        _trackBar->value() = actualValue;
        break;
    }
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
            _trackBar->ClickEvent().invoke();
            break;

        case NM_DBLCLK:
            _trackBar->DoubleClickEvent().invoke();
            break;

        case NM_RCLICK:
            _trackBar->RightClickEvent().invoke();
            break;

        case NM_RDBLCLK:
            _trackBar->DoubleRightClickEvent().invoke();
            break;

        case NM_SETFOCUS:
            _trackBar->focus() = true;
            break;

        case NM_KILLFOCUS:
            _trackBar->focus() = false;
            break;
        }
    }
    }
}

void TrackBarImpl::onVerticalChanged()
{
    if (hwnd() == 0)
        return;

    // update style
    SetWindowLong(hwnd(), GWL_STYLE, styles());
}

void TrackBarImpl::onValueChanged()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), TBM_SETPOS, TRUE, (WPARAM)_trackBar->value().value());
}

void TrackBarImpl::onRangeChanged()
{
    if (hwnd() == 0)
        return;

    const auto& range = _trackBar->range().value();
    SendMessage(hwnd(), TBM_SETRANGEMIN, TRUE, (LPARAM)range.first);
    SendMessage(hwnd(), TBM_SETRANGEMAX, TRUE, (LPARAM)range.second);
}

void TrackBarImpl::onPageSizeChanged()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), TBM_SETPAGESIZE, 0, (LPARAM)_trackBar->pageSize().value());
}
