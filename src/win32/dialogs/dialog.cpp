#include "controls/control_p.h"

#include "gfx/context_p.h"
#include "menus/menu_p.h"
#include "utility/convert.h"
#include "utility/key_mapper.h"
#include "utility/message.h"
#include "utility/string_encoder.h"

#include "dialog_p.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <CommCtrl.h>
#include <shellapi.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;
using namespace dlgcpp::gfx;
using namespace dlgcpp::menus;

DialogImpl::DialogImpl(
    IDialog& dialog,
    DialogType type,
    ISharedDialog parent) : _dialog(dialog)
{
    static bool init = false;
    if (!init)
    {
        InitCommonControls();
        init = true;
    }

    // note: the dialog cannot be built here as the shared_ptr is not ready.
    // this dialog has a parent; but is not a child of the parent.
    _type = type;
    _parent = parent;

    if (parent != nullptr)
    {
        // default position off-parent
        _p.x(parent->p().x());
        _p.y(parent->p().y());
    }
}

DialogImpl::~DialogImpl()
{
    destruct();

    if (_hbrBgColor != NULL)
    {
        DeleteObject(_hbrBgColor);
        _hbrBgColor = NULL;
    }

    if (_hImage != NULL)
    {
        DeleteObject(_hImage);
        _hImage = NULL;
    }
}

ISharedDialog DialogImpl::parent() const
{
    return _parent;
}

void DialogImpl::parent(ISharedDialog parent)
{
    if (_parent == parent)
        return;
    _parent = parent;
}

int DialogImpl::id() const
{
    return _id;
}

void DialogImpl::id(int value)
{
    _id = value;
}

int DialogImpl::idRange() const
{
    // no id range for dialogs, only for controls
    return 1;
}

int DialogImpl::nextId(int reservedRange)
{
    auto id = _nextId;
    _nextId += reservedRange;
    return id;
}

int DialogImpl::exec()
{
    if (_execRunning)
        return 0;

    show();
    if (_hwnd == NULL)
        return 0;

    // disable parent
    _execParentEnabled = false;
    if (_parent != nullptr)
    {
        _execParentEnabled = _parent->enabled();
        _parent->enabled(false);
    }

    _execRunning = true;

    int result = MessageProcessor::beginLoop(_hwnd);

    _execRunning = false;

    return result;
}

void DialogImpl::quit(int result)
{
    if (_execRunning)
    {
        // assumed our message loop is running
        PostQuitMessage(result);
    }
}

bool DialogImpl::enabled() const
{
    return _enabled;
}

void DialogImpl::enabled(bool value)
{
    if (_enabled == value)
        return;
    _enabled = value;
    if (_hwnd == NULL)
        return;

    EnableWindow(
        _hwnd,
        _enabled);
}

bool DialogImpl::visible() const
{
    return _visible;
}

void DialogImpl::visible(bool value)
{
    if (_visible == value)
        return;

    _visible = value;
    updateVisibility();
}

DisplayState DialogImpl::displayState() const
{
    return _displayState;
}

void DialogImpl::displayState(DisplayState value)
{
    if (_displayState == value)
        return;
    _displayState = value;

    updateDisplayState();
}

const Position& DialogImpl::p() const
{
    return _p;
}

void DialogImpl::p(const Position& p)
{
    if (_p == p)
        return;

    _p = p;
    updatePosition();
}

DialogType DialogImpl::type() const
{
    return _type;
}

bool DialogImpl::showHelp() const
{
    return _showHelp;
}

void DialogImpl::showHelp(bool value)
{
    _showHelp = value;

    if (_hwnd == NULL)
        return;

    // set extended style
    SetWindowLong(_hwnd, GWL_EXSTYLE, exStyles());
}

const std::string& DialogImpl::title() const
{
    return _title;
}

void DialogImpl::title(const std::string& value)
{
    if (_title == value)
        return;
    _title = value;
    if (_hwnd == NULL)
        return;

    SetWindowTextW(_hwnd,
        StringEncoder::toWide(_title).c_str());
}

const ImageSource& DialogImpl::image() const
{
    return _image;
}

void DialogImpl::image(const ImageSource& image)
{
    // must be an icon
    if (!image.isIcon)
        return;

    _image = image;
    updateIcon();
}

