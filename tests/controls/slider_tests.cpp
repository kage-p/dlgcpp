#include "dlgcpp/controls/slider.h"
#include "slider_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(SliderTests, constructor_default)
{
    auto target = std::make_shared<Slider>();

    EXPECT_EQ(target->text(), std::string());
    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(SliderTests, constructor_with_params)
{
    auto target = std::make_shared<Slider>("text", Position{ 1,2,3,4 });

    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(SliderTests, vertical)
{
    auto target = std::make_shared<Slider>();

    EXPECT_EQ(target->vertical(), false);

    target->vertical(true);
    EXPECT_EQ(target->vertical(), true);

    target->vertical(false);
    EXPECT_EQ(target->vertical(), false);
}

TEST_F(SliderTests, range)
{
    auto target = std::make_shared<Slider>();

    auto rangeVal = std::pair<int, int>(0, 100);
    EXPECT_EQ(target->range(), rangeVal);

    rangeVal = std::pair<int, int>(1, 10);
    target->range(rangeVal.first, rangeVal.second);
    EXPECT_EQ(target->range(), rangeVal);

    rangeVal = std::pair<int, int>(0, 0);
    target->range(rangeVal.first, rangeVal.second);
    EXPECT_EQ(target->range(), rangeVal);
}

TEST_F(SliderTests, value)
{
    auto target = std::make_shared<Slider>();

    EXPECT_EQ(target->value(), 0);

    // expect it to fail if out of range
    target->value(200);
    EXPECT_EQ(target->value(), 0);

    auto rangeVal = std::pair<int, int>(1, 200);
    target->range(rangeVal.first, rangeVal.second);

    target->value(200);
    EXPECT_EQ(target->value(), 200);
}

TEST_F(SliderTests, barColor)
{
    auto target = std::make_shared<Slider>();

    auto colorVal = Color::Default;
    EXPECT_EQ(target->barColor(), colorVal);

    colorVal = Color::Red;
    target->barColor(colorVal);
    EXPECT_EQ(target->barColor(), colorVal);

    colorVal = Color::Default;
    target->barColor(colorVal);
    EXPECT_EQ(target->barColor(), colorVal);
}

TEST_F(SliderTests, events)
{
    auto target = std::make_shared<Slider>();

    auto& changedEvent = target->ChangedEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}

TEST_F(SliderTests, text)
{
    auto target = std::make_shared<Slider>();

    EXPECT_EQ(target->text(), std::string());

    std::string testText = "test text";
    target->text(testText);

    EXPECT_EQ(target->text(), testText);
}