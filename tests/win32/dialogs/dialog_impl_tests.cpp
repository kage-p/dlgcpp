#include "controls/control_impl.h"
#include "dialog_impl_tests.h"
#include "mocks/fake_control.h"
#include "mocks/mock_dialog.h"
#include "utility/convert.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;
using namespace dlgcpp::controls;
using namespace dlgcpp::menus;
using namespace dlgcpp::tests;

using namespace testing;

void DialogImplTests::SetUp()
{
    // ensure fake dialog lifetime exceeds target lifetime
    _fakeDialog = std::make_shared<NiceMock<FakeDialog>>();
    _target = std::make_shared<DialogImpl>();
    _target->owner(_fakeDialog.get());

    // forward handle property change
    _target->handle().event() +=
        [&](auto)
        {
            if (_target)
                _fakeDialog->handle() = _target->handle().value();
            else
                _fakeDialog->handle() = nullptr;
        };
}

void DialogImplTests::TearDown()
{
    _target.reset();
    _fakeDialog.reset();
}

TEST_F(DialogImplTests, show)
{
    EXPECT_EQ(_target->hwnd(), nullptr);
    EXPECT_TRUE(_target->handle().empty());

    // this will construct the dialog
    _target->show();
    EXPECT_NE(_target->hwnd(), nullptr);
    EXPECT_FALSE(_target->handle().empty());
}

TEST_F(DialogImplTests, close)
{
    // no-op expected
    _target->close();

    _target->show();
    _target->close();

    EXPECT_EQ(_target->hwnd(), nullptr);
}

TEST_F(DialogImplTests, exec)
{
    int resultCode = 12345;

    _target->timer(500);
    _fakeDialog->TimerEvent() +=
        [&](ISharedDialog dlg)
        {
            // break out of the exec()
            _target->close(resultCode);
        };

    auto result = _target->exec();
    EXPECT_EQ(result, resultCode);
}

TEST_F(DialogImplTests, enabled_handler)
{
    // construct the dialog
    _target->show();

    EXPECT_TRUE(IsWindowEnabled(_target->hwnd()));

    _fakeDialog->enabled() = false;
    EXPECT_FALSE(IsWindowEnabled(_target->hwnd()));

    _fakeDialog->enabled() = true;
    EXPECT_TRUE(IsWindowEnabled(_target->hwnd()));
}

TEST_F(DialogImplTests, visible_handler)
{
    // construct the dialog
    _target->show();

    EXPECT_TRUE(IsWindowVisible(_target->hwnd()));

    _fakeDialog->visible() = false;
    EXPECT_FALSE(IsWindowVisible(_target->hwnd()));

    _fakeDialog->visible() = true;
    EXPECT_TRUE(IsWindowVisible(_target->hwnd()));
}

TEST_F(DialogImplTests, type_handler)
{
    auto hasStyles =
        [](HWND hwnd, UINT styles, UINT nonStyles) -> bool
        {
            auto windowStyles = (UINT)GetWindowLong(hwnd, GWL_STYLE);
            return (windowStyles & styles) != 0 && (windowStyles & nonStyles) == 0;
        };

    // construct the dialog
    _target->show();

    _fakeDialog->type() = DialogType::Application;
    EXPECT_TRUE(
        hasStyles(
            _target->hwnd(),
            WS_OVERLAPPED | WS_BORDER | WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
            WS_POPUP | DS_MODALFRAME));

    _fakeDialog->type() = DialogType::Popup;
    EXPECT_TRUE(
        hasStyles(
            _target->hwnd(),
            WS_OVERLAPPED | WS_BORDER | WS_DLGFRAME | WS_SYSMENU | WS_POPUP,
            WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | DS_MODALFRAME));

    _fakeDialog->type() = DialogType::Frameless;
    EXPECT_TRUE(
        hasStyles(
            _target->hwnd(),
            WS_POPUP,
            WS_OVERLAPPED | WS_BORDER | WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | DS_MODALFRAME));

    _fakeDialog->type() = DialogType::Tool;
    EXPECT_TRUE(
        hasStyles(_target->hwnd(),
            WS_POPUP | WS_BORDER | WS_DLGFRAME | WS_SYSMENU | DS_MODALFRAME,
            WS_OVERLAPPED | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME));
}

