#include "dialog_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

std::shared_ptr<Dialog> dlg;


void dialogs_modal_demo(ISharedDialog parent)
{
    dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Modal Popup Demo");
    dlg->resize({240,120});
    dlg->center();

    dlg->exec();
    dlg.reset();
}

void dialogs_modeless_demo(ISharedDialog parent)
{
    dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Modeless Popup Demo");
    dlg->resize({240,120});
    dlg->center();

    dlg->visible(true);
}

void dialogs_tool_demo(ISharedDialog parent)
{
    dlg = std::make_shared<Dialog>(DialogType::Tool, parent);
    dlg->title("Modeless Tool Demo");
    dlg->move({parent->p().x()+parent->p().width(), parent->p().y()});
    dlg->resize({80,parent->p().height()});
    //dlg->center();

    dlg->visible(true);
}

void dialogs_child_demo(ISharedDialog parent)
{
    dlg = std::make_shared<Dialog>(DialogType::Application, parent);
    dlg->title("Child Dialog Demo");
    dlg->resize({400,250});
    dlg->center();

    auto subDlg = std::make_shared<Dialog>();
    dlg->add(subDlg);
    auto button = std::make_shared<Button>("Color", Position{0,0,50,15});
    subDlg->add(button);

    subDlg->color(Color::Blue);
    subDlg->visible(true);

    dlg->SizeEvent() += [subDlg]()
    {
        int margin = 10;
        subDlg->move({margin, margin});
        subDlg->resize({dlg->p().width() - (margin * 2),
                        dlg->p().height() - (margin * 2)});
    };

    button->ClickEvent() += [subDlg]()
    {
        auto color = subDlg->color() == Color::Red ? Color::Blue : Color::Red;
        subDlg->color(color);
    };

    dlg->exec();
}
