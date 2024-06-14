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

Dialog::Dialog(DialogType type, ISharedDialog parent) :
    _props(new dlg_props()),
    _state(new dlg_state())
{
    static bool init = false;
    if (!init)
    {
        InitCommonControls();
        init = true;
    }

    // note: the dialog cannot be built here as the shared_ptr is not ready.
    // this dialog has a parent; but is not a child of the parent.
    _props->type = type;
    _props->parent = parent;
    _props->p = Position(0, 0, 600, 400);

    if (parent != nullptr)
    {
        // default position off-parent
        _props->p.x(parent->p().x());
        _props->p.y(parent->p().y());
    }
}

Dialog::~Dialog()
{
    destruct();

    if (_state->hbrBgColor != NULL)
    {
        DeleteObject(_state->hbrBgColor);
        _state->hbrBgColor = NULL;
    }

    if (_state->hImage != NULL)
    {
        DeleteObject(_state->hImage);
        _state->hImage = NULL;
    }

    delete _props;
    delete _state;
}

ISharedDialog Dialog::parent() const
{
    return _props->parent;
}

void Dialog::parent(ISharedDialog parent)
{
    if (_props->parent == parent)
        return;
    _props->parent = parent;
}

int Dialog::id() const
{
    return _props->id;
}

void Dialog::id(int value)
{
    _props->id = value;
}

ISharedDialog Dialog::dialog()
{
    return shared_from_this();
}

