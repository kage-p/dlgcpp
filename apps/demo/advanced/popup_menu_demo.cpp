#include "popup_menu_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void advanced_popup_menu_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Popup Menu Demo");
    dlg->resize({ 300,200 });
    dlg->center();

    auto note = std::make_shared<Label>("Right click anywhere on dialog...", Position{ 10,10,0,0 });
    note->autoSize(true);
    dlg->add(note);

    auto menu = std::make_shared<Menu>();
    std::shared_ptr<IMenuItem> item;

    item = std::make_shared<MenuItem>("Priority/default Item");
    item->defaultItem(true);
    item->ClickEvent() += [dlg](ISharedMenuItem m)
        {
            dlg->message("Priority Item clicked");
        };
    menu->add(item);

    item = std::make_shared<MenuItem>("");
    menu->add(item);

    item = std::make_shared<MenuItem>("Perform Task 1");
    item->ClickEvent() += [dlg](ISharedMenuItem m)
        {
            dlg->message("Perform Task 1 clicked");
            m->enabled(false);
        };
    menu->add(item);

    item = std::make_shared<MenuItem>("Perform Task 2");
    item->ClickEvent() += [dlg](ISharedMenuItem m)
        {
            dlg->message("Perform Task 2 clicked");
            m->enabled(false);
        };
    menu->add(item);

    item = std::make_shared<MenuItem>("");
    menu->add(item);

    item = std::make_shared<MenuItem>("Check Item 1");
    item->ClickEvent() += [dlg](ISharedMenuItem m)
        {
            m->checked(!m->checked());
        };
    menu->add(item);

    item = std::make_shared<MenuItem>("Check Item 2");
    item->ClickEvent() += [dlg](ISharedMenuItem m)
        {
            m->checked(!m->checked());
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

    dlg->MouseDownEvent() += [menu](ISharedDialog dlg, MouseEvent event)
        {
            if (event.button == MouseButton::Right)
                menu->popup(dlg, event.point);
        };

    dlg->exec();
}
