#include "control_tests.h"
#include "controls/control_impl.h"
#include "dlgcpp/controls/control.h"
#include "mocks/mock_dialog.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::tests;


// test inherited
class ControlInTest : public Control
{
public:
    explicit ControlInTest(const Position& p = Position())
        : ControlInTest(std::make_shared<ControlImpl>(), p)
    {
        // pass a reference to the implementation class
        _impl->owner(this);
    }

    ControlInTest(
        std::shared_ptr<ControlImpl> impl,
        const Position& p)
        : Control(impl, p), _impl(std::move(impl))
    {
    }

    ~ControlInTest() override = default;

private:
    std::shared_ptr<ControlImpl> _impl;
};

TEST_F(ControlTests, constructor_default)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->shared_from_this(), target);
    EXPECT_EQ(target->p(), Position(0, 0, 0, 0));
}

TEST_F(ControlTests, constructor_with_params)
{
    auto target = std::make_shared<ControlInTest>(Position{ 1,2,3,4 });

    EXPECT_EQ(target->p(), Position(1, 2, 3, 4));
}

TEST_F(ControlTests, enabled)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_TRUE(target->enabled());

    target->enabled() = false;
    EXPECT_FALSE(target->enabled());

    target->enabled() = true;
    EXPECT_TRUE(target->enabled());
}

TEST_F(ControlTests, visible)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_TRUE(target->visible());

    target->visible(false);
    EXPECT_FALSE(target->visible());

    target->visible(true);
    EXPECT_TRUE(target->visible());
}

TEST_F(ControlTests, p)
{
    auto target = std::make_shared<ControlInTest>();

    Position pos(0, 0, 0, 0);
    EXPECT_EQ(target->p(), pos);

    pos = { 10, 20, 30, 40 };
    target->p(pos);

    EXPECT_EQ(target->p(), pos);
}

TEST_F(ControlTests, move)
{
    auto target = std::make_shared<ControlInTest>();
    Point p(10, 20);
    target->move(p);
    EXPECT_EQ(target->p(), Position(10, 20, 0, 0));
}

TEST_F(ControlTests, resize)
{
    auto target = std::make_shared<ControlInTest>();
    Size s(100, 200);
    target->resize(s);
    EXPECT_EQ(target->p(), Position(0, 0, 100, 200));
}

TEST_F(ControlTests, border)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->border(), BorderStyle::None);

    target->border(BorderStyle::Raised);
    EXPECT_EQ(target->border(), BorderStyle::Raised);

    target->border(BorderStyle::Sunken);
    EXPECT_EQ(target->border(), BorderStyle::Sunken);

    target->border(BorderStyle::Thin);
    EXPECT_EQ(target->border(), BorderStyle::Thin);

    target->border(BorderStyle::None);
    EXPECT_EQ(target->border(), BorderStyle::None);
}

TEST_F(ControlTests, colors)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->colors(), std::make_pair(Color::Default, Color::Default));

    Color fgColor = Color::Red;
    Color bgColor = Color::Blue;
    target->colors(fgColor, bgColor);

    EXPECT_EQ(target->colors(), std::make_pair(fgColor, bgColor));
}

TEST_F(ControlTests, font)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->font(), Font());

    auto f = Font("Arial", 8, true);
    target->font(f);

    EXPECT_EQ(target->font(), f);
}

TEST_F(ControlTests, cursor)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->cursor(), Cursor::Default);

    Cursor c = Cursor::Busy;
    target->cursor(c);
    EXPECT_EQ(target->cursor(), c);
}

TEST_F(ControlTests, focus)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->focus(), false);

    // no action as control inactive.
    target->focus() = true;
    EXPECT_EQ(target->focus(), false);
}

TEST_F(ControlTests, mouseCapture)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->mouseCapture(), false);

    // control not active
    target->mouseCapture(true);
    EXPECT_EQ(target->mouseCapture(), false);
}

TEST_F(ControlTests, wantKeyboardEvents)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->wantKeyboardEvents(), false);

    target->wantKeyboardEvents(true);
    EXPECT_EQ(target->wantKeyboardEvents(), true);

    target->wantKeyboardEvents(false);
    EXPECT_EQ(target->wantKeyboardEvents(), false);
}

TEST_F(ControlTests, wantMouseEvents)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->wantMouseEvents(), false);

    target->wantMouseEvents(true);
    EXPECT_EQ(target->wantMouseEvents(), true);

    target->wantMouseEvents(false);
    EXPECT_EQ(target->wantMouseEvents(), false);
}

TEST_F(ControlTests, wantPaintEvents)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->wantPaintEvents(), false);

    target->wantPaintEvents(true);
    EXPECT_EQ(target->wantPaintEvents(), true);

    target->wantPaintEvents(false);
    EXPECT_EQ(target->wantPaintEvents(), false);
}

TEST_F(ControlTests, wantSizingEvents)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_EQ(target->wantSizingEvents(), false);

    target->wantSizingEvents(true);
    EXPECT_EQ(target->wantSizingEvents(), true);

    target->wantSizingEvents(false);
    EXPECT_EQ(target->wantSizingEvents(), false);
}

TEST_F(ControlTests, handle)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_TRUE(target->handle().empty());
}

TEST_F(ControlTests, user)
{
    auto target = std::make_shared<ControlInTest>();

    EXPECT_TRUE(target->user().empty());

    void* userData = reinterpret_cast<void*>(0x1234);
    target->user(userData);
    EXPECT_EQ(target->user(), userData);
}

TEST_F(ControlTests, bringToFront)
{
    auto target = std::make_shared<ControlInTest>();

    // no action
    target->bringToFront();
}

TEST_F(ControlTests, sendToBack)
{
    auto target = std::make_shared<ControlInTest>();

    // no action
    target->sendToBack();
}

TEST_F(ControlTests, redraw)
{
    auto target = std::make_shared<ControlInTest>();

    // no action
    target->redraw();
}

TEST_F(ControlTests, events)
{
    auto target = std::make_shared<ControlInTest>();

    auto& createEvent = target->CreateEvent();
    auto& destroyEvent = target->DestroyEvent();
    auto& keyDownEvent = target->KeyDownEvent();
    auto& keyUpEvent = target->KeyUpEvent();
    auto& mouseDownEvent = target->MouseDownEvent();
    auto& mouseUpEvent = target->MouseUpEvent();
    auto& mouseMoveEvent = target->MouseMoveEvent();
    auto& mouseDoubleClickEvent = target->MouseDoubleClickEvent();
    auto& mouseCaptureLostEvent = target->MouseCaptureLostEvent();
    auto& moveEvent = target->MoveEvent();
    auto& sizeEvent = target->SizeEvent();
    auto& paintEvent = target->PaintEvent();
    auto& userEvent = target->UserEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
