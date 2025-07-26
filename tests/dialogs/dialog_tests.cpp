#include "../mocks/mocks.h"
#include "dialog_tests.h"
#include "dlgcpp/dlgcpp.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;
using namespace dlgcpp::controls;
using namespace dlgcpp::menus;
using namespace dlgcpp::tests;

TEST(DialogTests, constructor_default)
{
    auto target = std::make_shared<Dialog>();
    EXPECT_EQ(target->type(), DialogType::Application);
    EXPECT_EQ(target->p(), Position(0, 0, 600, 400));
}

TEST(DialogTests, test_constructor_with_params)
{
    auto parent = std::make_shared<MockDialog>();

    ON_CALL(*parent, p()).WillByDefault(testing::ReturnRefOfCopy(Position(10, 20, 30, 40)));

    auto target = std::make_shared<Dialog>(DialogType::Popup, parent);
    EXPECT_EQ(target->type(), DialogType::Popup);
    EXPECT_EQ(target->parent(), parent);
    EXPECT_EQ(target->p(), Position(10, 20, 600, 400));
}

TEST(DialogTests, show)
{
    int resultCode = 12345;
    auto target = std::make_shared<Dialog>();

    target->timer(500);
    target->TimerEvent() += [resultCode](ISharedDialog dlg) { dlg->close(resultCode); };

    auto result = target->exec();
    EXPECT_EQ(result, resultCode);
}

TEST(DialogTests, parent)
{
    auto target = std::make_shared<Dialog>();
    EXPECT_EQ(target->parent(), nullptr);
}

TEST(DialogTests, enabled)
{
    auto target = std::make_shared<Dialog>();
    target->enabled(true);
    EXPECT_TRUE(target->enabled());
    target->enabled(false);
    EXPECT_FALSE(target->enabled());
}

TEST(DialogTests, visible)
{
    auto target = std::make_shared<Dialog>();
    target->visible(true);
    EXPECT_TRUE(target->visible());
    target->visible(false);
    EXPECT_FALSE(target->visible());
}

TEST(DialogTests, move)
{
    auto target = std::make_shared<Dialog>();
    Point p(10, 20);
    target->move(p);
    EXPECT_EQ(target->p().x(), 10);
    EXPECT_EQ(target->p().y(), 20);
}

TEST(DialogTests, resize)
{
    auto target = std::make_shared<Dialog>();
    Size s(100, 200);
    target->resize(s);
    EXPECT_EQ(target->p().width(), 100);
    EXPECT_EQ(target->p().height(), 200);
}

TEST(DialogTests, showHelp)
{
    auto target = std::make_shared<Dialog>();
    target->showHelp(true);
    EXPECT_TRUE(target->showHelp());
    target->showHelp(false);
    EXPECT_FALSE(target->showHelp());
}

TEST(DialogTests, color)
{
    auto target = std::make_shared<Dialog>();
    Color c = Color::Red;
    target->color(c);
    EXPECT_EQ(target->color(), c);
}

TEST(DialogTests, cursor)
{
    auto target = std::make_shared<Dialog>();
    Cursor c = Cursor::Busy;
    target->cursor(c);
    EXPECT_EQ(target->cursor(), c);
}

TEST(DialogTests, title)
{
    auto target = std::make_shared<Dialog>();

    EXPECT_EQ(target->title(), std::string());

    std::string title = "Test Dialog";
    target->title(title);
    EXPECT_EQ(target->title(), title);
}

TEST(DialogTests, image)
{
    auto target = std::make_shared<Dialog>();
    ImageSource img{ "path/to/image", false, false };

    // must be an icon
    target->image(img);
    EXPECT_EQ(target->image(), ImageSource());

    img.isIcon = true;
    target->image(img);
    EXPECT_EQ(target->image(), img);
}

TEST(DialogTests, menu)
{
    auto target = std::make_shared<Dialog>();
    auto menu = std::make_shared<Menu>();
    target->menu(menu);
    EXPECT_EQ(target->menu(), menu);
}

TEST(DialogTests, handle)
{
    auto target = std::make_shared<Dialog>();
    EXPECT_EQ(target->handle(), nullptr);
}

TEST(DialogTests, user)
{
    auto target = std::make_shared<Dialog>();
    void* userData = reinterpret_cast<void*>(0x1234);
    target->user(userData);
    EXPECT_EQ(target->user(), userData);
}

TEST(DialogTests, addRemoveChildControl)
{
    auto target = std::make_shared<Dialog>();
    auto child = std::make_shared<Button>();

    target->add(child);
    auto controls = target->controls();

    EXPECT_EQ(controls.size(), 1);
    target->remove(child);

    controls = target->controls();
    EXPECT_TRUE(controls.empty());
}

TEST(DialogTests, addRemoveChildDialog)
{
    auto target = std::make_shared<Dialog>();
    auto child = std::make_shared<Dialog>(DialogType::Frameless);

    target->add(child);
    auto dialogs = target->dialogs();
    EXPECT_EQ(dialogs.size(), 1);

    target->remove(child);
    dialogs = target->dialogs();
    EXPECT_TRUE(dialogs.empty());
}

TEST(DialogTests, events)
{
    auto target = std::make_shared<Dialog>();
    auto& dropEvent = target->DropEvent();
    auto& mouseDownEvent = target->MouseDownEvent();
    auto& mouseUpEvent = target->MouseUpEvent();
    auto& mouseMoveEvent = target->MouseMoveEvent();
    auto& mouseDoubleClickEvent = target->MouseDoubleClickEvent();
    auto& mouseCaptureLostEvent = target->MouseCaptureLostEvent();
    auto& helpEvent = target->HelpEvent();
    auto& moveEvent = target->MoveEvent();
    auto& sizeEvent = target->SizeEvent();
    auto& timerEvent = target->TimerEvent();

    // Additional checks can be added to verify the event handling logic
}