std::shared_ptr<Menu> DialogImpl::menu() const
{
    return _menu;
}

void DialogImpl::menu(std::shared_ptr<Menu> menu)
{
    if (_menu == menu)
        return;

    if (_hwnd != NULL)
        SetMenu(_hwnd, NULL);

    _menu = menu;

    if (_menu != nullptr)
    {
        // the menu will assign itself to the dialog
        _menu->impl()->id(MenuStartId);
        _menu->impl()->parent(_dialog.ptr());
        _menu->impl()->rebuild();
    }
}

Color DialogImpl::color() const
{
    return _backColor;
}

void DialogImpl::color(Color value)
{
    if (_backColor == value)
        return;

    _backColor = value;
    if (_hbrBgColor != NULL)
    {
        DeleteObject((HBRUSH)_hbrBgColor);
        _hbrBgColor = NULL;
    }

    // if using default, keep null
    if (value != Color::None &&
        value != Color::Default)
        _hbrBgColor = CreateSolidBrush((COLORREF)_backColor);

    redraw(true);
}

Cursor DialogImpl::cursor() const
{
    return _cursor;
}

void DialogImpl::cursor(Cursor value)
{
    _cursor = value;
}

bool DialogImpl::dropTarget() const
{
    return _dropTarget;
}

void DialogImpl::dropTarget(bool value)
{
    if (_dropTarget == value)
        return;
    _dropTarget = value;

    if (_hwnd == NULL)
        return;

    DragAcceptFiles(_hwnd, _dropTarget);
}

bool DialogImpl::mouseCapture() const
{
    return (_hwnd != NULL && GetCapture() == _hwnd);
}

void DialogImpl::mouseCapture(bool value)
{
    if (_hwnd == NULL)
        return;

    if (value)
        SetCapture(_hwnd);
    else
    {
        if (GetCapture() == _hwnd)
            ReleaseCapture();
    }
}

void* DialogImpl::handle() const
{
    return _hwnd;
}

void* DialogImpl::user() const
{
    return _user;
}

void DialogImpl::user(void* value)
{
    _user = value;
}

/// <summary>
/// Constructs and shows the dialog
/// </summary>
void DialogImpl::show()
{
    // ensure visible
    if (!_visible)
        visible(true);

    // ensure constructed
    if (_hwnd == NULL)
    {
        rebuild();

        // failed to construct
        if (_hwnd == NULL)
            return;
    }
}

/// <summary>
/// Close the dialog, removing it from view.
/// </summary>
void DialogImpl::close(int result)
{
    // we must re-enable parent before destroying this dialog.
    // if we don't do this then the parent recedes into the background.
    if (_execRunning &&
        _parent != nullptr)
    {
        _parent->enabled(_execParentEnabled);
    }

    destruct();
    quit(result);
}

void DialogImpl::setFocus()
{
    if (_hwnd == NULL)
        return;

    SetFocus(_hwnd);
}

void DialogImpl::bringToFront()
{
    if (_hwnd == NULL)
        return;

    SetWindowPos(_hwnd,
        HWND_TOP,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);
}

void DialogImpl::sendToBack()
{
    if (_hwnd == NULL)
        return;

    SetWindowPos(_hwnd,
        HWND_BOTTOM,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);
}

void DialogImpl::move(const Point& point)
{
    if (_p.point() == point)
        return;

    _p.x(point.x());
    _p.y(point.y());

    updatePosition();
}

void DialogImpl::resize(const Size& size)
{
    if (_p.size() == size)
        return;

    _p.width(size.width());
    _p.height(size.height());

    updatePosition();
}

void DialogImpl::center()
{
    // TODO: if child, center in parent
    if (_id > 0)
        return;

    // use dialog-independent mapping to units
    auto screenSize = Convert().toUnits(
        Size(
            GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN)));

    Point p(
        (screenSize.width() / 2) - (_p.width() / 2),
        (screenSize.height() / 2) - (_p.height() / 2));

    move(p);
}

void DialogImpl::message(
    const std::string& message,
    const std::string& title)
{
    // this function only supports OK button; see dlgcpp/dialogs/message for full implementation
    UINT flags = MB_ICONINFORMATION | MB_OK;

    std::wstring titleText = StringEncoder::toWide(title);
    if (titleText.empty())
        titleText = StringEncoder::toWide(_title);

    std::wstring messageText = StringEncoder::toWide(message);

    MessageBoxW(_hwnd,
        messageText.c_str(),
        titleText.c_str(), flags);
}

