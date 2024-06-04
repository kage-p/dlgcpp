#include "about_p.h"
#include "../utility.h"
#include <shellapi.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

AboutDialog::AboutDialog(std::shared_ptr<IDialog> parent) :
    _props(new about_props())
{
    _props->parent = parent;
}

AboutDialog::~AboutDialog()
{
    delete _props;
}

const std::string& AboutDialog::title() const
{
    return _props->titleText;
}

void AboutDialog::title(const std::string& value)
{
    _props->titleText = value;
}

const std::string& AboutDialog::appDetails() const
{
    return _props->appDetailsText;
}

void AboutDialog::appDetails(const std::string& value)
{
    _props->appDetailsText = value;
}

const std::string& AboutDialog::description() const
{
    return _props->descriptionText;
}

void AboutDialog::description(const std::string& value)
{
    _props->descriptionText = value;
}

const std::string& AboutDialog::homePageLink() const
{
    return _props->homePageLink;
}

void AboutDialog::homePageLink(const std::string& value)
{
    _props->homePageLink = value;
}

const std::string& AboutDialog::releaseDate() const
{
    return _props->releaseDateText;
}

void AboutDialog::releaseDate(const std::string& value)
{
    _props->releaseDateText = value;
}

const std::string& AboutDialog::logoBitmapId() const
{
    return _props->logoBitmapId;
}

void AboutDialog::logoBitmapId(const std::string& value)
{
    _props->logoBitmapId = value;
}

const std::string& AboutDialog::logoIconId() const
{
    return _props->logoIconId;
}

void AboutDialog::logoIconId(const std::string& value)
{
    _props->logoIconId = value;
}

void AboutDialog::show()
{
    auto dlg = std::make_shared<Dialog>(_props->parent);
    dlg->title(_props->titleText);

    // TODO: split into createInterface

    // Add image height
    int cxLogoOffset = 0;
    int cyLogoOffset = 0;
    if (!_props->logoBitmapId.empty())
        cyLogoOffset = 50+15;

    dlg->type(DialogType::Popup);
    dlg->resize(310, 130 + cyLogoOffset);
    dlg->center();

    if (!_props->logoBitmapId.empty())
    {
        // Use bitmap...
        auto logoImage = std::make_shared<Image>(dlg, Position{7, 7, 295, 65});
        logoImage->colors(Color::Black, Color::White);
        logoImage->image(ImageSource{_props->logoBitmapId, false, false});
        dlg->add(logoImage);
    }
    else if (!_props->logoIconId.empty())
    {
        // use application icon as alternativve
        auto iconImage = std::make_shared<Image>(dlg, Position{7, 13, 5, 5});
        iconImage->image(ImageSource{_props->logoIconId, true, false});
        dlg->add(iconImage);
        cxLogoOffset = iconImage->p()._x + iconImage->p()._cx;
    }

    auto appDetails = std::make_shared<Label>(dlg, _props->appDetailsText, Position{7, 15+cyLogoOffset, 295, 10});
    appDetails->font(Font{"sans serif", 8, true});
    dlg->add(appDetails);

    if (!_props->releaseDateText.empty())
    {
        auto releaseInfo = std::make_shared<Label>(dlg, _props->releaseDateText, Position{7+cxLogoOffset, 26+cyLogoOffset, 295-cxLogoOffset, 10});
        dlg->add(releaseInfo);
    }

    if (!_props->homePageLink.empty())
    {
        auto webLink = std::make_shared<Label>(dlg, _props->homePageLink, Position{7+cxLogoOffset, 36+cyLogoOffset, 150, 12});
        webLink->colors(Color::Blue, Color::None);
        webLink->cursor(Cursor::Hand);
        webLink->font(Font{"MS Sans Serif", 8, false, true});
        webLink->autoSize(true);
        webLink->CommandEvent() +=
            [this,dlg]()
        {
            auto hwnd = reinterpret_cast<HWND>(dlg->handle());
            ShellExecuteW(hwnd, L"open", toWide(_props->homePageLink).c_str(), NULL, NULL, SW_SHOWNORMAL);
        };
        dlg->add(webLink);
    }

    auto description = std::make_shared<TextBox>(dlg, _props->descriptionText, Position{7, 49+cyLogoOffset, 295, 50});
    description->readOnly(true);
    description->multiline(true);
    description->wrapText(true);
    dlg->add(description);

    auto sysInfoButton = std::make_shared<Button>(dlg, "&System Info...", Position{167, 104+cyLogoOffset, 65, 20});
    sysInfoButton->CommandEvent() +=
        [dlg]()
    {
        auto hwnd = reinterpret_cast<HWND>(dlg->handle());
        ShellExecuteW(hwnd, L"open", L"MSINFO32.EXE", NULL, NULL, SW_SHOWNORMAL);
    };
    dlg->add(sysInfoButton);

    auto closeButton = std::make_shared<Button>(dlg, "Close", Position{237, 104+cyLogoOffset, 65, 20});
    closeButton->CommandEvent() +=
        [dlg]()
    {
        dlg->close();
    };
    dlg->add(closeButton);

    dlg->visible(true);
    closeButton->setFocus();

    dlg->exec();
    dlg.reset();
}
