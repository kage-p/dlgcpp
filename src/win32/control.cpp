#include "control_p.h"
#include "utility.h"
#include "dlgcpp/dialog.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using namespace dlgcpp;


Control::Control(std::shared_ptr<IDialog> parent) :
    _props(new ctl_props()),
    _state(new ctl_state()),
    _parent(parent)
{
    _props->_p._cx = 0;
    _props->_p._cy = 0;
    _state->hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    // TODO: translate and store system font
    //auto lfw = LOGFONTW();
    //GetObject(_state->_font, sizeof(LOGFONTW), &lfw);
    //_props->font = Font{};
}

Control::~Control()
{
    dump();

    if (_state->hFont != nullptr)
        DeleteObject(_state->hFont);
    if (_state->hbrBack != NULL)
        DeleteObject(_state->hbrBack);

    delete _props;
    delete _state;
}

std::shared_ptr<Control> Control::shared_ptr()
{
    try
    {
        return shared_from_this();
    }
    catch (const std::exception&)
    {
        return nullptr;
    }
}

int Control::id() const
{
    return _props->_id;
}

void Control::id(int value)
{
    _props->_id = value;
    rebuild();
}

std::shared_ptr<IControl> Control::control()
{
    return shared_from_this();
}

ctl_state Control::state()
{
    return *_state;
}

bool Control::enabled() const
{
    return _props->_enabled;
}

void Control::enabled(bool value)
{
    if (_props->_enabled == value)
        return;
    _props->_enabled = value;
    if (_state->_hwnd == NULL)
        return;

    EnableWindow(_state->_hwnd, _props->_enabled);
}


bool Control::visible() const
{
    return _props->_visible;
}

void Control::visible(bool value)
{
    _props->_visible = value;

    if (_state->_hwnd == NULL)
        return;

    ShowWindow(_state->_hwnd,
               _props->_visible ? SW_SHOW : SW_HIDE);
}

const Position& Control::p() const
{
    //if (_state->_hwnd == NULL)
        return _props->_p;

    // auto hwndParent = reinterpret_cast<HWND>(_parent->handle());

    // auto rc = RECT();
    // GetWindowRect(_state->_hwnd, &rc);
    // MapWindowPoints(HWND_DESKTOP, hwndParent, (LPPOINT)&rc, 2);
    // MapDialogRect(hwndParent, &rc);

    // return Position{rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top};
}

void Control::p(const Position& p)
{
    _props->_p = p;

    if (_state->_hwnd == NULL)
        return;

    auto hwndParent = reinterpret_cast<HWND>(_parent->handle());

    // Convert units to pixels
    auto rc = RECT();
    SetRect(&rc, p._x, p._y, p._cx, p._cy);
    MapDialogRect(hwndParent, &rc);

    SetWindowPos(_state->_hwnd, 0, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER);
}

const std::string& Control::text() const
{
    return _props->_text;
}

void Control::text(const std::string& value)
{
    if (_props->_text == value)
        return;
    _props->_text = value;
    if (_state->_hwnd == NULL)
        return;

    SetWindowTextW(_state->_hwnd,
                   toWide(_props->_text).c_str());
}

std::pair<Color, Color> Control::colors() const
{
    return std::make_pair(_props->_fgColor, _props->_bgColor);
}

void Control::colors(Color fgColor, Color bgColor)
{
    _props->_fgColor = fgColor;
    _props->_bgColor = bgColor;

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
    return _props->_cursor;
}

void Control::cursor(Cursor value)
{
    _props->_cursor = value;
}

const Font& Control::font() const
{
    return _props->_font;
}

void Control::font(const Font& value)
{
    _props->_font = value;

    if (_state->hFont != NULL)
        DeleteObject(_state->hFont);

    if (!_props->_font.faceName.empty())
        _state->hFont = makeFont(_props->_font);
    else
        _state->hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    if (_state->_hwnd == NULL)
        return;

    SendMessage(_state->_hwnd, WM_SETFONT, (WPARAM)_state->hFont, TRUE);
}

void* Control::handle() const
{
    return _state->_hwnd;
}

void* Control::user() const
{
    return _props->_user;
}

void Control::user(void* value)
{
    _props->_user = value;
}

std::shared_ptr<IDialog> Control::parent()
{
    return _parent;
}

long long Control::sendMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (_state->_hwnd == NULL)
        return 0;
    return SendMessage(_state->_hwnd, msg, wParam, lParam);
}

std::string Control::className() const
{
    return "STATIC";
}

unsigned int Control::styles() const
{
    unsigned int styles = WS_CHILD | WS_TABSTOP | WS_CLIPSIBLINGS;
    if (_props->_visible)
        styles |= WS_VISIBLE;
    if (!_props->_enabled)
        styles |= WS_DISABLED;
    return styles;
}

unsigned int Control::exStyles() const
{
    return 0;
}

void Control::redraw()
{
    if (_state->_hwnd == NULL)
        return;

    RedrawWindow(_state->_hwnd, NULL, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
}

void Control::setFocus()
{
    if (_state->_hwnd == NULL)
        return;

    SetFocus(_state->_hwnd);
}

void Control::rebuild()
{
    dump();

    // safety checks
    if (_props->_id < 1)
        return;

    HWND hwndParent = reinterpret_cast<HWND>(_parent->handle());
    if (hwndParent == NULL)
        return;

    auto rc = RECT();
    SetRect(&rc,
            _props->_p._x,
            _props->_p._y,
            _props->_p._cx,
            _props->_p._cy);
    MapDialogRect(hwndParent, &rc);

    auto hwnd = CreateWindowExW(exStyles(),
                                toWide(className()).c_str(),
                                toWide(_props->_text).c_str(),
                                styles(),
                                rc.left,
                                rc.top,
                                rc.right,
                                rc.bottom,
                                hwndParent,
                                (HMENU)(UINT_PTR)_props->_id,
                                GetModuleHandle(NULL), NULL);
    if (hwnd == NULL)
        return;

    SendMessage(hwnd, WM_SETFONT, (WPARAM)_state->hFont, FALSE);
    _state->_hwnd = hwnd;
}

void Control::dump()
{
    if (_state->_hwnd == NULL)
        return;

    DestroyWindow(_state->_hwnd);
    _state->_hwnd = nullptr;
}

IEvent& Control::CommandEvent()
{
    return _commandEvent;
}
