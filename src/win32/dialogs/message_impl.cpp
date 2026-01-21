#include "dlgcpp/dialogs/dialog.h"
#include "message_impl.h"
#include "utility/string_encoder.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

MessageDialogImpl::MessageDialogImpl(
    MessageDialog* messageDialog) :
    _messageDialog(messageDialog)
{
}

MessageDialogButton MessageDialogImpl::show()
{
    auto hwndParent = (HWND)HWND_DESKTOP;
    if (!_messageDialog->parent().empty())
        hwndParent = reinterpret_cast<HWND>(_messageDialog->parent().value()->handle().value());

    UINT flags = 0;

    switch (_messageDialog->icon())
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

    auto defaultButton = _messageDialog->defaultButton().value();

    switch (_messageDialog->buttons())
    {
    case MessageDialogButtonGroup::OkCancel:
        flags |= MB_OKCANCEL;
        if (defaultButton == MessageDialogButton::Ok)
            flags |= MB_DEFBUTTON1;
        else if (defaultButton == MessageDialogButton::Cancel)
            flags |= MB_DEFBUTTON2;
        break;
    case MessageDialogButtonGroup::AbortRetryIgnore:
        flags |= MB_ABORTRETRYIGNORE;
        if (defaultButton == MessageDialogButton::Abort)
            flags |= MB_DEFBUTTON1;
        else if (defaultButton == MessageDialogButton::Retry)
            flags |= MB_DEFBUTTON2;
        else if (defaultButton == MessageDialogButton::Ignore)
            flags |= MB_DEFBUTTON3;
        break;
    case MessageDialogButtonGroup::YesNoCancel:
        flags |= MB_YESNOCANCEL;
        if (defaultButton == MessageDialogButton::Yes)
            flags |= MB_DEFBUTTON1;
        else if (defaultButton == MessageDialogButton::No)
            flags |= MB_DEFBUTTON2;
        else if (defaultButton == MessageDialogButton::Cancel)
            flags |= MB_DEFBUTTON3;
        break;
    case MessageDialogButtonGroup::YesNo:
        flags |= MB_YESNO;
        if (defaultButton == MessageDialogButton::Yes)
            flags |= MB_DEFBUTTON1;
        else if (defaultButton == MessageDialogButton::No)
            flags |= MB_DEFBUTTON2;
        break;
    case MessageDialogButtonGroup::RetryCancel:
        flags |= MB_RETRYCANCEL;
        if (defaultButton == MessageDialogButton::Retry)
            flags |= MB_DEFBUTTON1;
        else if (defaultButton == MessageDialogButton::Cancel)
            flags |= MB_DEFBUTTON2;
        break;
    case MessageDialogButtonGroup::Ok:
    default:
        flags |= MB_OK;
        if (defaultButton == MessageDialogButton::Ok)
            flags |= MB_DEFBUTTON1;
        break;
    }

    if (_messageDialog->showHelp())
        flags |= MB_HELP;

    auto titleText = StringEncoder::toWide(_messageDialog->title());
    if (titleText.empty() && !_messageDialog->parent().empty())
    {
        titleText = StringEncoder::toWide(_messageDialog->parent().value()->title());
    }

    auto messageText = StringEncoder::toWide(_messageDialog->message());

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

