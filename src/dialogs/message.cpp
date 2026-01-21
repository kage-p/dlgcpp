#include "dialogs/message_impl.h"
#include "dlgcpp/dialogs/message.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

MessageDialog::MessageDialog(ISharedDialog parent)
    : MessageDialog(std::make_shared<MessageDialogImpl>(this), parent)
{
}

MessageDialog::MessageDialog(
    std::shared_ptr<MessageDialogImpl> impl,
    ISharedDialog parent)
    : _impl(std::move(impl))
{
    auto ownerFn = [this]() -> ISharedMessageDialog { return shared_from_this(); };

    _parent.reset(parent, nullptr, ownerFn, "parent");
    _title.reset(std::string(), nullptr, ownerFn, "title");
    _message.reset(std::string(), nullptr, ownerFn, "message");
    _buttons.reset(MessageDialogButtonGroup::Ok, nullptr, ownerFn, "buttons");
    _icon.reset(MessageDialogIcon::Information, nullptr, ownerFn, "icon");
    _defaultButton.reset(MessageDialogButton::Undefined, nullptr, ownerFn, "defaultButton");
    _showHelp.reset(false, nullptr, ownerFn, "showHelp");
}

IReadOnlyProperty<ISharedDialog, ISharedMessageDialog>& MessageDialog::parent()
{
    return _parent;
}

IProperty<std::string, ISharedMessageDialog>& MessageDialog::title()
{
    return _title;
}

IProperty<std::string, ISharedMessageDialog>& MessageDialog::message()
{
    return _message;
}

IProperty<MessageDialogButtonGroup, ISharedMessageDialog>& MessageDialog::buttons()
{
    return _buttons;
}

IProperty<MessageDialogIcon, ISharedMessageDialog>& MessageDialog::icon()
{
    return _icon;
}

IProperty<MessageDialogButton, ISharedMessageDialog>& MessageDialog::defaultButton()
{
    return _defaultButton;
}

IProperty<bool, ISharedMessageDialog>& MessageDialog::showHelp()
{
    return _showHelp;
}

MessageDialogButton MessageDialog::show()
{
    return _impl->show();
}
