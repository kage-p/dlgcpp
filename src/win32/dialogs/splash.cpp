#include "dlgcpp/controls/image.h"
#include "dlgcpp/controls/label.h"
#include "splash_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

SplashDialogImpl::SplashDialogImpl(
    SplashDialog& splashDialog,
    ISharedDialog parent)
    : _splashDialog(splashDialog)
{
    _parent = parent;
}

const std::string& SplashDialogImpl::logoBitmapId() const
{
    return _logoBitmapId;
}

void SplashDialogImpl::logoBitmapId(const std::string& value)
{
    _logoBitmapId = value;
}

const std::string& SplashDialogImpl::message() const
{
    return _message;
}

void SplashDialogImpl::message(const std::string& value)
{
    _message = value;
}

int SplashDialogImpl::timeout() const
{
    return _timeout;
}

void SplashDialogImpl::timeout(int value)
{
    _timeout = value;
}

void SplashDialogImpl::show()
{
    _dialog.reset();

    if (_logoBitmapId.empty())
        return;

    auto dlg = std::make_shared<Dialog>(DialogType::Frameless, _parent);
    _dialog = dlg;

    auto logoImage = std::make_shared<Image>(Position{ 0, 0, 0, 0 });
    logoImage->colors(Color::Black, Color::White);
    logoImage->autoSize(true);
    logoImage->image(ImageSource{ _logoBitmapId,false,false });
    dlg->add(logoImage);

    auto imageSize = logoImage->p().size();

    if (!_message.empty())
    {
        auto msgPos = Position{ 3, imageSize.width() - 15, imageSize.height() - 6 , 12 };
        auto messageLabel = std::make_shared<Label>(_message, msgPos);
        messageLabel->font(Font{ "sans serif", 8, true });
        messageLabel->colors(Color::LtGray, Color::None);
        messageLabel->autoSize(true);
        dlg->add(messageLabel);
        BringWindowToTop((HWND)messageLabel->handle());
    }

    dlg->resize(imageSize);
    dlg->center();
    dlg->show();
    SetWindowPos((HWND)dlg->handle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    logoImage->ClickEvent() += [](ISharedControl ctl)
        {
            ctl->parent()->close();
        };

    if (_timeout > 0)
    {
        dlg->TimerEvent() += [](ISharedDialog dlg)
            {
                dlg->close();
            };
        dlg->timer(_timeout);
    }
}

void SplashDialogImpl::close()
{
    _dialog.reset();
}
