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

// private functions
int nextId(dlg_priv& pi);
void updateImage(dlg_priv& pi);
void updateTimer(dlg_priv& pi);
void destruct(dlg_priv& pi);
void quit(dlg_priv& pi, int result = 0);
std::shared_ptr<IChildControl> findControl(dlg_priv& pi, int id);
std::shared_ptr<IChildControl> findControl(dlg_priv& pi, HWND hwndChild);
LRESULT onSetCursor(dlg_priv& pi, HWND hwndChild);
LRESULT onColorDlg(dlg_priv& pi, HDC hdc);
LRESULT onColorCtl(dlg_priv& pi, HDC hdc, HWND hwndChild);
LRESULT CALLBACK dialogWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);


Dialog::Dialog(DialogType type, ISharedDialog parent) :
    _pi(new dlg_priv())
{
    static bool init = false;
    if (!init)
    {
        InitCommonControls();
        init = true;
    }

    // note: the dialog cannot be built here as the shared_ptr is not ready.
    // this dialog has a parent; but is not a child of the parent.
    _pi->props.type = type;
    _pi->props.parent = parent;
    _pi->props.p = Position(0, 0, 600, 400);

    if (parent != nullptr)
    {
        // default position off-parent
        _pi->props.p.x(parent->p().x());
        _pi->props.p.y(parent->p().y());
    }
}

Dialog::~Dialog()
{
    destruct(*_pi);

    if (_pi->state.hbrBgColor != NULL)
    {
        DeleteObject(_pi->state.hbrBgColor);
        _pi->state.hbrBgColor = NULL;
    }

    if (_pi->state.hImage != NULL)
    {
        DeleteObject(_pi->state.hImage);
        _pi->state.hImage = NULL;
    }

    delete _pi;
}

ISharedDialog Dialog::parent() const
{
    return _pi->props.parent;
}

void Dialog::parent(ISharedDialog parent)
{
    if (_pi->props.parent == parent)
        return;
    _pi->props.parent = parent;
}

int Dialog::id() const
{
    return _pi->props.id;
}

void Dialog::id(int value)
{
    _pi->props.id = value;
}

ISharedDialog Dialog::dialog()
{
    return shared_from_this();
}

int nextId(dlg_priv& pi)
{
    auto id = pi.props.nextId;
    pi.props.nextId++;
    return id;
}

