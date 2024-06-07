#include "control_p.h"
#include "utility.h"
#include "dlgcpp/dialog.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using namespace dlgcpp;


Control::Control() :
    _props(new ctl_props()),
    _state(new ctl_state())
{
    _state->hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    // TODO: translate and store system font
    //auto lfw = LOGFONTW();
    //GetObject(_state->font, sizeof(LOGFONTW), &lfw);
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

std::shared_ptr<IDialog> Control::parent() const
{
    return _props->parent;
}

void Control::parent(std::shared_ptr<IDialog> parent)
{
    if (_props->parent == parent)
        return;
    _props->parent = parent;
    rebuild();
}

int Control::id() const
{
    return _props->id;
}

void Control::id(int value)
{
    _props->id = value;
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

void Control::notify(struct dlgcpp::dlg_message&)
{
    // no default action
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
    auto rc = RECT();
    SetRect(&rc, p._x, p._y, p._cx, p._cy);
    MapDialogRect(hwndParent, &rc);

    SetWindowPos(_state->hwnd, 0, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER);
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
    dump();

    // safety checks
    if (_props->parent == nullptr)
        return;

    if (_props->id < 1)
        return;

    HWND hwndParent = reinterpret_cast<HWND>(_props->parent->handle());
    if (hwndParent == NULL)
        return;

    auto rc = RECT();
    SetRect(&rc,
            _props->p._x,
            _props->p._y,
            _props->p._cx,
            _props->p._cy);
    MapDialogRect(hwndParent, &rc);

    auto hwnd = CreateWindowExW(exStyles(),
                                toWide(className()).c_str(),
                                toWide(_props->text).c_str(),
                                styles(),
                                rc.left,
                                rc.top,
                                rc.right,
                                rc.bottom,
                                hwndParent,
                                (HMENU)(UINT_PTR)_props->id,
                                GetModuleHandle(NULL), NULL);
    if (hwnd == NULL)
        return;

    SendMessage(hwnd, WM_SETFONT, (WPARAM)_state->hFont, FALSE);
    _state->hwnd = hwnd;
}

void Control::dump()
{
    if (_state->hwnd == NULL)
        return;

    DestroyWindow(_state->hwnd);
    _state->hwnd = nullptr;
}

IEvent<>& Control::ClickEvent()
{
    return _props->clickEvent;
}

IEvent<>& Control::DoubleClickEvent()
{
    return _props->dblClickEvent;
}

IEvent<bool>& Control::FocusEvent()
{
    return _props->focusEvent;
}
