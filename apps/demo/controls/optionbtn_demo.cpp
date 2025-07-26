#include "optionbtn_demo.h"

using namespace dlgcpp;

void controls_optionbtn_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("OptionButton control demo");
    dlg->resize({ 200,95 });
    dlg->center();

    auto button1 = std::make_shared<OptionButton>("Option 1", Position{ 10, 10, 90, 15 });
    dlg->add(button1);

    auto button2 = std::make_shared<OptionButton>("Option 2", Position{ 10, 30, 90, 15 });
    dlg->add(button2);

    auto button3 = std::make_shared<OptionButton>("Option 3", Position{ 10, 50, 90, 15 });
    dlg->add(button3);

    auto label = std::make_shared<Label>("(status)", Position{ 10, 75, 90, 15 });
    label->font(Font("sans serif", 9, true));
    dlg->add(label);

    auto eventHandler = [label, button1, button2, button3](ISharedControl ctl)
        {
            // check the button we want
            button1->checked(ctl == button1);
            button2->checked(ctl == button2);
            button3->checked(ctl == button3);

            // update the status
            std::string status;
            if (button1->checked())
                status = "1";
            else if (button2->checked())
                status = "2";
            else if (button3->checked())
                status = "3";

            label->text("Selected button " + status);
        };

    button1->ClickEvent() += eventHandler;
    button2->ClickEvent() += eventHandler;
    button3->ClickEvent() += eventHandler;

    // initial update
    eventHandler(button1);

    dlg->exec();
}