int Dialog::exec()
{
    if (_pi->props.execRunning)
        return 0;

    if (_pi->state.hwnd == NULL)
    {
        // build failed
        rebuild();
        if (_pi->state.hwnd == NULL)
            return 0;
    }

    // show dialog
    if (!_pi->props.visible)
        visible(true);

    // disable parent
    _pi->state.execParentEnabled = false;
    if (_pi->props.parent != nullptr)
    {
        _pi->state.execParentEnabled = _pi->props.parent->enabled();
        _pi->props.parent->enabled(false);
    }

    auto msg = MSG();
    HACCEL hAccel = NULL;

    _pi->props.execRunning = true;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        // dialog handle may change
        if (hAccel == NULL || !TranslateAccelerator(_pi->state.hwnd, hAccel, &msg))
        {
            if (!IsDialogMessage(_pi->state.hwnd, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    _pi->props.execRunning = false;

    return (int)msg.wParam;
}

void quit(dlg_priv& pi, int result)
{
    if (pi.props.execRunning)
    {
        // assumed our message loop is running
        PostQuitMessage(result);
    }
}

bool Dialog::enabled() const
{
    return _pi->props.enabled;
}

void Dialog::enabled(bool value)
{
    if (_pi->props.enabled == value)
        return;
    _pi->props.enabled = value;
    if (_pi->state.hwnd == NULL)
        return;

    EnableWindow(_pi->state.hwnd, _pi->props.enabled);
}

bool Dialog::visible() const
{
    return _pi->props.visible;
}

void Dialog::visible(bool value)
{
    if (_pi->props.visible == value)
        return;

    _pi->props.visible = value;

    if (_pi->state.hwnd == NULL)
    {
        // build failed
        rebuild();
        if (_pi->state.hwnd == NULL)
            return;
    }
    ShowWindow(_pi->state.hwnd,
               _pi->props.visible ? SW_SHOW : SW_HIDE);
}

const Position& Dialog::p() const
{
    return _pi->props.p;
}

void Dialog::move(const Point& point)
{
    _pi->props.p.x(point.x());
    _pi->props.p.y(point.y());

    if (_pi->state.hwnd == NULL)
        return;

    auto pxPos = point;
    if (_pi->props.id > 0)
    {
        // child dialog; use parent client
        HWND hwndParent = (HWND)_pi->props.parent->handle();
        toPixels(hwndParent, pxPos, true);
    }
    else
        toPixels(_pi->state.hwnd, pxPos, false);

    SetWindowPos(_pi->state.hwnd,
                 0,
                 pxPos.x(),
                 pxPos.y(),
                 0,
                 0,
                 SWP_NOZORDER | SWP_NOSIZE);
}

void Dialog::resize(const Size& size)
{
    _pi->props.p.width(size.width());
    _pi->props.p.height(size.height());

    if (_pi->state.hwnd == NULL)
        return;

    auto pxSize = size;
    toPixels(_pi->state.hwnd, pxSize, false);

    SetWindowPos(_pi->state.hwnd,
                 0,
                 0,
                 0,
                 pxSize.width(),
                 pxSize.height(),
                 SWP_NOZORDER | SWP_NOMOVE);
}

void Dialog::center()
{
    // TODO: if child, center in parent
    if (_pi->props.id > 0)
        return;

    // use dialog-independent mapping to units
    auto screenSize = Size(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
    toUnits(HWND_DESKTOP, screenSize);

    Point p((screenSize.width() / 2) - (_pi->props.p.width() / 2),
            (screenSize.height() / 2) - (_pi->props.p.height() / 2));

    move(p);
}

DialogType Dialog::type() const
{
    return _pi->props.type;
}

bool Dialog::showHelp() const
{
    return _pi->props.showHelp;
}

void Dialog::showHelp(bool value)
{
    _pi->props.showHelp = value;

    if (_pi->state.hwnd == NULL)
        return;

    // set extended style
    SetWindowLong(_pi->state.hwnd, GWL_EXSTYLE, exStyles());
}

const std::string& Dialog::title() const
{
    return _pi->props.title;
}

void Dialog::title(const std::string& value)
{
    if (_pi->props.title == value)
        return;
    _pi->props.title = value;
    if (_pi->state.hwnd == NULL)
        return;

    SetWindowTextW(_pi->state.hwnd,
                   toWide(_pi->props.title).c_str());
}

const ImageSource& Dialog::image() const
{
    return _pi->props.image;
}

void Dialog::image(const ImageSource& image)
{
    // must be an icon
    if (!image.isIcon)
        return;

    _pi->props.image = image;
    updateImage(*_pi);
}

void updateImage(dlg_priv& pi)
{
    if (pi.state.hImage != NULL)
    {
        DeleteObject(pi.state.hImage);
        pi.state.hImage = NULL;
    }

    if (pi.state.hwnd == NULL)
        return;

    if (pi.props.image.id.empty())
    {
        SendMessage(pi.state.hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)NULL);
        SendMessage(pi.state.hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)NULL);
        return;
    }

    auto hInstRes = GetModuleHandle(NULL);
    auto imageType = (pi.props.image.isIcon ? IMAGE_ICON : IMAGE_BITMAP);
    pi.state.hImage = LoadImageW(hInstRes,
                                 toWide(pi.props.image.id).c_str(),
                                 imageType,
                                 0,
                                 0,
                                 LR_DEFAULTSIZE | (pi.props.image.isFile ? LR_LOADFROMFILE : 0));
    if (pi.state.hImage == NULL)
        return;

    SendMessage(pi.state.hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)pi.state.hImage);
    SendMessage(pi.state.hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)pi.state.hImage);
}

std::shared_ptr<IMenu> Dialog::menu() const
{
    return _pi->props.menu->menu();
}

