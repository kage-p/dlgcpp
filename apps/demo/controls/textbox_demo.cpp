#include "textbox_demo.h"

using namespace dlgcpp;

void controls_textbox_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("TextBox control demo");
    dlg->resize({ 300,180 });
    dlg->center();

    auto single = std::make_shared<TextBox>("Single Line", Position{ 10, 10, 280, 13 });
    single->multiline(false);
    dlg->add(single);

    auto multi = std::make_shared<TextBox>("Multiple\r\nLines", Position{ 10, 25, 280, 150 });
    multi->font(Font("Courier", 10));
    multi->colors(Color::Lime, Color::Black);
    multi->multiline(true);
    dlg->add(multi);

    dlg->exec();
}
