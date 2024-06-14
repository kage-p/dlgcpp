#include "clock_demo.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

using namespace dlgcpp;
using namespace dlgcpp::controls;

void advanced_clock_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Tool, parent);
    dlg->title("Digital Clock Demo");
    dlg->resize({250, 120});
    dlg->center();

    auto label = std::make_shared<Label>();
    label->colors(Color::Orange, Color::Black);
    label->horizontalAlignment(HorizontalAlign::Center);
    label->verticalAlignment(VerticalAlign::Center);
    label->font(Font{"courier", 48, true});
    dlg->add(label);

    dlg->SizeEvent() += [label](ISharedDialog dlg)
    {
        label->resize(dlg->p());
    };
    dlg->SizeEvent().invoke(dlg);

    dlg->timer(1000);
    dlg->TimerEvent() += [label]()
    {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(localTime, "%H:%M:%S");
        label->text(oss.str());
    };
    dlg->TimerEvent().invoke(dlg);

    dlg->exec();
}
