#include "dlgcpp/controls/option_btn.h"
#include "option_btn_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(OptionButtonTests, constructor_default)
{
    auto target = std::make_shared<OptionButton>();

    EXPECT_EQ(target->text(), std::string());
    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(OptionButtonTests, constructor_with_params)
{
    auto target = std::make_shared<OptionButton>("text", Position{ 1,2,3,4 });

    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(OptionButtonTests, checked)
{
    auto target = std::make_shared<OptionButton>();

    EXPECT_EQ(target->checked(), false);

    target->checked(true);
    EXPECT_EQ(target->checked(), true);

    target->checked(false);
    EXPECT_EQ(target->checked(), false);
}

TEST_F(OptionButtonTests, horizontalAlignment)
{
    auto target = std::make_shared<OptionButton>();

    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Left);

    target->horizontalAlignment(HorizontalAlign::Right);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Right);

    target->horizontalAlignment(HorizontalAlign::Center);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Center);

    target->horizontalAlignment(HorizontalAlign::Left);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Left);
}

TEST_F(OptionButtonTests, verticalAlignment)
{
    auto target = std::make_shared<OptionButton>();

    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Center);

    target->verticalAlignment(VerticalAlign::Bottom);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Bottom);

    target->verticalAlignment(VerticalAlign::Center);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Center);

    target->verticalAlignment(VerticalAlign::Top);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Top);
}

TEST_F(OptionButtonTests, text)
{
    auto target = std::make_shared<OptionButton>();

    EXPECT_EQ(target->text(), std::string());

    std::string testText = "test text";
    target->text(testText);

    EXPECT_EQ(target->text(), testText);
}