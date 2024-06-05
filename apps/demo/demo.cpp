
#include "demo.h"

#include "dlgcpp/dlgcpp.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

int main()
{
    // indirect method: create all components from external function

    auto dlg = std::make_shared<Dialog>();
    dlg->title("DLGCPP Demo Application");
    dlg->image(ImageSource{"#100", true, false});
    dlg->color(Color::White);
    dlg->resize(355,150);
    dlg->center();

    auto label = std::make_shared<Label>(dlg, "Label:", Position{10, 10, 50, 15});
    label->horizontalAlignment(HorizontalAlign::Right);
    label->verticalAlignment(VerticalAlign::Center);
    dlg->add(label);

    auto textbox = std::make_shared<TextBox>(dlg, "Text Entry", Position{65, 10, 120, 15});
    textbox->colors(Color::Blue, Color::Orange);
    dlg->add(textbox);

    auto button1 = std::make_shared<Button>(dlg, "Add", Position{190,10,50,15});
    button1->colors(Color::Magenta,Color::Yellow);
    button1->ClickEvent() += [textbox]()
    {
        auto text = textbox->text();
        textbox->text(text + "X");
    };
    dlg->add(button1);

    auto button2 = std::make_shared<Button>(dlg, "Remove", Position{245,10,50,15});
    button2->ClickEvent() += [textbox]()
    {
        auto text = textbox->text();
        if (text.empty())
            return;
        text.pop_back();
        textbox->text(text);
    };
    dlg->add(button2);

    auto button3 = std::make_shared<Button>(dlg, "Info", Position{300,10,50,15});
    button3->ClickEvent() += [dlg]()
    {
        dlg->message("Welcome to DLGCPP Demo App!", "DLGCPP Demo");
    };
    dlg->add(button3);

    dlg->ClickEvent() += [dlg]()
    {
        // change the frame
        if (dlg->type() != DialogType::Application)
        {
            dlg->type(DialogType::Application);
            dlg->color(Color::White);
        }
        else
        {
            dlg->type(DialogType::Tool);
            dlg->color(Color::Blue);
        }
    };

    dlg->exec();
    return 0;
}
