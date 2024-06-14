#include "textbox_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_textbox_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("TextBox control demo");
    dlg->resize({200,80});
    dlg->center();

    auto single = std::make_shared<TextBox>("Single Line", Position{10, 10, 180, 13});
    single->colors(Color::Blue, Color::Default);
    single->multiline(false);
    dlg->add(single);

    auto multi = std::make_shared<TextBox>("Multiple\r\nLines", Position{10, 25, 180, 50});
    multi->colors(Color::Blue, Color::Default);
    multi->multiline(true);
    dlg->add(multi);

    dlg->exec();
}
