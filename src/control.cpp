#include "control.h"
#include "utility.h"
#include "dialog.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
//#include <CommDlg.h>
//#include <ShellAPI.h>
//#include <ShlObj.h>

using namespace dlgcpp;

Control::Control(const std::string& className,
                 IDialog& parent) :
    _parent(parent)
{
    _props._className = className;
    _props._p._cx = 0;
    _props._p._cy = 0;

    _rebuild();
}

Control::~Control()
{
    _dump();
}

int Control::id() const
{
    return _props._id;
}

void Control::id(int value)
{
    _props._id = value;
    _rebuild();
}

bool Control::visible() const
{
    return _props._visible;
}

void Control::visible(bool value)
{
    _props._visible = value;

    if (_props._hwnd == NULL)
        return;

    auto hwnd = reinterpret_cast<HWND>(_props._hwnd);

    ShowWindow(hwnd,
               _props._visible ? SW_SHOW : SW_HIDE);
}

const Position& Control::p() const
{
    return _props._p;
}

void Control::p(const Position& p)
{
    _props._p = p;

    if (_props._hwnd == NULL)
        return;

    auto hwnd = reinterpret_cast<HWND>(_props._hwnd);

    // Convert units to pixels
    auto rc = RECT();
    SetRect(&rc, p._x, p._y, p._cx, p._cy);
    MapDialogRect(hwnd, &rc);

    bool redraw = true;
    SetWindowPos(hwnd, 0, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER | (redraw ? 0 : SWP_NOREDRAW) );
}

const std::string& Control::text() const
{
    return _props._text;
}

void Control::text(const std::string& value)
{
    _props._text = value;
    if (_props._hwnd == NULL)
        return;

    auto hwnd = reinterpret_cast<HWND>(_props._hwnd);
    SetWindowTextW(hwnd,
                   toWide(_props._text).c_str());
}

bool Control::tabbable() const
{
    return _props._tabbable;
}

void Control::tabbable(bool value)
{
    _props._tabbable = value;
    _rebuild();
}

void* Control::handle()
{
    return _props._hwnd;
}

IDialog& Control::parent()
{
    return _parent;
}

long long Control::sendMsg(unsigned int msg, long long wParam, long long lParam)
{
    auto hwnd = reinterpret_cast<HWND>(_props._hwnd);
    return SendMessage(hwnd, msg, wParam, lParam);
}

void Control::rebuild()
{
    _rebuild();
}

unsigned int Control::exStyles()
{
    return 0;
}

unsigned int Control::styles()
{
    unsigned int styles = WS_CHILD;
    if (_props._visible)
        styles |= WS_VISIBLE;
    if (_props._tabbable)
        styles |= WS_TABSTOP;
    return styles;
}

void Control::_rebuild()
{
    _dump();

    // safety checks
    if (_props._id < 1)
        return;
    if (_props._className.empty())
        return;
    HWND hwndParent = reinterpret_cast<HWND>(_parent.handle());
    if (hwndParent == NULL)
        return;

    auto rc = RECT();
    SetRect(&rc,
            _props._p._x,
            _props._p._y,
            _props._p._cx,
            _props._p._cy);
    MapDialogRect(hwndParent, &rc);

    auto hwnd = CreateWindowExW(exStyles(),
                                toWide(_props._className).c_str(),
                                toWide(_props._text).c_str(),
                                styles(),
                                rc.left,
                                rc.top,
                                rc.right,
                                rc.bottom,
                                hwndParent,
                                (HMENU)(UINT_PTR)_props._id,
                                GetModuleHandle(NULL), NULL);
    if (hwnd == NULL)
        return;

    //if (pDlgData->hFont) hFont = pDlgData->hFont; else
    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, FALSE);
    _props._hwnd = hwnd;
}

void Control::_dump()
{
    if (_props._hwnd == NULL)
        return;

    DestroyWindow(reinterpret_cast<HWND>(_props._hwnd));
    _props._hwnd = nullptr;
}
