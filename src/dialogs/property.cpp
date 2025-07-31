#include "dlgcpp/dialogs/property.h"
#include "dialogs/property_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

PropertyDialog::PropertyDialog(ISharedDialog parent)
    : PropertyDialog(std::make_shared<PropertyDialogImpl>(*this, parent))
{
}

PropertyDialog::PropertyDialog(std::shared_ptr<PropertyDialogImpl> impl)
    : _impl(std::move(impl))
{
}

const std::string& PropertyDialog::title() const
{
    return _impl->title();
}

void PropertyDialog::title(const std::string& value)
{
    if (_impl->title() == value)
        return;
    _impl->title(value);
}

const std::string& PropertyDialog::message() const
{
    return _impl->message();
}

void PropertyDialog::message(const std::string& value)
{
    if (_impl->message() == value)
        return;
    _impl->message(value);
}

int PropertyDialog::sectionWidth() const
{
    return _impl->sectionWidth();
}

void PropertyDialog::sectionWidth(int value)
{
    if (_impl->sectionWidth() == value)
        return;
    _impl->sectionWidth(value);
}

bool PropertyDialog::show()
{
    return _impl->show();
}
