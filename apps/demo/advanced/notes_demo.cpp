#include "notes_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void advanced_note_keeper_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Application, parent);
    dlg->title("Note Keeper Demo");
    dlg->color(Color::White);
    dlg->image(ImageSource{"#100",true});
    dlg->resize({400,165});
    dlg->center();

    auto entryLabel = std::make_shared<Label>("Entry:", Position{40, 10, 20, 15});
    entryLabel->colors(Color::Gray, Color::None);
    entryLabel->horizontalAlignment(HorizontalAlign::Right);
    entryLabel->verticalAlignment(VerticalAlign::Center);
    dlg->add(entryLabel);

    auto entryTextBox = std::make_shared<TextBox>("Text Entry", Position{65, 10, 120, 15});
    dlg->add(entryTextBox);

    auto addItemButton = std::make_shared<Button>("Add", Position{190,10,50,15});
    dlg->add(addItemButton);

    auto removeItemButton = std::make_shared<Button>("Remove", Position{245,10,50,15});
    dlg->add(removeItemButton);

    auto itemList = std::make_shared<ListBox>(Position{10,30,340,180});
    dlg->add(itemList);

    auto itemPosLabel = std::make_shared<Label>("", Position{310,13,0,0});
    itemPosLabel->colors(Color::Gray, Color::None);
    itemPosLabel->autoSize(true);
    dlg->add(itemPosLabel);

    {
        auto menu = std::make_shared<Menu>();

        std::shared_ptr<IMenuItem> item;

        auto file = std::make_shared<MenuItem>("&File");
        menu->add(file);

        item = std::make_shared<MenuItem>("&New");
        item->ClickEvent() += [dlg]() { dlg->message("New selected", "DLGCPP Demo"); };
        file->add(item);

        item = std::make_shared<MenuItem>("&Open...");
        item->ClickEvent() += [dlg]() { dlg->message("Open selected", "DLGCPP Demo"); };
        file->add(item);

        item = std::make_shared<MenuItem>("&Save");
        item->ClickEvent() += [dlg]() { dlg->message("Save selected", "DLGCPP Demo"); };
        file->add(item);

        item = std::make_shared<MenuItem>("Save &as...");
        item->ClickEvent() += [dlg]() { dlg->message("Save As selected", "DLGCPP Demo"); };
        file->add(item);

        item = std::make_shared<MenuItem>();
        file->add(item);

        item = std::make_shared<MenuItem>("E&xit");
        item->ClickEvent() += [dlg]() { dlg->close(); };
        file->add(item);

        auto edit = std::make_shared<MenuItem>("&Edit");
        menu->add(edit);

        item = std::make_shared<MenuItem>("&Add");
        item->ClickEvent() += [addItemButton]() { addItemButton->ClickEvent().invoke(); };
        edit->add(item);

        item = std::make_shared<MenuItem>("&Remove");
        item->ClickEvent() += [removeItemButton]() { removeItemButton->ClickEvent().invoke(); };
        edit->add(item);

        item = std::make_shared<MenuItem>();
        edit->add(item);

        item = std::make_shared<MenuItem>("&Clear");
        item->ClickEvent() += [itemList]() { itemList->items(std::vector<std::string>()); };
        edit->add(item);

        dlg->menu(menu);
    }

    entryTextBox->ChangedEvent() += [entryTextBox, addItemButton]()
    {
        bool isTextPresent = !entryTextBox->text().empty();
        addItemButton->enabled(isTextPresent);
    };

    addItemButton->ClickEvent() += [entryTextBox,itemList]()
    {
        auto items = itemList->items();
        items.push_back(entryTextBox->text());
        itemList->items(items);
    };

    removeItemButton->ClickEvent() += [entryTextBox,itemList]()
    {
        auto items = itemList->items();
        if (items.empty() || itemList->currentIndex() < 0)
            return;

        items.erase(items.begin() + itemList->currentIndex());
        itemList->items(items);
    };

    itemList->SelChangedEvent() += [itemList,entryTextBox,itemPosLabel,removeItemButton]()
    {
        if (itemList->currentIndex() < 0)
        {
            removeItemButton->enabled(false);
            itemPosLabel->text("No selection");
        }
        else
        {
            removeItemButton->enabled(true);
            itemPosLabel->text("Selected item: " + std::to_string(itemList->currentIndex()));
        }

        auto items = itemList->items();
        if (items.empty() || itemList->currentIndex() < 0)
            return;

        entryTextBox->text(items.at(itemList->currentIndex()));
    };

    dlg->SizeEvent() += [itemList, itemPosLabel](ISharedDialog dlg)
    {
        auto width = dlg->p().width();
        auto height = dlg->p().height();

        Position pos(5,
                     30,
                     width - 10,
                     height - 35);
        itemList->p(pos);
    };

    // add some items
    addItemButton->ClickEvent().invoke();
    addItemButton->ClickEvent().invoke();
    addItemButton->ClickEvent().invoke();
    addItemButton->ClickEvent().invoke();
    addItemButton->ClickEvent().invoke();
    itemList->SelChangedEvent().invoke();

    dlg->exec();
}
