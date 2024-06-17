#include "control_p.h"
#include "utility.h"
#include "dlgcpp/dialog.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using namespace dlgcpp;

// private functions
void destruct(ctl_priv& pi);
LRESULT CALLBACK controlWndProc(HWND hwnd, UINT wMsg, WPARAM wParam, LPARAM lParam);


Control::Control(const std::string& text, const Position& p) :
    _pi(new ctl_priv())
{
    _pi->props.p = p;
    _pi->props.text = text;
    _pi->state.hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    // TODO: translate and store system font
    //auto lfw = LOGFONTW();
    //GetObject(_pi->state.font, sizeof(LOGFONTW), &lfw);
    //_pi->props.font = Font{};
}

Control::~Control()
{
    destruct(*_pi);

    if (_pi->state.hFont != nullptr)
        DeleteObject(_pi->state.hFont);
    if (_pi->state.hbrBack != NULL)
        DeleteObject(_pi->state.hbrBack);

    delete _pi;
}

ISharedDialog Control::parent() const
{
    return _pi->props.parent;
}

void Control::parent(ISharedDialog parent)
{
    if (_pi->props.parent == parent)
        return;
    _pi->props.parent = parent;
}

int Control::id() const
{
    return _pi->props.id;
}

void Control::id(int value)
{
    _pi->props.id = value;
}

std::shared_ptr<IControl> Control::control()
{
    return shared_from_this();
}

ctl_state Control::state()
{
    return _pi->state;
}

void Control::notify(dlg_message&)
{
    // no default action
}

