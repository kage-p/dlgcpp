#include "about_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

void dialogs_about_demo(ISharedDialog parent)
{
    auto aboutDlg = std::make_shared<AboutDialog>(parent);

    aboutDlg->title("About DLGCPP");
    aboutDlg->appDetails("DLGCPP Demo Application v1.00, (c) 2024 Kevin Peel");
    aboutDlg->releaseDate("Released on [release date]");
    aboutDlg->description("Welcome to DLGCPP Demo App!");
    aboutDlg->homePageLink("https://github.com/kage-p/dlgcpp");
    aboutDlg->logoBitmapId("#100");
    aboutDlg->show();
}
