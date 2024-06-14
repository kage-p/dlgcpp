#include "splash_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

std::shared_ptr<SplashDialog> splashDlg;

void dialogs_splash_demo(ISharedDialog parent)
{
    splashDlg = std::make_shared<SplashDialog>(parent);

    splashDlg->logoBitmapId("#100");
    splashDlg->timeout(5000);
    splashDlg->message("Splash for DLGCPP Demo Application. Click to dismiss");

    splashDlg->show();
}
