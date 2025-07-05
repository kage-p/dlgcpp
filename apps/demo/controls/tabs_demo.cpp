#include "tabs_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_tabs_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("Tab control demo");
    dlg->resize({ 300,200 });
    dlg->center();

    auto tabs = std::make_shared<Tabs>(Position{ 10, 10, 280, 180 });

    auto tab1 = std::make_shared<TabItem>("Red");
    auto tab2 = std::make_shared<TabItem>("Green");
    auto tab3 = std::make_shared<TabItem>("Blue");
    tabs->items({ tab1, tab2, tab3 });
    tabs->currentIndex(0);
    dlg->add(tabs);

    auto subdlg = std::make_shared<Dialog>(DialogType::Frameless);
    subdlg->color(Color::Red);
    subdlg->visible(true);
    dlg->add(subdlg);

    dlg->SizeEvent() += [subdlg, tabs](ISharedDialog dlg)
        {
            auto area = tabs->area();
            subdlg->p(area);
            subdlg->bringToFront();
        };

    tabs->SelChangedEvent() += [tabs, subdlg](auto)
        {
            switch (tabs->currentIndex())
            {
            case 0:
                subdlg->color(Color::Red);
                break;
            case 1:
                subdlg->color(Color::Green);
                break;
            case 2:
                subdlg->color(Color::Blue);
                break;
            }
        };

    dlg->exec();
}
