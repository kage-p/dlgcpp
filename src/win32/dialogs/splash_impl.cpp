#include "dlgcpp/controls/image.h"
#include "dlgcpp/controls/label.h"
#include "splash_impl.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

SplashDialogImpl::SplashDialogImpl(
    SplashDialog* splashDialog)
    : _splashDialog(splashDialog)
{
}

void SplashDialogImpl::show()
{
    _dialog.reset();

    if (_splashDialog->logoImage().empty())
    {
        DLGCPP_CERR("SplashDialog: logoImage is not set");
        return;
    }

    _dialog = std::make_shared<Dialog>(DialogType::Frameless, _splashDialog->parent());

    auto logoImage = std::make_shared<Image>(Position{ 0, 0, 0, 0 });
    logoImage->colors(Color::Black, Color::White);
    logoImage->autoSize(true);
    logoImage->source(_splashDialog->logoImage());
    _dialog->add(logoImage);

    auto imageSize = logoImage->p()->size();

    if (!_splashDialog->message().empty())
    {
        auto msgPos = Position{ 3, imageSize.width() - 15, imageSize.height() - 6 , 12 };
        auto messageLabel = std::make_shared<Label>(_splashDialog->message(), msgPos);
        messageLabel->font(Font{ "sans serif", 8, true });
        messageLabel->colors(Color::LtGray, Color::None);
        messageLabel->autoSize(true);
        _dialog->add(messageLabel);
        BringWindowToTop((HWND)messageLabel->handle().value());
    }

    _dialog->resize(imageSize);
    _dialog->center();
    _dialog->show();
    SetWindowPos((HWND)_dialog->handle().value(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    logoImage->ClickEvent() += [&](auto)
        {
            _dialog->close();
        };

    if (_splashDialog->timeout() > 0)
    {
        _dialog->TimerEvent() += [&](auto)
            {
                _dialog->close();
            };
        _dialog->timer(_splashDialog->timeout());
    }
}

void SplashDialogImpl::close()
{
    _dialog.reset();
}
