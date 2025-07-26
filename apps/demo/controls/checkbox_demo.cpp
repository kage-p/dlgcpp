#include "checkbox_demo.h"

using namespace dlgcpp;

void controls_checkbox_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("CheckBox control demo");
    dlg->resize({ 200,80 });
    dlg->center();

    auto button1 = std::make_shared<CheckBox>("CheckBox 1", Position{ 10, 10, 90, 15 });
    dlg->add(button1);

    auto button2 = std::make_shared<CheckBox>("CheckBox 2", Position{ 10, 30, 90, 15 });
    dlg->add(button2);

    auto button3 = std::make_shared<CheckBox>("CheckBox 3 (Manual)", Position{ 10, 50, 90, 15 });
    // the button state is not changed automatically
    button3->autoCheck(false);
    dlg->add(button3);

    dlg->exec();
}
