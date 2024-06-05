#include "dialog_p.h"
#include "dlgmsg.h"
#include "control_p.h"
#include "utility.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <CommCtrl.h>
#include <shellapi.h>

using namespace dlgcpp;

Dialog::Dialog(std::shared_ptr<IDialog> parent) :
    _props(new dlg_props()),
    _state(new dlg_state())
{
    static bool init = false;
    if (!init)
    {
        InitCommonControls();
        init = true;
    }

    _props->parent = parent;
    _props->p._cx = 600;
    _props->p._cy = 400;
    rebuild();
}

Dialog::~Dialog()
{
    dump();

    // clean handles
    if (_state->hbrBgColor != NULL)
    {
        DeleteObject(_state->hbrBgColor);
        _state->hbrBgColor = NULL;
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
    auto id = _props->nextId;
    _props->nextId++;
    return id;
}

int Dialog::exec()
{
    if (_props->execRunning)
        return 0;

    if (_state->hwnd == NULL)
    {
        // build failed
        rebuild();
        if (_state->hwnd == NULL)
            return 0;
    }

    // show dialog
    if (!_props->visible)
        visible(true);

    // disable parent
    bool parentEnabled = false;
    if (_props->parent != nullptr)
    {
        parentEnabled = _props->parent->enabled();
        _props->parent->enabled(false);
    }

    auto msg = MSG();
    HACCEL hAccel = NULL;

    _props->execRunning = true;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // dialog handle may change
        if (hAccel == NULL || !TranslateAccelerator(_state->hwnd, hAccel, &msg))
        {
            if (!IsDialogMessage(_state->hwnd, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    _props->execRunning = false;

    if (_props->parent != nullptr)
    {
        _props->parent->enabled(parentEnabled);
        if (parentEnabled)
            SetActiveWindow(reinterpret_cast<HWND>(_props->parent->handle()));
    }

    return (int)msg.wParam;
}

void Dialog::quit(int result)
{
    if (_props->execRunning)
        // assumed our message loop is running
        PostQuitMessage(result);
}

bool Dialog::enabled() const
{
    return _props->enabled;
}

void Dialog::enabled(bool value)
{
    if (_props->enabled == value)
        return;
    _props->enabled = value;
    if (_state->hwnd == NULL)
        return;

    EnableWindow(_state->hwnd, _props->enabled);
}

bool Dialog::visible() const
{
    return _props->visible;
}

void Dialog::visible(bool value)
{
    _props->visible = value;

    if (_state->hwnd == NULL)
        return;

    ShowWindow(_state->hwnd,
               _props->visible ? SW_SHOW : SW_HIDE);
}

const Position& Dialog::p() const
{
    return _props->p;
}

void Dialog::move(int x, int y)
{
    _props->p._x = x;
    _props->p._y = y;

    if (_state->hwnd == NULL)
        return;

    // convert units to pixels
    auto px = toPixels(_state->hwnd, _props->p, false); // menu off

    SetWindowPos(_state->hwnd,
                 0,
                 px._cx,
                 px._cy,
                 0,
                 0,
                 SWP_NOZORDER | SWP_NOSIZE);
}

void Dialog::resize(int width, int height)
{
    _props->p._cx = width;
    _props->p._cy = height;

    if (_state->hwnd == NULL)
        return;

    auto px = toPixels(_state->hwnd, _props->p, false); // menu off

    SetWindowPos(_state->hwnd,
                 0,
                 0,
                 0,
                 px._cx,
                 px._cy,
                 SWP_NOZORDER | SWP_NOMOVE);
}

void Dialog::center()
{
    if (_state->hwnd == NULL)
        return;

    auto rc = RECT();
    GetWindowRect(_state->hwnd, &rc);

    int x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right-rc.left)) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom-rc.top)) / 2;

    SetWindowPos(_state->hwnd,
                 0,
                 x,
                 y,
                 0,
                 0,
                 SWP_NOZORDER | SWP_NOSIZE);
}

DialogType Dialog::type() const
{
    return _props->type;
}

