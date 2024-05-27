
#include "demo.h"

using namespace dlgcpp;

DemoDialog::DemoDialog()
{
    title("DLGCPP Demo Application");
}

DemoDialog::~DemoDialog()
{

}

int main()
{
    DemoDialog dialog;

    auto label = dialog.label("Label", {10, 10, 80, 22});
    auto textbox = dialog.textBox("Text", {95, 10, 150, 22});
    auto button = dialog.button("OK", {10,35,80,25});


    dialog.exec();
    return 0;
}
