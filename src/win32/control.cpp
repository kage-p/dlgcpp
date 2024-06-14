#include "control_p.h"
#include "utility.h"
#include "dlgcpp/dialog.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

LRESULT CALLBACK controlWndProc(HWND hwnd, UINT wMsg, WPARAM wParam, LPARAM lParam);

using namespace dlgcpp;


Control::Control(const std::string& text, const Position& p) :
    _props(new ctl_props()),
    _state(new ctl_state())
{
    _props->p = p;
    _props->text = text;
    _state->hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    // TODO: translate and store system font
    //auto lfw = LOGFONTW();
    //GetObject(_state->font, sizeof(LOGFONTW), &lfw);
    //_props->font = Font{};
}

Control::~Control()
{
    destruct();

    if (_state->hFont != nullptr)
        DeleteObject(_state->hFont);
    if (_state->hbrBack != NULL)
        DeleteObject(_state->hbrBack);

    delete _props;
    delete _state;
}

ISharedDialog Control::parent() const
{
    return _props->parent;
}

void Control::parent(ISharedDialog parent)
{
    if (_props->parent == parent)
        return;
    _props->parent = parent;
}

int Control::id() const
{
    return _props->id;
}

void Control::id(int value)
{
    _props->id = value;
}

std::shared_ptr<IControl> Control::control()
{
    return shared_from_this();
}

ctl_state Control::state()
{
    return *_state;
}

void Control::notify(dlg_message&)
{
    // no default action
}

