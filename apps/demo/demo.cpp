#include "demo.h"

#include "dlgcpp/dlgcpp.h"

#include "advanced/clock_demo.h"
#include "advanced/notes_demo.h"
#include "advanced/popup_menu_demo.h"

#include "controls/button_demo.h"
#include "controls/checkbox_demo.h"
#include "controls/combobox_demo.h"
#include "controls/custom_demo.h"
#include "controls/image_demo.h"
#include "controls/label_demo.h"
#include "controls/listbox_demo.h"
#include "controls/listview_demo.h"
#include "controls/progress_demo.h"
#include "controls/slider_demo.h"
#include "controls/tabs_demo.h"
#include "controls/textbox_demo.h"

#include "dialogs/about_demo.h"
#include "dialogs/dialog_demo.h"
#include "dialogs/file_demo.h"
#include "dialogs/folder_demo.h"
#include "dialogs/message_demo.h"
#include "dialogs/property_demo.h"
#include "dialogs/splash_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

int main()
{
    auto mainDlg = std::make_shared<Dialog>();
    mainDlg->title("DLGCPP Demo Application");
    mainDlg->image(ImageSource{ "#100", true, false });
    mainDlg->color(Color::White);
    mainDlg->resize({ 500,300 });
    mainDlg->center();

    auto label = std::make_shared<Label>("Select a demo from the menu", Position(0, 0, mainDlg->p().width(), mainDlg->p().height()));
    label->colors(Color::Gray, Color::White);
    label->horizontalAlignment(HorizontalAlign::Center);
    label->verticalAlignment(VerticalAlign::Center);
    mainDlg->add(label);
    mainDlg->SizeEvent() += [label](ISharedDialog dlg)
        {
            label->p(Position(0, 0, dlg->p().width(), dlg->p().height()));
            label->redraw();
        };

    auto menu = std::make_shared<Menu>();
    std::shared_ptr<IMenuItem> item;

    auto controls = std::make_shared<MenuItem>("&Controls");
    menu->add(controls);

    item = std::make_shared<MenuItem>("Button control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_button_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("CheckBox control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_checkbox_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("ComboBox control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_combobox_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("Custom control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_custom_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("Image control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_image_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("Label control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_label_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("ListBox control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_listbox_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("ListView control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_listview_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("Progress control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_progress_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("Slider control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_slider_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("Tab control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_tabs_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("TextBox control");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { controls_textbox_demo(mainDlg); };
    controls->add(item);

    item = std::make_shared<MenuItem>("");
    controls->add(item);

    item = std::make_shared<MenuItem>("E&xit");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { mainDlg->close(); };
    controls->add(item);

    auto dialogs = std::make_shared<MenuItem>("&Dialogs");
    menu->add(dialogs);

    item = std::make_shared<MenuItem>("Popup Dialog (Modal)");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_modal_demo(mainDlg); };
    dialogs->add(item);

    item = std::make_shared<MenuItem>("Popup Dialog (Modeless)");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_modeless_demo(mainDlg); };
    dialogs->add(item);

    item = std::make_shared<MenuItem>("Tool Dialog (Modeless)");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_tool_demo(mainDlg); };
    dialogs->add(item);

    item = std::make_shared<MenuItem>("Child Dialog (Embedded)");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_child_demo(mainDlg); };
    dialogs->add(item);

    item = std::make_shared<MenuItem>();
    dialogs->add(item);

    item = std::make_shared<MenuItem>("About Dialog");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_about_demo(mainDlg); };
    dialogs->add(item);
    item = std::make_shared<MenuItem>("File Dialog");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_file_demo(mainDlg); };
    dialogs->add(item);
    item = std::make_shared<MenuItem>("Folder Dialog");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_folder_demo(mainDlg); };
    dialogs->add(item);
    item = std::make_shared<MenuItem>("Message Dialog");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_message_demo(mainDlg); };
    dialogs->add(item);
    item = std::make_shared<MenuItem>("Property Dialog");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_property_demo(mainDlg); };
    dialogs->add(item);
    item = std::make_shared<MenuItem>("Splash Dialog");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { dialogs_splash_demo(mainDlg); };
    dialogs->add(item);

    auto advanced = std::make_shared<MenuItem>("&Advanced");
    menu->add(advanced);

    item = std::make_shared<MenuItem>("Popup Menu");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { advanced_popup_menu_demo(mainDlg); };
    advanced->add(item);

    item = std::make_shared<MenuItem>("Digital Clock Demo");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { advanced_clock_demo(mainDlg); };
    advanced->add(item);

    item = std::make_shared<MenuItem>("Note Keeper Demo");
    item->ClickEvent() += [mainDlg](ISharedMenuItem) { advanced_note_keeper_demo(mainDlg); };
    advanced->add(item);

    mainDlg->menu(menu);
    mainDlg->exec();

    return 0;
}
