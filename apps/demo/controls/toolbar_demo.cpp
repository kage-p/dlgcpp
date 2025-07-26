#include "toolbar_demo.h"

using namespace dlgcpp;

void controls_toolbar_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Application, parent);
    dlg->title("ToolBar control demo");
    dlg->color(Color::White);
    dlg->resize({ 350,80 });
    dlg->center();

    auto toolbar = std::make_shared<ToolBar>(Position{ 0, 0, 350, 50 });
    toolbar->colors(Color::None, Color::None);
    toolbar->imageSize(Size(16, 16));
    toolbar->buttonSize(Size(48, 32));

    auto buttons = std::vector<ISharedToolBarItem>();

    auto button1 = std::make_shared<ToolBarItem>(
        ImageSource("images/file.png", false, true),
        "New",
        "Creates a new document");
    button1->ClickEvent() += [dlg](auto) { dlg->message("New Clicked"); };
    buttons.push_back(button1);

    auto button2 = std::make_shared<ToolBarItem>(
        ImageSource("images/folder-open.png", false, true),
        "Open",
        "Opens an existing document");
    button2->ClickEvent() += [dlg](auto) { dlg->message("Open Clicked"); };
    buttons.push_back(button2);

    auto button3 = std::make_shared<ToolBarItem>(
        ImageSource("images/save.png", false, true),
        "Save",
        "Saves the open document");
    button3->ClickEvent() += [dlg](auto) { dlg->message("Save Clicked"); };
    buttons.push_back(button3);

    auto button4 = std::make_shared<ToolBarItem>(
        ImageSource("images/save-all.png", false, true),
        "Save all",
        "Saves all open documents");
    button4->ClickEvent() += [dlg](auto) { dlg->message("Save All Clicked"); };
    buttons.push_back(button4);

    auto button5 = std::make_shared<ToolBarItem>(
        ImageSource("images/settings.png", false, true),
        "Settings",
        "Show settings");
    button5->ClickEvent() += [dlg](auto) { dlg->message("Settings Clicked"); };
    buttons.push_back(button5);

    auto button6 = std::make_shared<ToolBarItem>(
        ImageSource("images/badge-question-mark.png", false, true),
        "Help",
        "Shows the help document");
    button6->ClickEvent() += [dlg](auto) { dlg->message("Help Clicked"); };
    buttons.push_back(button6);

    toolbar->items(buttons);

    dlg->add(toolbar);

    dlg->exec();
}
