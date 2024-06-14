#include "combobox_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_combobox_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("ComboBox control demo");
    dlg->resize({200,180});
    dlg->center();

    // editable with permanent list
    auto combo1 = std::make_shared<ComboBox>(Position{10, 10, 180, 100});
    combo1->colors(Color::Black, Color::Orange);
    combo1->items(std::vector<std::string>{ "Option A", "Option B", "Option C", "Option D", "Option E"});
    combo1->font(Font{"sans serif", 10, true});
    dlg->add(combo1);

    combo1->DoubleClickEvent() += [dlg,combo1]()
    {
        dlg->message("You picked: " + combo1->text());
    };

    // editable drop-down
    auto combo2 = std::make_shared<ComboBox>(Position{10, 115, 180, 50});
    combo2->dropDown(true);
    combo2->colors(Color::Black, Color::Orange);
    combo2->items(std::vector<std::string>{ "Option A", "Option B", "Option C", "Option D", "Option E"});
    combo2->font(Font{"sans serif", 10, true});
    dlg->add(combo2);

    // list drop-down
    auto combo3 = std::make_shared<ComboBox>(Position{10, 140, 180, 50});
    combo3->dropDown(true);
    combo3->editable(false);
    combo3->colors(Color::Black, Color::Orange);
    combo3->items(std::vector<std::string>{ "Option A", "Option B", "Option C", "Option D", "Option E"});
    combo3->font(Font{"sans serif", 10, true});
    dlg->add(combo3);

    dlg->exec();
}