void Dialog::menu(std::shared_ptr<IChildMenu> menu)
{
    if (_pi->props.menu == menu)
        return;

    if (_pi->state.hwnd != NULL)
        SetMenu(_pi->state.hwnd, NULL);

    _pi->props.menu = menu;

    if (_pi->props.menu != nullptr)
    {
        // the menu will assign itself to the dialog
        _pi->props.menu->id(MenuStartId);
        _pi->props.menu->parent(shared_from_this());
        _pi->props.menu->rebuild();
    }
}

Color Dialog::color() const
{
    return _pi->props.backColor;
}

void Dialog::color(Color value)
{
    if (_pi->props.backColor == value)
        return;

    _pi->props.backColor = value;
    if (_pi->state.hbrBgColor != NULL)
    {
        DeleteObject((HBRUSH)_pi->state.hbrBgColor);
        _pi->state.hbrBgColor = NULL;
    }

    // if using default, keep null
    if (value != Color::None &&
        value != Color::Default)
        _pi->state.hbrBgColor = CreateSolidBrush((COLORREF)_pi->props.backColor);

    redraw(true);
}

Cursor Dialog::cursor() const
{
    return _pi->props.cursor;
}

void Dialog::cursor(Cursor value)
{
    _pi->props.cursor = value;
}

void* Dialog::handle() const
{
    return _pi->state.hwnd;
}

void* Dialog::user() const
{
    return _pi->props.user;
}

void Dialog::user(void* value)
{
    _pi->props.user = value;
}

ISharedDialog Dialog::parent()
{
    return _pi->props.parent;
}

void Dialog::close(int result)
{
    // we must re-enable parent before destroying this dialog.
    // if we don't do this then the parent recedes into the background.
    if (_pi->props.execRunning &&
        _pi->props.parent != nullptr)
    {
        _pi->props.parent->enabled(_pi->state.execParentEnabled);
    }

    destruct(*_pi);
    quit(*_pi, result);
}

void Dialog::message(const std::string& message, const std::string& title)
{
    // this function only supports OK button; see dlgcpp/dialogs/message for full implementation
    UINT flags = MB_ICONINFORMATION | MB_OK;

    std::wstring titleText = toWide(title);
    if (titleText.empty())
        titleText = toWide(_pi->props.title);

    std::wstring messageText = toWide(message);

    MessageBoxW(_pi->state.hwnd,
                messageText.c_str(),
                titleText.c_str(), flags);
}

void Dialog::timer(int timeout)
{
    if ( _pi->props.timer.id == 0)
        _pi->props.timer.id = nextId(*_pi);

    // if timeout is negative the timer is removed.
    _pi->props.timer.timeout = timeout;
    updateTimer(*_pi);
}

bool Dialog::dropTarget() const
{
    return _pi->props.dropTarget;
}

void Dialog::dropTarget(bool value)
{
    if (_pi->props.dropTarget == value)
        return;
    _pi->props.dropTarget = value;

    if (_pi->state.hwnd == NULL)
        return;

    DragAcceptFiles(_pi->state.hwnd, _pi->props.dropTarget);
}

bool Dialog::mouseCapture() const
{
    return (_pi->state.hwnd != NULL && GetCapture() == _pi->state.hwnd);
}

void Dialog::mouseCapture(bool value)
{
    if (_pi->state.hwnd == NULL)
        return;

    if (value)
        SetCapture(_pi->state.hwnd);
    else
    {
        if (GetCapture() == _pi->state.hwnd)
            ReleaseCapture();
    }
}

void updateTimer(dlg_priv& pi)
{
    if (pi.state.hwnd == NULL)
        return;

    if (pi.props.timer.timeout > 0)
    {
        SetTimer(pi.state.hwnd,
                 pi.props.timer.id,
                 pi.props.timer.timeout,
                 NULL);
    }
    else
    {
        if (pi.props.timer.id > 0)
            KillTimer(pi.state.hwnd, (UINT_PTR)pi.props.timer.id);
    }
}

void Dialog::add(std::shared_ptr<IChildControl> child)
{
    if (child == nullptr ||
        child->parent() != nullptr)
        return;

    auto it = std::find(_pi->props.controls.begin(), _pi->props.controls.end(), child);
    if (it != _pi->props.controls.end())
        return;
    _pi->props.controls.push_back(child);

    child->parent(shared_from_this());
    child->id(nextId(*_pi));
    child->rebuild();
}