void Control::notify(ctl_message& msg)
{
    auto wMsg = msg.wMsg;
    auto wParam = msg.wParam;
    auto lParam = msg.lParam;

    switch (wMsg)
    {
    case WM_MOVE:
    {
        // translate using mapped value and store
        Point posPx((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
        Point posDu(posPx);
        toUnits(GetParent(_state->hwnd), posDu);

        DLGCPP_CMSG("WM_MOVE: " <<
                    "x = "  << posDu.x() << " (" << posPx.x() << ") " <<
                    "y = " << posDu.y() << " (" << posPx.y() << ") " <<
                    "text = " + _props->text);

        _props->p.x(posDu.x());
        _props->p.y(posDu.y());
        MoveEvent().invoke(shared_from_this());
        break;
    }
    case WM_SIZE:
    {
        // translate using mapped value and store
        Size sizePx({(int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)});
        Size sizeDu(sizePx);
        toUnits(GetParent(_state->hwnd), sizeDu);

        DLGCPP_CMSG("WM_SIZE: " <<
                    "width = "  << sizeDu.width() << " (" << sizePx.width() << ") " <<
                    "height = " << sizeDu.height() << " (" << sizePx.height() << ") " <<
                    "text = " + _props->text);

        _props->p.width(sizeDu.width());
        _props->p.height(sizeDu.height());
        SizeEvent().invoke(shared_from_this());
        break;
    }
    }

    // use default control action
    msg.result = CallWindowProc(msg.orgWndProc, _state->hwnd, msg.wMsg, msg.wParam, msg.lParam);
}

bool Control::enabled() const
{
    return _props->enabled;
}

void Control::enabled(bool value)
{
    if (_props->enabled == value)
        return;
    _props->enabled = value;
    if (_state->hwnd == NULL)
        return;

    EnableWindow(_state->hwnd, _props->enabled);
}


bool Control::visible() const
{
    return _props->visible;
}

void Control::visible(bool value)
{
    _props->visible = value;

    if (_state->hwnd == NULL)
        return;

    ShowWindow(_state->hwnd,
               _props->visible ? SW_SHOW : SW_HIDE);
}

const Position& Control::p() const
{
    return _props->p;
}

void Control::p(const Position& p)
{
    _props->p = p;

    if (_props->parent == nullptr)
        return;
    if (_state->hwnd == NULL)
        return;
    auto hwndParent = reinterpret_cast<HWND>(_props->parent->handle());

    // Convert units to pixels
    auto px = toPixels(hwndParent, p);

    SetWindowPos(_state->hwnd,
                 0,
                 px.x(),
                 px.y(),
                 px.width(),
                 px.height(),
                 SWP_NOZORDER);
}

void Control::move(const Point& point)
{
    _props->p.x(point.x());
    _props->p.y(point.y());

    if (_state->hwnd == NULL)
        return;

    auto px = toPixels(GetParent(_state->hwnd), _props->p);

    SetWindowPos(_state->hwnd,
                 0,
                 px.x(),
                 px.y(),
                 0,
                 0,
                 SWP_NOZORDER | SWP_NOSIZE);
}

void Control::resize(const Size& size)
{
    _props->p.width(size.width());
    _props->p.height(size.height());

    if (_state->hwnd == NULL)
        return;

    auto px = toPixels(GetParent(_state->hwnd), _props->p);

    SetWindowPos(_state->hwnd,
                 0,
                 0,
                 0,
                 px.width(),
                 px.height(),
                 SWP_NOZORDER | SWP_NOMOVE);
}

BorderStyle Control::border() const
{
    return _props->borderStyle;
}

void Control::border(BorderStyle value)
{
    if (_props->borderStyle == value)
        return;
    _props->borderStyle = value;

    rebuild();
}

const std::string& Control::text() const
{
    return _props->text;
}

void Control::text(const std::string& value)
{
    if (_props->text == value)
        return;

    _props->text = value;
    if (_state->hwnd == NULL)
        return;

    // only sync the text when it has changed
    auto cbText = (size_t)GetWindowTextLengthW(_state->hwnd);
    if (_props->text.size() == cbText)
    {
        cbText++;
        std::wstring buf(cbText, 0);
        GetWindowTextW(_state->hwnd, &buf[0], cbText);
        if (_props->text == toBytes(buf.c_str()))
            return;
    }

    SetWindowTextW(_state->hwnd,
                   toWide(_props->text).c_str());
}

std::pair<Color, Color> Control::colors() const
{
    return std::make_pair(_props->fgColor, _props->bgColor);
}

void Control::colors(Color fgColor, Color bgColor)
{
    bool changed = false;

    // foreground 'None' is not supported
    if (fgColor != Color::None &&
        _props->fgColor != fgColor)
    {
        _props->fgColor = fgColor;
        changed = true;
    }
    if (bgColor != fgColor)
    {
        _props->bgColor = bgColor;
        changed = true;
    }

    if (!changed)
        return;

    if (_state->hbrBack != NULL)
    {
        DeleteObject(_state->hbrBack);
        _state->hbrBack = NULL;
    }

    if (bgColor != Color::None && bgColor != Color::Default)
        _state->hbrBack = CreateSolidBrush((COLORREF)bgColor);
    redraw();
}

Cursor Control::cursor() const
{
    return _props->cursor;
}

void Control::cursor(Cursor value)
{
    _props->cursor = value;
}

const Font& Control::font() const
{
    return _props->font;
}

void Control::font(const Font& value)
{
    _props->font = value;

    if (_state->hFont != NULL)
        DeleteObject(_state->hFont);

    if (!_props->font.faceName.empty())
        _state->hFont = makeFont(_props->font);
    else
        _state->hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    if (_state->hwnd == NULL)
        return;

    SendMessage(_state->hwnd, WM_SETFONT, (WPARAM)_state->hFont, TRUE);
}

void* Control::handle() const
{
    return _state->hwnd;
}

void* Control::user() const
{
    return _props->user;
}

void Control::user(void* value)
{
    _props->user = value;
}

std::string Control::className() const
{
    return "STATIC";
}

unsigned int Control::styles() const
{
    unsigned int styles = WS_CHILD | WS_TABSTOP | WS_CLIPSIBLINGS;
    if (_props->visible)
        styles |= WS_VISIBLE;
    if (!_props->enabled)
        styles |= WS_DISABLED;
    return styles;
}

unsigned int Control::exStyles() const
{
    unsigned int styles = 0;

    switch (_props->borderStyle)
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
    if (_state->hwnd == NULL)
        return;

    RedrawWindow(_state->hwnd, NULL, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
}

void Control::setFocus()
{
    if (_state->hwnd == NULL)
        return;

    SetFocus(_state->hwnd);
}

void Control::rebuild()
{
    destruct();

    // safety checks
    if (_props->parent == nullptr)
        return;

    if (_props->id < 1)
        return;

    HWND hwndParent = reinterpret_cast<HWND>(_props->parent->handle());
    if (hwndParent == NULL)
        return;

    auto p = toPixels(hwndParent, _props->p);

    auto hwnd = CreateWindowExW(exStyles(),
                                toWide(className()).c_str(),
                                toWide(_props->text).c_str(),
                                styles(),
                                p.x(),
                                p.y(),
                                p.width(),
                                p.height(),
                                hwndParent,
                                (HMENU)(UINT_PTR)_props->id,
                                GetModuleHandle(NULL), NULL);
    if (hwnd == NULL)
        return;

    _state->hwnd = hwnd;

    if (_props->subclass)
    {
        SetProp(_state->hwnd, "this", this);
        _state->prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)controlWndProc);
        SetProp(_state->hwnd, "pproc", (HANDLE)_state->prevWndProc);
    }

    SendMessage(_state->hwnd, WM_SETFONT, (WPARAM)_state->hFont, FALSE);
}

void Control::destruct()
{
    if (_state->hwnd == NULL)
        return;

    if (_props->subclass)
    {
        // remove subclass
        SetWindowLongPtr(_state->hwnd, GWLP_WNDPROC, (LONG_PTR)_state->prevWndProc);
        SetProp(_state->hwnd, "pproc", (HANDLE)NULL);
    }

    DestroyWindow(_state->hwnd);
    _state->hwnd = nullptr;
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
    return _props->clickEvent;
}

IEvent<ISharedControl>& Control::DoubleClickEvent()
{
    return _props->dblClickEvent;
}

IEvent<ISharedControl, bool>& Control::FocusEvent()
{
    return _props->focusEvent;
}

IEvent<ISharedControl>& Control::MoveEvent()
{
    return _props->moveEvent;
}

IEvent<ISharedControl>& Control::SizeEvent()
{
    return _props->sizeEvent;
}