void DialogImpl::sendUserEvent(int param)
{
    if (_hwnd == NULL)
        return;
    PostMessage(_hwnd, WM_DLGCPP_USER, (WPARAM)param, 0);
}

void DialogImpl::timer(int timeout)
{
    if (_timer.id == 0)
        _timer.id = nextId();

    // if timeout is negative the timer is removed.
    _timer.timeout = timeout;
    updateTimer();
}

void DialogImpl::add(std::shared_ptr<Control> child)
{
    if (child == nullptr ||
        child->parent() != nullptr)
        return;

    auto it = std::find(_controls.begin(), _controls.end(), child);
    if (it != _controls.end())
        return;
    _controls.push_back(child);

    auto childImpl = child->impl();
    childImpl->parent(_dialog.ptr());
    childImpl->id(nextId(childImpl->idRange()));
    childImpl->rebuild();
}

void DialogImpl::remove(std::shared_ptr<Control> child)
{
    auto it = std::find(_controls.begin(), _controls.end(), child);
    if (it == _controls.end())
        return;
    // this will dispose of the child control
    auto childImpl = child->impl();
    childImpl->id(0);
    childImpl->parent(nullptr);
    _controls.erase(it);
}

std::vector<std::shared_ptr<Control>> DialogImpl::controls() const
{
    return _controls;
}

void DialogImpl::add(std::shared_ptr<Dialog> child)
{
    if (child == nullptr ||
        child->parent() != nullptr)
        return;

    auto it = std::find(_dialogs.begin(), _dialogs.end(), child);
    if (it != _dialogs.end())
        return;
    _dialogs.push_back(child);

    auto dialogImpl = child->impl();
    dialogImpl->parent(_dialog.ptr());
    dialogImpl->id(nextId());
    dialogImpl->rebuild();
}

void DialogImpl::remove(std::shared_ptr<Dialog> child)
{
    auto it = std::find(_dialogs.begin(), _dialogs.end(), child);
    if (it == _dialogs.end())
        return;

    //this will dispose of the dialog
    auto dialogImpl = child->impl();
    dialogImpl->id(0);
    dialogImpl->parent(nullptr);
    _dialogs.erase(it);
}

std::vector<std::shared_ptr<Dialog>> DialogImpl::dialogs() const
{
    return _dialogs;
}

void DialogImpl::redraw(bool drawChildren)
{
    if (_hwnd == NULL)
        return;

    RedrawWindow(_hwnd,
        NULL,
        0,
        RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | (drawChildren ? RDW_ALLCHILDREN : 0));
}

void DialogImpl::rebuild()
{
    MessageLocker sizeAndMoveLock(_inhibitSizeAndMoveMessages);

    destruct();

    // safety checks
    if (_id > 0 && _parent == nullptr)
    {
        DLGCPP_CERR("Called when no parent assigned but child id is assigned.");
        return;
    }

    // Use the Windows dialog font
    std::wstring fontFace = L"MS Shell Dlg";
    unsigned short fontSize = 8; // Pointsize
    std::wstring text;

    text = StringEncoder::toWide(_title);
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
    if (_parent != nullptr)
        hwndParent = reinterpret_cast<HWND>(_parent->handle());

    // Create the dialog, passing private data struct
    auto hwnd = CreateDialogIndirectParam(GetModuleHandle(NULL),
        &dlg,
        hwndParent,
        dialogWndProc, 0);

    if (hwnd == NULL)
        return;

    SetPropW(hwnd, L"this", this);
    _hwnd = hwnd;

    updatePosition();
    updateDisplayState();

    for (auto& c : _controls)
    {
        c->impl()->rebuild();
    }

    for (auto& d : _dialogs)
    {
        d->impl()->rebuild();
    }

    DragAcceptFiles(_hwnd, _dropTarget);
    updateIcon();
    updateTimer();

    if (_menu != nullptr)
        _menu->impl()->rebuild();

    updateVisibility();

    // always fire the size event
    SizeEvent().invoke(_dialog.ptr());
}

