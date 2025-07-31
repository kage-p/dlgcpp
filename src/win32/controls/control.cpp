#include "control_p.h"
#include "dlgcpp/dialogs/dialog.h"
#include "gfx/context_p.h"
#include "utility/convert.h"
#include "utility/font_loader.h"
#include "utility/key_mapper.h"
#include "utility/message.h"
#include "utility/string_encoder.h"
#include <map>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <CommCtrl.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::gfx;

ControlImpl::ControlImpl(
    const std::string& text,
    const Position& p) :
    _p(p),
    _text(text)
{

    // use system default font, but store the value
    _hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    _font = FontLoader::toFont(_hFont);
}

ControlImpl::~ControlImpl()
{
    destruct();

    if (_hFont != nullptr)
        DeleteObject(_hFont);
    if (_hbrBack != NULL)
        DeleteObject(_hbrBack);
}

void ControlImpl::control(Control* ptr)
{
    _control = ptr;
}

ISharedDialog ControlImpl::parent() const
{
    return _parent;
}

void ControlImpl::parent(ISharedDialog parent)
{
    if (_parent == parent)
        return;
    _parent = parent;
}

int ControlImpl::id() const
{
    return _id;
}

void ControlImpl::id(int value)
{
    _id = value;
}

int ControlImpl::idRange() const
{
    return 1;
}

HBRUSH ControlImpl::backgroundBrush() const
{
    return _hbrBack;
}

/// <summary>
/// The default handler for control WM_NOTIFY messages.
/// </summary>
void ControlImpl::notify(DialogMessage& msg)
{
    switch (msg.wMsg)
    {
    case WM_NOTIFY:
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);
        switch (nmhdr.code)
        {
        case NM_CLICK:
            _clickEvent.invoke(control());
            break;
        case NM_RCLICK:
            _rightClickEvent.invoke(control());
            break;
        case NM_DBLCLK:
            _dblClickEvent.invoke(control());
            break;
        case NM_RDBLCLK:
            _dblRightClickEvent.invoke(control());
            break;
        }
    }
    break;
    }
}

