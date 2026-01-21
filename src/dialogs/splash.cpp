#include "dialogs/splash_impl.h"
#include "dlgcpp/dialogs/splash.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

SplashDialog::SplashDialog(ISharedDialog parent)
    : SplashDialog(std::make_shared<SplashDialogImpl>(this), parent)
{
}

SplashDialog::SplashDialog(
    std::shared_ptr<SplashDialogImpl> impl,
    ISharedDialog parent)
    : _impl(std::move(impl))
{
    auto ownerFn = [this]() -> ISharedSplashDialog { return shared_from_this(); };

    _parent.reset(parent, nullptr, ownerFn, "parent");
    _message.reset(std::string(), nullptr, ownerFn, "title");
    _logoImage.reset(ImageSource(), nullptr, ownerFn, "logoImage");
    _timeout.reset(800, nullptr, ownerFn, "timeout");
}

IReadOnlyProperty<ISharedDialog, ISharedSplashDialog>& SplashDialog::parent()
{
    return _parent;
}

IProperty<std::string, ISharedSplashDialog>& SplashDialog::message()
{
    return _message;
}

IProperty<ImageSource, ISharedSplashDialog>& SplashDialog::logoImage()
{
    return _logoImage;
}

IProperty<int, ISharedSplashDialog>& SplashDialog::timeout()
{
    return _timeout;
}

void SplashDialog::show()
{
    _impl->show();
}

void SplashDialog::close()
{
    _impl->close();
}
