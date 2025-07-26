#include "dlgcpp/dialogs/dialog.h"
#include "message_p.h"
#include "utility/string.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

MessageDialogImpl::MessageDialogImpl(
    MessageDialog& messageDialog,
    ISharedDialog parent) :
    _messageDialog(messageDialog),
    _parent(parent)
{
}

const std::string& MessageDialogImpl::title() const
{
    return _title;
}

void MessageDialogImpl::title(const std::string& value)
{
    _title = value;
}

const std::string& MessageDialogImpl::message() const
{
    return _message;
}

void MessageDialogImpl::message(const std::string& value)
{
    _message = value;
}

MessageDialogButtonGroup MessageDialogImpl::buttons() const
{
    return _buttons;
}

void MessageDialogImpl::buttons(MessageDialogButtonGroup value)
{
    _buttons = value;
}

MessageDialogIcon MessageDialogImpl::icon() const
{
    return _icon;
}

void MessageDialogImpl::icon(MessageDialogIcon value)
{
    _icon = value;
}

MessageDialogButton MessageDialogImpl::defaultButton() const
{
    return _defaultButton;
}

void MessageDialogImpl::defaultButton(MessageDialogButton value)
{
    _defaultButton = value;
}

bool MessageDialogImpl::showHelp() const
{
    return _showHelp;
}

void MessageDialogImpl::showHelp(bool value)
{
    _showHelp = value;
}

MessageDialogButton MessageDialogImpl::show()
{
    auto hwndParent = (HWND)HWND_DESKTOP;
    if (_parent != nullptr)
        hwndParent = reinterpret_cast<HWND>(_parent->handle());

    UINT flags = 0;

    switch (_icon)
    {
    case MessageDialogIcon::Warning:
        flags |= MB_ICONWARNING;
        break;
    case MessageDialogIcon::Error:
        flags |= MB_ICONERROR;
        break;
    case MessageDialogIcon::Question:
        flags |= MB_ICONQUESTION;
        break;
    case MessageDialogIcon::Information:
    default:
        flags |= MB_ICONINFORMATION;
        break;
    };

    switch (_buttons)
    {
    case MessageDialogButtonGroup::OkCancel:
        flags |= MB_OKCANCEL;
        if (_defaultButton == MessageDialogButton::Ok)
            flags |= MB_DEFBUTTON1;
        else if (_defaultButton == MessageDialogButton::Cancel)
            flags |= MB_DEFBUTTON2;
        break;
    case MessageDialogButtonGroup::AbortRetryIgnore:
        flags |= MB_ABORTRETRYIGNORE;
        if (_defaultButton == MessageDialogButton::Abort)
            flags |= MB_DEFBUTTON1;
        else if (_defaultButton == MessageDialogButton::Retry)
            flags |= MB_DEFBUTTON2;
        else if (_defaultButton == MessageDialogButton::Ignore)
            flags |= MB_DEFBUTTON3;
        break;
    case MessageDialogButtonGroup::YesNoCancel:
        flags |= MB_YESNOCANCEL;
        if (_defaultButton == MessageDialogButton::Yes)
            flags |= MB_DEFBUTTON1;
        else if (_defaultButton == MessageDialogButton::No)
            flags |= MB_DEFBUTTON2;
        else if (_defaultButton == MessageDialogButton::Cancel)
            flags |= MB_DEFBUTTON3;
        break;
    case MessageDialogButtonGroup::YesNo:
        flags |= MB_YESNO;
        if (_defaultButton == MessageDialogButton::Yes)
            flags |= MB_DEFBUTTON1;
        else if (_defaultButton == MessageDialogButton::No)
            flags |= MB_DEFBUTTON2;
        break;
    case MessageDialogButtonGroup::RetryCancel:
        flags |= MB_RETRYCANCEL;
        if (_defaultButton == MessageDialogButton::Retry)
            flags |= MB_DEFBUTTON1;
        else if (_defaultButton == MessageDialogButton::Cancel)
            flags |= MB_DEFBUTTON2;
        break;
    case MessageDialogButtonGroup::Ok:
    default:
        flags |= MB_OK;
        if (_defaultButton == MessageDialogButton::Ok)
            flags |= MB_DEFBUTTON1;
        break;
    }

    if (_showHelp)
        flags |= MB_HELP;

    auto titleText = toWide(_title);
    if (titleText.empty() && _parent != nullptr)
        titleText = toWide(_parent->title());

    auto messageText = toWide(_message);

    auto buttonId = MessageBoxW(hwndParent,
        messageText.c_str(),
        titleText.c_str(),
        flags);

    // translate result
    MessageDialogButton result = MessageDialogButton::Undefined;
    switch (buttonId)
    {
    case IDABORT:
        result = MessageDialogButton::Abort;
        break;
    case IDCANCEL:
        result = MessageDialogButton::Cancel;
        break;
    case IDIGNORE:
        result = MessageDialogButton::Ignore;
        break;
    case IDNO:
        result = MessageDialogButton::No;
        break;
    case IDOK:
        result = MessageDialogButton::Ok;
        break;
    case IDRETRY:
        result = MessageDialogButton::Retry;
        break;
    case IDYES:
        result = MessageDialogButton::Yes;
        break;
    }
    return result;
}

