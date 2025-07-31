#include "dialog_tests.h"
#include "dlgcpp/controls/label.h"
#include "mocks/mock_dialog.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;
using namespace dlgcpp::controls;
using namespace dlgcpp::menus;
using namespace dlgcpp::tests;

TEST_F(DialogTests, constructor_default)
{
    auto target = std::make_shared<Dialog>();
    EXPECT_EQ(target->type(), DialogType::Application);
    EXPECT_EQ(target->p(), Position(0, 0, 600, 400));
}

TEST_F(DialogTests, test_constructor_with_params)
{
    auto parent = std::make_shared<MockDialog>();

    EXPECT_CALL(*parent, p()).WillRepeatedly(testing::ReturnRefOfCopy(Position(10, 20, 30, 40)));

    auto target = std::make_shared<Dialog>(DialogType::Popup, parent);
    EXPECT_EQ(target->type(), DialogType::Popup);
    EXPECT_EQ(target->parent(), parent);
    EXPECT_EQ(target->p(), Position(10, 20, 600, 400));
}

TEST_F(DialogTests, show)
{
    int resultCode = 12345;
    auto target = std::make_shared<Dialog>();

    target->timer(500);
    target->TimerEvent() += [resultCode](ISharedDialog dlg) { dlg->close(resultCode); };

    auto result = target->exec();
    EXPECT_EQ(result, resultCode);
}

TEST_F(DialogTests, parent)
{
    auto target = std::make_shared<Dialog>();
    EXPECT_EQ(target->parent(), nullptr);
}

TEST_F(DialogTests, enabled)
{
    auto target = std::make_shared<Dialog>();
    target->enabled(true);
    EXPECT_TRUE(target->enabled());
    target->enabled(false);
    EXPECT_FALSE(target->enabled());
}

TEST_F(DialogTests, visible)
{
    auto target = std::make_shared<Dialog>();
    target->visible(true);
    EXPECT_TRUE(target->visible());
    target->visible(false);
    EXPECT_FALSE(target->visible());
}

TEST_F(DialogTests, move)
{
    auto target = std::make_shared<Dialog>();
    Point p(10, 20);
    target->move(p);
    EXPECT_EQ(target->p().x(), 10);
    EXPECT_EQ(target->p().y(), 20);
}

TEST_F(DialogTests, resize)
{
    auto target = std::make_shared<Dialog>();
    Size s(100, 200);
    target->resize(s);
    EXPECT_EQ(target->p().width(), 100);
    EXPECT_EQ(target->p().height(), 200);
}

TEST_F(DialogTests, showHelp)
{
    auto target = std::make_shared<Dialog>();
    target->showHelp(true);
    EXPECT_TRUE(target->showHelp());
    target->showHelp(false);
    EXPECT_FALSE(target->showHelp());
}

TEST_F(DialogTests, color)
{
    auto target = std::make_shared<Dialog>();
    Color c = Color::Red;
    target->color(c);
    EXPECT_EQ(target->color(), c);
}

TEST_F(DialogTests, cursor)
{
    auto target = std::make_shared<Dialog>();
    Cursor c = Cursor::Busy;
    target->cursor(c);
    EXPECT_EQ(target->cursor(), c);
}

TEST_F(DialogTests, title)
{
    auto target = std::make_shared<Dialog>();

    EXPECT_EQ(target->title(), std::string());

    std::string title = "Test Dialog";
    target->title(title);
    EXPECT_EQ(target->title(), title);
}

TEST_F(DialogTests, image)
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

TEST_F(DialogTests, menu)
{
    auto target = std::make_shared<Dialog>();
    auto menu = std::make_shared<Menu>();
    target->menu(menu);
    EXPECT_EQ(target->menu(), menu);
}

TEST_F(DialogTests, handle)
{
    auto target = std::make_shared<Dialog>();
    EXPECT_EQ(target->handle(), nullptr);
}

TEST_F(DialogTests, user)
{
    auto target = std::make_shared<Dialog>();
    void* userData = reinterpret_cast<void*>(0x1234);
    target->user(userData);
    EXPECT_EQ(target->user(), userData);
}

TEST_F(DialogTests, addRemoveChildControl)
{
    auto target = std::make_shared<Dialog>();
    auto child = std::make_shared<Label>();

    target->add(child);
    auto controls = target->controls();

    EXPECT_EQ(controls.size(), 1);
    target->remove(child);

    controls = target->controls();
    EXPECT_TRUE(controls.empty());
}

TEST_F(DialogTests, addRemoveChildDialog)
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

TEST_F(DialogTests, events)
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