void Dialog::remove(std::shared_ptr<IChildControl> child)
{
    auto it = std::find(_pi->props.controls.begin(), _pi->props.controls.end(), child);
    if (it == _pi->props.controls.end())
        return;
    // this will dispose of the child control
    child->id(0);
    child->parent(nullptr);
    _pi->props.controls.erase(it);
}

std::vector<std::shared_ptr<IControl>> Dialog::controls() const
{
    auto r = std::vector<std::shared_ptr<IControl>>();
    for (auto child : _pi->props.controls)
        r.push_back(child->control());
    return r;
}

void Dialog::add(std::shared_ptr<IChildDialog> child)
{
    if (child == nullptr ||
        child->parent() != nullptr)
        return;

    auto it = std::find(_pi->props.dialogs.begin(), _pi->props.dialogs.end(), child);
    if (it != _pi->props.dialogs.end())
        return;
    _pi->props.dialogs.push_back(child);

    child->parent(shared_from_this());
    child->id(nextId(*_pi));
    child->rebuild();
}

void Dialog::remove(std::shared_ptr<IChildDialog> child)
{
    auto it = std::find(_pi->props.dialogs.begin(), _pi->props.dialogs.end(), child);
    if (it == _pi->props.dialogs.end())
        return;
    //this will dispose of the dialog
    child->id(0);
    child->parent(nullptr);
    _pi->props.dialogs.erase(it);
}

std::vector<ISharedDialog> Dialog::dialogs() const
{
    auto r = std::vector<ISharedDialog>();
    for (auto child : _pi->props.dialogs)
        r.push_back(child->dialog());
    return r;
}

void Dialog::redraw(bool drawChildren)
{
    if (_pi->state.hwnd == NULL)
        return;

    RedrawWindow(_pi->state.hwnd,
                 NULL,
                 0,
                 RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | (drawChildren ? RDW_ALLCHILDREN : 0) );
}

void Dialog::rebuild()
{
    destruct(*_pi);

    // safety checks
    if (_pi->props.id > 0 && _pi->props.parent == nullptr)
        return;

    // Use the Windows dialog font
    std::wstring fontFace = L"MS Shell Dlg";
    unsigned short fontSize = 8; // Pointsize
    std::wstring text;

    text = toWide(_pi->props.title);
    size_t cbCaption = text.size();
    size_t cbFont = fontFace.size();

    // Params ok. Create heap for dialog...
    size_t bufSize = sizeof(DLGTEMPLATE) + 4 + ((cbCaption+1) * 2) + ((cbFont+1) * 2) + 2;
    std::vector<char> buf(bufSize);
    DLGTEMPLATE& dlg = *(DLGTEMPLATE*)&buf[0];

    dlg.style = styles();
    dlg.dwExtendedStyle = exStyles();
    dlg.x = 0; //(short)_pi->props.p._x;
    dlg.y = 0; //(short)_pi->props.p._y;
    dlg.cx = 0; //(short)_pi->props.p._cx;
    dlg.cy = 0; //(short)_pi->props.p._cy;

    // WSTR:
    size_t offset = sizeof(DLGTEMPLATE);

    // menu
    offset += 2; // Terminator

    // dialog class
    offset += 2; // Terminator

    // caption
    if (cbCaption > 0)
    {
        memcpy(&buf[offset], text.c_str(), (cbCaption * 2));
        offset += (cbCaption * 2);
    }
    offset += 2; // Terminator

    if (cbFont > 0)
    {
        // font (optional)
        memcpy(&buf[offset], &fontSize, 2);
        offset += 2;
        memcpy(&buf[offset], fontFace.c_str(), (cbFont * 2));
        //offset += (cbFont * 2);
    }
    //offset += 2; // Terminator

    HWND hwndParent = NULL;
    if (_pi->props.parent != nullptr)
        hwndParent = reinterpret_cast<HWND>(_pi->props.parent->handle());

    // Create the dialog, passing private data struct
    auto hwnd = CreateDialogIndirectParam(GetModuleHandle(NULL),
                                          &dlg,
                                          hwndParent,
                                          dialogWndProc, 0);

    if (hwnd == NULL)
        return;

    SetPropW(hwnd, L"this", this);
    _pi->state.hwnd = hwnd;

    move(_pi->props.p);
    resize(_pi->props.p);

    for (auto& c : _pi->props.controls)
    {
        c->rebuild();
    }

    for (auto& d : _pi->props.dialogs)
    {
        d->rebuild();
    }

    DragAcceptFiles(_pi->state.hwnd, _pi->props.dropTarget);
    updateImage(*_pi);
    updateTimer(*_pi);

    if (_pi->props.menu != nullptr)
        _pi->props.menu->rebuild();

    // the dialog does not erase the background automatically
    if (_pi->props.visible)
        redraw(true);

    // always fire the size event
    SizeEvent().invoke(shared_from_this());
}

