#include "convert_tests.h"
#include "dlgcpp/dialogs/dialog.h"
#include "utility/convert.h"

using namespace dlgcpp;
using namespace dlgcpp::dialogs;
using namespace dlgcpp::tests;

TEST_F(ConvertTests, toPixels_WithNoHwnd_NonClientMapping)
{
    auto p = Position{ 100,200,300,400 };

    Convert target;
    auto result = target.toPixels(p, false);

    // needs HWND for nonclient
    EXPECT_EQ(Position{ }, result);
}

TEST_F(ConvertTests, toUnits_WithNoHwnd)
{
    auto p = Position{ 100,200,300,400 };

    Convert target;
    auto result = target.toUnits(p);

    EXPECT_NE(Position{ }, result);
}

TEST_F(ConvertTests, toPixels_ToUnits_WithNoHwnd_ClientMapping)
{
    auto p = Position{ 100,200,300,400 };

    Convert target;
    auto result = target.toPixels(p, true);
    result = target.toUnits(result);

    EXPECT_EQ(p, result);
}

TEST_F(ConvertTests, toPixels_ToUnits_WithHwnd_NonClientMapping)
{
    auto p = Position{ 100,200,300,400 };

    auto dlg = std::make_shared<Dialog>();
    dlg->show();
    auto hwnd = (HWND)dlg->handle().value();

    Convert target(hwnd);
    auto result = target.toPixels(p, false);
    result = target.toUnits(result, false);

    EXPECT_EQ(p, result);
}

TEST_F(ConvertTests, toPixels_ToUnits_WithHwnd_ClientMapping)
{
    auto p = Position{ 100,200,300,400 };

    auto dlg = std::make_shared<Dialog>();
    dlg->show();
    auto hwnd = (HWND)dlg->handle().value();

    Convert target(hwnd);
    auto result = target.toPixels(p, true);
    result = target.toUnits(result, true);

    EXPECT_EQ(p, result);
}
