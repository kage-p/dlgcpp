#include "control_p.h"
#include "dialog_p.h"
#include "utility/keys.h"
#include "utility/message.h"
#include "utility/string.h"
#include "utility/units.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <CommCtrl.h>
#include <shellapi.h>

using namespace dlgcpp;

// private functions (winspec)
std::shared_ptr<IChildControl> findControl(dlg_priv& pi, int id, HWND hwnd = NULL);
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

    if (parent != nullptr)
    {
        // default position off-parent
        _pi->props.p.x(parent->p().x());
        _pi->props.p.y(parent->p().y());
    }
}

Dialog::~Dialog()
{
    destruct();

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

int Dialog::idRange() const
{
    // no id range for dialogs, only for controls
    return 1;
}

ISharedDialog Dialog::dialog()
{
    return shared_from_this();
}

int Dialog::nextId(int reservedRange)
{
    auto id = _pi->props.nextId;
    _pi->props.nextId += reservedRange;
    return id;
}

int Dialog::exec()
{
    if (_pi->props.execRunning)
        return 0;

    show();
    if (_pi->state.hwnd == NULL)
        return 0;

    // disable parent
    _pi->state.execParentEnabled = false;
    if (_pi->props.parent != nullptr)
    {
        _pi->state.execParentEnabled = _pi->props.parent->enabled();
        _pi->props.parent->enabled(false);
    }

    _pi->props.execRunning = true;

    int result = MessageProcessor::beginLoop(_pi->state.hwnd);

    _pi->props.execRunning = false;

    return result;
}

void Dialog::quit(int result)
{
    if (_pi->props.execRunning)
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

    EnableWindow(
        _pi->state.hwnd,
        _pi->props.enabled);
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
    updateVisibility();
}

DisplayState Dialog::displayState() const
{
    return _pi->props.displayState;
}

void Dialog::displayState(DisplayState value)
{
    if (_pi->props.displayState == value)
        return;
    _pi->props.displayState = value;

    updateDisplayState();
}

const Position& Dialog::p() const
{
    return _pi->props.p;
}

void Dialog::p(const Position& p)
{
    if (_pi->props.p == p)
        return;

    _pi->props.p = p;
    updatePosition();
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
    updateIcon();
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

/// <summary>
/// Constructs and shows the dialog
/// </summary>
void Dialog::show()
{
    // ensure visible
    if (!_pi->props.visible)
        visible(true);

    // ensure constructed
    if (_pi->state.hwnd == NULL)
    {
        rebuild();

        // failed to construct
        if (_pi->state.hwnd == NULL)
            return;
    }
}

/// <summary>
/// Close the dialog, removing it from view.
/// </summary>
void Dialog::close(int result)
{
    // we must re-enable parent before destroying this dialog.
    // if we don't do this then the parent recedes into the background.
    if (_pi->props.execRunning &&
        _pi->props.parent != nullptr)
    {
        _pi->props.parent->enabled(_pi->state.execParentEnabled);
    }

    destruct();
    quit(result);
}

void Dialog::setFocus()
{
    if (_pi->state.hwnd == NULL)
        return;

    SetFocus(_pi->state.hwnd);
}

void Dialog::bringToFront()
{
    if (_pi->state.hwnd == NULL)
        return;

    SetWindowPos(_pi->state.hwnd,
        HWND_TOP,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);
}

void Dialog::sendToBack()
{
    if (_pi->state.hwnd == NULL)
        return;

    SetWindowPos(_pi->state.hwnd,
        HWND_BOTTOM,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);
}

void Dialog::move(const Point& point)
{
    if (_pi->props.p.point() == point)
        return;

    _pi->props.p.x(point.x());
    _pi->props.p.y(point.y());

    updatePosition();
}

void Dialog::resize(const Size& size)
{
    if (_pi->props.p.size() == size)
        return;

    _pi->props.p.width(size.width());
    _pi->props.p.height(size.height());

    updatePosition();
}

void Dialog::center()
{
    // TODO: if child, center in parent
    if (_pi->props.id > 0)
        return;

    // use dialog-independent mapping to units
    auto screenSize = Size(
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN));
    toUnits(HWND_DESKTOP, screenSize);

    Point p(
        (screenSize.width() / 2) - (_pi->props.p.width() / 2),
        (screenSize.height() / 2) - (_pi->props.p.height() / 2));

    move(p);
}

void Dialog::message(
    const std::string& message,
    const std::string& title)
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

void Dialog::sendUserEvent(int param)
{
    if (_pi->state.hwnd == NULL)
        return;
    PostMessage(_pi->state.hwnd, WM_DLGCPP_USER, (WPARAM)param, 0);
}

void Dialog::timer(int timeout)
{
    if (_pi->props.timer.id == 0)
        _pi->props.timer.id = nextId();

    // if timeout is negative the timer is removed.
    _pi->props.timer.timeout = timeout;
    updateTimer();
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
    child->id(nextId(child->idRange()));
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
    for (auto& child : _pi->props.controls)
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
    child->id(nextId());
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
    for (auto& child : _pi->props.dialogs)
    {
        r.push_back(child->dialog());
    }
    return r;
}

void Dialog::redraw(bool drawChildren)
{
    if (_pi->state.hwnd == NULL)
        return;

    RedrawWindow(_pi->state.hwnd,
        NULL,
        0,
        RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | (drawChildren ? RDW_ALLCHILDREN : 0));
}

void Dialog::rebuild()
{
    MessageLocker sizeAndMoveLock(_pi->props.inhibitSizeAndMoveMessages);

    destruct();

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
    size_t bufSize = sizeof(DLGTEMPLATE) + 4 + ((cbCaption + 1) * 2) + ((cbFont + 1) * 2) + 2;
    std::vector<char> buf(bufSize);
    DLGTEMPLATE& dlg = *(DLGTEMPLATE*)&buf[0];

    dlg.style = styles();
    dlg.dwExtendedStyle = exStyles();

    // don't set dialog position
    dlg.x = 0;
    dlg.y = 0;
    dlg.cx = 0;
    dlg.cy = 0;

    // WSTR:
    size_t offset = sizeof(DLGTEMPLATE);

    // menu
    offset += 2;

    // dialog class
    offset += 2;

    // caption
    if (cbCaption > 0)
    {
        memcpy(&buf[offset], text.c_str(), (cbCaption * 2));
        offset += (cbCaption * 2);
    }
    offset += 2;

    if (cbFont > 0)
    {
        // font (optional)
        memcpy(&buf[offset], &fontSize, 2);
        offset += 2;
        memcpy(&buf[offset], fontFace.c_str(), (cbFont * 2));
        //offset += (cbFont * 2);
    }
    //offset += 2;

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

    updatePosition();
    updateDisplayState();

    for (auto& c : _pi->props.controls)
    {
        c->rebuild();
    }

    for (auto& d : _pi->props.dialogs)
    {
        d->rebuild();
    }

    DragAcceptFiles(_pi->state.hwnd, _pi->props.dropTarget);
    updateIcon();
    updateTimer();

    if (_pi->props.menu != nullptr)
        _pi->props.menu->rebuild();

    updateVisibility();

    // always fire the size event
    SizeEvent().invoke(shared_from_this());
}

void Dialog::destruct()
{
    if (_pi->state.hwnd == NULL)
        return;

    // it does not use EndDialog as we may be simulating a model dialog with exec()
    if (_pi->props.timer.id > 0)
        KillTimer(_pi->state.hwnd, _pi->props.timer.id);
    DestroyWindow(_pi->state.hwnd);
    _pi->state.hwnd = NULL;
}

unsigned int Dialog::styles() const
{
    // not using DS_CENTER as center() is provided for this.
    unsigned int styles = DS_SETFONT | DS_SETFOREGROUND | DS_3DLOOK | WS_CLIPCHILDREN;

    if (_pi->props.id > 0)
    {
        // a child dialog
        styles |= WS_CHILD | WS_CLIPSIBLINGS | DS_CONTROL;
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

    if (pthis == nullptr)
        return FALSE;

    // wrap and transfer the message directly to the class.
    auto msg = dlg_message{ wMsg, wParam, lParam, 0 ,0 };
    pthis->notify(msg);

    if (msg.dlgResult != 0)
    {
        // return the message result code, if any
        SetWindowLongPtr(hDlg, DWLP_MSGRESULT, msg.msgResult);
    }
    return msg.dlgResult;
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
            if (lParam != NULL)
            {
                // message sent from a control

                // locate the sender; we may need to use the HWND
                auto child = findControl(*_pi, id, (HWND)lParam);

                if (child != nullptr)
                {
                    // wrap message and send to child for processing
                    // a result code is supported.
                    child->notify(msg);
                    return;
                }
            }
            else
            {
                // message sent from a menu, accelerator, or default key.

                if (HIWORD(wParam) == 0)
                {
                    // default keys for confirm/cancel
                    if (id == IDOK)
                    {
                        // Confirm event (ENTER)
                        _pi->props.confirmEvent.invoke();
                        return;
                    }
                    else if (id == IDCANCEL)
                    {
                        // Cancel event (ESC)
                        _pi->props.cancelEvent.invoke();
                        return;
                    }

                    // menu item
                    if (_pi->props.menu != nullptr)
                    {
                        _pi->props.menu->notify(msg);
                        return;
                    }
                }
                else if (HIWORD(wParam) == 1)
                {
                    // accelerator
                    DLGCPP_CMSG("WM_COMMAND: accel key " << id);
                }
            }

        }
        break;
    }

    case WM_NOTIFY:
    {
        auto& nmhdr = *((NMHDR*)lParam);

        // locate the sender; we may need to use the HWND
        auto child = findControl(*_pi, nmhdr.idFrom, nmhdr.hwndFrom);
        if (child != nullptr)
        {
            // wrap message and send to child for processing
            // a result code is supported.
            child->notify(msg);
            return;
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

    case WM_KEYDOWN:
    {
        KeyboardEvent event;
        event.key = MapToKey(static_cast<UINT>(msg.wParam));
        KeyDownEvent().invoke(shared_from_this(), event);
        break;
    }
    case WM_KEYUP:
    {
        KeyboardEvent event;
        event.key = MapToKey(static_cast<UINT>(msg.wParam));
        KeyUpEvent().invoke(shared_from_this(), event);
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
        onSetCursor(msg);
        break;

    case WM_MOVE:
    {
        if (_pi->props.inhibitSizeAndMoveMessages.get())
            break;

        // only store the position when dialog is in "normal" display state.
        if (IsIconic(hDlg) || IsZoomed(hDlg))
            break;

        // translate using mapped value and store
        Point posPx((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
        Point posDu(posPx);
        toUnits(_pi->state.hwnd, posDu);

        _pi->props.p.x(posDu.x());
        _pi->props.p.y(posDu.y());
        MoveEvent().invoke(shared_from_this());

        DLGCPP_CMSG("WM_MOVE: " <<
            "x = " << _pi->props.p.x() << " " <<
            "y = " << _pi->props.p.y() << " " <<
            "title = " + _pi->props.title);

        break;
    }

    case WM_SIZE:
    {
        if (_pi->props.inhibitSizeAndMoveMessages.get())
            break;

        auto state = (UINT)wParam;
        switch (state)
        {
        case SIZE_RESTORED:
            _pi->props.displayState = DisplayState::Normal;
            break;
        case SIZE_MINIMIZED:
            _pi->props.displayState = DisplayState::Minimized;
            break;
        case SIZE_MAXIMIZED:
            _pi->props.displayState = DisplayState::Maximized;
            break;
        };

        if (state == SIZE_RESTORED ||
            state == SIZE_MAXIMIZED)
        {
            // only store the size when dialog is not minimized.
            // note: we have to capture maximized state, as it will affect any potential child positioning

            // translate using mapped value and store
            Size sizePx({ (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) });
            Size sizeDu(sizePx);
            toUnits(_pi->state.hwnd, sizeDu);

            _pi->props.p.width(sizeDu.width());
            _pi->props.p.height(sizeDu.height());
            SizeEvent().invoke(shared_from_this());
        }

        DLGCPP_CMSG("WM_SIZE: " <<
            "displayState = " << (int)_pi->props.displayState << " "
            "width = " << _pi->props.p.width() << " " <<
            "height = " << _pi->props.p.height() << " " <<
            "title = " + _pi->props.title);

        break;
    }

    case WM_CTLCOLORDLG:
        onColorDlg(msg);
        break;

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORSCROLLBAR:
        onColorCtl(msg);
        break;

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
            break;

        std::vector<std::string> files;
        for (int i = 0; i < fileCount; i++)
        {
            std::wstring wfile(MAX_PATH, 0x0);
            if (DragQueryFileW(hDrop, i, &wfile[0], (UINT)wfile.size()) == 0)
                continue;
            files.push_back(toBytes(wfile.data()));
        }
        if (!files.empty())
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

    case WM_DLGCPP_USER:
    {
        UserEvent().invoke(shared_from_this(), (int)wParam);
        break;
    }
    }
}

void Dialog::onSetCursor(dlg_message& msg)
{
    auto hwndChild = reinterpret_cast<HWND>(msg.wParam);
    auto child = findControl(*_pi, 0, hwndChild);

    auto cursor = Cursor::Default;
    if (child != nullptr)
        cursor = child->control()->cursor();
    else
        cursor = _pi->props.cursor;

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
        return;
    }

    HCURSOR hCursor = LoadCursor(NULL, cursorId);
    if (hCursor != NULL)
    {
        SetCursor(hCursor);
        msg.dlgResult = TRUE;
    }
}

void Dialog::onColorDlg(dlg_message& msg)
{
    auto hdc = reinterpret_cast<HDC>(msg.wParam);

    // always return a brush
    bool usingSysDefault = false;
    if (_pi->props.backColor == Color::Default)
    {
        usingSysDefault = true;
    }
    else if (_pi->props.backColor == Color::None)
    {
        if (_pi->props.parent != nullptr)
        {
            // use parent brush; the message will call this same function for the parent.
            auto hwndParent = reinterpret_cast<HWND>(_pi->props.parent->handle());
            msg.dlgResult = SendMessage(hwndParent, WM_CTLCOLORDLG, (WPARAM)hdc, (LPARAM)hwndParent);
            return;
        }
        usingSysDefault = true;
    }

    if (usingSysDefault)
    {
        // use system default
        SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
        msg.dlgResult = (LRESULT)GetSysColorBrush(COLOR_BTNFACE);
        return;
    }

    SetBkColor(hdc, (COLORREF)_pi->props.backColor);
    msg.dlgResult = (LRESULT)_pi->state.hbrBgColor;
}

void Dialog::onColorCtl(dlg_message& msg)
{
    auto hdc = reinterpret_cast<HDC>(msg.wParam);
    auto hwndChild = reinterpret_cast<HWND>(msg.lParam);

    auto child = findControl(*_pi, 0, hwndChild);
    if (child == nullptr)
        return;

    // color handler. Returns a brush handle if required.

    auto colors = child->control()->colors();
    auto fgColor = colors.first;
    auto bgColor = colors.second;

    if (fgColor == Color::Default &&
        bgColor == Color::Default)
        // not using colors
        return;

    if (fgColor != Color::Default)
    {
        SetTextColor(hdc, (COLORREF)fgColor);

        // foreground only color requires a background
        if (bgColor == Color::Default)
        {
            // using system default
            int sysClr = COLOR_BTNFACE;
            auto tagMsg = MSG();
            PeekMessage(&tagMsg, _pi->state.hwnd, 0, 0, 0);
            switch (tagMsg.message)
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
                return;
            }
            SetBkColor(hdc, GetSysColor(sysClr));

            msg.dlgResult = (LRESULT)GetSysColorBrush(sysClr);
            return;
        }
    }

    if (bgColor == Color::None)
    {
        // use parent brush
        onColorDlg(msg);
        return;
    }

    // the child manages the background brush
    SetBkColor(hdc, (COLORREF)bgColor);
    msg.dlgResult = (LRESULT)child->state().hbrBack;
}

std::shared_ptr<IChildControl> findControl(dlg_priv& pi, int id, HWND hwnd)
{
    // TODO: findControl performance impact
    // Use map<handle,Child> with all child hwnd / id > child for quick lookup

    //DLGCPP_CMSG("findControl: id = " << id << "  hwnd = " << hwnd);

    if (id > 0)
    {
        // by explicit identifier
        for (auto& child : pi.props.controls)
        {
            if (child->id() == id)
                return child;
        }
    }

    if (hwnd != NULL)
    {
        // by HWND identifier
        id = GetDlgCtrlID(hwnd);
        for (auto& child : pi.props.controls)
        {
            if (child->id() == id)
                return child;
        }

        // by HWND
        for (auto& child : pi.props.controls)
        {
            if ((HWND)child->control()->isHandleEqual(hwnd))
                return child;
        }

        // by parent HWND 
        auto hwndParent = GetParent(hwnd);
        if (hwndParent != 0)
        {
            for (auto& child : pi.props.controls)
            {
                if ((HWND)child->control()->isHandleEqual(hwndParent))
                    return child;
            }
        }
    }

    return nullptr;
}

IEvent<ISharedDialog>& Dialog::ConfirmEvent()
{
    return _pi->props.confirmEvent;
}

IEvent<ISharedDialog>& Dialog::CancelEvent()
{
    return _pi->props.cancelEvent;
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

IEvent<ISharedDialog, KeyboardEvent>& Dialog::KeyDownEvent()
{
    return _pi->props.keyDownEvent;
}

IEvent<ISharedDialog, KeyboardEvent>& Dialog::KeyUpEvent()
{
    return _pi->props.keyUpEvent;
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
    return _pi->props.mouseCaptureLostEvent;
}

IEvent<ISharedDialog, std::vector<std::string>>& Dialog::DropEvent()
{
    return _pi->props.dropEvent;
}

IEvent<ISharedDialog, int>& Dialog::UserEvent()
{
    return _pi->props.userEvent;
}

void Dialog::updateDisplayState()
{
    if (_pi->state.hwnd == NULL)
        return;

    // use SetWindowPlacement which does not show the dialog
    WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
    GetWindowPlacement(_pi->state.hwnd, &wp);
    switch (_pi->props.displayState)
    {
    case DisplayState::Normal:    wp.showCmd = SW_SHOWNORMAL;    break;
    case DisplayState::Minimized: wp.showCmd = SW_SHOWMINIMIZED; break;
    case DisplayState::Maximized: wp.showCmd = SW_SHOWMAXIMIZED; break;
    }
    SetWindowPlacement(_pi->state.hwnd, &wp);
}

void Dialog::updateIcon()
{
    if (_pi->state.hImage != NULL)
    {
        DeleteObject(_pi->state.hImage);
        _pi->state.hImage = NULL;
    }

    if (_pi->state.hwnd == NULL)
        return;

    if (_pi->props.image.id.empty())
    {
        SendMessage(_pi->state.hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)NULL);
        SendMessage(_pi->state.hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)NULL);
        return;
    }

    auto hInstRes = GetModuleHandle(NULL);
    auto imageType = (_pi->props.image.isIcon ? IMAGE_ICON : IMAGE_BITMAP);
    _pi->state.hImage = LoadImageW(hInstRes,
        toWide(_pi->props.image.id).c_str(),
        imageType,
        0,
        0,
        LR_DEFAULTSIZE | (_pi->props.image.isFile ? LR_LOADFROMFILE : 0));
    if (_pi->state.hImage == NULL)
        return;

    SendMessage(_pi->state.hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)_pi->state.hImage);
    SendMessage(_pi->state.hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)_pi->state.hImage);
}

