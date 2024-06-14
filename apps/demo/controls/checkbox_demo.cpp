#include "checkbox_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_checkbox_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("CheckBox control demo");
    dlg->resize({200,80});
    dlg->center();

    auto button1 = std::make_shared<CheckBox>("Press Me", Position{10, 10, 60, 15});
    button1->ClickEvent() += [button1]() { button1->checked(!button1->checked()); };
    dlg->add(button1);

    auto button2 = std::make_shared<CheckBox>("Push Button", Position{10, 30, 60, 15});
    button2->ClickEvent() += [button2]() { button2->checked(!button2->checked()); };
    dlg->add(button2);

    dlg->exec();
}
