#include "dlgcpp/menus/menu.h"
#include "menus/menu_impl.h"
#include "menus/menu_item_impl.h"

using namespace dlgcpp;
using namespace dlgcpp::menus;

Menu::Menu()
{
    auto ownerFn = [this]() -> ISharedMenu { return std::static_pointer_cast<dlgcpp::menus::IMenu>(shared_from_this()); };

    _items.reset(std::vector<ISharedMenuItem>(), nullptr, ownerFn, "items");

    _impl = std::make_shared<MenuImpl>();
    _impl->owner(this);
}

Menu::~Menu()
{
    _impl.reset();
}

std::shared_ptr<MenuImpl> Menu::impl()
{
    return _impl;
}

void Menu::add(ISharedMenuItem item)
{
    std::vector<ISharedMenuItem> items = _items.value();

    auto it = std::find(items.begin(),
        items.end(),
        item);
    if (it != items.end())
        return;
    items.push_back(item);
    _items.set(items);
}

void Menu::remove(ISharedMenuItem item)
{
    std::vector<ISharedMenuItem> items = _items.value();

    auto it = std::find(items.begin(),
        items.end(),
        item);
    if (it == items.end())
        return;

    items.erase(it);

    _items.set(items);
}

void Menu::clear()
{
    _items.clear();
}

IReadOnlyProperty<std::vector<ISharedMenuItem>, ISharedMenu>& Menu::items()
{
    return _items;
}

void Menu::popup(ISharedDialog parent, const Point& coords)
{
    _impl->popup(parent, coords);
}

void Menu::popup(ISharedControl parent, const Point& coords)
{
    _impl->popup(parent, coords);
}
