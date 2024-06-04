
#include "demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

DemoDialog::DemoDialog() : Dialog()
{
    title("DLGCPP Demo Application");
}

DemoDialog::~DemoDialog()
{
}

int main()
{
    auto dlg = std::make_shared<DemoDialog>();
    dlg->resize(355,150);
    dlg->center();

    auto label = std::make_shared<Label>(dlg, "Label:", Position{10, 10, 50, 15});
    dlg->add(label);

    auto textbox = std::make_shared<TextBox>(dlg, "Text Entry", Position{65, 10, 120, 15});
    textbox->colors(Color::Blue, Color::Orange);
    dlg->add(textbox);

    auto button1 = std::make_shared<Button>(dlg, "Add", Position{190,10,50,15});
    button1->CommandEvent() += [textbox]()
    {
        auto text = textbox->text();
        textbox->text(text + "X");
    };
    dlg->add(button1);

    auto button2 = std::make_shared<Button>(dlg, "Remove", Position{245,10,50,15});
    button2->CommandEvent() += [textbox]()
    {
        auto text = textbox->text();
        text.pop_back();
        textbox->text(text);
    };
    dlg->add(button2);

    auto button3 = std::make_shared<Button>(dlg, "Info", Position{300,10,50,15});
    button3->CommandEvent() += [dlg]()
    {
        dlg->message("Welcome to DLGCPP Demo App!", "DLGCPP Demo");
    };
    dlg->add(button3);

    dlg->ClickEvent() += [dlg]()
    {
        if (dlg->color() == Color::Default)
            dlg->color(Color::White);
        else
            dlg->color(Color::Default);
    };

    dlg->exec();
    return 0;
}
