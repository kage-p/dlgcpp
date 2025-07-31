#include "dialog_tests.h"
#include "dlgcpp/controls/label.h"
#include "mocks/mock_dialog.h"
#include "win32/dialogs/dialog_p.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;
using namespace dlgcpp::controls;
using namespace dlgcpp::menus;
using namespace dlgcpp::tests;

void DialogImplTests::SetUp()
{
    _mockDialog = std::make_shared<testing::NiceMock<MockDialog>>();

    // create a weak reference to avoid leaked mocks
    std::weak_ptr<IDialog> weak = _mockDialog;
    ON_CALL(*_mockDialog, ptr()).WillByDefault([weak]() {
        return weak.lock(); // returns shared_ptr or nullptr
        });
}

void DialogImplTests::TearDown()
{
    _mockDialog.reset();
}

TEST_F(DialogImplTests, constructor_default)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    EXPECT_EQ(target->type(), DialogType::Application);
    EXPECT_EQ(target->p(), Position(0, 0, 600, 400));
}

TEST_F(DialogImplTests, test_constructor_with_params)
{
    auto parent = std::make_shared<MockDialog>();

    EXPECT_CALL(*parent, p()).WillRepeatedly(testing::ReturnRefOfCopy(Position(10, 20, 30, 40)));

    auto target = std::make_shared<DialogImpl>(*_mockDialog, DialogType::Popup, parent);
    EXPECT_EQ(target->type(), DialogType::Popup);
    EXPECT_EQ(target->parent(), parent);
    EXPECT_EQ(target->p(), Position(10, 20, 600, 400));
}

TEST_F(DialogImplTests, show)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    int resultCode = 12345;

    target->timer(500);
    target->TimerEvent() +=
        [resultCode, target](ISharedDialog dlg)
        {
            // break out of the exec()
            target->close(resultCode);
        };

    auto result = target->exec();
    EXPECT_EQ(result, resultCode);
}

TEST_F(DialogImplTests, parent)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    EXPECT_EQ(target->parent(), nullptr);
}

TEST_F(DialogImplTests, enabled)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    target->enabled(true);
    EXPECT_TRUE(target->enabled());
    target->enabled(false);
    EXPECT_FALSE(target->enabled());
}

TEST_F(DialogImplTests, visible)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    target->visible(true);
    EXPECT_TRUE(target->visible());
    target->visible(false);
    EXPECT_FALSE(target->visible());
}

TEST_F(DialogImplTests, move)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    Point p(10, 20);
    target->move(p);
    EXPECT_EQ(target->p().x(), 10);
    EXPECT_EQ(target->p().y(), 20);
}

TEST_F(DialogImplTests, resize)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    Size s(100, 200);
    target->resize(s);
    EXPECT_EQ(target->p().width(), 100);
    EXPECT_EQ(target->p().height(), 200);
}

TEST_F(DialogImplTests, showHelp)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    target->showHelp(true);
    EXPECT_TRUE(target->showHelp());
    target->showHelp(false);
    EXPECT_FALSE(target->showHelp());
}

TEST_F(DialogImplTests, color)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    Color c = Color::Red;
    target->color(c);
    EXPECT_EQ(target->color(), c);
}

TEST_F(DialogImplTests, cursor)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    Cursor c = Cursor::Busy;
    target->cursor(c);
    EXPECT_EQ(target->cursor(), c);
}

TEST_F(DialogImplTests, title)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);

    EXPECT_EQ(target->title(), std::string());

    std::string title = "Test Dialog";
    target->title(title);
    EXPECT_EQ(target->title(), title);
}

TEST_F(DialogImplTests, image)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    ImageSource img{ "path/to/image", false, false };

    // must be an icon
    target->image(img);
    EXPECT_EQ(target->image(), ImageSource());

    img.isIcon = true;
    target->image(img);
    EXPECT_EQ(target->image(), img);
}

TEST_F(DialogImplTests, menu)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    auto menu = std::make_shared<Menu>();
    target->menu(menu);
    EXPECT_EQ(target->menu(), menu);
}

TEST_F(DialogImplTests, handle)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    EXPECT_EQ(target->handle(), nullptr);
}

TEST_F(DialogImplTests, user)
{

    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    void* userData = reinterpret_cast<void*>(0x1234);
    target->user(userData);
    EXPECT_EQ(target->user(), userData);
}

TEST_F(DialogImplTests, addRemoveChildControl)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    auto child = std::make_shared<Label>();

    target->add(child);
    auto controls = target->controls();

    EXPECT_EQ(controls.size(), 1);
    target->remove(child);

    controls = target->controls();
    EXPECT_TRUE(controls.empty());
}

TEST_F(DialogImplTests, addRemoveChildDialog)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
    auto child = std::make_shared<Dialog>(DialogType::Frameless);

    target->add(child);
    auto dialogs = target->dialogs();
    EXPECT_EQ(dialogs.size(), 1);

    target->remove(child);
    dialogs = target->dialogs();
    EXPECT_TRUE(dialogs.empty());
}

TEST_F(DialogImplTests, events)
{
    auto target = std::make_shared<DialogImpl>(*_mockDialog);
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

