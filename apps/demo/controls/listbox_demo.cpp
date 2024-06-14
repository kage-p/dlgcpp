#include "listbox_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_listbox_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("ListBox control demo");
    dlg->resize({200,125});
    dlg->center();

    auto listbox = std::make_shared<ListBox>(Position{10, 10, 180, 105});
    listbox->colors(Color::Black, Color::Orange);
    listbox->items(std::vector<std::string>{ "Option A", "Option B", "Option C", "Option D", "Option E"});
    listbox->font(Font{"sans serif", 10, true});
    dlg->add(listbox);

    listbox->DoubleClickEvent() += [dlg,listbox]()
    {
        dlg->message("You picked: " + listbox->text());
    };

    dlg->exec();
}
