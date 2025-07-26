#include "about_p.h"
#include "dlgcpp/controls/button.h"
#include "dlgcpp/controls/image.h"
#include "dlgcpp/controls/label.h"
#include "dlgcpp/controls/textbox.h"
#include "dlgcpp/dialogs/dialog.h"
#include "utility/image.h"
#include "utility/string.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

AboutDialogImpl::AboutDialogImpl(
    AboutDialog& aboutDialog,
    ISharedDialog parent) :
    _aboutDialog(aboutDialog),
    _parent(parent)
{
}

const std::string& AboutDialogImpl::title() const
{
    return _title;
}

void AboutDialogImpl::title(const std::string& value)
{
    _title = value;
}

const std::string& AboutDialogImpl::appDetails() const
{
    return _appDetails;
}

void AboutDialogImpl::appDetails(const std::string& value)
{
    _appDetails = value;
}

const std::string& AboutDialogImpl::description() const
{
    return _description;
}

void AboutDialogImpl::description(const std::string& value)
{
    _description = value;
}

const std::string& AboutDialogImpl::homePageLink() const
{
    return _homePageLink;
}

void AboutDialogImpl::homePageLink(const std::string& value)
{
    _homePageLink = value;
}

const std::string& AboutDialogImpl::releaseDate() const
{
    return _releaseDate;
}

void AboutDialogImpl::releaseDate(const std::string& value)
{
    _releaseDate = value;
}

const std::string& AboutDialogImpl::logoBitmapId() const
{
    return _logoBitmapId;
}

void AboutDialogImpl::logoBitmapId(const std::string& value)
{
    _logoBitmapId = value;
}

const std::string& AboutDialogImpl::logoIconId() const
{
    return _logoIconId;
}

void AboutDialogImpl::logoIconId(const std::string& value)
{
    _logoIconId = value;
}

void AboutDialogImpl::show()
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, _parent);
    dlg->title(_title);

    // TODO: split into createInterface

    // Add image height
    int cxLogoOffset = 0;
    int cyLogoOffset = 0;
    if (!_logoBitmapId.empty())
        cyLogoOffset = 50 + 15;
    dlg->resize({ 310, 130 + cyLogoOffset });
    dlg->center();

    if (!_logoBitmapId.empty())
    {
        // Use bitmap...
        auto logoImage = std::make_shared<Image>(Position{ 7, 7, 295, 65 });
        logoImage->border(BorderStyle::Thin);
        logoImage->colors(Color::Black, Color::White);
        logoImage->image(ImageSource{ _logoBitmapId, false, false });
        dlg->add(logoImage);
    }
    else if (!_logoIconId.empty())
    {
        // use application icon as alternative
        auto iconImage = std::make_shared<Image>(Position{ 7, 13, 0, 0 });
        iconImage->autoSize(true);
        iconImage->border(BorderStyle::Thin);
        iconImage->image(ImageSource{ _logoIconId, true, false });
        dlg->add(iconImage);
        cxLogoOffset = iconImage->p().x() + iconImage->p().width();
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

        auto homtPageText = toWide(_homePageLink);
        webLink->ClickEvent() +=
            [homtPageText](ISharedControl control)
            {
                auto hwnd = reinterpret_cast<HWND>(control->parent()->handle());
                ShellExecuteW(hwnd, L"open", homtPageText.c_str(), NULL, NULL, SW_SHOWNORMAL);
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
        [](ISharedControl control)
        {
            auto hwnd = reinterpret_cast<HWND>(control->parent()->handle());
            ShellExecuteW(hwnd, L"open", L"MSINFO32.EXE", NULL, NULL, SW_SHOWNORMAL);
        };
    dlg->add(sysInfoButton);

    auto closeButton = std::make_shared<Button>("Close", Position{ 237, 104 + cyLogoOffset, 65, 20 });
    closeButton->ClickEvent() +=
        [](ISharedControl control)
        {
            control->parent()->close();
        };
    dlg->add(closeButton);

    dlg->visible(true);
    closeButton->setFocus();

    dlg->exec();
    dlg.reset();
}