void Dialog::notify(struct dlgcpp::dlg_message&)
{
    // no default action
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
    _state->execParentEnabled = false;
    if (_props->parent != nullptr)
    {
        _state->execParentEnabled = _props->parent->enabled();
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

    return (int)msg.wParam;
}

void Dialog::quit(int result)
{
    if (_props->execRunning)
    {
        // assumed our message loop is running
        PostQuitMessage(result);
    }
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
    if (_props->visible == value)
        return;

    _props->visible = value;

    if (_state->hwnd == NULL)
    {
        // build failed
        rebuild();
        if (_state->hwnd == NULL)
            return;
    }
    ShowWindow(_state->hwnd,
               _props->visible ? SW_SHOW : SW_HIDE);
}

const Position& Dialog::p() const
{
    return _props->p;
}

void Dialog::move(const Point& point)
{
    _props->p.x(point.x());
    _props->p.y(point.y());

    if (_state->hwnd == NULL)
        return;

    auto pxPos = point;
    if (_props->id > 0)
    {
        // child dialog; use parent client
        HWND hwndParent = (HWND)_props->parent->handle();
        toPixels(hwndParent, pxPos, true);
    }
    else
        toPixels(_state->hwnd, pxPos, false);

    SetWindowPos(_state->hwnd,
                 0,
                 pxPos.x(),
                 pxPos.y(),
                 0,
                 0,
                 SWP_NOZORDER | SWP_NOSIZE);
}

void Dialog::resize(const Size& size)
{
    _props->p.width(size.width());
    _props->p.height(size.height());

    if (_state->hwnd == NULL)
        return;

    auto pxSize = size;
    toPixels(_state->hwnd, pxSize, false);

    SetWindowPos(_state->hwnd,
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
    if (_props->id > 0)
        return;

    // use dialog-independent mapping to units
    auto screenSize = Size(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
    toUnits(HWND_DESKTOP, screenSize);

    Point p((screenSize.width() / 2) - (_props->p.width() / 2),
            (screenSize.height() / 2) - (_props->p.height() / 2));

    move(p);
}

DialogType Dialog::type() const
{
    return _props->type;
}

bool Dialog::showHelp() const
{
    return _props->showHelp;
}

void Dialog::showHelp(bool value)
{
    _props->showHelp = value;

    if (_state->hwnd == NULL)
        return;

    // set extended style
    SetWindowLong(_state->hwnd, GWL_EXSTYLE, exStyles());
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

std::shared_ptr<IMenu> Dialog::menu() const
{
    return _props->menu->menu();
}

void Dialog::menu(std::shared_ptr<IChildMenu> menu)
{
    if (_props->menu == menu)
        return;

    if (_state->hwnd != NULL)
        SetMenu(_state->hwnd, NULL);

    _props->menu = menu;

    if (_props->menu != nullptr)
    {
        // the menu will assign itself to the dialog
        _props->menu->id(MenuStartId);
        _props->menu->parent(shared_from_this());
        _props->menu->rebuild();
    }
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
    if (value != Color::None &&
        value != Color::Default)
        _state->hbrBgColor = CreateSolidBrush((COLORREF)_props->backColor);

    redraw(true);
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

ISharedDialog Dialog::parent()
{
    return _props->parent;
}

void Dialog::close(int result)
{
    // we must re-enable parent before destroying this dialog.
    // if we don't do this then the parent recedes into the background.
    if (_props->execRunning &&
        _props->parent != nullptr)
    {
        _props->parent->enabled(_state->execParentEnabled);
    }

    destruct();
    quit(result);
}

void Dialog::message(const std::string& message, const std::string& title)
{
    // this function only supports OK button; see dlgcpp/dialogs/message for full implementation
    UINT flags = MB_ICONINFORMATION | MB_OK;

    std::wstring titleText = toWide(title);
    if (titleText.empty())
        titleText = toWide(_props->title);

    std::wstring messageText = toWide(message);

    MessageBoxW(_state->hwnd,
                messageText.c_str(),
                titleText.c_str(), flags);
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

void Dialog::add(std::shared_ptr<IChildControl> child)
{
    if (child == nullptr ||
        child->parent() != nullptr)
        return;

    auto it = std::find(_props->controls.begin(), _props->controls.end(), child);
    if (it != _props->controls.end())
        return;
    _props->controls.push_back(child);

    child->parent(shared_from_this());
    child->id(nextId());
    child->rebuild();
}

void Dialog::remove(std::shared_ptr<IChildControl> child)
{
    auto it = std::find(_props->controls.begin(), _props->controls.end(), child);
    if (it == _props->controls.end())
        return;
    // this will dispose of the child control
    child->id(0);
    child->parent(nullptr);
    _props->controls.erase(it);
}

std::vector<std::shared_ptr<IControl>> Dialog::controls() const
{
    auto r = std::vector<std::shared_ptr<IControl>>();
    for (auto child : _props->controls)
        r.push_back(child->control());
    return r;
}

void Dialog::add(std::shared_ptr<IChildDialog> child)
{
    if (child == nullptr ||
        child->parent() != nullptr)
        return;

    auto it = std::find(_props->dialogs.begin(), _props->dialogs.end(), child);
    if (it != _props->dialogs.end())
        return;
    _props->dialogs.push_back(child);

    child->parent(shared_from_this());
    child->id(nextId());
    child->rebuild();
}

void Dialog::remove(std::shared_ptr<IChildDialog> child)
{
    auto it = std::find(_props->dialogs.begin(), _props->dialogs.end(), child);
    if (it == _props->dialogs.end())
        return;
    //this will dispose of the dialog
    child->id(0);
    child->parent(nullptr);
    _props->dialogs.erase(it);
}

std::vector<ISharedDialog> Dialog::dialogs() const
{
    auto r = std::vector<ISharedDialog>();
    for (auto child : _props->dialogs)
        r.push_back(child->dialog());
    return r;
}

std::shared_ptr<IChildControl> Dialog::controlFromId(int id)
{
    if (id == 0)
        return nullptr;

    for (auto child : _props->controls)
        if (child->id() == id)
            return child;
    return nullptr;
}

void Dialog::redraw(bool drawChildren)
{
    if (_state->hwnd == NULL)
        return;

    RedrawWindow(_state->hwnd,
                 NULL,
                 0,
                 RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | (drawChildren ? RDW_ALLCHILDREN : 0) );
}

void Dialog::rebuild()
{
    destruct();

    // safety checks
    if (_props->id > 0 && _props->parent == nullptr)
        return;

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
    DLGTEMPLATE& dlg = *(DLGTEMPLATE*)&buf[0];

    dlg.style = styles();
    dlg.dwExtendedStyle = exStyles();
    dlg.x = 0; //(short)_props->p._x;
    dlg.y = 0; //(short)_props->p._y;
    dlg.cx = 0; //(short)_props->p._cx;
    dlg.cy = 0; //(short)_props->p._cy;

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
    if (_props->parent != nullptr)
        hwndParent = reinterpret_cast<HWND>(_props->parent->handle());

    // Create the dialog, passing private data struct
    auto hwnd = CreateDialogIndirectParam(GetModuleHandle(NULL),
                                          &dlg,
                                          hwndParent,
                                          &Dialog::staticWndProc, 0);

    if (hwnd == NULL)
        return;

    SetProp(hwnd, "this", this);
    _state->hwnd = hwnd;

    move(_props->p);
    resize(_props->p);

    for (auto& c : _props->controls)
    {
        c->rebuild();
    }

    for (auto& d : _props->dialogs)
    {
        d->rebuild();
    }

    DragAcceptFiles(_state->hwnd, _props->dropTarget);
    updateImage();
    updateTimer();

    if (_props->menu != nullptr)
        _props->menu->rebuild();

    // the dialog does not erase the background automatically
    if (_props->visible)
        redraw(true);

    // always fire the size event
    SizeEvent().invoke(shared_from_this());
}

void Dialog::destruct()
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
    unsigned int styles = DS_SETFONT | DS_SETFOREGROUND | DS_3DLOOK | WS_CLIPCHILDREN;

    if (_props->id > 0)
    {
        // a child dialog
        styles |= WS_CHILD | DS_CONTROL;
    }
    else
    {
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
    }

    if (!_props->enabled)
        styles |= WS_DISABLED;

    if (_props->visible)
        styles |= WS_VISIBLE;

    return styles;
}

unsigned int Dialog::exStyles() const
{
    unsigned int styles = 0;

    if (_props->id > 0)
    {
        // parent gets control notifications
        styles |= WS_EX_CONTROLPARENT;
    }
    else
    {

        if (_props->type == DialogType::Frameless)
            styles |= WS_EX_TOOLWINDOW;

        if (_props->type == DialogType::Tool)
            styles |= WS_EX_TOOLWINDOW;

        if (_props->showHelp &&
            _props->type != DialogType::Application &&
            _props->type != DialogType::Frameless)
            styles |= WS_EX_CONTEXTHELP;
    }

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
            if (lParam == 0)
            {
                if (HIWORD(wParam) == 0)
                {
                    // menu item
                    if (_props->menu != nullptr)
                    {
                        auto msg = dlg_message{wMsg, wParam, lParam};
                        _props->menu->notify(msg);
                        return msg.result;
                    }
                }
                else if (HIWORD(wParam) == 0)
                {
                    // accelerator

                }
                return 0;
            }

            auto child = controlFromId(id);
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
            auto child = controlFromId(pNmHdr->idFrom);
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
            auto child = controlFromId(id);
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

    case WM_DRAWITEM:
    case WM_MEASUREITEM:
    {
        auto hwnd = (HWND)wParam;
        if (hwnd != NULL)
        {
            auto id = (int)GetDlgCtrlID(hwnd);
            auto child = controlFromId(id);
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
        Point posPx((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
        Point posDu(posPx);
        toUnits(_state->hwnd, posDu);

        DLGCPP_CMSG("WM_MOVE: " <<
                    "x = "  << posDu.x() << " (" << posPx.x() << ") " <<
                    "y = " << posDu.y() << " (" << posPx.y() << ") " <<
                    "title = " + _props->title);

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
        toUnits(_state->hwnd, sizeDu);

        DLGCPP_CMSG("WM_SIZE: " <<
                    "width = "  << sizeDu.width() << " (" << sizePx.width() << ") " <<
                    "height = " << sizeDu.height() << " (" << sizePx.height() << ") " <<
                    "title = " + _props->title);

        _props->p.width(sizeDu.width());
        _props->p.height(sizeDu.height());
        SizeEvent().invoke(shared_from_this());
        break;
    }

    case WM_LBUTTONDOWN:
    {
        Point cursorPos(LOWORD(lParam), HIWORD(lParam));
        toUnits(hDlg, cursorPos);
        ClickEvent().invoke(shared_from_this(), MouseButton::Left, cursorPos);
        break;
    }

    case WM_LBUTTONDBLCLK:
    {
        Point cursorPos(LOWORD(lParam), HIWORD(lParam));
        toUnits(hDlg, cursorPos);
        DoubleClickEvent().invoke(shared_from_this(), MouseButton::Left, cursorPos);
        break;
    }

    case WM_MBUTTONDOWN:
    {
        Point cursorPos(LOWORD(lParam), HIWORD(lParam));
        toUnits(hDlg, cursorPos);
        ClickEvent().invoke(shared_from_this(), MouseButton::Middle, cursorPos);
        break;
    }

    case WM_MBUTTONDBLCLK:
    {
        Point cursorPos(LOWORD(lParam), HIWORD(lParam));
        toUnits(hDlg, cursorPos);
        DoubleClickEvent().invoke(shared_from_this(), MouseButton::Middle, cursorPos);
        break;
    }

    case WM_RBUTTONDOWN:
    {
        Point cursorPos(LOWORD(lParam), HIWORD(lParam));
        toUnits(hDlg, cursorPos);
        ClickEvent().invoke(shared_from_this(), MouseButton::Right, cursorPos);
        break;
    }

    case WM_RBUTTONDBLCLK:
    {
        Point cursorPos(LOWORD(lParam), HIWORD(lParam));
        toUnits(hDlg, cursorPos);
        DoubleClickEvent().invoke(shared_from_this(), MouseButton::Right, cursorPos);
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
        if (timerId > 0 && timerId == _props->timer.id)
        {
            TimerEvent().invoke(shared_from_this());
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
    auto child = controlFromId(id);

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
    // always return a brush
    bool usingSysDefault = false;
    if (_props->backColor == Color::Default)
    {
        usingSysDefault = true;
    }
    else if (_props->backColor == Color::None)
    {
        if (_props->parent != nullptr)
        {
            // use parent brush; the message will call this same function for the parent.
            auto hwndParent = reinterpret_cast<HWND>(_props->parent->handle());
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

    SetBkColor(hdc, (COLORREF)_props->backColor);
    return (LRESULT)_state->hbrBgColor;
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
    auto child = controlFromId(id);
    if (child == nullptr)
        return 0;

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

    if (bgColor == Color::None)
    {
        // use parent brush
        return onColorDlg(hdc);
    }

    // the child manages the background brush
    SetBkColor(hdc, (COLORREF)bgColor);
    return (LRESULT)child->state().hbrBack;
}

IEvent<ISharedDialog, MouseButton, Point>& Dialog::ClickEvent()
{
    return _props->clickEvent;
}

IEvent<ISharedDialog, MouseButton, Point>& Dialog::DoubleClickEvent()
{
    return _props->dblClickEvent;
}

IEvent<ISharedDialog, std::vector<std::string>>& Dialog::DropEvent()
{
    return _props->dropEvent;
}

IEvent<ISharedDialog>& Dialog::HelpEvent()
{
    return _props->helpEvent;
}

IEvent<ISharedDialog>& Dialog::MoveEvent()
{
    return _props->moveEvent;
}

IEvent<ISharedDialog>& Dialog::SizeEvent()
{
    return _props->sizeEvent;
}

IEvent<ISharedDialog>& Dialog::TimerEvent()
{
    return _props->timerEvent;
}
