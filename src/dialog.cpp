#include "dialog_p.h"
#include "utility.h"
#include "control.h"
#include "control_p.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <CommCtrl.h>

using namespace dlgcpp;

Dialog::Dialog(std::shared_ptr<IDialog> parent) :
    _props(new dlg_props()),
    _state(new dlg_state()),
    _parent(parent)
{
    static bool init = false;
    if (!init)
    {
        InitCommonControls();
        init = true;
    }

    _props->_p._cx = 600;
    _props->_p._cy = 400;
    rebuild();
}

Dialog::~Dialog()
{
    dump();

    // clean handles
    if (_state->_hbrBgColor != NULL)
    {
        DeleteObject(_state->_hbrBgColor);
        _state->_hbrBgColor = NULL;
    }

    delete _props;
    delete _state;
}

std::shared_ptr<Dialog> Dialog::shared_ptr()
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

int Dialog::nextId()
{
    auto id = _nextId;
    _nextId++;
    return id;
}

int Dialog::exec()
{
    if (_execRunning)
        return 0;

    if (_state->_hwnd == NULL)
    {
        // build failed
        rebuild();
        if (_state->_hwnd == NULL)
            return 0;
    }

    // show dialog
    if (!_props->_visible)
        visible(true);

    // disable parent
    bool parentEnabled = false;
    if (_parent != nullptr)
    {
        parentEnabled = _parent->enabled();
        _parent->enabled(false);
    }

    auto msg = MSG();
    HACCEL hAccel = NULL;

    _execRunning = true;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // dialog handle may change
        if (hAccel == NULL || !TranslateAccelerator(_state->_hwnd, hAccel, &msg))
        {
            if (!IsDialogMessage(_state->_hwnd, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    _execRunning = false;

    if (_parent != nullptr)
    {
        _parent->enabled(parentEnabled);
        if (parentEnabled)
            SetActiveWindow(reinterpret_cast<HWND>(_parent->handle()));
    }

    return (int)msg.wParam;
}

void Dialog::quit(int result)
{
    if (_execRunning)
        // assumed our message loop is running
        PostQuitMessage(result);
}

bool Dialog::enabled() const
{
    return _props->_enabled;
}

void Dialog::enabled(bool value)
{
    if (_props->_enabled == value)
        return;
    _props->_enabled = value;
    if (_state->_hwnd == NULL)
        return;

    EnableWindow(_state->_hwnd, _props->_enabled);
}

bool Dialog::visible() const
{
    return _props->_visible;
}

void Dialog::visible(bool value)
{
    _props->_visible = value;

    if (_state->_hwnd == NULL)
        return;

    ShowWindow(_state->_hwnd,
               _props->_visible ? SW_SHOW : SW_HIDE);
}

const Position& Dialog::p() const
{
    return _props->_p;
}

void Dialog::move(int x, int y)
{
    _props->_p._x = x;
    _props->_p._y = y;

    if (_state->_hwnd == NULL)
        return;

    // convert units to pixels
    auto px = toPixels(_state->_hwnd, _props->_p, false); // menu off

    SetWindowPos(_state->_hwnd,
                 0,
                 px._cx,
                 px._cy,
                 0,
                 0,
                 SWP_NOZORDER | SWP_NOSIZE);
}

void Dialog::resize(int width, int height)
{
    _props->_p._cx = width;
    _props->_p._cy = height;

    if (_state->_hwnd == NULL)
        return;

    auto px = toPixels(_state->_hwnd, _props->_p, false); // menu off

    SetWindowPos(_state->_hwnd,
                 0,
                 0,
                 0,
                 px._cx,
                 px._cy,
                 SWP_NOZORDER | SWP_NOMOVE);
}

void Dialog::center()
{
    if (_state->_hwnd == NULL)
        return;

    auto rc = RECT();
    GetWindowRect(_state->_hwnd, &rc);

    int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right-rc.left)) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom-rc.top)) / 2;

    SetWindowPos(_state->_hwnd,
                 0,
                 x,
                 y,
                 0,
                 0,
                 SWP_NOZORDER | SWP_NOSIZE);
}

DialogType Dialog::type() const
{
    return _props->_type;
}

void Dialog::type(DialogType value)
{
    _props->_type = value;

    if (_state->_hwnd == NULL)
        return;

    // rebuild as setting styles does not work in all cases
    rebuild();
}

const std::string& Dialog::title() const
{
    return _props->_text;
}