/// <summary>
/// The default handler for subclassed control messages.
/// </summary>
/// <param name="msg"></param>
void ControlImpl::notify(ControlMessage& msg)
{
    if (_wantKeyboardEvents)
    {
        switch (msg.wMsg)
        {
        case WM_KEYDOWN:
        {
            KeyboardEvent event;
            event.key = KeyMapper::ToKey(static_cast<UINT>(msg.wParam));
            _keyDownEvent.invoke(control(), event);
            break;
        }
        case WM_KEYUP:
        {
            KeyboardEvent event;
            event.key = KeyMapper::ToKey(static_cast<UINT>(msg.wParam));
            _keyUpEvent.invoke(control(), event);
            break;
        }
        case WM_GETDLGCODE:
            msg.result = DLGC_WANTALLKEYS | DLGC_WANTCHARS | DLGC_WANTTAB;
            return;
        }
    }

    if (_wantMouseEvents)
    {
        static const std::map<UINT, MouseButton> mouseMsgButtonMap =
        {
            { WM_LBUTTONDOWN, MouseButton::Left },
            { WM_MBUTTONDOWN, MouseButton::Middle },
            { WM_RBUTTONDOWN, MouseButton::Right },
            { WM_LBUTTONUP, MouseButton::Left },
            { WM_MBUTTONUP, MouseButton::Middle },
            { WM_RBUTTONUP, MouseButton::Right },
            { WM_LBUTTONDBLCLK, MouseButton::Left },
            { WM_MBUTTONDBLCLK, MouseButton::Middle },
            { WM_RBUTTONDBLCLK, MouseButton::Right }
        };
        switch (msg.wMsg)
        {
        case WM_MOUSEMOVE:
        {
            MouseEvent event;
            UINT keys = GET_KEYSTATE_WPARAM(msg.wParam);
            if (keys & MK_LBUTTON)
                event.button = MouseButton::Left;
            else if (keys & MK_MBUTTON)
                event.button = MouseButton::Middle;
            else if (keys & MK_RBUTTON)
                event.button = MouseButton::Right;

            event.point = Convert(_hwndParent).toUnits(Point(LOWORD(msg.lParam), HIWORD(msg.lParam)));
            _mouseMoveEvent.invoke(control(), event);
            break;
        }

        case WM_CAPTURECHANGED:
        {
            _mouseCaptureLostEvent.invoke(control());
            break;
        }

        default:
            if (mouseMsgButtonMap.find(msg.wMsg) != mouseMsgButtonMap.end())
            {
                MouseEvent event;
                event.button = mouseMsgButtonMap.at(msg.wMsg);
                event.point = Convert(_hwndParent).toUnits(Point(LOWORD(msg.lParam), HIWORD(msg.lParam)));
                switch (msg.wMsg)
                {
                case WM_LBUTTONDOWN:
                case WM_MBUTTONDOWN:
                case WM_RBUTTONDOWN:
                    _mouseDownEvent.invoke(control(), event);
                    break;
                case WM_LBUTTONUP:
                case WM_MBUTTONUP:
                case WM_RBUTTONUP:
                    _mouseUpEvent.invoke(control(), event);
                    break;
                case WM_LBUTTONDBLCLK:
                case WM_MBUTTONDBLCLK:
                case WM_RBUTTONDBLCLK:
                    _mouseDblClickEvent.invoke(control(), event);
                    break;
                }
            }
        }
    }

    if (_wantSizingEvents)
    {
        switch (msg.wMsg)
        {
        case WM_MOVE:
        {
            // translate using mapped value and store
            Point posPx((int)(short)LOWORD(msg.lParam), (int)(short)HIWORD(msg.lParam));
            Point posDu(posPx);
            Convert(_hwndParent).toUnits(posDu);

            DLGCPP_CMSG("WM_MOVE: " <<
                "x = " << posDu.x() << " (" << posPx.x() << ") " <<
                "y = " << posDu.y() << " (" << posPx.y() << ") " <<
                "text = " + _text);

            _p.x(posDu.x());
            _p.y(posDu.y());
            _moveEvent.invoke(control());
            break;
        }
        case WM_SIZE:
        {
            // translate using mapped value and store
            Size sizePx({ (int)(short)LOWORD(msg.lParam), (int)(short)HIWORD(msg.lParam) });
            Size sizeDu(sizePx);
            Convert(_hwndParent).toUnits(sizeDu);

            DLGCPP_CMSG("WM_SIZE: " <<
                "width = " << sizeDu.width() << " (" << sizePx.width() << ") " <<
                "height = " << sizeDu.height() << " (" << sizePx.height() << ") " <<
                "text = " + _text);

            _p.width(sizeDu.width());
            _p.height(sizeDu.height());
            _sizeEvent.invoke(control());
            break;
        }
        }
    }

    if (_wantPaintEvents)
    {
        switch (msg.wMsg)
        {
        case WM_ERASEBKGND:
            if (_paintEvent.count() > 0)
            {
                // drawing is handled in WM_PAINT
                msg.result = TRUE;
                return;
            }
            break;

        case WM_PAINT:
            if (_paintEvent.count() > 0)
            {
                auto ps = PAINTSTRUCT{};
                BeginPaint(_hwnd, &ps);

                auto context = std::make_shared<DrawingContext>(
                    std::make_shared<DrawingContextImpl>(_hwnd, ps.hdc));

                _paintEvent.invoke(control(), context);
                context->render();

                EndPaint(_hwnd, &ps);

                if (context->handled())
                {
                    // no further drawing
                    msg.result = 0;
                    return;
                }
                break;
            }
        }
    }

    // use default control action
    msg.result = CallWindowProc(msg.orgWndProc, _hwnd, msg.wMsg, msg.wParam, msg.lParam);
}

bool ControlImpl::enabled() const
{
    return _enabled;
}

void ControlImpl::enabled(bool value)
{
    if (_enabled == value)
        return;
    _enabled = value;
    if (_hwnd == NULL)
        return;

    EnableWindow(_hwnd, _enabled);
}


bool ControlImpl::visible() const
{
    return _visible;
}

