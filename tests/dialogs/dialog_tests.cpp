#include "dialog_tests.h"
#include "dlgcpp/controls/label.h"
#include "mocks/mock_dialog.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;
using namespace dlgcpp::controls;
using namespace dlgcpp::menus;
using namespace dlgcpp::tests;

void DialogTests::SetUp()
{
    // TODO: needs to use mock impl.
    _target = std::make_shared<Dialog>();
}

void DialogTests::TearDown()
{
    _target.reset();
}

TEST_F(DialogTests, constructor_with_params)
{
    auto parent = std::make_shared<MockDialog>();

    auto posProperty = Property<Position, ISharedDialog>(Position(111, 333, 30, 40));
    EXPECT_CALL(*parent.get(), p()).WillRepeatedly(testing::ReturnRef(posProperty));

    auto target = std::make_shared<Dialog>(DialogType::Popup, parent);
    EXPECT_EQ(target->type(), DialogType::Popup);
    EXPECT_EQ(target->parent().lock(), parent);
    EXPECT_EQ(target->p(), Position(111, 333, 600, 400));

    testing::Mock::VerifyAndClearExpectations(parent.get());
}

TEST_F(DialogTests, show)
{
    int resultCode = 12345;

    _target->timer(500);
    _target->TimerEvent() += [resultCode](ISharedDialog dlg) { dlg->close(resultCode); };

    auto result = _target->exec();
    EXPECT_EQ(result, resultCode);
}

TEST_F(DialogTests, parent)
{
    EXPECT_TRUE(_target->parent().expired());
}

TEST_F(DialogTests, enabled)
{
    EXPECT_TRUE(_target->enabled());

    _target->enabled(false);
    EXPECT_FALSE(_target->enabled());

    _target->enabled(true);
    EXPECT_TRUE(_target->enabled());
}

TEST_F(DialogTests, enabled_after_show)
{
    _target->show();

    EXPECT_TRUE(_target->enabled());

    _target->enabled(false);
    EXPECT_FALSE(_target->enabled());

    _target->enabled(true);
    EXPECT_TRUE(_target->enabled());
}

TEST_F(DialogTests, visible)
{
    EXPECT_FALSE(_target->visible());

    _target->visible(true);
    EXPECT_TRUE(_target->visible());

    _target->visible(false);
    EXPECT_FALSE(_target->visible());
}

TEST_F(DialogTests, visible_after_show)
{
    _target->show();

    EXPECT_TRUE(_target->visible());

    _target->visible(false);
    EXPECT_FALSE(_target->visible());

    _target->visible(true);
    EXPECT_TRUE(_target->visible());
}

TEST_F(DialogTests, type)
{
    EXPECT_EQ(_target->type(), DialogType::Application);

    _target->type() = DialogType::Frameless;
    EXPECT_EQ(_target->type(), DialogType::Frameless);

    _target->type() = DialogType::Popup;
    EXPECT_EQ(_target->type(), DialogType::Popup);

    _target->type() = DialogType::Tool;
    EXPECT_EQ(_target->type(), DialogType::Tool);

    _target->type() = DialogType::Application;
    EXPECT_EQ(_target->type(), DialogType::Application);
}

TEST_F(DialogTests, type_after_show)
{
    _target->show();

    _target->type() = DialogType::Frameless;
    EXPECT_FALSE(_target->handle().empty());

    _target->type() = DialogType::Popup;
    EXPECT_FALSE(_target->handle().empty());

    _target->type() = DialogType::Tool;
    EXPECT_FALSE(_target->handle().empty());

    _target->type() = DialogType::Application;
    EXPECT_FALSE(_target->handle().empty());
}

TEST_F(DialogTests, p)
{
    Position expectedPos(0, 0, 600, 400);
    EXPECT_EQ(_target->p().value(), expectedPos);

    expectedPos = Position(100, 100, 300, 200);
    _target->p() = expectedPos;
    EXPECT_EQ(_target->p().value(), expectedPos);
}

TEST_F(DialogTests, p_after_show)
{
    _target->show();

    Position expectedPos(0, 0, 600, 400);
    EXPECT_EQ(_target->p().value(), expectedPos);

    expectedPos = Position(100, 100, 300, 200);
    _target->p() = expectedPos;

    Position newPos = _target->p().value();

    EXPECT_EQ(_target->p().value(), expectedPos);
}

TEST_F(DialogTests, move)
{
    Size size = _target->p()->size();

    Point p(10, 20);
    _target->move(p);

    EXPECT_EQ(_target->p()->x(), 10);
    EXPECT_EQ(_target->p()->y(), 20);
    EXPECT_EQ(_target->p()->width(), size.width());
    EXPECT_EQ(_target->p()->height(), size.height());
}

TEST_F(DialogTests, move_after_show)
{
    _target->show();

    Size size = _target->p()->size();

    Point p(10, 20);
    _target->move(p);

    EXPECT_EQ(_target->p()->x(), 10);
    EXPECT_EQ(_target->p()->y(), 20);
    EXPECT_EQ(_target->p()->width(), size.width());
    EXPECT_EQ(_target->p()->height(), size.height());
}

