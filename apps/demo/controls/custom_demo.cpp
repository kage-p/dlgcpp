#include "custom_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_custom_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Custom control demo");
    dlg->resize({200,100});
    dlg->center();

    auto custom1 = std::make_shared<Custom>("EDIT", Position{10, 10, 150, 25});
    custom1->border(BorderStyle::Thin);
    custom1->text("This is a (EDIT) custom control");
    dlg->add(custom1);

    auto custom2 = std::make_shared<Custom>("COMBOBOX", Position{10, 40, 150, 50});
    custom2->border(BorderStyle::Thin);
    custom2->text("This is a (COMBOBOX) custom control");
    dlg->add(custom2);

    dlg->exec();
}
