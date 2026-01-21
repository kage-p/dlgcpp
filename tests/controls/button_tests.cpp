#include "button_tests.h"
#include "dlgcpp/controls/button.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(ButtonTests, constructor_default)
{
    auto target = std::make_shared<Button>();

    EXPECT_EQ(target->text(), std::string());
    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(ButtonTests, constructor_with_params)
{
    auto target = std::make_shared<Button>("text", Position{ 1,2,3,4 });

    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(ButtonTests, horizontalAlignment)
{
    auto target = std::make_shared<Button>();

    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Center);

    target->horizontalAlignment(HorizontalAlign::Right);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Right);

    target->horizontalAlignment(HorizontalAlign::Center);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Center);

    target->horizontalAlignment(HorizontalAlign::Left);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Left);
}

TEST_F(ButtonTests, verticalAlignment)
{
    auto target = std::make_shared<Button>();

    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Center);

    target->verticalAlignment(VerticalAlign::Bottom);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Bottom);

    target->verticalAlignment(VerticalAlign::Center);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Center);

    target->verticalAlignment(VerticalAlign::Top);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Top);
}

TEST_F(ButtonTests, text)
{
    auto target = std::make_shared<Button>();

    EXPECT_EQ(target->text(), std::string());

    std::string testText = "test text";
    target->text(testText);

    EXPECT_EQ(target->text(), testText);
}