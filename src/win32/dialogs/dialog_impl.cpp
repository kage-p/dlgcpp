#include "controls/control_impl.h"

#include "gfx/context_impl.h"
#include "menus/menu_impl.h"
#include "utility/convert.h"
#include "utility/key_mapper.h"
#include "utility/message.h"
#include "utility/string_encoder.h"

#include "dialog_impl.h"

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

DialogImpl::DialogImpl()
{
    static bool init = false;
    if (!init)
    {
        InitCommonControls();
        init = true;
    }
    _handle.reset(nullptr, nullptr, []() { return 0; }, "handle");
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

void DialogImpl::owner(IDialog* dialog)
{
    // connect properties to internal event handlers
    _dialog = dialog;
    _dialog->enabled().event() += [&](auto) { onEnabledChanged(); };
    _dialog->visible().event() += [&](auto) { onVisibleChanged(); };
    _dialog->type().event() += [&](auto) { rebuild(); };
    _dialog->title().event() += [&](auto) { onTitleChanged(); };
    _dialog->image().event() += [&](auto) { onImageChanged(); };
    _dialog->display().event() += [&](auto) { onDisplayChanged(); };
    _dialog->p().event() += [&](auto) { onPosChanged(); };
    _dialog->menu().event() += [&](auto) { onMenuChanged(); };
    _dialog->color().event() += [&](auto) { onColorChanged(); };
    _dialog->active().event() += [&](auto) { onActiveChanged(); };
    _dialog->dropTarget().event() += [&](auto) { onDropTargetChanged(); };
    _dialog->mouseCapture().event() += [&](auto) { onMouseCaptureChanged(); };
    _dialog->showHelp().event() += [&](auto) { onShowHelpChanged(); };
}

IReadOnlyProperty<void*, int>& DialogImpl::handle()
{
    return _handle;
}

HWND DialogImpl::hwnd() const
{
    return _hwnd;
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
    if (!_dialog->parent().expired())
    {
        auto parent = _dialog->parent().lock();
        _execParentEnabled = parent->enabled();
        parent->enabled() = false;
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

void DialogImpl::onEnabledChanged()
{
    if (_hwnd == NULL)
        return;

    EnableWindow(
        _hwnd,
        _dialog->enabled());
}

void DialogImpl::onVisibleChanged()
{
    updateVisibility();
}

void DialogImpl::onDisplayChanged()
{
    updateDisplayState();
}

void DialogImpl::onPosChanged()
{
    updatePosition();
}

void DialogImpl::onShowHelpChanged()
{
    if (_hwnd == NULL)
        return;

    // set extended style
    SetWindowLong(_hwnd, GWL_EXSTYLE, exStyles());
}

void DialogImpl::onTitleChanged()
{
    if (_hwnd == NULL)
        return;

    SetWindowTextW(_hwnd,
        StringEncoder::toWide(_dialog->title()).c_str());
}

void DialogImpl::onImageChanged()
{
    // must be an icon
    if (!_dialog->image()->isIcon())
    {
        DLGCPP_CERR("Dialog image must be icon type");
        return;
    }

    //_image = image;
    updateIcon();
}

void DialogImpl::onMenuChanged()
{
    if (_hwnd != NULL)
    {
        SetMenu(_hwnd, NULL);
        DrawMenuBar(_hwnd);
        RedrawWindow(_hwnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
    }

    if (!_dialog->menu().empty())
    {
        // prepare the menu
        auto& menu = _dialog->menu().value();

        if (auto impl = dynamic_cast<Dialog*>(_dialog))
        {
            auto weakDialog = impl->weak_from_this();
            menu->impl()->parent(weakDialog);
        }
        else
        {
            DLGCPP_CERR("onMenuChanged: _dialog is not a Dialog");
            return;
        }

        menu->impl()->id(MenuStartId);
        menu->impl()->rebuild();

        if (_hwnd != NULL)
        {
            SetMenu(_hwnd, menu->impl()->handle());
            DrawMenuBar(_hwnd);
            RedrawWindow(_hwnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
        }
    }
}

void DialogImpl::onColorChanged()
{
    if (_hbrBgColor != NULL)
    {
        DeleteObject((HBRUSH)_hbrBgColor);
        _hbrBgColor = NULL;
    }

    // if using default, keep null
    auto backColor = _dialog->color().value();
    if (backColor != Color::None &&
        backColor != Color::Default)
        _hbrBgColor = CreateSolidBrush((COLORREF)backColor);

    redraw(true);
}

void DialogImpl::onActiveChanged()
{
    if (_hwnd == NULL)
    {
        _dialog->active() = false;
        return;
    }

    if (_dialog->active())
    {
        if (GetActiveWindow() != _hwnd)
        {
            SetActiveWindow(_hwnd);

            if (GetActiveWindow() != _hwnd)
            {
                // failed to activate
                _dialog->active() = false;
            }
        }
    }
    else
    {
        if (GetActiveWindow() == _hwnd)
        {
            // setting inactive state is not supported
            _dialog->active() = true;
        }
    }
}

void DialogImpl::onDropTargetChanged()
{
    if (_hwnd == NULL)
        return;

    DragAcceptFiles(_hwnd, _dialog->dropTarget());
}

void DialogImpl::onMouseCaptureChanged()
{
    if (_hwnd == NULL)
    {
        _dialog->mouseCapture() = false;
        return;
    }

    if (_dialog->mouseCapture())
    {
        SetCapture(_hwnd);
    }
    else
    {
        if (GetCapture() == _hwnd)
            ReleaseCapture();
    }
}

/// <summary>
/// Constructs and shows the dialog
/// </summary>
void DialogImpl::show()
{
    // ensure visible
    if (!_dialog->visible())
        _dialog->visible() = true;

    // ensure constructed
    if (_hwnd == nullptr)
    {
        build();
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
        !_dialog->parent().expired())
    {
        auto parent = _dialog->parent().lock();
        parent->enabled() = _execParentEnabled;
    }

    destruct();
    quit(result);
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

void DialogImpl::center()
{
    // TODO: Move to dialog - need utility fn to get screen size

    // TODO: if child, center in parent
    if (_id > 0)
        return;

    // use dialog-independent mapping to units
    auto screenSize = Convert().toUnits(
        Size(
            GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN)));

    Size dlgSize = _dialog->p()->size();
    Point p(
        (screenSize.width() / 2) - (dlgSize.width() / 2),
        (screenSize.height() / 2) - (dlgSize.height() / 2));

    _dialog->move(p);
}

void DialogImpl::message(
    const std::string& message,
    const std::string& title)
{
    // this function only supports OK button; see dlgcpp/dialogs/message for full implementation
    UINT flags = MB_ICONINFORMATION | MB_OK;

    std::wstring titleText = StringEncoder::toWide(title);
    if (titleText.empty())
        titleText = StringEncoder::toWide(_dialog->title());

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

void DialogImpl::add(ISharedControl control)
{
    auto controlImpl = control->impl();
    controlImpl->id(nextId(controlImpl->idRange()));

    if (_hwnd != nullptr)
    {
        controlImpl->build();
    }
}

void DialogImpl::remove(ISharedControl control)
{
    // this will destroy the control
    auto controlImpl = control->impl();
    controlImpl->id(0);
    controlImpl->destruct();
}

void DialogImpl::add(ISharedDialog dialog)
{
    auto dialogImpl = dialog->impl();
    dialogImpl->id(nextId());

    if (_hwnd != nullptr)
        dialogImpl->build();
}

void DialogImpl::remove(ISharedDialog dialog)
{
    // this will destroy the dialog
    auto dialogImpl = dialog->impl();
    dialogImpl->id(0);
    dialogImpl->destruct();
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


/// <summary>
/// Destroys and rebuilds (recreates) the dialog.
/// Does nothing if the control was not already built.
/// </summary>
void DialogImpl::rebuild()
{
    if (_hwnd == NULL)
        return;

    destruct();
    build();
}

/// <summary>
/// Creates the dialog
/// Does nothing if the dialog is already created; use rebuild() instead.
/// </summary>
void DialogImpl::build()
{
    // safety checks
    if (_id > 0 &&
        _dialog->parent().expired())
    {
        DLGCPP_CERR("Called when no parent assigned but child id is assigned.");
        return;
    }

    // Use the Windows dialog font
    std::wstring fontFace = L"MS Shell Dlg";
    unsigned short fontSize = 8; // Pointsize
    std::wstring text;

    text = StringEncoder::toWide(_dialog->title());
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
    if (!_dialog->parent().expired())
    {
        auto parent = _dialog->parent().lock();
        hwndParent = reinterpret_cast<HWND>(parent->handle().value());
    }

    // Create the dialog, passing private data struct
    auto hwnd = CreateDialogIndirectParam(GetModuleHandle(NULL),
        &dlg,
        hwndParent,
        dialogWndProc,
        reinterpret_cast<LPARAM>(this));

    if (hwnd == NULL)
    {
        DLGCPP_CERR("Dialog build failed. Error " << GetLastError());
        return;
    }

    if (!_dialog->menu().empty())
    {
        // prepare the menu now before updating the dialog position.
        // setting the menu triggers a size change, so prevent this with locks.
        MessageLocker sizeAndMoveLock(_inhibitSizeAndMoveMessages);

        auto menu = _dialog->menu().value();
        menu->impl()->rebuild();
        SetMenu(hwnd, menu->impl()->handle());
    }

    updatePosition();
    updateDisplayState();

    const auto& controls = _dialog->controls();
    for (auto& c : controls)
    {
        c->impl()->build();
    }

    const auto& dialogs = _dialog->dialogs();
    for (auto& d : dialogs)
    {
        d->impl()->build();
    }

    DragAcceptFiles(_hwnd, _dialog->dropTarget().value());
    updateIcon();
    updateTimer();

    updateVisibility();

    // fire create event
    _dialog->CreateEvent().invoke();

    // always fire the size event
    _dialog->SizeEvent().invoke();
}

void DialogImpl::destruct()
{
    if (_hwnd == NULL)
        return;

    // it does not use EndDialog as we may be simulating a model dialog with exec()
    if (_timer.id > 0)
        KillTimer(_hwnd, _timer.id);

    // remove all child controls and dialogs
    const auto& controls = _dialog->controls();
    for (auto& c : controls)
    {
        c->impl()->destruct();
    }

    const auto& dialogs = _dialog->controls();
    for (auto& d : dialogs)
    {
        d->impl()->destruct();
    }

    DestroyWindow(_hwnd);

    _hwnd = nullptr;
    _handle = nullptr;
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
        // note: WS_CAPTION is WS_BORDER+WS_DLGFRAME
        switch (_dialog->type())
        {
        case DialogType::Application:
            styles |= WS_OVERLAPPED | WS_BORDER | WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
            break;
        case DialogType::Popup:
            styles |= WS_OVERLAPPED | WS_BORDER | WS_DLGFRAME | WS_SYSMENU | WS_POPUP;
            if (!_dialog->parent().expired())
            {
                styles |= DS_MODALFRAME; // no icon
            }
            break;
        case DialogType::Frameless:
            styles |= WS_POPUP;
            break;
        case DialogType::Tool:
            styles |= WS_POPUP | WS_BORDER | WS_DLGFRAME | WS_SYSMENU | DS_MODALFRAME;
            break;
        };
    }

    if (!_dialog->enabled())
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

        if (_dialog->type() == DialogType::Frameless ||
            _dialog->type() == DialogType::Tool)
        {
            styles |= WS_EX_TOOLWINDOW;
        }

        if (_dialog->showHelp() &&
            _dialog->type() != DialogType::Application &&
            _dialog->type() != DialogType::Frameless)
            styles |= WS_EX_CONTEXTHELP;
    }

    return styles;
}

LRESULT CALLBACK DialogImpl::dialogWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    if (wMsg == WM_INITDIALOG)
    {
        // store the 'this' pointer for later use
        auto pthis = reinterpret_cast<DialogImpl*>(lParam);
        SetPropW(hDlg, L"this", (HANDLE)pthis);
        pthis->_hwnd = hDlg;
        pthis->_handle = (void*)hDlg;
    }

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
    auto hDlg = _hwnd;
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
                        _dialog->ConfirmEvent().invoke();
                        return;
                    }
                    else if (id == IDCANCEL)
                    {
                        // Cancel event (ESC)
                        _dialog->CancelEvent().invoke();
                        return;
                    }

                    // menu item
                    if (_dialog->menu() != nullptr)
                    {
                        _dialog->menu().value()->impl()->notify(msg);
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
        _dialog->KeyDownEvent().invoke(event);
        break;
    }
    case WM_KEYUP:
    {
        KeyboardEvent event;
        event.key = KeyMapper::ToKey(static_cast<UINT>(msg.wParam));
        _dialog->KeyUpEvent().invoke(event);
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

        _dialog->MouseDownEvent().invoke(event);
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

        _dialog->MouseUpEvent().invoke(event);
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

        _dialog->MouseDoubleClickEvent().invoke(event);
        break;
    }

    case WM_MOUSEMOVE:
    {
        MouseEvent event;

        if (wParam & MK_LBUTTON)
            event.button = MouseButton::Left;
        else if (wParam & MK_RBUTTON)
            event.button = MouseButton::Right;
        else if (wParam & MK_MBUTTON)
            event.button = MouseButton::Middle;
        else
            event.button = MouseButton::None;

        event.point = Convert(hDlg).toUnits(
            Point(
                LOWORD(lParam),
                HIWORD(lParam)));

        _dialog->MouseMoveEvent().invoke(event);
        break;
    }

    case WM_CAPTURECHANGED:
    {
        // lost capture
        _dialog->mouseCapture() = false;
        _dialog->MouseCaptureLostEvent().invoke();
        break;
    }

    case WM_SETCURSOR:
        handleSetCursor(msg);
        break;

    case WM_ACTIVATE:
    {
        // update active property
        _dialog->active() = (wParam != WA_INACTIVE);
        break;
    }

    case WM_MOVE:
    case WM_SIZE:
    {
        storePosition();
        break;
    }

    case WM_ERASEBKGND:
        if (_dialog->PaintEvent().count() > 0)
        {
            // drawing is handled in WM_PAINT
            msg.msgResult = TRUE;
            msg.dlgResult = TRUE;
        }
        break;

    case WM_PAINT:
    {
        if (_dialog->PaintEvent().count() > 0)
        {
            auto ps = PAINTSTRUCT{};
            BeginPaint(_hwnd, &ps);

            auto context = std::make_shared<DrawingContext>(
                std::make_shared<DrawingContextImpl>(_hwnd, ps.hdc));

            _dialog->PaintEvent().invoke(context);
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
        handleCtlColorDlg(msg);
        break;

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORSCROLLBAR:
        handleCtlColor(msg);
        break;

    case WM_SYSCOMMAND:
        if ((wParam & 0xFFF0) == SC_CLOSE)
        {
            // user pressed close [X] button or menu
            CloseDialogEvent event;
            _dialog->CloseEvent().invoke(event);
            if (event.cancel)
                return;

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

        DropFilesEvent files;
        for (int i = 0; i < fileCount; i++)
        {
            std::wstring wfile(MAX_PATH, 0x0);
            if (DragQueryFileW(hDrop, i, &wfile[0], (UINT)wfile.size()) == 0)
                continue;
            files.push_back(StringEncoder::toBytes(wfile.data()));
        }
        if (!files.empty())
            _dialog->DropEvent().invoke(files);
        break;
    }

    case WM_HELP:
    {
        _dialog->HelpEvent().invoke();
        break;
    }

    case WM_TIMER:
    {
        auto timerId = (int)wParam;
        if (timerId > 0 && timerId == _timer.id)
        {
            _dialog->TimerEvent().invoke();
        }
        break;
    }

    case WM_DLGCPP_USER:
    {
        _dialog->UserEvent().invoke((int)wParam);
        break;
    }

    case WM_DESTROY:
    {
        // dialog is being destroyed
        _dialog->DestroyEvent().invoke();
        break;
    }
    }
}

void DialogImpl::storePosition()
{
    if (_hwnd == NULL)
        return;

    if (_inhibitSizeAndMoveMessages.get())
    {
        return;
    }

    Position p = _dialog->p();
    bool sendMoveEvent = false;
    bool sendSizeEvent = false;

    // save state first
    auto display = _dialog->display().value();
    if (IsIconic(_hwnd))
        display = DisplayState::Minimized;
    else if (IsZoomed(_hwnd))
        display = DisplayState::Maximized;
    else
        display = DisplayState::Normal;

    // only store the x/y position when dialog is in "normal" display state.
    if (display == DisplayState::Normal)
    {
        RECT rc;
        GetWindowRect(_hwnd, &rc);
        POINT px{ rc.left,rc.top };

        if (_id > 0 &&
            !_dialog->parent().expired())
        {
            // map to parent
            auto parent = _dialog->parent().lock();
            auto hwndParent = reinterpret_cast<HWND>(parent->handle().value());
            ScreenToClient(hwndParent, &px);
        }

        // translate using mapped value and store         
        const Point pointDu(Convert(_hwnd).toUnits(Point(px.x, px.y)));

        sendMoveEvent = (p.point() != pointDu);
        p.x(pointDu.x());
        p.y(pointDu.y());
    }

    // only store the size when dialog is not minimized.
    // note: we have to capture maximized state, as it will affect any potential child positioning
    if (display == DisplayState::Normal ||
        display == DisplayState::Maximized)
    {
        RECT rcClient;
        GetClientRect(_hwnd, &rcClient);
        int pxW = rcClient.right - rcClient.left;
        int pxH = rcClient.bottom - rcClient.top;

        // MapDialogRect maps DLUs -> pixels. Use a large DU rect to derive the scale,
        // then invert it to convert pixels -> DLUs.
        RECT map = { 0, 0, 1000, 1000 };
        if (!MapDialogRect(_hwnd, &map))
        {
            // handle error
            return;
        }
        if (map.right == 0 || map.bottom == 0)
            return; // avoid divide-by-zero

        int duW = MulDiv(pxW, 1000, map.right);
        int duH = MulDiv(pxH, 1000, map.bottom);

        // Store usable size (what controls can use)
        Size sizeDu(duW, duH);
        sendSizeEvent = (p.size() != sizeDu);
        p.width(sizeDu.width());
        p.height(sizeDu.height());

    }

#if 0
    DLGCPP_CMSG(
        "display = " << (int)display << " " <<
        "x = " << p.x() << " " <<
        "y = " << p.y() << " " <<
        "p.width = " << p.width() << " " <<
        "p.height = " << p.height() << " " <<
        "title = " << _dialog->title().value());
#endif

    _dialog->display() = display;
    _dialog->p() = p;

    if (sendMoveEvent)
        _dialog->MoveEvent().invoke();
    if (sendSizeEvent)
        _dialog->SizeEvent().invoke();
}

void DialogImpl::handleSetCursor(DialogMessage& msg)
{
    auto hwndChild = reinterpret_cast<HWND>(msg.wParam);
    auto child = findControl(0, hwndChild);

    auto cursor = Cursor::Default;
    if (child != nullptr)
        cursor = child->cursor();
    else
        cursor = _dialog->cursor();

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

void DialogImpl::handleCtlColorDlg(DialogMessage& msg)
{
    auto hdc = reinterpret_cast<HDC>(msg.wParam);

    // always return a brush
    Color backColor = _dialog->color().value();
    bool usingSysDefault = false;
    if (backColor == Color::Default)
    {
        usingSysDefault = true;
    }
    else if (backColor == Color::None)
    {
        if (!_dialog->parent().expired())
        {
            // use parent brush; the message will call this same function for the parent.
            auto parent = _dialog->parent().lock();
            auto hwndParent = reinterpret_cast<HWND>(parent->handle().value());
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

    SetBkColor(hdc, (COLORREF)backColor);
    msg.dlgResult = (LRESULT)_hbrBgColor;
}

void DialogImpl::handleCtlColor(DialogMessage& msg)
{
    auto hdc = reinterpret_cast<HDC>(msg.wParam);
    auto hwndChild = reinterpret_cast<HWND>(msg.lParam);

    auto child = findControl(0, hwndChild);
    if (child == nullptr)
        return;

    // color handler. Returns a brush handle if required.

    auto colors = child->colors().value();
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
        handleCtlColorDlg(msg);
        return;
    }

    // the child manages the background brush
    SetBkColor(hdc, (COLORREF)bgColor);
    msg.dlgResult = (LRESULT)child->impl()->backgroundBrush();
}

ISharedControl DialogImpl::findControl(int id, HWND hwnd)
{
    // TODO: findControl performance impact
    // Use map<handle,Child> with all child hwnd / id > child for quick lookup

    //DLGCPP_CMSG("findControl: id = " << id << "  hwnd = " << hwnd);

    const auto& controls = _dialog->controls();

    if (id > 0)
    {
        // by explicit identifier
        for (auto& child : controls)
        {
            if (child->impl()->id() == id)
                return child;
        }
    }

    if (hwnd != NULL)
    {
        // by HWND identifier
        id = GetDlgCtrlID(hwnd);
        for (auto& child : controls)
        {
            if (child->impl()->id() == id)
                return child;
        }

        // by HWND
        for (auto& child : controls)
        {
            if ((HWND)child->impl()->isHandleEqual(hwnd))
                return child;
        }

        // by parent HWND 
        auto hwndParent = GetParent(hwnd);
        if (hwndParent != 0)
        {
            for (auto& child : controls)
            {
                if ((HWND)child->impl()->isHandleEqual(hwndParent))
                    return child;
            }
        }
    }

    return nullptr;
}

void DialogImpl::updateDisplayState()
{
    if (_hwnd == NULL)
        return;

    // use SetWindowPlacement which does not show the dialog
    WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
    GetWindowPlacement(_hwnd, &wp);
    switch (_dialog->display())
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

    const auto& image = _dialog->image().value();
    if (image.id().empty())
    {
        SendMessage(_hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)NULL);
        SendMessage(_hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)NULL);
        return;
    }

    auto hInstRes = GetModuleHandle(NULL);
    auto imageType = (image.isIcon() ? IMAGE_ICON : IMAGE_BITMAP);

    _hImage = LoadImageW(hInstRes,
        StringEncoder::toWide(image.id()).c_str(),
        imageType,
        0,
        0,
        LR_DEFAULTSIZE | (image.isFile() ? LR_LOADFROMFILE : 0));

    if (_hImage == NULL)
        return;

    SendMessage(_hwnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)_hImage);
    SendMessage(_hwnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)_hImage);
}

void DialogImpl::updatePosition()
{
    if (_inhibitSizeAndMoveMessages.get())
        return;
    MessageLocker sizeAndMoveLock(_inhibitSizeAndMoveMessages);

    if (_hwnd == NULL)
        return;
    if (_dialog->display() != DisplayState::Normal)
        return;

    auto p = _dialog->p().value();

    // 1. Convert just the size (client width/height in DLUs pixels)
    RECT sizeRc = { 0, 0, p.width(), p.height() };
    MapDialogRect(_hwnd, &sizeRc);
    int clientW = sizeRc.right - sizeRc.left;
    int clientH = sizeRc.bottom - sizeRc.top;

    // 2. Convert just the position (x/y DLUs pixels)
    RECT posRc = { p.x(), p.y(), p.x(), p.y() };
    MapDialogRect(_hwnd, &posRc);
    int pxX = posRc.left;
    int pxY = posRc.top;

    // 3. Expand client size to full window rect
    RECT wndRc = { 0, 0, clientW, clientH };
    AdjustWindowRectEx(
        &wndRc,
        GetWindowLong(_hwnd, GWL_STYLE),
        GetMenu(_hwnd) != NULL,
        GetWindowLong(_hwnd, GWL_EXSTYLE));

    int winW = wndRc.right - wndRc.left;
    int winH = wndRc.bottom - wndRc.top;

    SetWindowPos(
        _hwnd,
        0,
        pxX,
        pxY,
        winW,
        winH,
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
    else if (_timer.id > 0)
    {
        KillTimer(_hwnd, (UINT_PTR)_timer.id);
    }
}

void DialogImpl::updateVisibility()
{
    if (_hwnd == NULL)
        return;

    ShowWindow(
        _hwnd,
        _dialog->visible() ? SW_SHOW : SW_HIDE);

    if (GetActiveWindow() == _hwnd)
    {
        // ensure active state is correct
        _dialog->active() = true;
    }
}

Point DialogImpl::toPixels(const Point& point) const
{
    Point pointPx;
    if (_id > 0 &&
        !_dialog->parent().expired())
    {
        // child dialog; use parent client
        auto parent = _dialog->parent().lock();
        HWND hwndParent = (HWND)parent->handle().value();
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
    if (_id > 0 &&
        !_dialog->parent().expired())
    {
        // child dialog; use parent client
        auto parent = _dialog->parent().lock();
        HWND hwndParent = (HWND)parent->handle().value();
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
    if (_id > 0 &&
        !_dialog->parent().expired())
    {
        // child dialog; use parent client
        auto parent = _dialog->parent().lock();
        HWND hwndParent = (HWND)parent->handle().value();
        posPx = Convert(hwndParent).toPixels(pos, true);
    }
    else
    {
        posPx = Convert(_hwnd).toPixels(pos, true);
    }
    return posPx;
}
