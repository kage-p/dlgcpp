#include "custom_demo.h"

using namespace dlgcpp;

void controls_custom_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Custom control demo");
    dlg->resize({ 300,250 });
    dlg->center();

    auto label = std::make_shared<Label>("Class name:", Position{ 10, 10, 60, 15 });
    label->horizontalAlignment() = HorizontalAlign::Right;
    label->verticalAlignment() = VerticalAlign::Center;
    dlg->add(label);

    auto textBox = std::make_shared<TextBox>("static", Position{ 75, 10, 100, 15 });
    dlg->add(textBox);

    auto custom = std::make_shared<Custom>(textBox->text(), Position{ 10, 30, 280, 210 });
    custom->wantMouseEvents(true);
    custom->border(BorderStyle::Thin);
    dlg->add(custom);

    custom->MouseUpEvent() +=
        [&](auto, MouseEvent)
        {
            dlg->message("Mouse button pressed");
        };

    textBox->text().event() +=
        [&](auto)
        {
            custom->className() = textBox->text();
        };

    dlg->exec();
}