void Dialog::title(const std::string& value)
{
    if (_props->_text == value)
        return;
    _props->_text = value;
    if (_state->_hwnd == NULL)
        return;

    SetWindowTextW(_state->_hwnd,
                   toWide(_props->_text).c_str());
}

Color Dialog::color() const
{
    return _props->_backColor;
}

void Dialog::color(Color value)
{
    if (_props->_backColor == value)
        return;

    _props->_backColor = value;
    if (_state->_hbrBgColor != NULL)
    {
        DeleteObject((HBRUSH)_state->_hbrBgColor);
        _state->_hbrBgColor = NULL;
    }

    // if using default, keep null
    if (value == Color::None)
        _state->_hbrBgColor = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    else if (value != Color::Default)
        _state->_hbrBgColor = CreateSolidBrush((COLORREF)_props->_backColor);

    redraw();
}

Cursor Dialog::cursor() const
{
    return _props->_cursor;
}

void Dialog::cursor(Cursor value)
{
    _props->_cursor = value;
}

void* Dialog::handle() const
{
    return _state->_hwnd;
}

void* Dialog::user() const
{
    return _props->_user;
}

void Dialog::user(void* value)
{
    _props->_user = value;
}

std::shared_ptr<IDialog> Dialog::parent()
{
    return _parent;
}

std::vector<std::shared_ptr<IControl>> Dialog::children() const
{
    auto r = std::vector<std::shared_ptr<IControl>>();
    for (auto child : _children)
        r.push_back(child->control());
    return r;
}

void Dialog::close(int result)
{
    dump();
    quit(result);
}

void Dialog::message(const std::string& message, const std::string& title, DialogMessageType type)
{
    UINT flags = 0;

    // TODO: button encapsulation and result
    // +implement as dialog
    flags |= MB_OK;

    if (type == DialogMessageType::Information)
        flags |= MB_ICONINFORMATION;
    if (type == DialogMessageType::Warning)
        flags |= MB_ICONWARNING;
    if (type == DialogMessageType::Error)
        flags |= MB_ICONERROR;

    MessageBoxW(_state->_hwnd,
                toWide(message).c_str(),
                toWide(title).c_str(), flags);
}

void Dialog::timer(int timeout, std::function<void(void)> handler)
{
    if ( _props->_timer.id == 0)
        _props->_timer.id = nextId();

    // if timeout is negative the timer is removed.
    _props->_timer.timeout = timeout;
    _props->_timer.handler = handler;
    updateTimer();
}

void Dialog::updateTimer()
{
    if (_state->_hwnd == NULL)
        return;

    if (_props->_timer.timeout > 0)
    {
        SetTimer(_state->_hwnd, _props->_timer.id, _props->_timer.timeout, NULL);
    }
    else
    {
        if (_props->_timer.id > 0)
            KillTimer(_state->_hwnd, (UINT_PTR)_props->_timer.id);
    }
}

void Dialog::add(std::shared_ptr<IChild> child)
{
    if (child == nullptr ||
        child->control() == nullptr ||
        child->control()->parent() != shared_ptr())
        return;

    auto it = std::find(_children.begin(), _children.end(), child);
    if (it != _children.end())
        return;
    _children.push_back(child);

    child->id(nextId());
}

void Dialog::remove(std::shared_ptr<IChild> child)
{
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it == _children.end())
        return;
    _children.erase(it);
}

std::shared_ptr<IChild> Dialog::childFromId(int id)
{
    if (id == 0)
        return nullptr;

    for (auto child : _children)
        if (child->id() == id)
            return child;
    return nullptr;
}

LRESULT Dialog::sendMsg(UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    if (_state->_hwnd == NULL)
        return 0;

    return SendMessage(_state->_hwnd, wMsg, wParam, lParam);
}

