#include "dlgcpp/menus/menu.h"
#include "menu_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::menus;
using namespace dlgcpp::tests;

TEST_F(MenuTests, constructor_default)
{
    auto target = std::make_shared<Menu>();

    EXPECT_EQ(target->shared_from_this(), target);
}

TEST_F(MenuTests, add)
{
    auto target = std::make_shared<Menu>();

    auto item = std::make_shared<MenuItem>();

    target->add(item);

    EXPECT_EQ(target->items().value(), std::vector<ISharedMenuItem>({ item }));
}

TEST_F(MenuTests, remove)
{
    auto target = std::make_shared<Menu>();

    // no-op
    target->remove(nullptr);

    auto item = std::make_shared<MenuItem>();
    target->add(item);
    target->remove(item);

    EXPECT_EQ(target->items().value(), std::vector<ISharedMenuItem>());
}

TEST_F(MenuTests, clear)
{
    auto target = std::make_shared<Menu>();

    auto item = std::make_shared<MenuItem>();

    target->add(item);
    target->clear();

    EXPECT_EQ(target->items().value(), std::vector<ISharedMenuItem>());
}

TEST_F(MenuTests, items)
{
    auto target = std::make_shared<Menu>();

    EXPECT_EQ(target->items().value(), std::vector<ISharedMenuItem>());
}

TEST_F(MenuTests, popup_for_control)
{
    auto target = std::make_shared<Menu>();

    // no-op; no control
    ISharedControl control;
    target->popup(control, Point{ 1,1 });
}

TEST_F(MenuTests, popup_for_dialog)
{
    auto target = std::make_shared<Menu>();

    // no-op; no dialog
    ISharedDialog dialog;
    target->popup(dialog, Point{ 1,1 });
}
