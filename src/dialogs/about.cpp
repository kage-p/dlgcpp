#include "dialogs/about_impl.h"
#include "dlgcpp/controls/button.h"
#include "dlgcpp/controls/image.h"
#include "dlgcpp/controls/label.h"
#include "dlgcpp/controls/textbox.h"
#include "dlgcpp/dialogs/about.h"
#include "dlgcpp/dialogs/dialog.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

AboutDialog::AboutDialog(ISharedDialog parent)
    : AboutDialog(std::make_shared<AboutDialogImpl>(this), parent)
{
}

AboutDialog::AboutDialog(
    std::shared_ptr<AboutDialogImpl> impl,
    ISharedDialog parent)
    : _impl(std::move(impl))
{
    auto ownerFn = [this]() -> ISharedAboutDialog { return shared_from_this(); };

    _parent.reset(parent, nullptr, ownerFn, "parent");
    _title.reset(std::string(), nullptr, ownerFn, "title");
    _appDetails.reset(std::string(), nullptr, ownerFn, "appDetails");
    _description.reset(std::string(), nullptr, ownerFn, "description");
    _homePageLink.reset(std::string(), nullptr, ownerFn, "homePageLink");
    _releaseDate.reset(std::string(), nullptr, ownerFn, "releaseDate");
    _logoImage.reset(ImageSource(), nullptr, ownerFn, "logoImage");
}

IReadOnlyProperty<ISharedDialog, ISharedAboutDialog>& AboutDialog::parent()
{
    return _parent;
}

IProperty<std::string, ISharedAboutDialog>& AboutDialog::title()
{
    return _title;
}

IProperty<std::string, ISharedAboutDialog>& AboutDialog::appDetails()
{
    return _appDetails;
}

IProperty<std::string, ISharedAboutDialog>& AboutDialog::description()
{
    return _description;
}

IProperty<std::string, ISharedAboutDialog>& AboutDialog::homePageLink()
{
    return _homePageLink;
}

IProperty<std::string, ISharedAboutDialog>& AboutDialog::releaseDate()
{
    return _releaseDate;
}

IProperty<ImageSource, ISharedAboutDialog>& AboutDialog::logoImage()
{
    return _logoImage;
}

void AboutDialog::show()
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, _parent);
    dlg->title() = _title;

    // Add image height
    int cxLogoOffset = 0;
    int cyLogoOffset = 0;
    if (!_logoImage->id().empty() && !_logoImage->isIcon())
        cyLogoOffset = 50 + 15;

    dlg->resize({ 310, 130 + cyLogoOffset });
    dlg->center();

    if (!_logoImage->id().empty())
    {
        if (_logoImage->isIcon())
        {
            // use icon with different placement
            auto iconImage = std::make_shared<Image>(Position{ 7, 13, 0, 0 });
            iconImage->autoSize(true);
            iconImage->border(BorderStyle::Thin);
            iconImage->source(_logoImage.value());
            dlg->add(iconImage);
            cxLogoOffset = iconImage->p()->x() + iconImage->p()->width();
        }
        else
        {
            // logo image
            auto logoImage = std::make_shared<Image>(Position{ 7, 7, 295, 65 });
            logoImage->border(BorderStyle::Thin);
            logoImage->colors(Color::Black, Color::White);
            logoImage->source(_logoImage.value());
            dlg->add(logoImage);
        }
    }

    auto appDetails = std::make_shared<Label>(_appDetails, Position{ 7, 15 + cyLogoOffset, 295, 10 });
    appDetails->font(Font{ "sans serif", 8, true });
    dlg->add(appDetails);

    if (!_releaseDate.empty())
    {
        auto releaseInfo = std::make_shared<Label>(_releaseDate, Position{ 7 + cxLogoOffset, 26 + cyLogoOffset, 295 - cxLogoOffset, 10 });
        dlg->add(releaseInfo);
    }

    if (!_homePageLink.empty())
    {
        auto webLink = std::make_shared<Label>(_homePageLink, Position{ 7 + cxLogoOffset, 36 + cyLogoOffset, 150, 12 });
        webLink->colors(Color::Blue, Color::None);
        webLink->cursor(Cursor::Hand);
        webLink->font(Font{ "MS Sans Serif", 8, false, false, true });
        webLink->autoSize(true);

        webLink->ClickEvent() +=
            [&,dlg](auto)
            {
                _impl->openUrl(dlg, _homePageLink);
            };
        dlg->add(webLink);
    }

    auto description = std::make_shared<TextBox>(_description, Position{ 7, 49 + cyLogoOffset, 295, 50 });
    description->readOnly(true);
    description->multiline(true);
    description->wrapText(true);
    dlg->add(description);

    auto sysInfoButton = std::make_shared<Button>("&System Info...", Position{ 167, 104 + cyLogoOffset, 65, 20 });
    sysInfoButton->ClickEvent() +=
        [&, dlg](auto)
        {
            _impl->openSystemInfo(dlg);
        };
    dlg->add(sysInfoButton);

    auto closeButton = std::make_shared<Button>("Close", Position{ 237, 104 + cyLogoOffset, 65, 20 });
    closeButton->ClickEvent() +=
        [dlg](auto)
        {
            dlg->close();
        };
    dlg->add(closeButton);

    dlg->visible(true);
    closeButton->focus(true);

    dlg->exec();
    dlg.reset();
}