void Dialog::type(DialogType value)
{
    _props->type = value;

    if (_state->hwnd == NULL)
        return;

    // rebuild as setting styles does not work in all cases
    rebuild();
}

const std::string& Dialog::title() const
{
    return _props->title;
}

void Dialog::title(const std::string& value)
{
    if (_props->title == value)
        return;
    _props->title = value;
    if (_state->hwnd == NULL)
        return;

    SetWindowTextW(_state->hwnd,
                   toWide(_props->title).c_str());
}

const ImageSource& Dialog::image() const
{
    return _props->image;
}

void Dialog::image(const ImageSource& image)
{
    // must be an icon
    if (!image.isIcon)
        return;

    _props->image = image;
    updateImage();
}

void Dialog::updateImage()
{
    if (_state->hImage != NULL)
    {
        DeleteObject(_state->hImage);
        _state->hImage = NULL;
    }

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_props->image.id.empty())
    {
        SendMessage(hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)NULL);
        SendMessage(hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)NULL);
        return;
    }

    auto hInstRes = GetModuleHandle(NULL);
    auto imageType = (_props->image.isIcon ? IMAGE_ICON : IMAGE_BITMAP);
    _state->hImage = LoadImage(hInstRes,
                               _props->image.id.c_str(),
                               imageType,
                               0,
                               0,
                               LR_DEFAULTSIZE | (_props->image.isFile ? LR_LOADFROMFILE : 0));
    if (_state->hImage == NULL)
        return;

    SendMessage(hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)_state->hImage);
    SendMessage(hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)_state->hImage);
}

Color Dialog::color() const
{
    return _props->backColor;
}

void Dialog::color(Color value)
{
    if (_props->backColor == value)
        return;

    _props->backColor = value;
    if (_state->hbrBgColor != NULL)
    {
        DeleteObject((HBRUSH)_state->hbrBgColor);
        _state->hbrBgColor = NULL;
    }

    // if using default, keep null
    if (value == Color::None)
        _state->hbrBgColor = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    else if (value != Color::Default)
        _state->hbrBgColor = CreateSolidBrush((COLORREF)_props->backColor);

    redraw();
}

Cursor Dialog::cursor() const
{
    return _props->cursor;
}

void Dialog::cursor(Cursor value)
{
    _props->cursor = value;
}

void* Dialog::handle() const
{
    return _state->hwnd;
}

void* Dialog::user() const
{
    return _props->user;
}

void Dialog::user(void* value)
{
    _props->user = value;
}

std::shared_ptr<IDialog> Dialog::parent()
{
    return _props->parent;
}

std::vector<std::shared_ptr<IControl>> Dialog::children() const
{
    auto r = std::vector<std::shared_ptr<IControl>>();
    for (auto child : _props->children)
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

    MessageBoxW(_state->hwnd,
                toWide(message).c_str(),
                toWide(title).c_str(), flags);
}

void Dialog::timer(int timeout)
{
    if ( _props->timer.id == 0)
        _props->timer.id = nextId();

    // if timeout is negative the timer is removed.
    _props->timer.timeout = timeout;
    updateTimer();
}

bool Dialog::dropTarget() const
{
    return _props->dropTarget;
}

void Dialog::dropTarget(bool value)
{
    if (_props->dropTarget == value)
        return;
    _props->dropTarget = value;

    if (_state->hwnd == NULL)
        return;

    DragAcceptFiles(_state->hwnd, _props->dropTarget);
}

void Dialog::updateTimer()
{
    if (_state->hwnd == NULL)
        return;

    if (_props->timer.timeout > 0)
    {
        SetTimer(_state->hwnd,
                 _props->timer.id,
                 _props->timer.timeout,
                 NULL);
    }
    else
    {
        if (_props->timer.id > 0)
            KillTimer(_state->hwnd, (UINT_PTR)_props->timer.id);
    }
}

void Dialog::add(std::shared_ptr<IChild> child)
{
    if (child == nullptr ||
        child->control() == nullptr ||
        child->control()->parent() != shared_ptr())
        return;

    auto it = std::find(_props->children.begin(), _props->children.end(), child);
    if (it != _props->children.end())
        return;
    _props->children.push_back(child);

    child->id(nextId());
}