void destruct(dlg_priv& pi)
{
    if (pi.state.hwnd == NULL)
        return;

    // it does not use EndDialog as we may be simulating a model dialog with exec()
    if (pi.props.timer.id > 0)
        KillTimer(pi.state.hwnd, pi.props.timer.id);
    DestroyWindow(pi.state.hwnd);
    pi.state.hwnd = NULL;
}

unsigned int Dialog::styles() const
{
    // not using DS_CENTER as center() is provided for this.
    unsigned int styles = DS_SETFONT | DS_SETFOREGROUND | DS_3DLOOK | WS_CLIPCHILDREN;

    if (_pi->props.id > 0)
    {
        // a child dialog
        styles |= WS_CHILD | DS_CONTROL;
    }
    else
    {
        switch (_pi->props.type)
        {
        case DialogType::Application:
            styles |= WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
            break;
        case DialogType::Popup:
            styles |= WS_OVERLAPPED | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_POPUP;
            if (_pi->props.parent != nullptr)
                styles |= DS_MODALFRAME; // no icon
            break;
        case DialogType::Frameless:
            styles |= WS_POPUP;
            break;
        case DialogType::Tool:
            styles |= WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME;
            break;
        };
    }

    if (!_pi->props.enabled)
        styles |= WS_DISABLED;

    if (_pi->props.visible)
        styles |= WS_VISIBLE;

    return styles;
}

unsigned int Dialog::exStyles() const
{
    unsigned int styles = 0;

    if (_pi->props.id > 0)
    {
        // parent gets control notifications
        styles |= WS_EX_CONTROLPARENT;
    }
    else
    {

        if (_pi->props.type == DialogType::Frameless)
            styles |= WS_EX_TOOLWINDOW;

        if (_pi->props.type == DialogType::Tool)
            styles |= WS_EX_TOOLWINDOW;

        if (_pi->props.showHelp &&
            _pi->props.type != DialogType::Application &&
            _pi->props.type != DialogType::Frameless)
            styles |= WS_EX_CONTEXTHELP;
    }

    return styles;
}

LRESULT CALLBACK dialogWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    auto pthis = reinterpret_cast<Dialog*>(GetPropW(hDlg, L"this"));

    if (pthis != nullptr)
    {
        // wrap and transfer the message directly to the class.
        auto msg = dlg_message{wMsg, wParam, lParam};
        pthis->notify(msg);
        return msg.result;
    }
    return 0;
}

