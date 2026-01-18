#include "dlgcpp/menus/menu_item.h"
#include "menus/menu_item_impl.h"

using namespace dlgcpp;
using namespace dlgcpp::menus;

MenuItem::MenuItem(const std::string& text)
{
    auto ownerFn = [this]() -> ISharedMenuItem
        {
            return std::static_pointer_cast<dlgcpp::menus::IMenuItem>(shared_from_this());
        };

    // properties
    _text.reset(text, nullptr, ownerFn, "text");
    _enabled.reset(true, nullptr, ownerFn, "enabled");
    _checked.reset(false, nullptr, ownerFn, "checked");
    _defaultItem.reset(false, nullptr, ownerFn, "defaultItem");
    _separator.reset(text.empty(), nullptr, ownerFn, "separator");
    _items.reset(std::vector<ISharedMenuItem>(), nullptr, ownerFn, "items");

    // events
    _clickEvent.reset(ownerFn, "ClickEvent");

    _impl = std::make_shared<MenuItemImpl>();
    _impl->owner(this);
}

MenuItem::~MenuItem()
{
    _impl.reset();
}

std::shared_ptr<MenuItemImpl> MenuItem::impl()
{
    return _impl;
}

IProperty<std::string, ISharedMenuItem>& MenuItem::text()
{
    return _text;
}

IProperty<bool, ISharedMenuItem>& MenuItem::enabled()
{
    return _enabled;
}

IProperty<bool, ISharedMenuItem>& MenuItem::checked()
{
    return _checked;
}

IProperty<bool, ISharedMenuItem>& MenuItem::defaultItem()
{
    return _defaultItem;
}

IProperty<bool, ISharedMenuItem>& MenuItem::separator()
{
    return _separator;
}

void MenuItem::add(ISharedMenuItem item)
{
    std::vector<ISharedMenuItem> items = _items.value();

    auto it = std::find(items.begin(),
        items.end(),
        item);
    if (it != items.end())
        return;

    // TODO: dynamic add/remove subitem not yet supported
    items.push_back(item);

    _items.set(items);
}

void MenuItem::remove(ISharedMenuItem item)
{
    std::vector<ISharedMenuItem> items = _items.value();

    auto it = std::find(items.begin(),
        items.end(),
        item);
    if (it == items.end())
        return;

    // TODO: dynamic add/remove subitem not yet supported
    items.erase(it);

    _items.set(items);
}

void MenuItem::clear()
{
    _items.clear();
}

IReadOnlyProperty<std::vector<ISharedMenuItem>, ISharedMenuItem>& MenuItem::items()
{
    return _items;
}

IEvent<ISharedMenuItem>& MenuItem::ClickEvent()
{
    return _clickEvent;
}
