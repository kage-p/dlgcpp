#include "dialog_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

std::shared_ptr<Dialog> dlg;


void dialogs_modal_demo(ISharedDialog parent)
{
    dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Modal Popup Demo");
    dlg->resize({ 240,120 });
    dlg->center();

    dlg->exec();
    dlg.reset();
}

void dialogs_modeless_demo(ISharedDialog parent)
{
    dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Modeless Popup Demo");
    dlg->resize({ 240,120 });
    dlg->center();

    dlg->visible(true);
}

void dialogs_tool_demo(ISharedDialog parent)
{
    dlg = std::make_shared<Dialog>(DialogType::Tool, parent);
    dlg->title("Modeless Tool Demo");
    dlg->move({ parent->p().x() + parent->p().width(), parent->p().y() });
    dlg->resize({ 80,parent->p().height() });

    dlg->visible(true);
}

void dialogs_child_demo(ISharedDialog parent)
{
    dlg = std::make_shared<Dialog>(DialogType::Application, parent);
    dlg->title("Child Dialog Demo");
    dlg->resize({ 400,250 });
    dlg->center();

    auto subDlg = std::make_shared<Dialog>();
    subDlg->color(Color::Blue);
    subDlg->visible(true);
    dlg->add(subDlg);

    auto button = std::make_shared<Button>("Color", Position{ 5,5,50,15 });
    subDlg->add(button);

    auto label = std::make_shared<Label>("", Position{ 5,25,150,15 });
    label->colors(Color::White, Color::None);
    label->clickable(false);
    subDlg->add(label);

    dlg->SizeEvent() += [subDlg](auto)
        {
            int margin = 10;
            subDlg->move({ margin, margin });
            subDlg->resize({ dlg->p().width() - (margin * 2),
                            dlg->p().height() - (margin * 2) });
        };

    subDlg->MouseMoveEvent() += [label](ISharedDialog dlg, MouseEvent event)
        {
            label->text(
                "Mouse Position: " +
                std::to_string(event.point.x()) + ", " +
                std::to_string(event.point.y()));
        };

    button->ClickEvent() += [subDlg](auto)
        {
            auto color = subDlg->color() == Color::Red ? Color::Blue : Color::Red;
            subDlg->color(color);
        };

    dlg->exec();
}
