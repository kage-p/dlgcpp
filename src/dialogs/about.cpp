#include "about.h"
#include "../utility.h"
#include <shellapi.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

AboutDialog::AboutDialog(std::shared_ptr<IDialog> parent)
    : _parent(parent)
{     
}

const std::string& AboutDialog::title() const
{
    return _titleText;
}

void AboutDialog::title(const std::string& value)
{
    _titleText = value;
}

const std::string& AboutDialog::appDetails() const
{
    return _appDetailsText;
}

void AboutDialog::appDetails(const std::string& value)
{
    _appDetailsText = value;
}

const std::string& AboutDialog::description() const
{
    return _descriptionText;
}

void AboutDialog::description(const std::string& value)
{
    _descriptionText = value;
}

const std::string& AboutDialog::homePageLink() const
{
    return _homePageLink;
}

void AboutDialog::homePageLink(const std::string& value)
{
    _homePageLink = value;
}

const std::string& AboutDialog::releaseDate() const
{
    return _releaseDateText;
}

void AboutDialog::releaseDate(const std::string& value)
{
    _releaseDateText = value;
}

const std::string& AboutDialog::logoBitmapId() const
{
    return _logoBitmapId;
}

void AboutDialog::logoBitmapId(const std::string& value)
{
    _logoBitmapId = value;
}


void AboutDialog::show()
{
    auto dlg = std::make_shared<Dialog>(_parent);
    dlg->title(_titleText);

    // TODO: split into createInterface
    auto hwnd = reinterpret_cast<HWND>(dlg->handle());

    int lw = 0;

    // Add image height
    int cyBmp = 0;
    if (!_logoBitmapId.empty())
        cyBmp = 50+15;

    dlg->type(DialogType::Popup);
    dlg->resize(310, 130 + cyBmp);
    dlg->center();

    if (!_logoBitmapId.empty())
    {
        // Use bitmap...
        _logoImage = std::make_shared<Image>(dlg, Position{7, 7, 295, 65});
        _logoImage->colors(Color::Black, Color::White);
        _logoImage->imageId(_logoBitmapId);
        dlg->add(_logoImage);
    }

    //if (lw)
    //{
    //   // Use icon...
    //   dlgImage(hDlg, IDC_ABOUT_ICO, NULL, 7, 13+cyBmp, 5, 5, WS_CHILD | WS_VISIBLE | SS_ICON, 0);
    //   dlgSend(hDlg, IDC_ABOUT_ICO, STM_SETICON, (WPARAM)hIconLogo, 0);
    //}

    _appDetails = std::make_shared<Label>(dlg, _appDetailsText, Position{7+lw, 15+cyBmp, 295-lw, 10});
    _appDetails->font(Font{"sans serif", 8, true});
    dlg->add(_appDetails);

    if (!_releaseDateText.empty())
    {
        _releaseInfo = std::make_shared<Label>(dlg, _releaseDateText, Position{7+lw, 26+cyBmp, 295-lw, 10});
        dlg->add(_releaseInfo);
    }

    if (!_homePageLink.empty())
    {
        _webLink = std::make_shared<Label>(dlg, _homePageLink, Position{7+lw, 36+cyBmp, 150, 12});
        _webLink->colors(Color::Blue, Color::None);
        _webLink->cursor(Cursor::Hand);
        _webLink->font(Font{"MS Sans Serif", 8, false, true});
        _webLink->autoSize(true);
        _webLink->CommandEvent() +=
            [this,dlg]()
        {
            auto hwnd = reinterpret_cast<HWND>(dlg->handle());
            ShellExecuteW(hwnd, L"open", toWide(_homePageLink).c_str(), NULL, NULL, SW_SHOWNORMAL);
        };
        dlg->add(_webLink);
    }

    _description = std::make_shared<TextBox>(dlg, _descriptionText, Position{7, 49+cyBmp, 295, 50});
    _description->readOnly(true);
    _description->multiline(true);
    _description->wrapText(true);
    dlg->add(_description);

    _sysInfoButton = std::make_shared<Button>(dlg, "&System Info...", Position{167, 104+cyBmp, 65, 20});
    dlg->add(_sysInfoButton);
    _sysInfoButton->CommandEvent() +=
        [dlg]()
    {
            auto hwnd = reinterpret_cast<HWND>(dlg->handle());
            ShellExecuteW(hwnd, L"open", L"MSINFO32.EXE", NULL, NULL, SW_SHOWNORMAL);
    };

    _closeButton = std::make_shared<Button>(dlg, "Close", Position{237, 104+cyBmp, 65, 20});
    dlg->add(_closeButton);
    _closeButton->CommandEvent() +=
        [dlg]()
    {
        dlg->close();
    };

    dlg->visible(true);
    _closeButton->setFocus();

    dlg->exec();
    dlg.reset();
}