void Dialog::redraw()
{
    if (_state->_hwnd == NULL)
        return;

    RedrawWindow(_state->_hwnd, NULL, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
}

void Dialog::rebuild()
{
    dump();

    // Use the Windows dialog font
    std::wstring fontFace = L"MS Shell Dlg";
    unsigned short fontSize = 8; // Pointsize
    std::wstring text;

    text = toWide(_props->_text);
    size_t cbCaption = text.size();
    size_t cbFont = fontFace.size();

    // Params ok. Create heap for dialog...
    size_t bufSize = sizeof(DLGTEMPLATE) + 4 + ((cbCaption+1) * 2) + ((cbFont+1) * 2) + 2;
    std::vector<char> buf(bufSize);
    auto dlg = (DLGTEMPLATE*)&buf[0];
    if (dlg == NULL)
        return;

    dlg->style = styles();
    dlg->dwExtendedStyle = exStyles();
    dlg->x = (short)_props->_p._x;
    dlg->y = (short)_props->_p._y;
    dlg->cx = (short)_props->_p._cx;
    dlg->cy = (short)_props->_p._cy;

    // WSTR:
    size_t offset = sizeof(DLGTEMPLATE);

    // menu
    offset += 2; // Terminator

    // dialog class
    offset += 2; // Terminator

    // caption
    if (cbCaption > 0)
    {
        memcpy((char*)dlg + offset, text.c_str(), (cbCaption * 2));
        offset += (cbCaption * 2);
    }
    offset += 2; // Terminator

    if (cbFont > 0)
    {
        // font (optional)
        memcpy((char*)dlg + offset, &fontSize, 2);
        offset += 2;
        memcpy((char*)dlg + offset, fontFace.c_str(), (cbFont * 2));
        //offset += (cbFont * 2);
    }
    //offset += 2; // Terminator

    HWND hwndParent = NULL;
    if (_parent != nullptr)
        hwndParent = reinterpret_cast<HWND>(_parent->handle());

    // Create the dialog, passing private data struct
    auto hwnd = CreateDialogIndirectParam(GetModuleHandle(NULL), dlg, hwndParent, &Dialog::staticWndProc, 0);

    if (hwnd == NULL)
        return;

    SetProp(hwnd, "this", this);
    _state->_hwnd = hwnd;

    updateTimer();
}

void Dialog::dump()
{
    if (_state->_hwnd == NULL)
        return;

    // it does not use EndDialog as we may be simulating a model dialog with exec()
    if (_props->_timer.id > 0)
        KillTimer(_state->_hwnd, _props->_timer.id);
    DestroyWindow(_state->_hwnd);
    _state->_hwnd = NULL;
}

unsigned int Dialog::styles() const
{
    // not using DS_CENTER as center() is provided for this.
    unsigned int styles = DS_SETFONT | DS_SETFOREGROUND | DS_3DLOOK;

    switch (_props->_type)
    {
    case DialogType::Application:
        styles |= WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
        break;
    case DialogType::Popup:
        styles |= WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_POPUP;
        if (_parent != nullptr)
            styles |= DS_MODALFRAME; // no icon
        break;
    case DialogType::Frameless:
        styles |= WS_POPUP;
        break;
    case DialogType::Tool:
        styles |= WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME;
        break;
    };

    if (!_props->_enabled)
        styles |= WS_DISABLED;

    if (_props->_visible)
        styles |= WS_VISIBLE;

    return styles;
}

unsigned int Dialog::exStyles() const
{
    unsigned int styles = 0;

    if (_props->_type == DialogType::Frameless)
        styles |= WS_EX_TOOLWINDOW;

    if (_props->_type == DialogType::Tool)
        styles |= WS_EX_TOOLWINDOW;

    return styles;
}

LRESULT CALLBACK Dialog::staticWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    auto pthis = reinterpret_cast<Dialog*>(GetProp(hDlg, "this"));
    if (pthis != nullptr)
        return pthis->defaultWndProc(hDlg, wMsg, wParam, lParam);
    return 0;
}

LRESULT Dialog::defaultWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    switch (wMsg)
    {
    case WM_COMMAND:
    {
        auto id = (int)LOWORD(wParam);
        if (id != 0)
        {
            auto child = childFromId(id);
            if (child != nullptr)
                child->control()->CommandEvent().invoke();
            else if (id == IDCANCEL)
                close();
        }
        break;
    }

    case WM_MOVE:
    {
        // translate using mapped value and store
        auto pos = toUnits(_state->_hwnd, Position{(int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), 0, 0});
        _props->_p._x = pos._x;
        _props->_p._y = pos._y;
        break;
    }

    case WM_SIZE:
    {
        // translate using mapped value and store
        auto pos = toUnits(_state->_hwnd, Position{0, 0, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)});
        _props->_p._cx = pos._cx;
        _props->_p._cy = pos._cy;
        break;
    }

    case WM_LBUTTONDOWN:
    {
        ClickEvent().invoke();
    }

    case WM_SETCURSOR:
        return onSetCursor((HWND)wParam);

    case WM_CTLCOLORDLG:
        return onColorDlg((HDC)wParam);

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORSCROLLBAR:
        return onColorCtl((HDC)wParam, (HWND)lParam);

    case WM_SYSCOMMAND:
        // Close X button on dialog...
        if ((wParam & 0xFFF0) == SC_CLOSE)
        {
            close();
        }
        break;

    case WM_TIMER:
    {
        auto timerId = (int)wParam;
        if (timerId > 0 && timerId == _props->_timer.id)
        {
            _props->_timer.handler();
        }
        break;
    }
    }

    return 0;
}

