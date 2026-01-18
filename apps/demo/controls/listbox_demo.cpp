#include "listbox_demo.h"

using namespace dlgcpp;

void controls_listbox_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("ListBox control demo");
    dlg->resize({ 200,125 });
    dlg->center();

    auto itemList = std::vector<std::string>{ "Option A", "Option B", "Option C", "Option D", "Option E" };

    auto listbox = std::make_shared<ListBox>(Position{ 10, 10, 180, 105 });
    listbox->colors(Color::Black, Color::Orange);
    listbox->items(itemList);
    dlg->add(listbox);

    listbox->DoubleClickEvent() += [dlg, listbox, itemList](auto)
        {
            if (listbox->selectedIndex() < 0)
                return;
            dlg->message("You picked: " + itemList.at(listbox->selectedIndex()));
        };

    dlg->exec();
}
