
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

    auto lblEntry = std::make_shared<Label>("Entry:", Position{40, 10, 20, 15});
    lblEntry->colors(Color::Gray, Color::None);
    lblEntry->horizontalAlignment(HorizontalAlign::Right);
    lblEntry->verticalAlignment(VerticalAlign::Center);
    dlg->add(lblEntry);

    auto txtEntry = std::make_shared<TextBox>("Text Entry", Position{65, 10, 120, 15});
    txtEntry->colors(Color::Blue, Color::Orange);
    dlg->add(txtEntry);

    auto btnAdd = std::make_shared<Button>("Add", Position{190,10,50,15});
    dlg->add(btnAdd);

    auto btnRemove = std::make_shared<Button>("Remove", Position{245,10,50,15});
    dlg->add(btnRemove);

    auto btnAbout = std::make_shared<Button>("About", Position{300,10,50,15});
    dlg->add(btnAbout);

    auto lstItems = std::make_shared<ListBox>(Position{10,30,340,180});
    lstItems->colors(Color::Black, Color::None);
    dlg->add(lstItems);

    auto lblPosition = std::make_shared<Label>("", Position{10,215,0,0});
    lblPosition->colors(Color::Gray, Color::None);
    lblPosition->autoSize(true);
    dlg->add(lblPosition);

    if (true)
    {
        auto menu = std::make_shared<Menu>();

        std::shared_ptr<IMenuItem> item;

        auto file = std::make_shared<MenuItem>("&File");
        menu->add(file);

        item = std::make_shared<MenuItem>("&New");
        item->Clicked() += [dlg]() { dlg->message("New selected"); };
        file->add(item);

        item = std::make_shared<MenuItem>("&Open...");
        item->Clicked() += [dlg]() { dlg->message("Open selected"); };
        file->add(item);

        item = std::make_shared<MenuItem>("&Save");
        item->Clicked() += [dlg]() { dlg->message("Save selected"); };
        file->add(item);

        item = std::make_shared<MenuItem>("Save &as...");
        item->Clicked() += [dlg]() { dlg->message("Save As selected"); };
        file->add(item);

        item = std::make_shared<MenuItem>();
        file->add(item);

        item = std::make_shared<MenuItem>("E&xit");
        item->Clicked() += [dlg]() { dlg->close(); };
        file->add(item);

        auto edit = std::make_shared<MenuItem>("&Edit");
        menu->add(edit);

        item = std::make_shared<MenuItem>("&Add");
        item->Clicked() += [btnAdd]() { btnAdd->ClickEvent().invoke(); };
        edit->add(item);

        item = std::make_shared<MenuItem>("&Remove");
        item->Clicked() += [btnRemove]() { btnRemove->ClickEvent().invoke(); };
        edit->add(item);

        item = std::make_shared<MenuItem>();
        edit->add(item);

        item = std::make_shared<MenuItem>("&Clear");
        item->Clicked() += [lstItems]() { lstItems->items(std::vector<std::string>()); };
        edit->add(item);

        auto view = std::make_shared<MenuItem>("&View");
        menu->add(view);

        auto help = std::make_shared<MenuItem>("&Help");
        menu->add(help);

        item = std::make_shared<MenuItem>("About");
        item->Clicked() += [btnAbout]() { btnAbout->ClickEvent().invoke(); };
        help->add(item);

        dlg->menu(menu);
    }

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

    btnAbout->ClickEvent() += [dlg]()
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

    // auto subDlg = std::make_shared<Dialog>(dlg);
    // subDlg->title("Subdialog");
    // subDlg->type(DialogType::Tool);
    // subDlg->color(Color::Gray);
    // subDlg->resize(80,120);
    // subDlg->visible(true);

    dlg->exec();
    return 0;
}
