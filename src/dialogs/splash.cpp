#include "dlgcpp/dialogs/splash.h"
#include "dialogs/splash_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

SplashDialog::SplashDialog(ISharedDialog parent)
    : SplashDialog(std::make_shared<SplashDialogImpl>(*this, parent))
{
}

SplashDialog::SplashDialog(std::shared_ptr<SplashDialogImpl> impl)
    : _impl(std::move(impl))
{
}

const std::string& SplashDialog::logoBitmapId() const
{
    return _impl->logoBitmapId();
}

void SplashDialog::logoBitmapId(const std::string& value)
{
    if (_impl->logoBitmapId() == value)
        return;
    _impl->logoBitmapId(value);
}

const std::string& SplashDialog::message() const
{
    return _impl->message();
}

void SplashDialog::message(const std::string& value)
{
    if (_impl->message() == value)
        return;
    _impl->message(value);
}

int SplashDialog::timeout() const
{
    return _impl->timeout();
}

void SplashDialog::timeout(int value)
{
    if (_impl->timeout() == value)
        return;
    _impl->timeout(value);
}

void SplashDialog::show()
{
    _impl->show();
}

void SplashDialog::close()
{
    _impl->close();
}
