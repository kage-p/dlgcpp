
#include "demo.h"

#include "dlgcpp/dlgcpp.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

int main()
{
    // indirect method: create all components from external function

    auto dlg = std::make_shared<Dialog>();
    dlg->title("DLGCPP Demo Application");
    dlg->type(DialogType::Popup);
    dlg->image(ImageSource{"#100", true, false});
    dlg->color(Color::White);
    dlg->resize(360,230);
    dlg->center();

    auto lblEntry = std::make_shared<Label>(dlg, "Entry:", Position{10, 10, 50, 15});
    lblEntry->colors(Color::Gray, Color::None);
    lblEntry->horizontalAlignment(HorizontalAlign::Right);
    lblEntry->verticalAlignment(VerticalAlign::Center);
    dlg->add(lblEntry);

    auto txtEntry = std::make_shared<TextBox>(dlg, "Text Entry", Position{65, 10, 120, 15});
    txtEntry->colors(Color::Blue, Color::Orange);
    dlg->add(txtEntry);

    auto btnAdd = std::make_shared<Button>(dlg, "Add", Position{190,10,50,15});
    dlg->add(btnAdd);

    auto btnRemove = std::make_shared<Button>(dlg, "Remove", Position{245,10,50,15});
    dlg->add(btnRemove);

    auto btnInfo = std::make_shared<Button>(dlg, "Info", Position{300,10,50,15});
    dlg->add(btnInfo);

    auto lstItems = std::make_shared<ListBox>(dlg, Position{10,30,340,180});
    lstItems->colors(Color::Black, Color::None);
    dlg->add(lstItems);

    auto lblPosition = std::make_shared<Label>(dlg, "", Position{10,215,0,0});
    lblPosition->colors(Color::Gray, Color::None);
    lblPosition->autoSize(true);
    dlg->add(lblPosition);

    btnAdd->ClickEvent() += [txtEntry,lstItems]()
    {
        auto items = lstItems->items();
        items.push_back(txtEntry->text());
        lstItems->items(items);
    };

    btnRemove->ClickEvent() += [txtEntry,lstItems]()
    {
        auto items = lstItems->items();
        if (items.empty() || lstItems->currentIndex() < 0)
            return;

        items.erase(items.begin() + lstItems->currentIndex());
        lstItems->items(items);
    };

    btnInfo->ClickEvent() += [dlg]()
    {
        dlg->message("Welcome to DLGCPP Demo App!", "DLGCPP Demo");
    };

    lstItems->SelChangedEvent() += [dlg,lstItems,txtEntry,lblPosition]()
    {
        if (lstItems->currentIndex() < 0)
            lblPosition->text("No selection");
        else
            lblPosition->text("Selected item: " + std::to_string(lstItems->currentIndex()));

        auto items = lstItems->items();
        if (items.empty() || lstItems->currentIndex() < 0)
            return;

        txtEntry->text(items.at(lstItems->currentIndex()));
    };

    dlg->ClickEvent() += [dlg]()
    {
        if (dlg->color() != Color::White)
        {
            dlg->color(Color::White);
        }
        else
        {
            dlg->color(Color::Blue);
        }
    };

    // add some items
    btnAdd->ClickEvent().invoke();
    btnAdd->ClickEvent().invoke();
    btnAdd->ClickEvent().invoke();
    btnAdd->ClickEvent().invoke();
    btnAdd->ClickEvent().invoke();
    lstItems->SelChangedEvent().invoke();

    dlg->exec();
    return 0;
}