void ControlImpl::visible(bool value)
{
    _visible = value;

    if (_hwnd == NULL)
        return;

    ShowWindow(_hwnd,
        _visible ? SW_SHOW : SW_HIDE);
}

const Position& ControlImpl::p() const
{
    return _p;
}

void ControlImpl::p(const Position& p)
{
    _p = p;

    if (_parent == nullptr)
        return;
    if (_hwnd == NULL)
        return;
    auto hwndParent = reinterpret_cast<HWND>(_parent->handle());

    // Convert units to pixels
    auto px = Convert(hwndParent).toPixels(p);

    SetWindowPos(_hwnd,
        0,
        px.x(),
        px.y(),
        px.width(),
        px.height(),
        SWP_NOZORDER);
}

void ControlImpl::move(const Point& point)
{
    _p.x(point.x());
    _p.y(point.y());

    if (_hwnd == NULL)
        return;

    auto px = Convert(_hwndParent).toPixels(_p);

    SetWindowPos(_hwnd,
        0,
        px.x(),
        px.y(),
        0,
        0,
        SWP_NOZORDER | SWP_NOSIZE);
}

void ControlImpl::resize(const Size& size)
{
    _p.width(size.width());
    _p.height(size.height());

    if (_hwnd == NULL)
        return;

    auto px = Convert(_hwndParent).toPixels(_p);

    SetWindowPos(_hwnd,
        0,
        0,
        0,
        px.width(),
        px.height(),
        SWP_NOZORDER | SWP_NOMOVE);
}

void ControlImpl::setFocus()
{
    if (_hwnd == NULL)
        return;

    SetFocus(_hwnd);
}

void ControlImpl::bringToFront()
{
    if (_hwnd == NULL)
        return;

    SetWindowPos(_hwnd,
        HWND_TOP,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);
}

void ControlImpl::sendToBack()
{
    if (_hwnd == NULL)
        return;

    SetWindowPos(_hwnd,
        HWND_BOTTOM,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);
}

BorderStyle ControlImpl::border() const
{
    return _borderStyle;
}

