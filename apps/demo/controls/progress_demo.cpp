#include "progress_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_progress_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Progress control demo");
    dlg->resize({200,80});
    dlg->center();

    auto progress1 = std::make_shared<Progress>(Position{10, 10, 180, 15});
    //progress1->colors();
    progress1->range(0,99);
    dlg->add(progress1);

    auto progress2 = std::make_shared<Progress>(Position{10, 30, 180, 15});
    //progress2->colors();
    progress2->range(0,99);
    dlg->add(progress2);

    int prog = 0;
    dlg->TimerEvent() += [progress1, progress2, &prog](ISharedDialog dlg)
    {
        prog += 5;
        if (prog >= 100)
            prog = 0;

        progress1->value(prog);
        progress2->value(100 - (prog+1));
    };
    dlg->timer(40);

    dlg->exec();
}
