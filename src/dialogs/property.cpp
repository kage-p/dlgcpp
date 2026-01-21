#include "dialogs/property_impl.h"
#include "dlgcpp/dialogs/property.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

PropertyDialog::PropertyDialog(ISharedDialog parent)
    : PropertyDialog(std::make_shared<PropertyDialogImpl>(this), parent)
{
}

PropertyDialog::PropertyDialog(
    std::shared_ptr<PropertyDialogImpl> impl,
    ISharedDialog parent)
    : _impl(std::move(impl))
{
    auto ownerFn = [this]() -> ISharedPropertyDialog { return shared_from_this(); };

    _parent.reset(parent, nullptr, ownerFn, "parent");
    _title.reset(std::string(), nullptr, ownerFn, "title");
    _message.reset(std::string(), nullptr, ownerFn, "message");
    _sectionWidth.reset(150, nullptr, ownerFn, "sectionWidth");
}

IReadOnlyProperty<ISharedDialog, ISharedPropertyDialog>& PropertyDialog::parent()
{
    return _parent;
}

IProperty<std::string, ISharedPropertyDialog>& PropertyDialog::title()
{
    return _title;
}

IProperty<std::string, ISharedPropertyDialog>& PropertyDialog::message()
{
    return _message;
}

IProperty<int, ISharedPropertyDialog>& PropertyDialog::sectionWidth()
{
    return _sectionWidth;
}

bool PropertyDialog::show()
{
    return _impl->show();
}
