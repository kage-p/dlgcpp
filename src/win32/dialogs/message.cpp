#include "message_p.h"
#include "../utility.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

MessageDialog::MessageDialog(ISharedDialog parent) :
    _props(new msgdlg_props())
{
    _props->parent = parent;
}

MessageDialog::~MessageDialog()
{
    delete _props;
}

const std::string& MessageDialog::title() const
{
    return _props->title;
}

void MessageDialog::title(const std::string& value)
{
    _props->title = value;
}

const std::string& MessageDialog::message() const
{
    return _props->message;
}

void MessageDialog::message(const std::string& value)
{
    _props->message = value;
}

MessageDialogButtonGroup MessageDialog::buttons() const
{
    return _props->buttons;
}

void MessageDialog::buttons(MessageDialogButtonGroup value)
{
    _props->buttons = value;
}

MessageDialogIcon MessageDialog::icon() const
{
    return _props->icon;
}

void MessageDialog::icon(MessageDialogIcon value)
{
    _props->icon = value;
}

MessageDialogButton MessageDialog::defaultButton() const
{
    return _props->defaultButton;
}

void MessageDialog::defaultButton(MessageDialogButton value)
{
    _props->defaultButton = value;
}

bool MessageDialog::showHelp() const
{
    return _props->showHelp;
}

void MessageDialog::showHelp(bool value)
{
    _props->showHelp = value;
}

MessageDialogButton MessageDialog::show()
{
    auto hwndParent = (HWND)HWND_DESKTOP;
    if (_props->parent != nullptr)
        hwndParent = reinterpret_cast<HWND>(_props->parent->handle());

    UINT flags = 0;

    switch (_props->icon)
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

    switch (_props->buttons)
    {
    case MessageDialogButtonGroup::OkCancel:
        flags |= MB_OKCANCEL;
        if (_props->defaultButton == MessageDialogButton::Ok)
            flags |= MB_DEFBUTTON1;
        else if (_props->defaultButton == MessageDialogButton::Cancel)
            flags |= MB_DEFBUTTON2;
        break;
    case MessageDialogButtonGroup::AbortRetryIgnore:
        flags |= MB_ABORTRETRYIGNORE;
        if (_props->defaultButton == MessageDialogButton::Abort)
            flags |= MB_DEFBUTTON1;
        else if (_props->defaultButton == MessageDialogButton::Retry)
            flags |= MB_DEFBUTTON2;
        else if (_props->defaultButton == MessageDialogButton::Ignore)
            flags |= MB_DEFBUTTON3;
        break;
    case MessageDialogButtonGroup::YesNoCancel:
        flags |= MB_YESNOCANCEL;
        if (_props->defaultButton == MessageDialogButton::Yes)
            flags |= MB_DEFBUTTON1;
        else if (_props->defaultButton == MessageDialogButton::No)
            flags |= MB_DEFBUTTON2;
        else if (_props->defaultButton == MessageDialogButton::Cancel)
            flags |= MB_DEFBUTTON3;
        break;
    case MessageDialogButtonGroup::YesNo:
        flags |= MB_YESNO;
        if (_props->defaultButton == MessageDialogButton::Yes)
            flags |= MB_DEFBUTTON1;
        else if (_props->defaultButton == MessageDialogButton::No)
            flags |= MB_DEFBUTTON2;
        break;
    case MessageDialogButtonGroup::RetryCancel:
        flags |= MB_RETRYCANCEL;
        if (_props->defaultButton == MessageDialogButton::Retry)
            flags |= MB_DEFBUTTON1;
        else if (_props->defaultButton == MessageDialogButton::Cancel)
            flags |= MB_DEFBUTTON2;
        break;
    case MessageDialogButtonGroup::Ok:
    default:
        flags |= MB_OK;
        if (_props->defaultButton == MessageDialogButton::Ok)
            flags |= MB_DEFBUTTON1;
        break;
    }

    if (_props->showHelp)
        flags |= MB_HELP;

    auto titleText = toWide(_props->title);
    if (titleText.empty() && _props->parent != nullptr)
        titleText = toWide(_props->parent->title());

    auto messageText = toWide(_props->message);

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

