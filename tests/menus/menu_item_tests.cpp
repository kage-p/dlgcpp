#include "dlgcpp/menus/menu_item.h"
#include "menu_item_tests.h"

using namespace dlgcpp;
using namespace dlgcpp::menus;
using namespace dlgcpp::tests;

TEST_F(MenuItemTests, constructor_default)
{
    auto target = std::make_shared<MenuItem>();

    EXPECT_EQ(target->shared_from_this(), target);
}

TEST_F(MenuItemTests, constructor_with_params)
{
    auto target = std::make_shared<MenuItem>("menu-item-text");

    EXPECT_EQ(target->text(), std::string("menu-item-text"));
    EXPECT_EQ(target->separator(), false);
}

TEST_F(MenuItemTests, text)
{
    auto target = std::make_shared<MenuItem>();

    EXPECT_EQ(target->text(), std::string());

    target->text() = "menu-item-text";
    EXPECT_EQ(target->text(), std::string("menu-item-text"));

    target->text().clear();
    EXPECT_EQ(target->text(), std::string());
}

TEST_F(MenuItemTests, enabled)
{
    auto target = std::make_shared<MenuItem>();

    EXPECT_EQ(target->enabled(), true);

    target->enabled() = false;
    EXPECT_EQ(target->enabled(), false);

    target->enabled() = true;
    EXPECT_EQ(target->enabled(), true);
}

TEST_F(MenuItemTests, checked)
{
    auto target = std::make_shared<MenuItem>();

    EXPECT_EQ(target->checked(), false);

    target->checked() = true;
    EXPECT_EQ(target->checked(), true);

    target->checked() = false;
    EXPECT_EQ(target->checked(), false);
}


TEST_F(MenuItemTests, defaultItem)
{
    auto target = std::make_shared<MenuItem>();

    EXPECT_EQ(target->defaultItem(), false);

    target->defaultItem() = true;
    EXPECT_EQ(target->defaultItem(), true);

    target->defaultItem() = false;
    EXPECT_EQ(target->defaultItem(), false);
}

TEST_F(MenuItemTests, separator)
{
    auto target = std::make_shared<MenuItem>();

    // default uses separator if no text
    EXPECT_EQ(target->separator(), true);

    target->separator() = false;
    EXPECT_EQ(target->separator(), false);

    target->separator() = true;
    EXPECT_EQ(target->separator(), true);
}

TEST_F(MenuItemTests, add)
{
    auto target = std::make_shared<MenuItem>();

    auto item = std::make_shared<MenuItem>();

    target->add(item);

    EXPECT_EQ(target->items().value(), std::vector<ISharedMenuItem>({ item }));
}

TEST_F(MenuItemTests, remove)
{
    auto target = std::make_shared<MenuItem>();

    // no-op
    target->remove(nullptr);

    auto item = std::make_shared<MenuItem>();
    target->add(item);
    target->remove(item);

    EXPECT_EQ(target->items().value(), std::vector<ISharedMenuItem>());
}

TEST_F(MenuItemTests, clear)
{
    auto target = std::make_shared<MenuItem>();

    auto item = std::make_shared<MenuItem>();

    target->add(item);
    target->clear();

    EXPECT_EQ(target->items().value(), std::vector<ISharedMenuItem>());

}

TEST_F(MenuItemTests, items)
{
    auto target = std::make_shared<MenuItem>();

    EXPECT_EQ(target->items().value(), std::vector<ISharedMenuItem>());
}

TEST_F(MenuItemTests, events)
{
    auto target = std::make_shared<MenuItem>();

    auto& clickEvent = target->ClickEvent();

    // TODO: Additional checks can be made to verify the event handling logic
}
