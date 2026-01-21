#include "dlgcpp/controls/progress.h"
#include "progress_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(ProgressTests, constructor_default)
{
    auto target = std::make_shared<Progress>();

    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(ProgressTests, constructor_with_params)
{
    auto target = std::make_shared<Progress>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(ProgressTests, vertical)
{
    auto target = std::make_shared<Progress>();

    EXPECT_EQ(target->vertical(), false);

    target->vertical(true);
    EXPECT_EQ(target->vertical(), true);

    target->vertical(false);
    EXPECT_EQ(target->vertical(), false);
}

TEST_F(ProgressTests, range)
{
    auto target = std::make_shared<Progress>();

    auto rangeVal = std::pair<int, int>(0, 100);
    EXPECT_EQ(target->range(), rangeVal);

    rangeVal = std::pair<int, int>(1, 10);
    target->range(rangeVal.first, rangeVal.second);
    EXPECT_EQ(target->range(), rangeVal);

    rangeVal = std::pair<int, int>(0, 0);
    target->range(rangeVal.first, rangeVal.second);
    EXPECT_EQ(target->range(), rangeVal);
}

TEST_F(ProgressTests, value)
{
    auto target = std::make_shared<Progress>();

    EXPECT_EQ(target->value(), 0);

    // expect it to fail if out of range
    target->value(200);
    EXPECT_EQ(target->value(), 0);

    auto rangeVal = std::pair<int, int>(1, 200);
    target->range(rangeVal.first, rangeVal.second);

    target->value(200);
    EXPECT_EQ(target->value(), 200);
}

TEST_F(ProgressTests, events)
{
    auto target = std::make_shared<Progress>();

    auto& clickEvent = target->ClickEvent();
    auto& dblClickEvent = target->DoubleClickEvent();
    auto& rightClickEvent = target->RightClickEvent();
    auto& dblRightClickEvent = target->DoubleRightClickEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
