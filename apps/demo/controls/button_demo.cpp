#include "button_demo.h"

using namespace dlgcpp;

void controls_button_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Button control demo");
    dlg->resize({ 200,80 });
    dlg->center();

    auto button1 = std::make_shared<Button>("Button 1", Position{ 10, 10, 60, 15 });
    button1->ClickEvent() += [dlg](auto) { dlg->message("Button 1 pressed!"); };
    dlg->add(button1);

    auto button2 = std::make_shared<Button>("Button 2", Position{ 10, 30, 60, 15 });
    button2->ClickEvent() += [dlg](auto) { dlg->message("Button 2 pressed!"); };
    dlg->add(button2);

    auto button3 = std::make_shared<Button>("Button 3", Position{ 10, 50, 60, 15 });
    button3->ClickEvent() += [dlg](auto) { dlg->message("Button 3 pressed!"); };
    dlg->add(button3);

    dlg->exec();
}
