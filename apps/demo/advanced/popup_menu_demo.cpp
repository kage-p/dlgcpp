#include "popup_menu_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void advanced_popup_menu_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Popup Menu Demo");
    dlg->resize({300,200});
    dlg->center();

    auto label = std::make_shared<Label>("Right click anywhere on dialog...", Position{10,10,0,0});
    label->autoSize(true);
    dlg->add(label);

    auto menu = std::make_shared<Menu>();
    std::shared_ptr<IMenuItem> item;

    item = std::make_shared<MenuItem>("Menu Item 1");
    item->ClickEvent() += [dlg](ISharedMenuItem m)
    {
        dlg->message("Menu Item 1 clicked");
        m->enabled(false);
    };

    menu->add(item);

    item = std::make_shared<MenuItem>("Menu Item 2");
    item->ClickEvent() += [dlg](ISharedMenuItem m)
    {
        dlg->message("Menu Item 2 clicked");
        m->enabled(false);
    };
    menu->add(item);

    item = std::make_shared<MenuItem>("Menu Item 3");
    item->ClickEvent() += [dlg](ISharedMenuItem m)
    {
        dlg->message("Menu Item 3 clicked");
        m->enabled(false);
    };
    menu->add(item);

    item = std::make_shared<MenuItem>("");
    menu->add(item);

    item = std::make_shared<MenuItem>("Close Dialog");
    item->ClickEvent() += [dlg]()
    {
        dlg->close();
    };
    menu->add(item);

    dlg->ClickEvent() += [menu](ISharedDialog dlg, MouseButton button, Point point)
    {
        if (button == MouseButton::Right)
            menu->popup(dlg, point);
    };

    dlg->exec();
}