void DialogImpl::destruct()
{
    if (_hwnd == NULL)
        return;

    // it does not use EndDialog as we may be simulating a model dialog with exec()
    if (_timer.id > 0)
        KillTimer(_hwnd, _timer.id);
    DestroyWindow(_hwnd);
    _hwnd = NULL;
}

unsigned int DialogImpl::styles() const
{
    // not using DS_CENTER as center() is provided for this.
    unsigned int styles = DS_SETFONT | DS_SETFOREGROUND | DS_3DLOOK | WS_CLIPCHILDREN;

    if (_id > 0)
    {
        // a child dialog
        styles |= WS_CHILD | WS_CLIPSIBLINGS | DS_CONTROL;
    }
    else
    {
        switch (_type)
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
    }

    if (!_enabled)
        styles |= WS_DISABLED;

    return styles;
}

unsigned int DialogImpl::exStyles() const
{
    unsigned int styles = 0;

    if (_id > 0)
    {
        // parent gets control notifications
        styles |= WS_EX_CONTROLPARENT;
    }
    else
    {

        if (_type == DialogType::Frameless)
            styles |= WS_EX_TOOLWINDOW;

        if (_type == DialogType::Tool)
            styles |= WS_EX_TOOLWINDOW;

        if (_showHelp &&
            _type != DialogType::Application &&
            _type != DialogType::Frameless)
            styles |= WS_EX_CONTEXTHELP;
    }

    return styles;
}

LRESULT CALLBACK DialogImpl::dialogWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    auto pthis = reinterpret_cast<DialogImpl*>(GetPropW(hDlg, L"this"));

    if (pthis == nullptr)
        return FALSE;

    // wrap and transfer the message directly to the class.
    auto msg = DialogMessage{ wMsg, wParam, lParam, 0 ,0 };
    pthis->notify(msg);

    if (msg.dlgResult != 0)
    {
        // return the message result code, if any
        SetWindowLongPtr(hDlg, DWLP_MSGRESULT, msg.msgResult);
    }
    return msg.dlgResult;
}

