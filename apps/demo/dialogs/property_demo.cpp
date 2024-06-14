#include "property_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;

void dialogs_property_demo(ISharedDialog parent)
{
    auto propertyDlg = std::make_shared<PropertyDialog>(parent);

    propertyDlg->title("Property Pages");

    // TODO: add options

    auto r = propertyDlg->show();
    if (r)
        parent->message("Property dialog saved");
}