TEST_F(DialogTests, resize)
{
    Point point = _target->p()->point();

    Size s(100, 200);
    _target->resize(s);

    EXPECT_EQ(_target->p()->width(), 100);
    EXPECT_EQ(_target->p()->height(), 200);
    EXPECT_EQ(_target->p()->x(), point.x());
    EXPECT_EQ(_target->p()->y(), point.y());
}

TEST_F(DialogTests, resize_after_show)
{
    _target->show();

    Point point = _target->p()->point();

    Size s(100, 200);
    _target->resize(s);

    EXPECT_EQ(_target->p()->width(), 100);
    EXPECT_EQ(_target->p()->height(), 200);
    EXPECT_EQ(_target->p()->x(), point.x());
    EXPECT_EQ(_target->p()->y(), point.y());
}

TEST_F(DialogTests, center)
{
    Point point = _target->p()->point();

    _target->center();

    EXPECT_EQ(_target->p()->width(), 600);
    EXPECT_EQ(_target->p()->height(), 400);
    EXPECT_NE(_target->p()->x(), point.x());
    EXPECT_NE(_target->p()->y(), point.y());
}

TEST_F(DialogTests, center_after_show)
{
    _target->show();

    Point point = _target->p()->point();

    _target->center();

    EXPECT_EQ(_target->p()->width(), 600);
    EXPECT_EQ(_target->p()->height(), 400);
    EXPECT_NE(_target->p()->x(), point.x());
    EXPECT_NE(_target->p()->y(), point.y());
}

TEST_F(DialogTests, showHelp)
{
    _target->showHelp(true);
    EXPECT_TRUE(_target->showHelp());
    _target->showHelp(false);
    EXPECT_FALSE(_target->showHelp());
}

TEST_F(DialogTests, color)
{
    Color c = Color::Red;
    _target->color(c);
    EXPECT_EQ(_target->color(), c);
}

TEST_F(DialogTests, cursor)
{
    Cursor c = Cursor::Busy;
    _target->cursor(c);
    EXPECT_EQ(_target->cursor(), c);
}

TEST_F(DialogTests, title)
{
    EXPECT_EQ(_target->title(), std::string());

    std::string title = "Test Dialog";
    _target->title(title);
    EXPECT_EQ(_target->title(), title);
}

TEST_F(DialogTests, image)
{
    ImageSource img{ "path/to/image", false, false };
    _target->image(img);
    EXPECT_EQ(_target->image(), img);

    _target->image().clear();
    EXPECT_EQ(_target->image(), ImageSource());
}

TEST_F(DialogTests, menu)
{
    auto menu = std::make_shared<Menu>();
    _target->menu(menu);
    EXPECT_EQ(_target->menu(), menu);
}

TEST_F(DialogTests, handle)
{
    EXPECT_TRUE(_target->handle().empty());
}

TEST_F(DialogTests, handle_after_show)
{
    _target->show();
    EXPECT_FALSE(_target->handle().empty());
}

TEST_F(DialogTests, user)
{
    void* userData = reinterpret_cast<void*>(0x1234);
    _target->user(userData);
    EXPECT_EQ(_target->user(), userData);
}

TEST_F(DialogTests, addRemoveChildControl)
{
    auto child = std::make_shared<Label>();

    _target->add(child);
    auto controls = _target->controls();

    EXPECT_EQ(controls.size(), 1);
    _target->remove(child);

    controls = _target->controls();
    EXPECT_TRUE(controls.empty());
}

TEST_F(DialogTests, addRemoveChildDialog)
{
    auto child = std::make_shared<Dialog>(DialogType::Frameless);

    _target->add(child);
    auto dialogs = _target->dialogs();
    EXPECT_EQ(dialogs.size(), 1);

    _target->remove(child);
    dialogs = _target->dialogs();
    EXPECT_TRUE(dialogs.empty());
}

TEST_F(DialogTests, events)
{
    auto& confirmEvent = _target->ConfirmEvent();
    auto& cancelEvent = _target->CancelEvent();
    auto& closeEvent = _target->CloseEvent();
    auto& createEvent = _target->CreateEvent();
    auto& destroyEvent = _target->DestroyEvent();
    auto& dropEvent = _target->DropEvent();
    auto& mouseDownEvent = _target->MouseDownEvent();
    auto& mouseUpEvent = _target->MouseUpEvent();
    auto& mouseMoveEvent = _target->MouseMoveEvent();
    auto& mouseDoubleClickEvent = _target->MouseDoubleClickEvent();
    auto& mouseCaptureLostEvent = _target->MouseCaptureLostEvent();
    auto& helpEvent = _target->HelpEvent();
    auto& moveEvent = _target->MoveEvent();
    auto& sizeEvent = _target->SizeEvent();
    auto& timerEvent = _target->TimerEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}