// intercepts dialog messages and performs the default actions.
void DialogImpl::notify(DialogMessage& msg)
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
            if (lParam != 0)
            {
                // message sent from a control

                // locate the sender; we may need to use the HWND
                auto child = findControl(id, (HWND)lParam);

                if (child != nullptr)
                {
                    // wrap message and send to child for processing
                    // a result code is supported.
                    child->impl()->notify(msg);
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
                        _confirmEvent.invoke();
                        return;
                    }
                    else if (id == IDCANCEL)
                    {
                        // Cancel event (ESC)
                        _cancelEvent.invoke();
                        return;
                    }

                    // menu item
                    if (_menu != nullptr)
                    {
                        _menu->impl()->notify(msg);
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
        auto child = findControl(nmhdr.idFrom, nmhdr.hwndFrom);
        if (child != nullptr)
        {
            // wrap message and send to child for processing
            // a result code is supported.
            child->impl()->notify(msg);
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
            auto child = findControl(id);
            if (child != nullptr)
            {
                // wrap message and send to child for processing
                // a result code is supported.
                child->impl()->notify(msg);
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
            auto child = findControl(id);
            if (child != nullptr)
            {
                // wrap message and send to child for processing
                // a result code is supported.
                child->impl()->notify(msg);
                return;
            }
        }
        break;
    }

    case WM_KEYDOWN:
    {
        KeyboardEvent event;
        event.key = KeyMapper::ToKey(static_cast<UINT>(msg.wParam));
        KeyDownEvent().invoke(_dialog.ptr(), event);
        break;
    }
    case WM_KEYUP:
    {
        KeyboardEvent event;
        event.key = KeyMapper::ToKey(static_cast<UINT>(msg.wParam));
        KeyUpEvent().invoke(_dialog.ptr(), event);
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

        event.point = Convert(hDlg).toUnits(
            Point(
                LOWORD(lParam),
                HIWORD(lParam)));

        MouseDownEvent().invoke(_dialog.ptr(), event);
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

        event.point = Convert(hDlg).toUnits(
            Point(
                LOWORD(lParam),
                HIWORD(lParam)));

        MouseUpEvent().invoke(_dialog.ptr(), event);
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

        event.point = Convert(hDlg).toUnits(
            Point(
                LOWORD(lParam),
                HIWORD(lParam)));

        MouseDoubleClickEvent().invoke(_dialog.ptr(), event);
        break;
    }

    case WM_MOUSEMOVE:
    {
        MouseEvent event;
        event.button = wMsg == WM_LBUTTONDOWN ? MouseButton::Left : wMsg == WM_RBUTTONDOWN ? MouseButton::Right : MouseButton::Middle;

        event.point = Convert(hDlg).toUnits(
            Point(
                LOWORD(lParam),
                HIWORD(lParam)));

        MouseMoveEvent().invoke(_dialog.ptr(), event);
        break;
    }

    case WM_CAPTURECHANGED:
    {
        MouseCaptureLostEvent().invoke(_dialog.ptr());
        break;
    }

    case WM_SETCURSOR:
        onSetCursor(msg);
        break;

    case WM_MOVE:
    {
        if (_inhibitSizeAndMoveMessages.get())
            break;

        // only store the position when dialog is in "normal" display state.
        if (IsIconic(hDlg) || IsZoomed(hDlg))
            break;

        // translate using mapped value and store         
        Point posDu = Convert(_hwnd).toUnits(
            Point(
                (int)(short)LOWORD(lParam),
                (int)(short)HIWORD(lParam)));

        _p.x(posDu.x());
        _p.y(posDu.y());
        MoveEvent().invoke(_dialog.ptr());

        DLGCPP_CMSG("WM_MOVE: " <<
            "x = " << _p.x() << " " <<
            "y = " << _p.y() << " " <<
            "title = " + _title);

        break;
    }

    case WM_SIZE:
    {
        if (_inhibitSizeAndMoveMessages.get())
            break;

        auto state = (UINT)wParam;
        switch (state)
        {
        case SIZE_RESTORED:
            _displayState = DisplayState::Normal;
            break;
        case SIZE_MINIMIZED:
            _displayState = DisplayState::Minimized;
            break;
        case SIZE_MAXIMIZED:
            _displayState = DisplayState::Maximized;
            break;
        };

        if (state == SIZE_RESTORED ||
            state == SIZE_MAXIMIZED)
        {
            // only store the size when dialog is not minimized.
            // note: we have to capture maximized state, as it will affect any potential child positioning

            // translate using mapped value and store
            Size sizeDu = Convert(_hwnd).toUnits(
                Size(
                    (int)(short)LOWORD(lParam),
                    (int)(short)HIWORD(lParam)));

            _p.width(sizeDu.width());
            _p.height(sizeDu.height());
            SizeEvent().invoke(_dialog.ptr());
        }

        DLGCPP_CMSG("WM_SIZE: " <<
            "displayState = " << (int)_displayState << " "
            "width = " << _p.width() << " " <<
            "height = " << _p.height() << " " <<
            "title = " + _title);

        break;
    }

    case WM_ERASEBKGND:
        if (_paintEvent.count() > 0)
        {
            // drawing is handled in WM_PAINT
            msg.msgResult = TRUE;
            msg.dlgResult = TRUE;
        }
        break;

    case WM_PAINT:
    {
        if (_paintEvent.count() > 0)
        {
            auto ps = PAINTSTRUCT{};
            BeginPaint(_hwnd, &ps);

            auto context = std::make_shared<DrawingContext>(
                std::make_shared<DrawingContextImpl>(_hwnd, ps.hdc));

            _paintEvent.invoke(_dialog.ptr(), context);
            context->render();

            EndPaint(_hwnd, &ps);

            if (context->handled())
            {
                // no further drawing
                msg.msgResult = TRUE;
                msg.dlgResult = TRUE;
                return;
            }
        }

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
            files.push_back(StringEncoder::toBytes(wfile.data()));
        }
        if (!files.empty())
            DropEvent().invoke(_dialog.ptr(), files);
        break;
    }

    case WM_HELP:
    {
        HelpEvent().invoke(_dialog.ptr());
        break;
    }

    case WM_TIMER:
    {
        auto timerId = (int)wParam;
        if (timerId > 0 && timerId == _timer.id)
        {
            TimerEvent().invoke(_dialog.ptr());
        }
        break;
    }

    case WM_DLGCPP_USER:
    {
        UserEvent().invoke(_dialog.ptr(), (int)wParam);
        break;
    }
    }
}

