#include "dlgcpp/controls/textbox.h"
#include "textbox_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;

TEST_F(TextBoxTests, constructor_default)
{
    auto target = std::make_shared<TextBox>();

    EXPECT_EQ(target->text(), std::string());
    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(TextBoxTests, constructor_with_params)
{
    auto target = std::make_shared<TextBox>("text", Position{ 1,2,3,4 });

    EXPECT_EQ(target->text(), std::string("text"));
    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(TextBoxTests, maxChars)
{
    auto target = std::make_shared<TextBox>();

    EXPECT_EQ(target->maxChars(), 0);

    target->maxChars(10);
    EXPECT_EQ(target->maxChars(), 10);

    target->maxChars(0);
    EXPECT_EQ(target->maxChars(), 0);
}

TEST_F(TextBoxTests, password)
{
    auto target = std::make_shared<TextBox>();

    EXPECT_EQ(target->password(), false);

    target->password(true);
    EXPECT_EQ(target->password(), true);

    target->password(false);
    EXPECT_EQ(target->password(), false);
}

TEST_F(TextBoxTests, readOnly)
{
    auto target = std::make_shared<TextBox>();

    EXPECT_EQ(target->readOnly(), false);

    target->readOnly(true);
    EXPECT_EQ(target->readOnly(), true);

    target->readOnly(false);
    EXPECT_EQ(target->readOnly(), false);
}

TEST_F(TextBoxTests, multiline)
{
    auto target = std::make_shared<TextBox>();

    EXPECT_EQ(target->multiline(), false);

    target->multiline(true);
    EXPECT_EQ(target->multiline(), true);

    target->multiline(false);
    EXPECT_EQ(target->multiline(), false);
}

TEST_F(TextBoxTests, wrapText)
{
    auto target = std::make_shared<TextBox>();

    EXPECT_EQ(target->wrapText(), false);

    target->wrapText(true);
    EXPECT_EQ(target->wrapText(), true);

    target->wrapText(false);
    EXPECT_EQ(target->wrapText(), false);
}

TEST_F(TextBoxTests, horizontalAlignment)
{
    auto target = std::make_shared<TextBox>();

    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Left);

    target->horizontalAlignment(HorizontalAlign::Right);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Right);

    target->horizontalAlignment(HorizontalAlign::Center);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Center);

    target->horizontalAlignment(HorizontalAlign::Left);
    EXPECT_EQ(target->horizontalAlignment(), HorizontalAlign::Left);
}

TEST_F(TextBoxTests, text)
{
    auto target = std::make_shared<TextBox>();

    EXPECT_EQ(target->text(), std::string());

    std::string testText = "test text";
    target->text(testText);

    EXPECT_EQ(target->text(), testText);
}

TEST_F(TextBoxTests, events)
{
    auto target = std::make_shared<TextBox>();

    auto& changedEvent = target->ChangedEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