TEST_F(DialogImplTests, title_handler)
{
    auto getText =
        [](HWND hwnd) -> std::string
        {
            int len = GetWindowTextLengthW(hwnd);
            if (len < 1)
                return {};

            std::wstring buf(len, L'\0');
            GetWindowTextW(hwnd, buf.data(), len + 1);
            return StringEncoder::toBytes(buf.c_str());
        };

    // construct the dialog
    _target->show();

    EXPECT_EQ(getText(_target->hwnd()), std::string());

    std::string title = "Test Dialog";
    _fakeDialog->title() = title;
    EXPECT_EQ(getText(_target->hwnd()), title);
}

TEST_F(DialogImplTests, image_handler)
{
    auto getIcons =
        [](HWND hwnd) -> std::pair<ICONINFO, ICONINFO>
        {
            auto hIconBig = (HICON)SendMessage(hwnd, WM_GETICON, ICON_BIG, (LPARAM)NULL);
            ICONINFO iiBig = {};
            if (hIconBig != NULL)
                GetIconInfo(hIconBig, &iiBig);

            auto hIconSmall = (HICON)SendMessage(hwnd, WM_GETICON, ICON_SMALL, (LPARAM)NULL);
            ICONINFO iiSmall = {};
            if (hIconSmall != NULL)
                GetIconInfo(hIconSmall, &iiSmall);

            return { iiBig, iiSmall };
        };

    // construct the dialog
    _target->show();

    auto icons1 = getIcons(_target->hwnd());
    EXPECT_TRUE(icons1.first.fIcon == 0 && icons1.first.hbmColor == nullptr);
    EXPECT_TRUE(icons1.second.fIcon == 0 && icons1.second.hbmColor == nullptr);

    auto image = ImageSource{ "assets/demo.ico", true, true };
    _fakeDialog->image() = image;

    auto icons2 = getIcons(_target->hwnd());
    EXPECT_TRUE(icons2.first.fIcon == 1 && icons2.first.hbmColor != nullptr);
    EXPECT_TRUE(icons2.second.fIcon == 1 && icons2.second.hbmColor != nullptr);

    _fakeDialog->image() = ImageSource();

    auto icons3 = getIcons(_target->hwnd());
    EXPECT_TRUE(icons1.first.fIcon == 0 && icons1.first.hbmColor == nullptr);
    EXPECT_TRUE(icons1.second.fIcon == 0 && icons1.second.hbmColor == nullptr);
}

TEST_F(DialogImplTests, display_handler)
{
    // construct the dialog
    _target->show();

    EXPECT_TRUE(!IsZoomed(_target->hwnd()) && !IsIconic(_target->hwnd()));

    _fakeDialog->display() = DisplayState::Maximized;
    EXPECT_TRUE(IsZoomed(_target->hwnd()));

    _fakeDialog->display() = DisplayState::Minimized;
    EXPECT_TRUE(IsIconic(_target->hwnd()));

    _fakeDialog->display() = DisplayState::Normal;
    EXPECT_TRUE(!IsZoomed(_target->hwnd()) && !IsIconic(_target->hwnd()));
}

TEST_F(DialogImplTests, p_handler_move_keep_size)
{
    _fakeDialog->type() = DialogType::Application;

    auto size = _fakeDialog->p()->size();

    _fakeDialog->resize(size);
    _fakeDialog->center();

    // construct the dialog
    _target->show();

    const auto& sizeActual = _fakeDialog->p()->size();

    EXPECT_EQ(sizeActual, size);
}

TEST_F(DialogImplTests, color_handler)
{
    auto getBrushColor =
        [](HWND hwnd) -> Color
        {
            auto hbrBack = (HBRUSH)SendMessage(hwnd, WM_CTLCOLORDLG, 0, 0);
            LOGBRUSH lb = {};
            GetObject(hbrBack, sizeof(LOGBRUSH), &lb);
            return (Color)lb.lbColor;
        };

    // construct the dialog
    _target->show();

    // system default
    EXPECT_EQ(getBrushColor(_target->hwnd()), (Color)GetSysColor(COLOR_BTNFACE));

    // color red
    _fakeDialog->color() = Color::Red;
    EXPECT_EQ(getBrushColor(_target->hwnd()), Color::Red);

    _fakeDialog->color() = Color::Default;
    EXPECT_EQ(getBrushColor(_target->hwnd()), (Color)GetSysColor(COLOR_BTNFACE));
}

TEST_F(DialogImplTests, active_handler)
{
    auto isActive =
        [](HWND hwnd) -> bool
        {
            return GetActiveWindow() == hwnd;
        };

    // construct the dialog
    _target->show();

    // may be false; depends on Windows environment
    EXPECT_EQ(_fakeDialog->active(), isActive(_target->hwnd()));

    _fakeDialog->active() = true;
    EXPECT_EQ(_fakeDialog->active(), isActive(_target->hwnd()));
}

