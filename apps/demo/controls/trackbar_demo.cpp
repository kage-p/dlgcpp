#include "trackbar_demo.h"

using namespace dlgcpp;

void controls_trackbar_demo(ISharedDialog parent)
{
    auto dlg = std::make_shared<Dialog>(DialogType::Popup, parent);
    dlg->title("TrackBar control demo");
    dlg->color(Color::White);
    dlg->resize({ 200,170 });
    dlg->center();

    auto trackbar1 = std::make_shared<TrackBar>(Position{ 10, 10, 180, 25 });
    trackbar1->range(0, 99);
    trackbar1->pageSize(10);
    trackbar1->tickMarks(TrackBarTickMark::Below);
    trackbar1->colors(Color::LtGray, Color::White);
    trackbar1->barColor(Color::Blue);
    trackbar1->thumbColor(Color::DarkBlue);
    dlg->add(trackbar1);

    auto trackbar2 = std::make_shared<TrackBar>(Position{ 10, 40, 180, 25 });
    trackbar2->range(0, 99);
    trackbar2->tickMarks(TrackBarTickMark::Above);
    trackbar2->colors(Color::LtGray, Color::White);
    trackbar2->barColor(Color::Blue);
    trackbar2->thumbColor(Color::DarkBlue);
    dlg->add(trackbar2);

    trackbar1->value().event() +=
        [trackbar1, trackbar2](auto)
        {
            trackbar2->value(trackbar1->value());
        };

    auto trackbar3 = std::make_shared<TrackBar>(Position{ 60, 70, 25, 90 });
    trackbar3->range(0, 99);
    trackbar3->vertical(true);
    trackbar3->pageSize(10);
    trackbar3->tickMarks(TrackBarTickMark::Both);
    trackbar3->colors(Color::LtGray, Color::White);
    trackbar3->barColor(Color::Green);
    trackbar3->thumbColor(Color::DarkGreen);
    dlg->add(trackbar3);

    auto trackbar4 = std::make_shared<TrackBar>(Position{ 120, 70, 25, 90 });
    trackbar4->range(0, 99);
    trackbar4->vertical(true);
    trackbar4->pageSize(10);
    trackbar4->tickMarks(TrackBarTickMark::Both);
    trackbar4->colors(Color::LtGray, Color::White);
    trackbar4->barColor(Color::Green);
    trackbar4->thumbColor(Color::DarkGreen);
    dlg->add(trackbar4);

    trackbar3->value().event() +=
        [trackbar3, trackbar4](auto)
        {
            trackbar4->value(trackbar3->value());
        };

    dlg->exec();
}
