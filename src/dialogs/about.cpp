#include "dialogs/about_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

AboutDialog::AboutDialog(ISharedDialog parent)
    : AboutDialog(std::make_shared<AboutDialogImpl>(*this, parent))
{
}

AboutDialog::AboutDialog(std::shared_ptr<AboutDialogImpl> impl)
    : _impl(std::move(impl))
{
}

const std::string& AboutDialog::title() const
{
    return _impl->title();
}

void AboutDialog::title(const std::string& value)
{
    if (_impl->title() == value)
        return;
    _impl->title(value);
}

const std::string& AboutDialog::appDetails() const
{
    return _impl->appDetails();
}

void AboutDialog::appDetails(const std::string& value)
{
    if (_impl->appDetails() == value)
        return;
    _impl->appDetails(value);
}

const std::string& AboutDialog::description() const
{
    return _impl->description();
}

void AboutDialog::description(const std::string& value)
{
    if (_impl->description() == value)
        return;
    _impl->description(value);
}

const std::string& AboutDialog::homePageLink() const
{
    return _impl->homePageLink();
}

void AboutDialog::homePageLink(const std::string& value)
{
    if (_impl->homePageLink() == value)
        return;
    _impl->homePageLink(value);
}

const std::string& AboutDialog::releaseDate() const
{
    return _impl->releaseDate();
}

void AboutDialog::releaseDate(const std::string& value)
{
    if (_impl->releaseDate() == value)
        return;
    _impl->releaseDate(value);
}

const std::string& AboutDialog::logoBitmapId() const
{
    return _impl->logoBitmapId();
}

void AboutDialog::logoBitmapId(const std::string& value)
{
    if (_impl->logoBitmapId() == value)
        return;
    _impl->logoBitmapId(value);
}

const std::string& AboutDialog::logoIconId() const
{
    return _impl->logoIconId();
}

void AboutDialog::logoIconId(const std::string& value)
{
    if (_impl->logoIconId() == value)
        return;
    _impl->logoIconId(value);
}

void AboutDialog::show()
{
    _impl->show();
}
