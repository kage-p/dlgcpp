#include "checkbox_tests.h"
#include "dlgcpp/controls/checkbox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(CheckBoxTests, constructor_default)
{
    auto target = std::make_shared<CheckBox>();

    EXPECT_EQ(target->text(), std::string());
    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(CheckBoxTests, constructor_with_params)
{
    auto target = std::make_shared<CheckBox>("text", Position{ 1,2,3,4 });

    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(CheckBoxTests, autoCheck)
{
    auto target = std::make_shared<CheckBox>();

    EXPECT_EQ(target->autoCheck(), true);

    target->autoCheck(false);
    EXPECT_EQ(target->autoCheck(), false);

    target->autoCheck(true);
    EXPECT_EQ(target->autoCheck(), true);
}

TEST_F(CheckBoxTests, checked)
{
    auto target = std::make_shared<CheckBox>();

    EXPECT_EQ(target->checked(), false);

    target->checked(true);
    EXPECT_EQ(target->checked(), true);

    target->checked(false);
    EXPECT_EQ(target->checked(), false);
}

TEST_F(CheckBoxTests, horizontalAlignment)
{
    auto target = std::make_shared<CheckBox>();

    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Left);

    target->horizontalAlignment(HorizontalAlign::Right);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Right);

    target->horizontalAlignment(HorizontalAlign::Center);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Center);

    target->horizontalAlignment(HorizontalAlign::Left);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Left);
}

TEST_F(CheckBoxTests, verticalAlignment)
{
    auto target = std::make_shared<CheckBox>();

    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Center);

    target->verticalAlignment(VerticalAlign::Bottom);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Bottom);

    target->verticalAlignment(VerticalAlign::Center);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Center);

    target->verticalAlignment(VerticalAlign::Top);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Top);
}

TEST_F(CheckBoxTests, text)
{
    auto target = std::make_shared<CheckBox>();

    EXPECT_EQ(target->text(), std::string());

    std::string testText = "test text";
    target->text(testText);

    EXPECT_EQ(target->text(), testText);
}