void ControlImpl::border(BorderStyle value)
{
    if (_borderStyle == value)
        return;
    _borderStyle = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

const std::string& ControlImpl::text() const
{
    return _text;
}

void ControlImpl::text(const std::string& value)
{
    if (_text == value)
        return;

    _text = value;
    if (_hwnd == NULL)
        return;

    // only sync the text when it has changed
    auto cbText = (size_t)GetWindowTextLengthW(_hwnd);
    if (_text.size() == cbText)
    {
        cbText++;
        std::wstring buf(cbText, 0);
        GetWindowTextW(_hwnd, &buf[0], cbText);
        if (_text == StringEncoder::toBytes(buf.c_str()))
            return;
    }

    SetWindowTextW(_hwnd,
        StringEncoder::toWide(_text).c_str());
}

std::pair<Color, Color> ControlImpl::colors() const
{
    return std::make_pair(_fgColor, _bgColor);
}

void ControlImpl::colors(Color fgColor, Color bgColor)
{
    bool changed = false;

    // foreground 'None' is not supported
    if (fgColor != Color::None &&
        fgColor != _fgColor)
    {
        _fgColor = fgColor;
        changed = true;
    }
    if (bgColor != _bgColor)
    {
        _bgColor = bgColor;
        changed = true;
    }

    if (!changed)
        return;

    if (_hbrBack != NULL)
    {
        DeleteObject(_hbrBack);
        _hbrBack = NULL;
    }

    if (bgColor != Color::None && bgColor != Color::Default)
        _hbrBack = CreateSolidBrush((COLORREF)bgColor);
    redraw();
}

const Font& ControlImpl::font() const
{
    return _font;
}

void ControlImpl::font(const Font& value)
{
    _font = value;

    if (_hFont != NULL)
        DeleteObject(_hFont);

    if (!_font.family.empty())
        _hFont = FontLoader::toGdiFont(_font);
    else
        _hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    if (_hwnd == NULL)
        return;

    SendMessage(_hwnd, WM_SETFONT, (WPARAM)_hFont, TRUE);
}

Cursor ControlImpl::cursor() const
{
    return _cursor;
}

void ControlImpl::cursor(Cursor value)
{
    _cursor = value;
}

bool ControlImpl::mouseCapture() const
{
    return (_hwnd != NULL && GetCapture() == _hwnd);
}

void ControlImpl::mouseCapture(bool value)
{
    if (_hwnd == NULL)
        return;

    if (value)
        SetCapture(_hwnd);
    else
    {
        if (GetCapture() == _hwnd)
            ReleaseCapture();
    }
}

/// <summary>
/// Checks whether the control receives internal events (messages).
/// </summary>
bool ControlImpl::wantInternalEvents() const
{
    return _wantInternalEvents;
}

/// <summary>
/// Enables the control to receive internal event messages through subclassing.
/// Note: notify(ctl_msg) may still be called if subclass is enabled elsewhere.
/// </summary>
void ControlImpl::wantInternalEvents(bool value)
{
    if (_wantInternalEvents == value)
        return;
    _wantInternalEvents = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

/// <summary>
/// Checks whether the control receives all keyboard events.
/// </summary>
bool ControlImpl::wantKeyboardEvents() const
{
    return _wantKeyboardEvents;
}

/// <summary>
/// Enables the control to receive all keyboard events.
/// </summary>
void ControlImpl::wantKeyboardEvents(bool value)
{
    if (_wantKeyboardEvents == value)
        return;
    _wantKeyboardEvents = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

/// <summary>
/// Checks whether the control receives all mouse events.
/// </summary>
bool ControlImpl::wantMouseEvents() const
{
    return _wantMouseEvents;
}

/// <summary>
/// Enables the control to receive all mouse events.
/// </summary>
void ControlImpl::wantMouseEvents(bool value)
{
    if (_wantMouseEvents == value)
        return;
    _wantMouseEvents = value;

    if (handle() == nullptr)
        return;
    rebuild();
}


/// <summary>
/// Checks whether the control receives paint events.
/// </summary>
bool ControlImpl::wantPaintEvents() const
{
    return _wantPaintEvents;
}

/// <summary>
/// Enables the control to receive paint events.
/// </summary>
void ControlImpl::wantPaintEvents(bool value)
{
    if (_wantPaintEvents == value)
        return;
    _wantPaintEvents = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

/// <summary>
/// Checks whether the control receives move/size events.
/// </summary>
bool ControlImpl::wantSizingEvents() const
{
    return _wantSizingEvents;
}

/// <summary>
/// Enables the control to receive move/size events.
/// </summary>
/// <param name="value"></param>
void ControlImpl::wantSizingEvents(bool value)
{
    if (_wantSizingEvents == value)
        return;
    _wantSizingEvents = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

void* ControlImpl::handle() const
{
    return _hwnd;
}

bool ControlImpl::isHandleEqual(void* otherHandle) const
{
    auto hwndOther = reinterpret_cast<HWND>(otherHandle);
    return (hwndOther == _hwnd);
}

void* ControlImpl::user() const
{
    return _user;
}

void ControlImpl::user(void* value)
{
    _user = value;
}

ISharedControl ControlImpl::control()
{
    return _control->shared_from_this();
}

std::string ControlImpl::className() const
{
    return "STATIC";
}

unsigned int ControlImpl::styles() const
{
    unsigned int styles = WS_CHILD | WS_TABSTOP | WS_CLIPSIBLINGS;
    if (_visible)
        styles |= WS_VISIBLE;
    if (!_enabled)
        styles |= WS_DISABLED;
    return styles;
}

unsigned int ControlImpl::exStyles() const
{
    unsigned int styles = 0;

    switch (_borderStyle)
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

void ControlImpl::redraw()
{
    if (_hwnd == NULL)
        return;

    RedrawWindow(_hwnd, NULL, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

void ControlImpl::rebuild()
{
    destruct();

    // safety checks
    if (_parent == nullptr ||
        _id < 1)

    {
        DLGCPP_CERR("Called when no parent or control id is assigned.");
        return;
    }

    HWND hwndParent = reinterpret_cast<HWND>(_parent->handle());
    if (hwndParent == NULL)
        return;

    auto p = Convert(hwndParent).toPixels(_p);

    auto hwnd = CreateWindowExW(exStyles(),
        StringEncoder::toWide(className()).c_str(),
        StringEncoder::toWide(_text).c_str(),
        styles(),
        p.x(),
        p.y(),
        p.width(),
        p.height(),
        hwndParent,
        (HMENU)(UINT_PTR)_id,
        GetModuleHandle(NULL), NULL);
    if (hwnd == NULL)
        return;

    _hwnd = hwnd;
    _hwndParent = hwndParent;

    if (mustBeSubclassed())
    {
        SetPropW(_hwnd, L"this", this);
        _prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)controlWndProc);
        SetPropW(_hwnd, L"pproc", (HANDLE)_prevWndProc);
    }

    SendMessage(_hwnd, WM_SETFONT, (WPARAM)_hFont, FALSE);
}

/// <summary>
/// Determines if the control needs to be subclassed.
/// </summary>
bool ControlImpl::mustBeSubclassed() const
{
    bool subclass =
        _wantKeyboardEvents ||
        _wantMouseEvents ||
        _wantPaintEvents ||
        _wantSizingEvents ||
        _wantInternalEvents;

    return subclass;
}

void ControlImpl::destruct()
{
    if (_hwnd == NULL)
        return;

    if (_prevWndProc != NULL)
    {
        // remove subclass
        SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)_prevWndProc);
        SetPropW(_hwnd, L"pproc", (HANDLE)NULL);
    }

    DestroyWindow(_hwnd);
    _hwnd = 0;
    _hwndParent = 0;
}

LRESULT CALLBACK ControlImpl::controlWndProc(HWND hwnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    auto pthis = reinterpret_cast<ControlImpl*>(GetPropW(hwnd, L"this"));
    auto pproc = reinterpret_cast<WNDPROC>(GetPropW(hwnd, L"pproc"));

    if (pthis != nullptr)
    {
        // wrap and transfer the message directly to the class.
        // note: all derivatives will need to process the message and use CallWindowProc
        auto msg = ControlMessage{ wMsg, wParam, lParam, 0, pproc };
        pthis->notify(msg);
        return msg.result;
    }
    return CallWindowProc(pproc, hwnd, wMsg, wParam, lParam);
}

IEvent<ISharedControl>& ControlImpl::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& ControlImpl::RightClickEvent()
{
    return _rightClickEvent;
}

IEvent<ISharedControl>& ControlImpl::DoubleClickEvent()
{
    return _dblClickEvent;
}

IEvent<ISharedControl>& ControlImpl::DoubleRightClickEvent()
{
    return _dblRightClickEvent;
}

IEvent<ISharedControl, bool>& ControlImpl::FocusEvent()
{
    return _focusEvent;
}

IEvent<ISharedControl, KeyboardEvent>& ControlImpl::KeyDownEvent()
{
    return _keyDownEvent;
}

IEvent<ISharedControl, KeyboardEvent>& ControlImpl::KeyUpEvent()
{
    return _keyUpEvent;
}

IEvent<ISharedControl, MouseEvent>& ControlImpl::MouseDownEvent()
{
    return _mouseDownEvent;
}

IEvent<ISharedControl, MouseEvent>& ControlImpl::MouseUpEvent()
{
    return _mouseUpEvent;
}

IEvent<ISharedControl, MouseEvent>& ControlImpl::MouseMoveEvent()
{
    return _mouseMoveEvent;
}

IEvent<ISharedControl, MouseEvent>& ControlImpl::MouseDoubleClickEvent()
{
    return _mouseDblClickEvent;
}

IEvent<ISharedControl>& ControlImpl::MouseCaptureLostEvent()
{
    return _mouseCaptureLostEvent;
}

IEvent<ISharedControl>& ControlImpl::MoveEvent()
{
    return _moveEvent;
}

IEvent<ISharedControl>& ControlImpl::SizeEvent()
{
    return _sizeEvent;
}

IEvent<ISharedControl, ISharedDrawingContext>& ControlImpl::PaintEvent()
{
    return _paintEvent;
}

IEvent<int>& ControlImpl::UserEvent()
{
    return _userEvent;
}