// intercepts dialog messages and performs the default actions.
void Dialog::notify(dlg_message& msg)
{
    auto hDlg = (HWND)handle();
    auto wMsg = msg.wMsg;
    auto wParam = msg.wParam;
    auto lParam = msg.lParam;

    switch (wMsg)
    {
    case WM_COMMAND:
    {
        auto id = (int)LOWORD(wParam);
        if (id != 0)
        {
            if (lParam == 0)
            {
                if (HIWORD(wParam) == 0)
                {
                    // menu item
                    if (_pi->props.menu != nullptr)
                    {
                        _pi->props.menu->notify(msg);
                        return;
                    }
                }
                else if (HIWORD(wParam) == 0)
                {
                    // accelerator

                }
                return;
            }

            auto child = findControl(*_pi, id);
            if (child != nullptr)
            {
                // wrap message and send to child for processing
                // a result code is supported.
                child->notify(msg);
                return;
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
            auto child = findControl(*_pi, (int)pNmHdr->idFrom);
            if (child != nullptr)
            {
                // wrap message and send to child for processing
                // a result code is supported.
                child->notify(msg);
                return;
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
            auto child = findControl(*_pi, id);
            if (child != nullptr)
            {
                // wrap message and send to child for processing
                // a result code is supported.
                child->notify(msg);
                return;
            }
        }
        break;
    }

    case WM_DRAWITEM:
    case WM_MEASUREITEM:
    {
        auto hwnd = (HWND)wParam;
        if (hwnd != NULL)
        {
            auto id = (int)GetDlgCtrlID(hwnd);
            auto child = findControl(*_pi, id);
            if (child != nullptr)
            {
                // wrap message and send to child for processing
                // a result code is supported.
                child->notify(msg);
                return;
            }
        }
        break;
    }

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
        toUnits(hDlg, event.point);

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
        toUnits(hDlg, event.point);

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
        toUnits(hDlg, event.point);

        MouseDoubleClickEvent().invoke(shared_from_this(), event);
        break;
    }

    case WM_MOUSEMOVE:
    {
        MouseEvent event;
        event.button = wMsg == WM_LBUTTONDOWN ? MouseButton::Left : wMsg == WM_RBUTTONDOWN ? MouseButton::Right : MouseButton::Middle;
        event.point = Point(LOWORD(lParam), HIWORD(lParam));
        toUnits(hDlg, event.point);

        MouseMoveEvent().invoke(shared_from_this(), event);
        break;
    }

    case WM_CAPTURECHANGED:
    {
        MouseCaptureLostEvent().invoke(shared_from_this());
        break;
    }

    case WM_SETCURSOR:
        msg.result = onSetCursor(*_pi, (HWND)wParam);
        return;

    case WM_MOVE:
    {
        // translate using mapped value and store
        Point posPx((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
        Point posDu(posPx);
        toUnits(_pi->state.hwnd, posDu);

        DLGCPP_CMSG("WM_MOVE: " <<
                    "x = "  << posDu.x() << " (" << posPx.x() << ") " <<
                    "y = " << posDu.y() << " (" << posPx.y() << ") " <<
                    "title = " + _pi->props.title);

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
        toUnits(_pi->state.hwnd, sizeDu);

        DLGCPP_CMSG("WM_SIZE: " <<
                    "width = "  << sizeDu.width() << " (" << sizePx.width() << ") " <<
                    "height = " << sizeDu.height() << " (" << sizePx.height() << ") " <<
                    "title = " + _pi->props.title);

        _pi->props.p.width(sizeDu.width());
        _pi->props.p.height(sizeDu.height());
        SizeEvent().invoke(shared_from_this());
        break;
    }

    case WM_CTLCOLORDLG:
        msg.result = onColorDlg(*_pi, (HDC)wParam);
        return;

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORSCROLLBAR:
        msg.result = onColorCtl(*_pi, (HDC)wParam, (HWND)lParam);
        return;

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
            return;

        std::vector<std::string> files;
        for (int i = 0; i < fileCount; i++)
        {
            std::wstring wfile(MAX_PATH, 0x0);
            if (DragQueryFileW(hDrop, i, &wfile[0], (UINT)wfile.size()) == 0)
                continue;
            files.push_back(toBytes(wfile.data()));
        }
        if (files.empty())
            return;
        DropEvent().invoke(shared_from_this(), files);
        break;
    }

    case WM_HELP:
    {
        HelpEvent().invoke(shared_from_this());
        break;
    }

    case WM_TIMER:
    {
        auto timerId = (int)wParam;
        if (timerId > 0 && timerId == _pi->props.timer.id)
        {
            TimerEvent().invoke(shared_from_this());
        }
        break;
    }
    }
}

LRESULT onSetCursor(dlg_priv& pi, HWND hwndChild)
{
    auto child = findControl(pi, hwndChild);

    auto cursor = Cursor::Default;
    if (child != nullptr)
        cursor = child->control()->cursor();
    else
        cursor = pi.props.cursor;

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
        SetWindowLong(pi.state.hwnd, DWLP_MSGRESULT, TRUE);
        return TRUE;
    }
    return FALSE;
}

LRESULT onColorDlg(dlg_priv& pi, HDC hdc)
{
    // always return a brush
    bool usingSysDefault = false;
    if (pi.props.backColor == Color::Default)
    {
        usingSysDefault = true;
    }
    else if (pi.props.backColor == Color::None)
    {
        if (pi.props.parent != nullptr)
        {
            // use parent brush; the message will call this same function for the parent.
            auto hwndParent = reinterpret_cast<HWND>(pi.props.parent->handle());
            return SendMessage(hwndParent, WM_CTLCOLORDLG, (WPARAM)hdc, (LPARAM)hwndParent);
        }
        usingSysDefault = true;
    }

    if (usingSysDefault)
    {
        // use system default
        SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
        return (LRESULT)GetSysColorBrush(COLOR_BTNFACE);
    }

    SetBkColor(hdc, (COLORREF)pi.props.backColor);
    return (LRESULT)pi.state.hbrBgColor;
}

LRESULT onColorCtl(dlg_priv& pi, HDC hdc, HWND hwndChild)
{
    auto child = findControl(pi, hwndChild);
    if (child == nullptr)
        return 0;

    // color handler. Returns a brush handle if required.

    auto colors = child->control()->colors();
    auto fgColor = colors.first;
    auto bgColor = colors.second;

    if (fgColor == Color::Default &&
        bgColor == Color::Default)
        // not using colors
        return 0;

    if (fgColor != Color::Default)
    {
        SetTextColor(hdc, (COLORREF)fgColor);

        // foreground only color requires a background
        if (bgColor == Color::Default)
        {
            // using system default
            int sysClr = COLOR_BTNFACE;
            auto msg = MSG();
            PeekMessage(&msg, pi.state.hwnd, 0, 0, 0);
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

    if (bgColor == Color::None)
    {
        // use parent brush
        return onColorDlg(pi, hdc);
    }

    // the child manages the background brush
    SetBkColor(hdc, (COLORREF)bgColor);
    return (LRESULT)child->state().hbrBack;
}

std::shared_ptr<IChildControl> findControl(dlg_priv& pi, int id)
{
    if (id == 0)
        return nullptr;

    for (auto child : pi.props.controls)
        if (child->id() == id)
            return child;
    return nullptr;
}

std::shared_ptr<IChildControl> findControl(dlg_priv& pi, HWND hwndChild)
{
    if (hwndChild != pi.state.hwnd &&
        GetParent(hwndChild) != pi.state.hwnd)
    {
        // some controls have a different parent
        hwndChild = GetParent(hwndChild);
    }

    auto id = GetDlgCtrlID(hwndChild);
    return findControl(pi, id);
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseDownEvent()
{
    return _pi->props.mouseDownEvent;
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseUpEvent()
{
    return _pi->props.mouseUpEvent;
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseMoveEvent()
{
    return _pi->props.mouseMoveEvent;
}

IEvent<ISharedDialog, MouseEvent>& Dialog::MouseDoubleClickEvent()
{
    return _pi->props.mouseDblClickEvent;
}

IEvent<ISharedDialog>& Dialog::MouseCaptureLostEvent()
{
    return _pi->props.mouseCaptureLost;
}

IEvent<ISharedDialog, std::vector<std::string>>& Dialog::DropEvent()
{
    return _pi->props.dropEvent;
}

IEvent<ISharedDialog>& Dialog::HelpEvent()
{
    return _pi->props.helpEvent;
}

IEvent<ISharedDialog>& Dialog::MoveEvent()
{
    return _pi->props.moveEvent;
}

IEvent<ISharedDialog>& Dialog::SizeEvent()
{
    return _pi->props.sizeEvent;
}

IEvent<ISharedDialog>& Dialog::TimerEvent()
{
    return _pi->props.timerEvent;
}