LRESULT Dialog::onSetCursor(HWND hwndChild)
{

    if (hwndChild != _state->_hwnd && GetParent(hwndChild) != _state->_hwnd)
    {
        // some controls have a different parent
        hwndChild = GetParent(hwndChild);
    }

    auto id = GetDlgCtrlID(hwndChild);
    auto child = childFromId(id);

    auto cursor = Cursor::Default;
    if (child != nullptr)
        cursor = child->control()->cursor();
    else
        cursor = _props->_cursor;

    auto cursorId = IDC_ARROW;
    switch (cursor)
    {
    case Cursor::Arrow:
        cursorId = IDC_ARROW;
        break;
    case Cursor::ArrowBusy:
        cursorId = IDC_APPSTARTING;
        break;
    case Cursor::Busy:
        cursorId = IDC_WAIT;
        break;
    case Cursor::Cross:
        cursorId = IDC_CROSS;
        break;
    case Cursor::Hand:
        cursorId = IDC_HAND;
        break;
    case Cursor::Help:
        cursorId = IDC_HELP;
        break;
    case Cursor::Text:
        cursorId = IDC_IBEAM;
        break;
    case Cursor::Unavailable:
        cursorId = IDC_NO;
        break;
    default:
        return FALSE;
    }

    HCURSOR hCursor = LoadCursor(NULL, cursorId);
    if (hCursor != NULL)
    {
        SetCursor(hCursor);
        SetWindowLong(_state->_hwnd, DWLP_MSGRESULT, TRUE);
        return TRUE;
    }
    return FALSE;
}

LRESULT Dialog::onColorDlg(HDC hdc)
{
    if (_state->_hbrBgColor == NULL)
        return 0;

    // set back color of DC for controls with no background
    auto hbrBack = (HBRUSH)_state->_hbrBgColor;
    auto lb = LOGBRUSH();
    GetObject(hbrBack, sizeof(LOGBRUSH), &lb);
    SetBkColor(hdc, lb.lbColor);
    return (LRESULT)hbrBack;
}

LRESULT Dialog::onColorCtl(HDC hdc, HWND hwndChild)
{
    // color handler. Returns a brush handle if required.
    if (hwndChild != _state->_hwnd &&
        GetParent(hwndChild) != _state->_hwnd)
    {
        // some controls have a different parent
        hwndChild = GetParent(hwndChild);
    }

    auto id = GetDlgCtrlID(hwndChild);
    auto child = childFromId(id);
    if (child == nullptr)
        return 0;

    auto colors = child->control()->colors();
    auto fgColor = colors.first;
    auto bgColor = colors.second;

    if (fgColor != Color::None && fgColor != Color::Default)
    {
        SetTextColor(hdc, (COLORREF)fgColor);

        // If bgcolor not set with fgcolor, return a system color.
        if (bgColor == Color::None)
        {
            // Attempt transparency. This works with themes.
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)GetStockObject(HOLLOW_BRUSH);
        }

        if (bgColor == Color::Default)
        {
            // using system default
            int sysClr = COLOR_BTNFACE;
            auto msg = MSG();
            PeekMessage(&msg, _state->_hwnd, 0, 0, 0);
            switch (msg.message)
            {
            case WM_CTLCOLOREDIT:
            case WM_CTLCOLORLISTBOX:
                sysClr = COLOR_WINDOW;
                break;
            case WM_CTLCOLORSCROLLBAR:
                sysClr = COLOR_SCROLLBAR;
                break;
            case WM_CTLCOLORSTATIC:
                sysClr = COLOR_BTNFACE;
                break;
            default:
                return 0;
            }
            SetBkColor(hdc, GetSysColor(sysClr));
            return (LRESULT)GetSysColorBrush(sysClr);
        }
    }

    // the child manages the background brush
    if (child->state().hbrBack != NULL)
    {
        SetBkColor(hdc, (COLORREF)bgColor);
        return (LRESULT)child->state().hbrBack;
    }

    auto hbrBack = (HBRUSH)_state->_hbrBgColor;
    auto lb = LOGBRUSH();
    GetObject(hbrBack, sizeof(LOGBRUSH), &lb);
    SetBkColor(hdc, lb.lbColor);
    return (LRESULT)hbrBack;
}

IEvent& Dialog::ClickEvent()
{
    return _clickEvent;
}