TEST_F(DialogImplTests, dropTarget_handler)
{
    auto isAcceptingFiles =
        [](HWND hwnd) -> bool
        {
            return (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_ACCEPTFILES);
        };

    // construct the dialog
    _target->show();

    // DragAcceptFiles sets a window style flag, so just test for it
    EXPECT_FALSE(isAcceptingFiles(_target->hwnd()));

    _fakeDialog->dropTarget() = true;
    EXPECT_TRUE(isAcceptingFiles(_target->hwnd()));

    _fakeDialog->dropTarget() = false;
    EXPECT_FALSE(isAcceptingFiles(_target->hwnd()));
}

TEST_F(DialogImplTests, mouseCapture_handler)
{
    // construct the dialog
    _target->show();

    EXPECT_NE(GetCapture(), _target->hwnd());

    _fakeDialog->mouseCapture() = true;
    EXPECT_EQ(GetCapture(), _target->hwnd());

    _fakeDialog->mouseCapture() = false;
    EXPECT_NE(GetCapture(), _target->hwnd());
}

TEST_F(DialogImplTests, showHelp_handler)
{
    // it must have the correct type to show the help button
    _fakeDialog->type() = DialogType::Popup;

    // construct the dialog
    _target->show();

    LONG styles;

    styles = GetWindowLong(_target->hwnd(), GWL_EXSTYLE);
    EXPECT_TRUE((styles & WS_EX_CONTEXTHELP) == 0);

    _fakeDialog->showHelp() = true;
    styles = GetWindowLong(_target->hwnd(), GWL_EXSTYLE);
    EXPECT_TRUE((styles & WS_EX_CONTEXTHELP) != 0);
}

TEST_F(DialogImplTests, add_remove_control)
{
    // construct the dialog
    _target->show();

    auto child = std::make_shared<testing::NiceMock<FakeControl>>();
    auto childImpl = std::make_shared<controls::ControlImpl>();

    childImpl->owner(child.get());

    // link impl and parent
    ON_CALL(*child, impl())
        .WillByDefault(Return(childImpl));
    ON_CALL(*child, parent())
        .WillByDefault(Return(_fakeDialog->shared_from_this()));

    _target->add(child);

    EXPECT_TRUE(childImpl->id() != 0);
    EXPECT_FALSE(childImpl->handle().empty());

    _target->remove(child);

    EXPECT_TRUE(childImpl->id() == 0);
    EXPECT_TRUE(childImpl->handle().empty());
}

TEST_F(DialogImplTests, add_remove_dialog)
{
    // construct the dialog
    _target->show();

    auto child = std::make_shared<testing::NiceMock<FakeDialog>>();
    auto childImpl = std::make_shared<dialogs::DialogImpl>();

    childImpl->owner(child.get());

    // link impl and parent
    ON_CALL(*child, impl())
        .WillByDefault(Return(childImpl));
    ON_CALL(*child, parent())
        .WillByDefault(Return(_fakeDialog->shared_from_this()));

    _target->add(child);
    EXPECT_TRUE(childImpl->id() != 0);
    EXPECT_FALSE(childImpl->handle().empty());

    _target->remove(child);
    EXPECT_TRUE(childImpl->id() == 0);
    EXPECT_TRUE(child->handle().empty());
}

TEST_F(DialogImplTests, move_by_user)
{
    auto menu = std::make_shared<Menu>();
    _fakeDialog->menu() = menu;

    Size size(600, 400);
    EXPECT_EQ(_fakeDialog->p()->size(), size);

    _target->show();

    EXPECT_EQ(_fakeDialog->p()->size(), size);

    // simulate user moving the dialog; this should not change the size
    auto hwnd = (HWND)_target->handle().value();

    RECT rc = {};
    GetWindowRect(hwnd, &rc);

    OffsetRect(&rc, 1, 1);
    SetWindowPos(
        hwnd,
        NULL,
        rc.left,
        rc.top,
        rc.right - rc.left,
        rc.bottom - rc.top,
        SWP_NOZORDER | SWP_NOACTIVATE);

    EXPECT_EQ(_fakeDialog->p()->size(), size);

    OffsetRect(&rc, 1, 1);
    SetWindowPos(
        hwnd,
        NULL,
        rc.left,
        rc.top,
        rc.right - rc.left,
        rc.bottom - rc.top,
        SWP_NOZORDER | SWP_NOACTIVATE);

    EXPECT_EQ(_fakeDialog->p()->size(), size);
}