void Dialog::remove(std::shared_ptr<IChild> child)
{
    auto it = std::find(_props->children.begin(), _props->children.end(), child);
    if (it == _props->children.end())
        return;
    // this will dispose of the child control
    child->id(0);
    _props->children.erase(it);
}

std::shared_ptr<IChild> Dialog::childFromId(int id)
{
    if (id == 0)
        return nullptr;

    for (auto child : _props->children)
        if (child->id() == id)
            return child;
    return nullptr;
}

void Dialog::redraw()
{
    if (_state->hwnd == NULL)
        return;

    RedrawWindow(_state->hwnd, NULL, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
}

void Dialog::rebuild()
{
    dump();

    // Use the Windows dialog font
    std::wstring fontFace = L"MS Shell Dlg";
    unsigned short fontSize = 8; // Pointsize
    std::wstring text;

    text = toWide(_props->title);
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
    dlg->x = (short)_props->p._x;
    dlg->y = (short)_props->p._y;
    dlg->cx = (short)_props->p._cx;
    dlg->cy = (short)_props->p._cy;

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
    if (_props->parent != nullptr)
        hwndParent = reinterpret_cast<HWND>(_props->parent->handle());

    // Create the dialog, passing private data struct
    auto hwnd = CreateDialogIndirectParam(GetModuleHandle(NULL), dlg, hwndParent, &Dialog::staticWndProc, 0);

    if (hwnd == NULL)
        return;

    SetProp(hwnd, "this", this);
    _state->hwnd = hwnd;

    for (auto& c : _props->children)
    {
        // ideally it supports rebuild
        //c->rebuild();
        c->id(c->id());
    }

    DragAcceptFiles(_state->hwnd, _props->dropTarget);
    updateImage();
    updateTimer();

    // the dialog does not erase the background automatically
    if (_props->visible)
        redraw();
}

void Dialog::dump()
{
    if (_state->hwnd == NULL)
        return;

    // it does not use EndDialog as we may be simulating a model dialog with exec()
    if (_props->timer.id > 0)
        KillTimer(_state->hwnd, _props->timer.id);
    DestroyWindow(_state->hwnd);
    _state->hwnd = NULL;
}

unsigned int Dialog::styles() const
{
    // not using DS_CENTER as center() is provided for this.
    unsigned int styles = DS_SETFONT | DS_SETFOREGROUND | DS_3DLOOK;

    switch (_props->type)
    {
    case DialogType::Application:
        styles |= WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
        break;
    case DialogType::Popup:
        styles |= WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_POPUP;
        if (_props->parent != nullptr)
            styles |= DS_MODALFRAME; // no icon
        break;
    case DialogType::Frameless:
        styles |= WS_POPUP;
        break;
    case DialogType::Tool:
        styles |= WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME;
        break;
    };

    if (!_props->enabled)
        styles |= WS_DISABLED;

    if (_props->visible)
        styles |= WS_VISIBLE;

    return styles;
}

unsigned int Dialog::exStyles() const
{
    unsigned int styles = 0;

    if (_props->type == DialogType::Frameless)
        styles |= WS_EX_TOOLWINDOW;

    if (_props->type == DialogType::Tool)
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
            {
                // wrap message and send to child for processing
                // a result code is supported.
                auto msg = dlg_message{wMsg, wParam, lParam};
                child->notify(msg);
                return msg.result;
            }
            //else if (id == IDOK)
                // TODO: Confirm event
            else if (id == IDCANCEL)
                // TODO: Close event
                close();
        }
        break;
    }

    case WM_NOTIFY:
    {
        auto pNmHdr = (NMHDR*)lParam;
        if (pNmHdr->idFrom != 0)
        {
            auto child = childFromId(pNmHdr->idFrom);
            if (child != nullptr)
            {
                // wrap message and send to child for processing
                // a result code is supported.
                auto msg = dlg_message{wMsg, wParam, lParam};
                child->notify(msg);
                return msg.result;
            }
        }
        break;
    }

    case WM_HSCROLL:
    case WM_VSCROLL:
    {
        auto hwnd = (HWND)lParam;
        if (hwnd != NULL)
        {
            auto id = (int)GetDlgCtrlID(hwnd);
            auto child = childFromId(id);
            if (child != nullptr)
            {
                // wrap message and send to child for processing
                // a result code is supported.
                auto msg = dlg_message{wMsg, wParam, lParam};
                child->notify(msg);
                return msg.result;
            }
        }
        break;
    }

    case WM_MOVE:
    {
        // translate using mapped value and store
        auto pos = toUnits(_state->hwnd, Position{(int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), 0, 0});
        _props->p._x = pos._x;
        _props->p._y = pos._y;
        MoveEvent().invoke();
        break;
    }

    case WM_SIZE:
    {
        // translate using mapped value and store
        auto pos = toUnits(_state->hwnd, Position{0, 0, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)});
        _props->p._cx = pos._cx;
        _props->p._cy = pos._cy;
        SizeEvent().invoke();
        break;
    }

    case WM_LBUTTONDOWN:
    {
        ClickEvent().invoke();
        break;
    }

    case WM_LBUTTONDBLCLK:
    {
        DoubleClickEvent().invoke();
        break;
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

    case WM_DROPFILES:
    {
        // extract dropped files and invoke drop event
        auto hDrop = (HDROP)wParam;
        auto fileCount = (int)DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
        if (fileCount == 0)
            return 0;

        std::vector<std::string> files;
        for (int i = 0; i < fileCount; i++)
        {
            std::wstring wfile(MAX_PATH, 0x0);
            if (DragQueryFileW(hDrop, i, &wfile[0], wfile.size()) == 0)
                continue;
            files.push_back(toBytes(wfile.data()));
        }
        if (files.empty())
            return 0;
        DropEvent().invoke(files);
        break;
    }

    case WM_TIMER:
    {
        auto timerId = (int)wParam;
        if (timerId > 0 && timerId == _props->timer.id)
        {
            TimerEvent().invoke();
        }
        break;
    }
    }

    return 0;
}