void Control::notify(ctl_message& msg)
{
    HWND hwndParent = GetParent(_pi->state.hwnd);
    auto wMsg = msg.wMsg;
    auto wParam = msg.wParam;
    auto lParam = msg.lParam;

    switch (wMsg)
    {
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    {
        MouseEvent event;

        if (wMsg == WM_LBUTTONDOWN)
            event.button = MouseButton::Left;
        else if (wMsg == WM_RBUTTONDOWN)
            event.button = MouseButton::Right;
        else
            event.button = MouseButton::Middle;

        event.point = Point(LOWORD(lParam), HIWORD(lParam));
        toUnits(hwndParent, event.point);

        MouseDownEvent().invoke(shared_from_this(), event);
        break;
    }

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    {
        MouseEvent event;

        if (wMsg == WM_LBUTTONUP)
            event.button = MouseButton::Left;
        else if (wMsg == WM_RBUTTONUP)
            event.button = MouseButton::Right;
        else
            event.button = MouseButton::Middle;

        event.point = Point(LOWORD(lParam), HIWORD(lParam));
        toUnits(hwndParent, event.point);

        MouseUpEvent().invoke(shared_from_this(), event);
        break;
    }

    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
    {
        MouseEvent event;

        if (wMsg == WM_LBUTTONDBLCLK)
            event.button = MouseButton::Left;
        else if (wMsg == WM_RBUTTONDBLCLK)
            event.button = MouseButton::Right;
        else
            event.button = MouseButton::Middle;

        event.point = Point(LOWORD(lParam), HIWORD(lParam));
        toUnits(hwndParent, event.point);

        MouseDoubleClickEvent().invoke(shared_from_this(), event);
        break;
    }

    case WM_MOUSEMOVE:
    {
        MouseEvent event;
        event.button = wMsg == WM_LBUTTONDOWN ? MouseButton::Left : wMsg == WM_RBUTTONDOWN ? MouseButton::Right : MouseButton::Middle;
        event.point = Point(LOWORD(lParam), HIWORD(lParam));
        toUnits(hwndParent, event.point);

        MouseMoveEvent().invoke(shared_from_this(), event);
        break;
    }

    case WM_CAPTURECHANGED:
    {
        MouseCaptureLostEvent().invoke(shared_from_this());
        break;
    }

    case WM_MOVE:
    {
        // translate using mapped value and store
        Point posPx((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
        Point posDu(posPx);
        toUnits(GetParent(_pi->state.hwnd), posDu);

        DLGCPP_CMSG("WM_MOVE: " <<
                    "x = "  << posDu.x() << " (" << posPx.x() << ") " <<
                    "y = " << posDu.y() << " (" << posPx.y() << ") " <<
                    "text = " + _pi->props.text);

        _pi->props.p.x(posDu.x());
        _pi->props.p.y(posDu.y());
        MoveEvent().invoke(shared_from_this());
        break;
    }
    case WM_SIZE:
    {
        // translate using mapped value and store
        Size sizePx({(int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)});
        Size sizeDu(sizePx);
        toUnits(GetParent(_pi->state.hwnd), sizeDu);

        DLGCPP_CMSG("WM_SIZE: " <<
                    "width = "  << sizeDu.width() << " (" << sizePx.width() << ") " <<
                    "height = " << sizeDu.height() << " (" << sizePx.height() << ") " <<
                    "text = " + _pi->props.text);

        _pi->props.p.width(sizeDu.width());
        _pi->props.p.height(sizeDu.height());
        SizeEvent().invoke(shared_from_this());
        break;
    }
    }

    // use default control action
    msg.result = CallWindowProc(msg.orgWndProc, _pi->state.hwnd, msg.wMsg, msg.wParam, msg.lParam);
}

bool Control::enabled() const
{
    return _pi->props.enabled;
}

void Control::enabled(bool value)
{
    if (_pi->props.enabled == value)
        return;
    _pi->props.enabled = value;
    if (_pi->state.hwnd == NULL)
        return;

    EnableWindow(_pi->state.hwnd, _pi->props.enabled);
}


bool Control::visible() const
{
    return _pi->props.visible;
}

void Control::visible(bool value)
{
    _pi->props.visible = value;

    if (_pi->state.hwnd == NULL)
        return;

    ShowWindow(_pi->state.hwnd,
               _pi->props.visible ? SW_SHOW : SW_HIDE);
}

const Position& Control::p() const
{
    return _pi->props.p;
}

void Control::p(const Position& p)
{
    _pi->props.p = p;

    if (_pi->props.parent == nullptr)
        return;
    if (_pi->state.hwnd == NULL)
        return;
    auto hwndParent = reinterpret_cast<HWND>(_pi->props.parent->handle());

    // Convert units to pixels
    auto px = toPixels(hwndParent, p);

    SetWindowPos(_pi->state.hwnd,
                 0,
                 px.x(),
                 px.y(),
                 px.width(),
                 px.height(),
                 SWP_NOZORDER);
}

void Control::move(const Point& point)
{
    _pi->props.p.x(point.x());
    _pi->props.p.y(point.y());

    if (_pi->state.hwnd == NULL)
        return;

    auto px = toPixels(GetParent(_pi->state.hwnd), _pi->props.p);

    SetWindowPos(_pi->state.hwnd,
                 0,
                 px.x(),
                 px.y(),
                 0,
                 0,
                 SWP_NOZORDER | SWP_NOSIZE);
}

void Control::resize(const Size& size)
{
    _pi->props.p.width(size.width());
    _pi->props.p.height(size.height());

    if (_pi->state.hwnd == NULL)
        return;

    auto px = toPixels(GetParent(_pi->state.hwnd), _pi->props.p);

    SetWindowPos(_pi->state.hwnd,
                 0,
                 0,
                 0,
                 px.width(),
                 px.height(),
                 SWP_NOZORDER | SWP_NOMOVE);
}

BorderStyle Control::border() const
{
    return _pi->props.borderStyle;
}

void Control::border(BorderStyle value)
{
    if (_pi->props.borderStyle == value)
        return;
    _pi->props.borderStyle = value;

    rebuild();
}

const std::string& Control::text() const
{
    return _pi->props.text;
}

void Control::text(const std::string& value)
{
    if (_pi->props.text == value)
        return;

    _pi->props.text = value;
    if (_pi->state.hwnd == NULL)
        return;

    // only sync the text when it has changed
    auto cbText = (size_t)GetWindowTextLengthW(_pi->state.hwnd);
    if (_pi->props.text.size() == cbText)
    {
        cbText++;
        std::wstring buf(cbText, 0);
        GetWindowTextW(_pi->state.hwnd, &buf[0], cbText);
        if (_pi->props.text == toBytes(buf.c_str()))
            return;
    }

    SetWindowTextW(_pi->state.hwnd,
                   toWide(_pi->props.text).c_str());
}

std::pair<Color, Color> Control::colors() const
{
    return std::make_pair(_pi->props.fgColor, _pi->props.bgColor);
}

void Control::colors(Color fgColor, Color bgColor)
{
    bool changed = false;

    // foreground 'None' is not supported
    if (fgColor != Color::None &&
        _pi->props.fgColor != fgColor)
    {
        _pi->props.fgColor = fgColor;
        changed = true;
    }
    if (bgColor != fgColor)
    {
        _pi->props.bgColor = bgColor;
        changed = true;
    }

    if (!changed)
        return;

    if (_pi->state.hbrBack != NULL)
    {
        DeleteObject(_pi->state.hbrBack);
        _pi->state.hbrBack = NULL;
    }

    if (bgColor != Color::None && bgColor != Color::Default)
        _pi->state.hbrBack = CreateSolidBrush((COLORREF)bgColor);
    redraw();
}

Cursor Control::cursor() const
{
    return _pi->props.cursor;
}

void Control::cursor(Cursor value)
{
    _pi->props.cursor = value;
}

const Font& Control::font() const
{
    return _pi->props.font;
}

void Control::font(const Font& value)
{
    _pi->props.font = value;

    if (_pi->state.hFont != NULL)
        DeleteObject(_pi->state.hFont);

    if (!_pi->props.font.faceName.empty())
        _pi->state.hFont = makeFont(_pi->props.font);
    else
        _pi->state.hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    if (_pi->state.hwnd == NULL)
        return;

    SendMessage(_pi->state.hwnd, WM_SETFONT, (WPARAM)_pi->state.hFont, TRUE);
}

void* Control::handle() const
{
    return _pi->state.hwnd;
}

void* Control::user() const
{
    return _pi->props.user;
}

void Control::user(void* value)
{
    _pi->props.user = value;
}

std::string Control::className() const
{
    return "STATIC";
}

unsigned int Control::styles() const
{
    unsigned int styles = WS_CHILD | WS_TABSTOP | WS_CLIPSIBLINGS;
    if (_pi->props.visible)
        styles |= WS_VISIBLE;
    if (!_pi->props.enabled)
        styles |= WS_DISABLED;
    return styles;
}

unsigned int Control::exStyles() const
{
    unsigned int styles = 0;

    switch (_pi->props.borderStyle)
    {
    case BorderStyle::Raised:
        styles |= WS_EX_DLGMODALFRAME;
        break;
    case BorderStyle::Thin:
        styles |= WS_EX_STATICEDGE;
        break;
    case BorderStyle::Sunken:
        styles |= WS_EX_CLIENTEDGE;
        break;
    case BorderStyle::None:
        break;
    }

    return styles;
}

void Control::redraw()
{
    if (_pi->state.hwnd == NULL)
        return;

    RedrawWindow(_pi->state.hwnd, NULL, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
}

void Control::setFocus()
{
    if (_pi->state.hwnd == NULL)
        return;

    SetFocus(_pi->state.hwnd);
}

void Control::rebuild()
{
    destruct(*_pi);

    // safety checks
    if (_pi->props.parent == nullptr)
        return;

    if (_pi->props.id < 1)
        return;

    HWND hwndParent = reinterpret_cast<HWND>(_pi->props.parent->handle());
    if (hwndParent == NULL)
        return;

    auto p = toPixels(hwndParent, _pi->props.p);

    auto hwnd = CreateWindowExW(exStyles(),
                                toWide(className()).c_str(),
                                toWide(_pi->props.text).c_str(),
                                styles(),
                                p.x(),
                                p.y(),
                                p.width(),
                                p.height(),
                                hwndParent,
                                (HMENU)(UINT_PTR)_pi->props.id,
                                GetModuleHandle(NULL), NULL);
    if (hwnd == NULL)
        return;

    _pi->state.hwnd = hwnd;

    if (_pi->props.subclass)
    {
        SetProp(_pi->state.hwnd, "this", this);
        _pi->state.prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)controlWndProc);
        SetProp(_pi->state.hwnd, "pproc", (HANDLE)_pi->state.prevWndProc);
    }

    SendMessage(_pi->state.hwnd, WM_SETFONT, (WPARAM)_pi->state.hFont, FALSE);
}

void destruct(ctl_priv& pi)
{
    if (pi.state.hwnd == NULL)
        return;

    if (pi.props.subclass)
    {
        // remove subclass
        SetWindowLongPtr(pi.state.hwnd, GWLP_WNDPROC, (LONG_PTR)pi.state.prevWndProc);
        SetProp(pi.state.hwnd, "pproc", (HANDLE)NULL);
    }

    DestroyWindow(pi.state.hwnd);
    pi.state.hwnd = nullptr;
}

LRESULT CALLBACK controlWndProc(HWND hwnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    auto pthis = reinterpret_cast<Control*>(GetProp(hwnd, "this"));
    auto pproc = reinterpret_cast<WNDPROC>(GetProp(hwnd, "pproc"));

    if (pthis != nullptr)
    {
        // wrap and transfer the message directly to the class.
        // note: all derivatives will need to process the message and use CallWindowProc
        auto msg = ctl_message{wMsg, wParam, lParam, 0, pproc};
        pthis->notify(msg);
        return msg.result;
    }
    return CallWindowProc(pproc, hwnd, wMsg, wParam, lParam);
}

IEvent<ISharedControl>& Control::ClickEvent()
{
    return _pi->props.clickEvent;
}

IEvent<ISharedControl>& Control::DoubleClickEvent()
{
    return _pi->props.dblClickEvent;
}

IEvent<ISharedControl, bool>& Control::FocusEvent()
{
    return _pi->props.focusEvent;
}

IEvent<ISharedControl, MouseEvent>& Control::MouseDownEvent()
{
    return _pi->props.mouseDownEvent;
}

IEvent<ISharedControl, MouseEvent>& Control::MouseUpEvent()
{
    return _pi->props.mouseUpEvent;
}

IEvent<ISharedControl, MouseEvent>& Control::MouseMoveEvent()
{
    return _pi->props.mouseMoveEvent;
}

IEvent<ISharedControl, MouseEvent>& Control::MouseDoubleClickEvent()
{
    return _pi->props.mouseDblClickEvent;
}

IEvent<ISharedControl>& Control::MouseCaptureLostEvent()
{
    return _pi->props.mouseCaptureLost;
}

IEvent<ISharedControl>& Control::MoveEvent()
{
    return _pi->props.moveEvent;
}

IEvent<ISharedControl>& Control::SizeEvent()
{
    return _pi->props.sizeEvent;
}
