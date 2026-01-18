#include "control_impl.h"
#include "dlgcpp/dialogs/dialog.h"
#include "gfx/context_impl.h"
#include "utility/convert.h"
#include "utility/font_loader.h"
#include "utility/key_mapper.h"
#include "utility/message.h"
#include "utility/string_encoder.h"
#include <map>

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::gfx;

ControlImpl::ControlImpl()
{
    // use system default font
    _hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    _handle.reset(nullptr, nullptr, []() { return 0; }, "handle");
}

ControlImpl::~ControlImpl()
{
    destruct();

    if (_hFont != nullptr)
        DeleteObject(_hFont);
    if (_hbrBack != NULL)
        DeleteObject(_hbrBack);
}

void ControlImpl::owner(IControl* control)
{
    // connect properties to internal event handlers
    _control = control;
    _control->enabled().event() += [&](auto) { onEnabledChanged(); };
    _control->visible().event() += [&](auto) { onVisibleChanged(); };
    _control->p().event() += [&](auto) { onPosChanged(); };
    _control->border().event() += [&](auto) { rebuild(); };
    _control->colors().event() += [&](auto) { onColorsChanged(); };
    _control->font().event() += [&](auto) { onFontChanged(); };
    _control->focus().event() += [&](auto) { onFocusChanged(); };
    _control->mouseCapture().event() += [&](auto) { onMouseCaptureChanged(); };

    _control->wantKeyboardEvents().event() += [&](auto) { rebuild(); };
    _control->wantMouseEvents().event() += [&](auto) { rebuild(); };
    _control->wantPaintEvents().event() += [&](auto) { rebuild(); };
    _control->wantSizingEvents().event() += [&](auto) { rebuild(); };
}

IReadOnlyProperty<void*, int>& ControlImpl::handle()
{
    return _handle;
}

HWND ControlImpl::hwnd() const
{
    return _hwnd;
}