LRESULT Dialog::onSetCursor(HWND hwndChild)
{

    if (hwndChild != _state->hwnd && GetParent(hwndChild) != _state->hwnd)
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
        cursor = _props->cursor;

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
        SetWindowLong(_state->hwnd, DWLP_MSGRESULT, TRUE);
        return TRUE;
    }
    return FALSE;
}

LRESULT Dialog::onColorDlg(HDC hdc)
{
    if (_state->hbrBgColor == NULL)
        return 0;

    // set back color of DC for controls with no background
    auto hbrBack = (HBRUSH)_state->hbrBgColor;
    auto lb = LOGBRUSH();
    GetObject(hbrBack, sizeof(LOGBRUSH), &lb);
    SetBkColor(hdc, lb.lbColor);
    return (LRESULT)hbrBack;
}

LRESULT Dialog::onColorCtl(HDC hdc, HWND hwndChild)
{
    // color handler. Returns a brush handle if required.
    if (hwndChild != _state->hwnd &&
        GetParent(hwndChild) != _state->hwnd)
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
            PeekMessage(&msg, _state->hwnd, 0, 0, 0);
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

    auto hbrBack = (HBRUSH)_state->hbrBgColor;
    auto lb = LOGBRUSH();
    GetObject(hbrBack, sizeof(LOGBRUSH), &lb);
    SetBkColor(hdc, lb.lbColor);
    return (LRESULT)hbrBack;
}

IEvent<>& Dialog::ClickEvent()
{
    return _props->clickEvent;
}

IEvent<>& Dialog::DoubleClickEvent()
{
    return _props->dblClickEvent;
}

IEvent<std::vector<std::string>>& Dialog::DropEvent()
{
    return _props->dropEvent;
}

IEvent<>& Dialog::MoveEvent()
{
    return _props->moveEvent;
}

IEvent<>& Dialog::SizeEvent()
{
    return _props->sizeEvent;
}

IEvent<>& Dialog::TimerEvent()
{
    return _props->timerEvent;
}
