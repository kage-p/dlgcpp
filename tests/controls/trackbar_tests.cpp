#include "dlgcpp/controls/trackbar.h"
#include "trackbar_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(TrackBarTests, constructor_default)
{
    auto target = std::make_shared<TrackBar>();

    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(TrackBarTests, constructor_with_params)
{
    auto target = std::make_shared<TrackBar>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(TrackBarTests, vertical)
{
    auto target = std::make_shared<TrackBar>();

    EXPECT_EQ(target->vertical(), false);

    target->vertical(true);
    EXPECT_EQ(target->vertical(), true);

    target->vertical(false);
    EXPECT_EQ(target->vertical(), false);
}

TEST_F(TrackBarTests, range)
{
    auto target = std::make_shared<TrackBar>();

    auto rangeVal = std::pair<int, int>(0, 100);
    EXPECT_EQ(target->range(), rangeVal);

    rangeVal = std::pair<int, int>(1, 10);
    target->range(rangeVal.first, rangeVal.second);
    EXPECT_EQ(target->range(), rangeVal);

    rangeVal = std::pair<int, int>(0, 0);
    target->range(rangeVal.first, rangeVal.second);
    EXPECT_EQ(target->range(), rangeVal);
}

TEST_F(TrackBarTests, value)
{
    auto target = std::make_shared<TrackBar>();

    EXPECT_EQ(target->value(), 0);

    // expect it to fail if out of range
    target->value(200);
    EXPECT_EQ(target->value(), 0);

    auto rangeVal = std::pair<int, int>(1, 200);
    target->range(rangeVal.first, rangeVal.second);

    target->value(200);
    EXPECT_EQ(target->value(), 200);
}

TEST_F(TrackBarTests, pageSize)
{
    auto target = std::make_shared<TrackBar>();

    EXPECT_EQ(target->pageSize(), 1);

    target->pageSize(10);
    EXPECT_EQ(target->pageSize(), 10);

    target->pageSize(0);
    EXPECT_EQ(target->pageSize(), 0);
}

TEST_F(TrackBarTests, tickMarks)
{
    auto target = std::make_shared<TrackBar>();

    EXPECT_EQ(target->tickMarks(), TrackBarTickMark::Above);

    target->tickMarks(TrackBarTickMark::Below);
    EXPECT_EQ(target->tickMarks(), TrackBarTickMark::Below);

    target->tickMarks(TrackBarTickMark::Both);
    EXPECT_EQ(target->tickMarks(), TrackBarTickMark::Both);

    target->tickMarks(TrackBarTickMark::Above);
    EXPECT_EQ(target->tickMarks(), TrackBarTickMark::Above);
}

TEST_F(TrackBarTests, barColor)
{
    auto target = std::make_shared<TrackBar>();

    auto colorVal = Color::Default;
    EXPECT_EQ(target->barColor(), colorVal);

    colorVal = Color::Red;
    target->barColor(colorVal);
    EXPECT_EQ(target->barColor(), colorVal);

    colorVal = Color::Default;
    target->barColor(colorVal);
    EXPECT_EQ(target->barColor(), colorVal);
}

TEST_F(TrackBarTests, thumbColor)
{
    auto target = std::make_shared<TrackBar>();

    auto colorVal = Color::Default;
    EXPECT_EQ(target->thumbColor(), colorVal);

    colorVal = Color::Red;
    target->thumbColor(colorVal);
    EXPECT_EQ(target->thumbColor(), colorVal);

    colorVal = Color::Default;
    target->thumbColor(colorVal);
    EXPECT_EQ(target->thumbColor(), colorVal);
}

TEST_F(TrackBarTests, events)
{
    auto target = std::make_shared<TrackBar>();

    auto& clickEvent = target->ClickEvent();
    auto& dblClickEvent = target->DoubleClickEvent();
    auto& rightClickEvent = target->RightClickEvent();
    auto& dblRightClickEvent = target->DoubleRightClickEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