bool ControlImpl::isHandleEqual(void* otherHandle) const
{
    auto hwndOther = reinterpret_cast<HWND>(otherHandle);
    return (hwndOther == _hwnd);
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

void ControlImpl::notify(DialogMessage&)
{
}

/// <summary>
/// The default handler for subclassed control messages.
/// </summary>
/// <param name="msg"></param>
void ControlImpl::notify(ControlMessage& msg)
{
    if (_control->wantKeyboardEvents())
    {
        switch (msg.wMsg)
        {
        case WM_KEYDOWN:
        {
            KeyboardEvent event;
            event.key = KeyMapper::ToKey(static_cast<UINT>(msg.wParam));
            _control->KeyDownEvent().invoke(event);
            break;
        }
        case WM_KEYUP:
        {
            KeyboardEvent event;
            event.key = KeyMapper::ToKey(static_cast<UINT>(msg.wParam));
            _control->KeyUpEvent().invoke(event);
            break;
        }
        case WM_GETDLGCODE:
            msg.result = DLGC_WANTALLKEYS | DLGC_WANTCHARS | DLGC_WANTTAB;
            return;
        }
    }

    if (_control->wantMouseEvents())
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

            if (msg.wParam & MK_LBUTTON)
                event.button = MouseButton::Left;
            else if (msg.wParam & MK_RBUTTON)
                event.button = MouseButton::Right;
            else if (msg.wParam & MK_MBUTTON)
                event.button = MouseButton::Middle;
            else
                event.button = MouseButton::None;

            event.point = Convert(_hwndParent).toUnits(Point(LOWORD(msg.lParam), HIWORD(msg.lParam)));
            _control->MouseMoveEvent().invoke(event);
            break;
        }

        case WM_CAPTURECHANGED:
        {
            // lost capture
            _control->mouseCapture() = false;
            _control->MouseCaptureLostEvent().invoke();
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
                    _control->MouseDownEvent().invoke(event);
                    break;
                case WM_LBUTTONUP:
                case WM_MBUTTONUP:
                case WM_RBUTTONUP:
                    _control->MouseUpEvent().invoke(event);
                    break;
                case WM_LBUTTONDBLCLK:
                case WM_MBUTTONDBLCLK:
                case WM_RBUTTONDBLCLK:
                    _control->MouseDoubleClickEvent().invoke(event);
                    break;
                }
            }
        }
    }

    if (_control->wantSizingEvents())
    {
        switch (msg.wMsg)
        {
        case WM_MOVE:
        {
            // translate using mapped value and store
            Point posPx((int)(short)LOWORD(msg.lParam), (int)(short)HIWORD(msg.lParam));
            Point posDu = Convert(_hwndParent).toUnits(posPx);

            DLGCPP_CMSG("WM_MOVE: " <<
                "x = " << posDu.x() << " (" << posPx.x() << ") " <<
                "y = " << posDu.y() << " (" << posPx.y() << ") " <<
                "id = " + _id);

            const auto& curPos = _control->p().value();
            _control->p() = Position(posDu.x(), posDu.y(), curPos.width(), curPos.height());
            _control->MoveEvent().invoke();
            break;
        }
        case WM_SIZE:
        {
            // translate using mapped value and store
            Size sizePx({ (int)(short)LOWORD(msg.lParam), (int)(short)HIWORD(msg.lParam) });
            Size sizeDu = Convert(_hwndParent).toUnits(sizePx);

            DLGCPP_CMSG("WM_SIZE: " <<
                "width = " << sizeDu.width() << " (" << sizePx.width() << ") " <<
                "height = " << sizeDu.height() << " (" << sizePx.height() << ") " <<
                "id = " + _id);

            const auto& curPos = _control->p().value();
            _control->p() = Position(curPos.x(), curPos.y(), sizeDu.width(), sizeDu.height());
            _control->SizeEvent().invoke();
            break;
        }
        }
    }

    if (_control->wantPaintEvents())
    {
        switch (msg.wMsg)
        {
        case WM_ERASEBKGND:
            if (_control->PaintEvent().count() > 0)
            {
                // drawing is handled in WM_PAINT
                msg.result = TRUE;
                return;
            }
            break;

        case WM_PAINT:
            if (_control->PaintEvent().count() > 0)
            {
                auto ps = PAINTSTRUCT{};
                BeginPaint(_hwnd, &ps);

                auto context = std::make_shared<DrawingContext>(
                    std::make_shared<DrawingContextImpl>(_hwnd, ps.hdc));

                _control->PaintEvent().invoke(context);
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

void ControlImpl::onEnabledChanged()
{
    if (_hwnd == NULL)
        return;
    EnableWindow(_hwnd, _control->enabled().value());
}

void ControlImpl::onVisibleChanged()
{
    if (_hwnd == NULL)
        return;

    ShowWindow(_hwnd,
        _control->visible() ? SW_SHOW : SW_HIDE);
}

void ControlImpl::onPosChanged()
{
    if (_hwnd == NULL)
        return;

    auto hwndParent = GetParent(_hwnd);
    if (hwndParent == NULL)
        return;

    // Convert units to pixels
    auto px = Convert(hwndParent).toPixels(_control->p());

    SetWindowPos(_hwnd,
        0,
        px.x(),
        px.y(),
        px.width(),
        px.height(),
        SWP_NOZORDER);
}

void ControlImpl::onFocusChanged()
{
    if (_hwnd == NULL)
    {
        // no focus; control not active
        _control->focus() = false;
        return;
    }

    if (_control->focus())
    {
        // set focus
        SetFocus(_hwnd);
    }
    else
    {
        // unset focus
        if (GetFocus() == _hwnd)
            SetFocus((HWND)0);
    }
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

std::string ControlImpl::getText(HWND hwnd) const
{
    if (hwnd == NULL)
        return {};

    int len = GetWindowTextLengthW(hwnd);
    if (len < 1)
        return {};

    std::wstring buf(len, L'\0');
    GetWindowTextW(_hwnd, buf.data(), len + 1);
    return StringEncoder::toBytes(buf.c_str());
}

void ControlImpl::setText(const std::string& text)
{
    if (_hwnd == NULL)
        return;

    auto newText = StringEncoder::toWide(text);

    // only sync the text when it has changed
    auto len = (size_t)GetWindowTextLengthW(_hwnd);
    if (newText.size() == len)
    {
        std::wstring buf(len, L'\0');
        GetWindowTextW(_hwnd, buf.data(), len + 1);
        if (newText == buf)
            return;
    }

    SetWindowTextW(_hwnd, newText.c_str());
}

void ControlImpl::onColorsChanged()
{
    bool changed = false;

    if (_hbrBack != NULL)
    {
        DeleteObject(_hbrBack);
        _hbrBack = NULL;
    }

    auto bgColor = _control->colors()->second;

    if (bgColor != Color::None && bgColor != Color::Default)
    {
        _hbrBack = CreateSolidBrush((COLORREF)bgColor);
    }

    redraw();
}

void ControlImpl::onFontChanged()
{
    if (_hFont != NULL)
        DeleteObject(_hFont);

    if (!_control->font()->family().empty())
        _hFont = FontLoader::toGdiFont(_control->font());
    else
        _hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    if (_hwnd == NULL)
        return;

    SendMessage(_hwnd, WM_SETFONT, (WPARAM)_hFont, TRUE);
}

void ControlImpl::onMouseCaptureChanged()
{
    if (_hwnd == NULL)
    {
        // no control active; abort and reset property
        _control->mouseCapture() = false;
        return;
    }

    if (_control->mouseCapture())
    {
        // either set by us or by user

        // already captured
        if (GetCapture() == _hwnd)
        {
            return;
        }

        // try and set capture
        SetCapture(_hwnd);
        if (GetCapture() != _hwnd)
        {
            _control->mouseCapture() = false;
            return;
        }
    }
    else
    {
        if (GetCapture() == _hwnd)
        {
            ReleaseCapture();
        }
    }
}

/// <summary>
/// This enables the control to receive internal event messages through subclassing.
/// Note: notify(ctl_msg) may still be called if subclass is enabled elsewhere.
//
/// Can be overridden by derived class
/// </summary>
bool ControlImpl::wantInternalEvents() const
{
    return false;
}

std::string ControlImpl::className() const
{
    return "STATIC";
}

unsigned int ControlImpl::styles() const
{
    unsigned int styles = WS_CHILD | WS_TABSTOP | WS_CLIPSIBLINGS;
    if (_control->visible())
        styles |= WS_VISIBLE;
    if (!_control->enabled())
        styles |= WS_DISABLED;
    return styles;
}

unsigned int ControlImpl::exStyles() const
{
    unsigned int styles = 0;

    switch (_control->border())
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

/// <summary>
/// Creates the control
/// Does nothing if the control is already created; use rebuild() instead.
/// </summary>
void ControlImpl::build()
{
    if (_hwnd != NULL)
        return;

    // safety checks
    if (_control->parent().expired() ||
        _id < 1)
    {
        DLGCPP_CERR("Called when no parent or control id is assigned.");
        return;
    }

    auto parent = _control->parent().lock();
    HWND hwndParent = reinterpret_cast<HWND>(parent->handle().value());
    if (hwndParent == NULL)
    {
        DLGCPP_CERR("Control parent not built.");
        return;
    }

    auto p = Convert(hwndParent).toPixels(_control->p());

    auto hwnd = CreateWindowExW(exStyles(),
        StringEncoder::toWide(className()).c_str(),
        NULL,
        styles(),
        p.x(),
        p.y(),
        p.width(),
        p.height(),
        hwndParent,
        (HMENU)(UINT_PTR)_id,
        GetModuleHandle(NULL), NULL);

    if (hwnd == NULL)
    {
        DLGCPP_CERR("Control build failed. Error " << GetLastError());
        return;
    }

    _hwnd = hwnd;
    _hwndParent = hwndParent;

    if (mustBeSubclassed())
    {
        SetPropW(_hwnd, L"this", this);
        _prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)controlWndProc);
        SetPropW(_hwnd, L"pproc", (HANDLE)_prevWndProc);
    }

    SendMessage(_hwnd, WM_SETFONT, (WPARAM)_hFont, FALSE);

    _handle = (void*)_hwnd;

    _control->CreateEvent().invoke();
}

/// <summary>
/// Destroys and rebuilds (recreates) the control.
/// Does nothing if the control was not already built.
/// </summary>
void ControlImpl::rebuild()
{
    if (_hwnd == NULL)
        return;

    destruct();
    build();
}

/// <summary>
/// Determines if the control needs to be subclassed.
/// </summary>
bool ControlImpl::mustBeSubclassed() const
{
    bool subclass =
        _control->wantKeyboardEvents() ||
        _control->wantMouseEvents() ||
        _control->wantPaintEvents() ||
        _control->wantSizingEvents() ||
        wantInternalEvents();

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
    _handle.clear();

    _control->DestroyEvent().invoke();
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
