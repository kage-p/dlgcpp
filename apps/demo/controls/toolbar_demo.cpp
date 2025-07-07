#include "toolbar_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_toolbar_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Application, parent);
    dlg->title("ToolBar control demo");
    dlg->resize({ 200,80 });
    dlg->center();

    auto toolbar = std::make_shared<ToolBar>(Position{ 0, 0, 200, 50 });
    toolbar->colors(Color::Blue, Color::Default);
    toolbar->buttonSize(Size(48, 32));

    auto buttons = std::vector<ISharedToolBarItem>();

    auto button1 = std::make_shared<ToolBarItem>(ImageSource("id1", true), "Button 1");
    button1->ClickEvent() += [dlg](auto) { dlg->message("Button1 Clicked"); };
    buttons.push_back(button1);

    auto button2 = std::make_shared<ToolBarItem>(ImageSource("id2", true), "Button 2");
    button2->ClickEvent() += [dlg](auto) { dlg->message("Button2 Clicked"); };
    buttons.push_back(button2);

    auto button3 = std::make_shared<ToolBarItem>(ImageSource("id3", true), "Button 3");
    button3->ClickEvent() += [dlg](auto) { dlg->message("Button3 Clicked"); };
    buttons.push_back(button3);

    toolbar->items(buttons);

    dlg->add(toolbar);

    dlg->exec();
}
