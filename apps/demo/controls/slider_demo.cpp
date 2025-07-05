#include "slider_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_slider_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Slider control demo");
    dlg->resize({ 170,200 });
    dlg->center();

    auto slider1 = std::make_shared<Slider>("Slider", Position{ 10, 10, 150, 25 });
    slider1->colors(Color::White, Color::Gray);
    slider1->barColor(Color::Green);
    slider1->font(Font{ "sans serif", 16 });
    dlg->add(slider1);

    auto slider2 = std::make_shared<Slider>("Slider", Position{ 10, 40, 25, 150 });
    slider2->colors(Color::White, Color::Gray);
    slider2->barColor(Color::Green);
    slider2->font(Font{ "sans serif", 6 });
    slider2->vertical(true);
    dlg->add(slider2);

    // link the slider position values
    slider1->ChangedEvent() += [slider1, slider2](auto) { slider2->value(slider1->value()); };
    slider2->ChangedEvent() += [slider1, slider2](auto) { slider1->value(slider2->value()); };

    dlg->exec();
}