void DialogImpl::onSetCursor(DialogMessage& msg)
{
    auto hwndChild = reinterpret_cast<HWND>(msg.wParam);
    auto child = findControl(0, hwndChild);

    auto cursor = Cursor::Default;
    if (child != nullptr)
        cursor = child->cursor();
    else
        cursor = _cursor;

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
        msg.msgResult = TRUE;
    }
}

void DialogImpl::onColorDlg(DialogMessage& msg)
{
    auto hdc = reinterpret_cast<HDC>(msg.wParam);

    // always return a brush
    bool usingSysDefault = false;
    if (_backColor == Color::Default)
    {
        usingSysDefault = true;
    }
    else if (_backColor == Color::None)
    {
        if (_parent != nullptr)
        {
            // use parent brush; the message will call this same function for the parent.
            auto hwndParent = reinterpret_cast<HWND>(_parent->handle());
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

    SetBkColor(hdc, (COLORREF)_backColor);
    msg.dlgResult = (LRESULT)_hbrBgColor;
}

void DialogImpl::onColorCtl(DialogMessage& msg)
{
    auto hdc = reinterpret_cast<HDC>(msg.wParam);
    auto hwndChild = reinterpret_cast<HWND>(msg.lParam);

    auto child = findControl(0, hwndChild);
    if (child == nullptr)
        return;

    // color handler. Returns a brush handle if required.

    auto colors = child->colors();
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
            switch (msg.wMsg)
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
                break;
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
    msg.dlgResult = (LRESULT)child->impl()->backgroundBrush();
}

std::shared_ptr<Control> DialogImpl::findControl(int id, HWND hwnd)
{
    // TODO: findControl performance impact
    // Use map<handle,Child> with all child hwnd / id > child for quick lookup

    //DLGCPP_CMSG("findControl: id = " << id << "  hwnd = " << hwnd);

    if (id > 0)
    {
        // by explicit identifier
        for (auto& child : _controls)
        {
            if (child->impl()->id() == id)
                return child;
        }
    }

    if (hwnd != NULL)
    {
        // by HWND identifier
        id = GetDlgCtrlID(hwnd);
        for (auto& child : _controls)
        {
            if (child->impl()->id() == id)
                return child;
        }

        // by HWND
        for (auto& child : _controls)
        {
            if ((HWND)child->impl()->isHandleEqual(hwnd))
                return child;
        }

        // by parent HWND 
        auto hwndParent = GetParent(hwnd);
        if (hwndParent != 0)
        {
            for (auto& child : _controls)
            {
                if ((HWND)child->impl()->isHandleEqual(hwndParent))
                    return child;
            }
        }
    }

    return nullptr;
}

IEvent<ISharedDialog>& DialogImpl::ConfirmEvent()
{
    return _confirmEvent;
}

IEvent<ISharedDialog>& DialogImpl::CancelEvent()
{
    return _cancelEvent;
}

IEvent<ISharedDialog>& DialogImpl::HelpEvent()
{
    return _helpEvent;
}

IEvent<ISharedDialog>& DialogImpl::MoveEvent()
{
    return _moveEvent;
}

IEvent<ISharedDialog>& DialogImpl::SizeEvent()
{
    return _sizeEvent;
}

IEvent<ISharedDialog, ISharedDrawingContext>& DialogImpl::PaintEvent()
{
    return _paintEvent;
}

IEvent<ISharedDialog>& DialogImpl::TimerEvent()
{
    return _timerEvent;
}

IEvent<ISharedDialog, KeyboardEvent>& DialogImpl::KeyDownEvent()
{
    return _keyDownEvent;
}

IEvent<ISharedDialog, KeyboardEvent>& DialogImpl::KeyUpEvent()
{
    return _keyUpEvent;
}

IEvent<ISharedDialog, MouseEvent>& DialogImpl::MouseDownEvent()
{
    return _mouseDownEvent;
}

IEvent<ISharedDialog, MouseEvent>& DialogImpl::MouseUpEvent()
{
    return _mouseUpEvent;
}

IEvent<ISharedDialog, MouseEvent>& DialogImpl::MouseMoveEvent()
{
    return _mouseMoveEvent;
}

IEvent<ISharedDialog, MouseEvent>& DialogImpl::MouseDoubleClickEvent()
{
    return _mouseDblClickEvent;
}

IEvent<ISharedDialog>& DialogImpl::MouseCaptureLostEvent()
{
    return _mouseCaptureLostEvent;
}

IEvent<ISharedDialog, std::vector<std::string>>& DialogImpl::DropEvent()
{
    return _dropEvent;
}

IEvent<ISharedDialog, int>& DialogImpl::UserEvent()
{
    return _userEvent;
}

void DialogImpl::updateDisplayState()
{
    if (_hwnd == NULL)
        return;

    // use SetWindowPlacement which does not show the dialog
    WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
    GetWindowPlacement(_hwnd, &wp);
    switch (_displayState)
    {
    case DisplayState::Normal:    wp.showCmd = SW_SHOWNORMAL;    break;
    case DisplayState::Minimized: wp.showCmd = SW_SHOWMINIMIZED; break;
    case DisplayState::Maximized: wp.showCmd = SW_SHOWMAXIMIZED; break;
    }
    SetWindowPlacement(_hwnd, &wp);
}

void DialogImpl::updateIcon()
{
    if (_hImage != NULL)
    {
        DeleteObject(_hImage);
        _hImage = NULL;
    }

    if (_hwnd == NULL)
        return;

    if (_image.id.empty())
    {
        SendMessage(_hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)NULL);
        SendMessage(_hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)NULL);
        return;
    }

    auto hInstRes = GetModuleHandle(NULL);
    auto imageType = (_image.isIcon ? IMAGE_ICON : IMAGE_BITMAP);
    _hImage = LoadImageW(hInstRes,
        StringEncoder::toWide(_image.id).c_str(),
        imageType,
        0,
        0,
        LR_DEFAULTSIZE | (_image.isFile ? LR_LOADFROMFILE : 0));
    if (_hImage == NULL)
        return;

    SendMessage(_hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)_hImage);
    SendMessage(_hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)_hImage);
}

