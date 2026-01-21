#include "dlgcpp/controls/tab_item.h"
#include "tab_item_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(TabItemTests, constructor_default)
{
    auto target = std::make_shared<TabItem>();

    EXPECT_EQ(target->text(), std::string());
    EXPECT_EQ(target->toolTipText(), std::string());
    EXPECT_EQ(target->image(), ImageSource{});
}

TEST_F(TabItemTests, constructor_with_params)
{
    auto image = ImageSource{ "img_id" };
    auto target = std::make_shared<TabItem>("text", image, "tipText");

    EXPECT_EQ(target->image(), image);
    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->toolTipText(), std::string("tipText"));
}

TEST_F(TabItemTests, highlight)
{
    auto target = std::make_shared<TabItem>();

    EXPECT_EQ(target->highlight(), false);

    target->highlight() = true;
    EXPECT_EQ(target->highlight(), true);

    target->highlight() = false;
    EXPECT_EQ(target->highlight(), false);
}

TEST_F(TabItemTests, text)
{
    auto target = std::make_shared<TabItem>();

    EXPECT_EQ(target->text(), std::string());

    std::string testText = "test text";
    target->text() = testText;

    EXPECT_EQ(target->text(), testText);
}

TEST_F(TabItemTests, toolTipText)
{
    auto target = std::make_shared<TabItem>();

    EXPECT_EQ(target->toolTipText(), std::string());

    std::string testText = "test text";
    target->toolTipText() = testText;

    EXPECT_EQ(target->toolTipText(), testText);
}

TEST_F(TabItemTests, image)
{
    auto target = std::make_shared<TabItem>();

    EXPECT_EQ(target->image(), ImageSource());

    ImageSource img{ "path/to/image", false, false };
    target->image() = img;

    EXPECT_EQ(target->image(), img);
}

TEST_F(TabItemTests, events)
{
    auto target = std::make_shared<TabItem>();

    auto& clickEvent = target->ClickEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
