#include "dlgcpp/controls/label.h"
#include "label_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(LabelTests, constructor_default)
{
    auto target = std::make_shared<Label>();

    EXPECT_EQ(target->text(), std::string());
    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(LabelTests, constructor_with_params)
{
    auto target = std::make_shared<Label>("text", Position{ 1,2,3,4 });

    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(LabelTests, autoSize)
{
    auto target = std::make_shared<Label>();

    EXPECT_EQ(target->autoSize(), false);

    target->autoSize(true);
    EXPECT_EQ(target->autoSize(), true);

    target->autoSize(false);
    EXPECT_EQ(target->autoSize(), false);
}

TEST_F(LabelTests, clickable)
{
    auto target = std::make_shared<Label>();

    EXPECT_EQ(target->clickable(), true);

    target->clickable(false);
    EXPECT_EQ(target->clickable(), false);

    target->clickable(true);
    EXPECT_EQ(target->clickable(), true);
}

TEST_F(LabelTests, elipsis)
{
    auto target = std::make_shared<Label>();

    EXPECT_EQ(target->elipsis(), false);

    target->elipsis(true);
    EXPECT_EQ(target->elipsis(), true);

    target->elipsis(false);
    EXPECT_EQ(target->elipsis(), false);
}

TEST_F(LabelTests, horizontalAlignment)
{
    auto target = std::make_shared<Label>();

    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Left);

    target->horizontalAlignment(HorizontalAlign::Right);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Right);

    target->horizontalAlignment(HorizontalAlign::Center);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Center);

    target->horizontalAlignment(HorizontalAlign::Left);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Left);
}

TEST_F(LabelTests, verticalAlignment)
{
    auto target = std::make_shared<Label>();

    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Top);

    target->verticalAlignment(VerticalAlign::Bottom);
#ifdef _WIN32
    // Bottom alignment is not supported in win32
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Top);
#else
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Bottom);
#endif

    target->verticalAlignment(VerticalAlign::Center);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Center);

    target->verticalAlignment(VerticalAlign::Top);
    EXPECT_EQ(target->verticalAlignment(), VerticalAlign::Top);
}

TEST_F(LabelTests, text)
{
    auto target = std::make_shared<Label>();

    EXPECT_EQ(target->text(), std::string());

    std::string testText = "test text";
    target->text(testText);

    EXPECT_EQ(target->text(), testText);
}