void DialogImpl::updatePosition()
{
    if (_hwnd == NULL)
        return;

    Position posPx = toPixels(_p);

    // map the size dimensions to non-client
    RECT rc = { 0, 0, posPx.width(), posPx.height() };
    AdjustWindowRectEx(
        &rc,
        GetWindowLong(_hwnd, GWL_STYLE),
        GetMenu(_hwnd) != NULL,
        GetWindowLong(_hwnd, GWL_EXSTYLE));

    SetWindowPos(
        _hwnd,
        0,
        posPx.x(),
        posPx.y(),
        rc.right - rc.left,
        rc.bottom - rc.top,
        SWP_NOZORDER);
}

void DialogImpl::updateTimer()
{
    if (_hwnd == NULL)
        return;

    if (_timer.timeout > 0)
    {
        SetTimer(
            _hwnd,
            _timer.id,
            _timer.timeout,
            NULL);
    }
    else
    {
        if (_timer.id > 0)
            KillTimer(_hwnd, (UINT_PTR)_timer.id);
    }
}

void DialogImpl::updateVisibility()
{
    if (_hwnd == NULL)
        return;

    ShowWindow(
        _hwnd,
        _visible ? SW_SHOW : SW_HIDE);
}

Point DialogImpl::toPixels(const Point& point) const
{
    Point pointPx;
    if (_id > 0)
    {
        // child dialog; use parent client
        HWND hwndParent = (HWND)_parent->handle();
        pointPx = Convert(hwndParent).toPixels(point, true);
    }
    else
    {
        pointPx = Convert(_hwnd).toPixels(point, true);
    }
    return pointPx;
}

Size DialogImpl::toPixels(const Size& size) const
{
    Size sizePx;
    if (_id > 0)
    {
        // child dialog; use parent client
        HWND hwndParent = (HWND)_parent->handle();
        sizePx = Convert(hwndParent).toPixels(size, true);
    }
    else
    {
        sizePx = Convert(_hwnd).toPixels(size, true);
    }
    return sizePx;
}

Position DialogImpl::toPixels(const Position& pos) const
{
    Position posPx;
    if (_id > 0)
    {
        // child dialog; use parent client
        HWND hwndParent = (HWND)_parent->handle();
        posPx = Convert(hwndParent).toPixels(pos, true);
    }
    else
    {
        posPx = Convert(_hwnd).toPixels(pos, true);
    }
    return posPx;
}
