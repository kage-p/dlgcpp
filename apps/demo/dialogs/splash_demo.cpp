#include "splash_demo.h"

using namespace dlgcpp;

std::shared_ptr<SplashDialog> splashDlg;

void dialogs_splash_demo(ISharedDialog parent)
{
    splashDlg = std::make_shared<SplashDialog>(parent);

    splashDlg->logoImage() = ImageSource("#100", false, false);
    splashDlg->timeout() = 5000;
    splashDlg->message() = "Splash for DLGCPP Demo Application. Click to dismiss";

    splashDlg->show();
}
