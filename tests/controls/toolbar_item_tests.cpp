#include "dlgcpp/controls/toolbar_item.h"
#include "toolbar_item_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(ToolBarItemTests, constructor_default)
{
    auto target = std::make_shared<ToolBarItem>();

    EXPECT_EQ(target->image(), ImageSource{});
    EXPECT_EQ(target->text(), std::string());
    EXPECT_EQ(target->toolTipText(), std::string());

    // separator set if text not specified
    EXPECT_EQ(target->separator(), true);
}

TEST_F(ToolBarItemTests, constructor_with_params)
{
    auto image = ImageSource{ "img_id" };
    auto target = std::make_shared<ToolBarItem>(image, "text", "tipText");

    EXPECT_EQ(target->image(), image);
    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->toolTipText(), std::string("tipText"));

    // separator not set if text specified
    EXPECT_EQ(target->separator(), false);
}

TEST_F(ToolBarItemTests, enabled)
{
    auto target = std::make_shared<ToolBarItem>();

    EXPECT_EQ(target->enabled(), true);

    target->enabled() = false;
    EXPECT_EQ(target->enabled(), false);

    target->enabled() = true;
    EXPECT_EQ(target->enabled(), true);
}

TEST_F(ToolBarItemTests, checked)
{
    auto target = std::make_shared<ToolBarItem>();

    EXPECT_EQ(target->checked(), false);

    target->checked() = true;
    EXPECT_EQ(target->checked(), true);

    target->checked() = false;
    EXPECT_EQ(target->checked(), false);

}

TEST_F(ToolBarItemTests, separator)
{
    auto target = std::make_shared<ToolBarItem>();

    EXPECT_EQ(target->separator(), true);

    target->separator() = false;
    EXPECT_EQ(target->separator(), false);

    target->separator() = true;
    EXPECT_EQ(target->separator(), true);
}

TEST_F(ToolBarItemTests, text)
{
    auto target = std::make_shared<ToolBarItem>();

    EXPECT_EQ(target->text(), std::string());

    std::string testText = "test text";
    target->text() = testText;

    EXPECT_EQ(target->text(), testText);
}

TEST_F(ToolBarItemTests, toolTipText)
{
    auto target = std::make_shared<ToolBarItem>();

    EXPECT_EQ(target->toolTipText(), std::string());

    std::string testText = "test text";
    target->toolTipText() = testText;

    EXPECT_EQ(target->toolTipText(), testText);
}

TEST_F(ToolBarItemTests, image)
{
    auto target = std::make_shared<ToolBarItem>();

    EXPECT_EQ(target->image(), ImageSource());

    ImageSource img{ "path/to/image", false, false };
    target->image() = img;

    EXPECT_EQ(target->image(), img);
}

TEST_F(ToolBarItemTests, ClickEvent)
{
    auto target = std::make_shared<ToolBarItem>();

    auto& clickEvent = target->ClickEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
