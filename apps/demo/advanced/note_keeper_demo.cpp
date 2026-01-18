#include "note_keeper_demo.h"

using namespace dlgcpp;

void advanced_note_keeper_demo(ISharedDialog parent)
{
    static ISharedDialog dlg;
    if (dlg != nullptr && dlg->visible())
    {
        dlg->exec();
        return;
    }

    dlg = std::make_shared<Dialog>(DialogType::Application, parent);
    dlg->title() = "Note Keeper Demo";
    dlg->color() = Color::White;
    dlg->image() = ImageSource{ "#100",true };
    dlg->resize({ 400,165 });
    dlg->center();

    auto entryLabel = std::make_shared<Label>("Entry:", Position{ 10, 10, 50, 15 });
    entryLabel->colors(Color::Gray, Color::None);
    entryLabel->horizontalAlignment(HorizontalAlign::Right);
    entryLabel->verticalAlignment(VerticalAlign::Center);
    dlg->add(entryLabel);

    auto entryTextBox = std::make_shared<TextBox>("Text Entry", Position{ 65, 10, 120, 15 });
    dlg->add(entryTextBox);

    auto addItemButton = std::make_shared<Button>("Add", Position{ 190,10,50,15 });
    dlg->add(addItemButton);

    auto removeItemButton = std::make_shared<Button>("Remove", Position{ 245,10,50,15 });
    dlg->add(removeItemButton);

    auto itemList = std::make_shared<ListBox>(Position{ 10,10,10,10 });
    dlg->add(itemList);

    auto itemPosLabel = std::make_shared<Label>("", Position{ 310,13,0,0 });
    itemPosLabel->colors(Color::Gray, Color::None);
    itemPosLabel->autoSize(true);
    dlg->add(itemPosLabel);

    {
        auto menu = std::make_shared<Menu>();

        ISharedMenuItem item;

        auto file = std::make_shared<MenuItem>("&File");
        menu->add(file);

        item = std::make_shared<MenuItem>("&New");
        item->ClickEvent() += [&](auto) { dlg->message("New selected", dlg->title()); };
        file->add(item);

        item = std::make_shared<MenuItem>("&Open...");
        item->ClickEvent() += [&](auto) { dlg->message("Open selected", dlg->title()); };
        file->add(item);

        item = std::make_shared<MenuItem>("&Save");
        item->ClickEvent() += [&](auto) { dlg->message("Save selected", dlg->title()); };
        file->add(item);

        item = std::make_shared<MenuItem>("Save &as...");
        item->ClickEvent() += [&](auto) { dlg->message("Save As selected", dlg->title()); };
        file->add(item);

        item = std::make_shared<MenuItem>();
        file->add(item);

        item = std::make_shared<MenuItem>("E&xit");
        item->ClickEvent() += [&](auto) { dlg->close(); };
        file->add(item);

        auto edit = std::make_shared<MenuItem>("&Edit");
        menu->add(edit);

        item = std::make_shared<MenuItem>("&Add");
        item->ClickEvent() += [addItemButton](auto) { addItemButton->ClickEvent().invoke(); };
        edit->add(item);

        item = std::make_shared<MenuItem>("&Remove");
        item->ClickEvent() += [removeItemButton](auto) { removeItemButton->ClickEvent().invoke(); };
        edit->add(item);

        item = std::make_shared<MenuItem>();
        edit->add(item);

        item = std::make_shared<MenuItem>("&Clear");
        item->ClickEvent() += [itemList](auto) { itemList->items(std::vector<std::string>()); };
        edit->add(item);

        dlg->menu() = menu;
    }

    entryTextBox->ChangedEvent() += [entryTextBox, addItemButton](auto)
        {
            bool isTextPresent = !entryTextBox->text()->empty();
            addItemButton->enabled(isTextPresent);
        };

    addItemButton->ClickEvent() += [entryTextBox, itemList](auto)
        {
            std::vector<std::string> items = itemList->items().value();
            items.push_back(entryTextBox->text());
            itemList->items(items);
        };

    removeItemButton->ClickEvent() += [entryTextBox, itemList](auto)
        {
            std::vector<std::string> items = itemList->items().value();
            if (items.empty() || itemList->selectedIndex() < 0)
                return;

            items.erase(items.begin() + itemList->selectedIndex());
            itemList->items(items);
        };

    itemList->SelChangedEvent() += [itemList, entryTextBox, itemPosLabel, removeItemButton](auto)
        {
            if (itemList->selectedIndex() < 0)
            {
                removeItemButton->enabled(false);
                itemPosLabel->text("No selection");
            }
            else
            {
                removeItemButton->enabled(true);
                itemPosLabel->text("Selected item: " + std::to_string(itemList->selectedIndex()));
            }

            const auto& items = itemList->items().value();
            if (items.empty() || itemList->selectedIndex() < 0)
                return;

            entryTextBox->text(items.at(itemList->selectedIndex()));
        };

    auto resizeHandler = [itemList](ISharedDialog dlg)
        {
            auto width = dlg->p()->width();
            auto height = dlg->p()->height();

            Position pos(5,
                30,
                width - 10,
                height - 35);
            itemList->p(pos);
        };

    dlg->p().event() += resizeHandler;
    resizeHandler(dlg);

    // add some items
    itemList->items() = { "List Item 1", "List Item 2", "List Item 3", "List Item 4", "List Item 5" };

    dlg->exec();
}
