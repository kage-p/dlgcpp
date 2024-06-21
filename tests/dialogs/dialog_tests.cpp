#include "dialog_tests.h"
#include "dlgcpp/dlgcpp.h"
#include "../mocks/mocks.h"

using namespace dlgcpp;
using namespace dlgcpp::tests;

TEST(DialogTests, constructor_default)
{
    auto target = std::make_shared<Dialog>();
    EXPECT_EQ(target->type(), DialogType::Application);
    EXPECT_EQ(target->p(), Position(0, 0, 600, 400));
}

TEST(DialogTests, test_constructor_with_params)
{
    auto mockParent = std::make_shared<MockDialog>();

    ON_CALL(*mockParent, p()).WillByDefault(testing::ReturnRefOfCopy(Position(10,20,30,40)));

    auto target = std::make_shared<Dialog>(DialogType::Popup, mockParent);
    EXPECT_EQ(target->type(), DialogType::Popup);
    EXPECT_EQ(target->parent(), mockParent);
    EXPECT_EQ(target->p(), Position(10, 20, 600, 400));
}

TEST(DialogTests, exec)
{
    auto target = std::make_shared<Dialog>();

    // TODO: test exec() when we have a means to exit the event loop
    //EXPECT_EQ(target->exec(), 0);
}

TEST(DialogTests, parent)
{
    auto target = std::make_shared<Dialog>();
    auto mockParent = std::make_shared<MockDialog>();
    target->parent(mockParent);
    EXPECT_EQ(target->parent(), mockParent);
}

TEST(DialogTests, id)
{
    auto target = std::make_shared<Dialog>();
    target->id(42);
    EXPECT_EQ(target->id(), 42);
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
    ImageSource img{"path/to/image", false, false};

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

    auto mockMenu = std::make_shared<MockMenu>();
    auto mockChildMenu = std::make_shared<MockChildMenu>();
    ON_CALL(*mockChildMenu, menu()).WillByDefault(testing::Return(mockMenu));
    target->menu(mockChildMenu);

    EXPECT_EQ(target->menu(), mockMenu);
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
    // TODO: need to remove the ctl_state member
    // auto target = std::make_shared<Dialog>();
    // auto mockChildControl = std::make_shared<MockChildControl>();
    // target->add(mockChildControl);
    // auto controls = target->controls();
    // EXPECT_EQ(controls.size(), 1);
    // target->remove(mockChildControl);
    // controls = target->controls();
    // EXPECT_TRUE(controls.empty());
}

TEST(DialogTests, addRemoveChildDialog)
{
    auto target = std::make_shared<Dialog>();
    auto mockChildDialog = std::make_shared<MockChildDialog>();
    target->add(mockChildDialog);
    auto dialogs = target->dialogs();
    EXPECT_EQ(dialogs.size(), 1);
    target->remove(mockChildDialog);
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

