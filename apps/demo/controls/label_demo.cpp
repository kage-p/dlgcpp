#include "label_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_label_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Label control demo");
    dlg->resize({200,80});
    dlg->center();

    auto label1 = std::make_shared<Label>("Hello DLGCPP!", Position{10, 10, 0, 0});
    label1->colors(Color::Blue, Color::Default);
    label1->autoSize(true);
    label1->font(Font{"sans serif", 16});
    dlg->add(label1);

    auto label2 = std::make_shared<Label>("Hello DLGCPP!", Position{10, 30, 100, 30});
    label2->colors(Color::Yellow, Color::Black);
    label2->font(Font{"sans serif", 16});
    dlg->add(label2);

    dlg->exec();
}