void Dialog::updatePosition()
{
    if (_pi->state.hwnd == NULL)
        return;

    Position posPx;
    if (_pi->props.id > 0)
    {
        // child dialog; use parent client
        HWND hwndParent = (HWND)_pi->props.parent->handle();
        posPx = toPixels(hwndParent, _pi->props.p, true);
    }
    else
    {
        posPx = toPixels(_pi->state.hwnd, _pi->props.p, true);
    }

    // map the size dimensions to non-client
    RECT rc = { 0, 0, posPx.width(), posPx.height() };
    AdjustWindowRectEx(
        &rc,
        GetWindowLong(_pi->state.hwnd, GWL_STYLE),
        GetMenu(_pi->state.hwnd) != NULL,
        GetWindowLong(_pi->state.hwnd, GWL_EXSTYLE));

    SetWindowPos(
        _pi->state.hwnd,
        0,
        posPx.x(),
        posPx.y(),
        rc.right - rc.left,
        rc.bottom - rc.top,
        SWP_NOZORDER);
}

void Dialog::updateTimer()
{
    if (_pi->state.hwnd == NULL)
        return;

    if (_pi->props.timer.timeout > 0)
    {
        SetTimer(
            _pi->state.hwnd,
            _pi->props.timer.id,
            _pi->props.timer.timeout,
            NULL);
    }
    else
    {
        if (_pi->props.timer.id > 0)
            KillTimer(_pi->state.hwnd, (UINT_PTR)_pi->props.timer.id);
    }
}

void Dialog::updateVisibility()
{
    if (_pi->state.hwnd == NULL)
        return;

    ShowWindow(
        _pi->state.hwnd,
        _pi->props.visible ? SW_SHOW : SW_HIDE);
}
