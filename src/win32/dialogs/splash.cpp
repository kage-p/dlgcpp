#include "splash_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

SplashDialog::SplashDialog(std::shared_ptr<IDialog> parent)
    : _props(new splash_props())
{
    _props->parent = parent;
}

SplashDialog::~SplashDialog()
{
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

    auto dlg = std::make_shared<Dialog>(_props->parent);
    _props->splashDialog = dlg;
    dlg->type(DialogType::Frameless);

    auto logoImage = std::make_shared<Image>(dlg, Position{0, 0, 0, 0});
    logoImage->colors(Color::Black, Color::White);
    logoImage->autoSize(true);
    logoImage->image(ImageSource{_props->logoBitmapId,false,false});
    dlg->add(logoImage);

    auto pos = logoImage->p();

    if (!_props->message.empty())
    {
        auto messageLabel = std::make_shared<Label>(dlg, _props->message, Position{3,pos._cy-15,pos._cx-6,12});
        messageLabel->font(Font{"sans serif", 8, true});
        messageLabel->colors(Color::LtGray, Color::Blue);
        dlg->add(messageLabel);
        BringWindowToTop((HWND)messageLabel->handle());
    }

    dlg->resize(pos._cx, pos._cy);
    dlg->center();
    dlg->visible(true);
    SetWindowPos((HWND)dlg->handle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    dlg->ClickEvent() += [this](){
        _props->splashDialog->close();
    };

    if (_props->timeout > 0)
    {
        dlg->timer(_props->timeout, [this](){
            _props->splashDialog->close();
        });
    }
}

void SplashDialog::close()
{
    _props->splashDialog.reset();
}
