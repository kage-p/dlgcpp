#include "splash_p.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

SplashDialog::SplashDialog(ISharedDialog parent)
    : _props(new splash_props())
{
    _props->parent = parent;
}

SplashDialog::~SplashDialog()
{
    // remove the modeless dialog
    _props->splashDialog.reset();
    delete _props;
}

const std::string& SplashDialog::logoBitmapId() const
{
    return _props->logoBitmapId;
}

void SplashDialog::logoBitmapId(const std::string& value)
{
    _props->logoBitmapId = value;
}

const std::string& SplashDialog::message() const
{
    return _props->message;
}

void SplashDialog::message(const std::string& value)
{
    _props->message = value;
}

int SplashDialog::timeout() const
{
    return _props->timeout;
}

void SplashDialog::timeout(int value)
{
    _props->timeout = value;
}

void SplashDialog::show()
{
    _props->splashDialog.reset();

    if (_props->logoBitmapId.empty())
        return;

    auto dlg = std::make_shared<Dialog>(DialogType::Frameless, _props->parent);
    _props->splashDialog = dlg;

    auto logoImage = std::make_shared<Image>(Position{0, 0, 0, 0});
    logoImage->colors(Color::Black, Color::White);
    logoImage->autoSize(true);
    logoImage->image(ImageSource{_props->logoBitmapId,false,false});
    dlg->add(logoImage);

    auto imageSize = logoImage->p().size();

    if (!_props->message.empty())
    {
        auto msgPos = Position{3, imageSize.width() - 15, imageSize.height() - 6 , 12};
        auto messageLabel = std::make_shared<Label>(_props->message, msgPos);
        messageLabel->font(Font{"sans serif", 8, true});
        messageLabel->colors(Color::LtGray, Color::None);
        messageLabel->autoSize(true);
        dlg->add(messageLabel);
        BringWindowToTop((HWND)messageLabel->handle());
    }

    dlg->resize(imageSize);
    dlg->center();
    dlg->visible(true);
    SetWindowPos((HWND)dlg->handle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    logoImage->ClickEvent() += [](ISharedControl ctl)
    {
        ctl->parent()->close();
    };

    if (_props->timeout > 0)
    {
        dlg->TimerEvent() += [](ISharedDialog dlg)
        {
            dlg->close();
        };
        dlg->timer(_props->timeout);
    }
}

void SplashDialog::close()
{
    _props->splashDialog.reset();
}
