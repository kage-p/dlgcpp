#include "dialogs/message_p.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

MessageDialog::MessageDialog(ISharedDialog parent)
    : MessageDialog(std::make_shared<MessageDialogImpl>(*this, parent))
{
}

MessageDialog::MessageDialog(std::shared_ptr<MessageDialogImpl> impl)
    : _impl(std::move(impl))
{
}

const std::string& MessageDialog::title() const
{
    return _impl->title();
}

void MessageDialog::title(const std::string& value)
{
    if (_impl->title() == value)
        return;
    _impl->title(value);
}

const std::string& MessageDialog::message() const
{
    return _impl->message();
}

void MessageDialog::message(const std::string& value)
{
    if (_impl->message() == value)
        return;
    _impl->message(value);
}

MessageDialogButtonGroup MessageDialog::buttons() const
{
    return _impl->buttons();
}

void MessageDialog::buttons(MessageDialogButtonGroup value)
{
    if (_impl->buttons() == value)
        return;
    _impl->buttons(value);
}

MessageDialogIcon MessageDialog::icon() const
{
    return _impl->icon();
}

void MessageDialog::icon(MessageDialogIcon value)
{
    if (_impl->icon() == value)
        return;
    _impl->icon(value);
}

MessageDialogButton MessageDialog::defaultButton() const
{
    return _impl->defaultButton();
}

void MessageDialog::defaultButton(MessageDialogButton value)
{
    if (_impl->defaultButton() == value)
        return;
    _impl->defaultButton(value);
}

bool MessageDialog::showHelp() const
{
    return _impl->showHelp();
}

void MessageDialog::showHelp(bool value)
{
    if (_impl->showHelp() == value)
        return;
    _impl->showHelp(value);
}

MessageDialogButton MessageDialog::show()
{
    return _impl->show();
}
