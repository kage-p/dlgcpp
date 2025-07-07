#include "trackbar_demo.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

void controls_trackbar_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("TrackBar control demo");
    dlg->color(Color::White);
    dlg->resize({ 200,170 });
    dlg->center();

    auto trackbar1 = std::make_shared<TrackBar>(Position{ 10, 10, 180, 20 });
    trackbar1->range(0, 99);
    trackbar1->pageSize(10);
    trackbar1->tickMarks(TrackBarTickMark::Below);
    trackbar1->colors(Color::Default, Color::White);
    dlg->add(trackbar1);

    auto trackbar2 = std::make_shared<TrackBar>(Position{ 10, 35, 180, 20 });
    trackbar2->range(0, 99);
    trackbar2->tickMarks(TrackBarTickMark::Above);
    trackbar2->colors(Color::Default, Color::White);
    dlg->add(trackbar2);

    trackbar1->ChangedEvent() +=
        [trackbar1, trackbar2](auto)
        {
            trackbar2->value(trackbar1->value());
        };

    auto trackbar3 = std::make_shared<TrackBar>(Position{ 60, 60, 25, 100 });
    trackbar3->range(0, 99);
    trackbar3->vertical(true);
    trackbar3->pageSize(10);
    trackbar3->tickMarks(TrackBarTickMark::Both);
    trackbar3->colors(Color::Default, Color::White);
    dlg->add(trackbar3);

    auto trackbar4 = std::make_shared<TrackBar>(Position{ 120, 60, 25, 100 });
    trackbar4->range(0, 99);
    trackbar4->vertical(true);
    trackbar4->pageSize(10);
    trackbar4->tickMarks(TrackBarTickMark::Both);
    trackbar4->colors(Color::Default, Color::White);
    dlg->add(trackbar4);

    trackbar3->ChangedEvent() +=
        [trackbar3, trackbar4](auto)
        {
            trackbar4->value(trackbar3->value());
        };

    dlg->exec();
}
