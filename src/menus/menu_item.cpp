#include "menus/menu_item_p.h"

using namespace dlgcpp;
using namespace dlgcpp::menus;

MenuItem::MenuItem(const std::string& text)
{
    _impl = std::make_shared<MenuItemImpl>(*this, text);
}

MenuItem::~MenuItem()
{
    _impl.reset();
}

const std::string& MenuItem::text() const
{
    return _impl->text();
}

void MenuItem::text(const std::string& value)
{
    if (_impl->text() == value)
        return;
    _impl->text(value);
}

bool MenuItem::enabled() const
{
    return _impl->enabled();
}

void MenuItem::enabled(bool value)
{
    if (_impl->enabled() == value)
        return;
    _impl->enabled(value);
}

bool MenuItem::checked() const
{
    return _impl->checked();
}

void MenuItem::checked(bool value)
{
    if (_impl->checked() == value)
        return;
    _impl->checked(value);
}

bool MenuItem::defaultItem() const
{
    return _impl->defaultItem();
}

void MenuItem::defaultItem(bool value)
{
    if (_impl->defaultItem() == value)
        return;
    _impl->defaultItem(value);
}

bool MenuItem::separator() const
{
    return _impl->separator();
}

void MenuItem::separator(bool value)
{
    if (_impl->separator() == value)
        return;
    _impl->separator(value);
}

void MenuItem::add(ISharedMenuItem item)
{
    _impl->add(item);
}

void MenuItem::remove(ISharedMenuItem item)
{
    _impl->remove(item);
}

void MenuItem::clear()
{
    _impl->clear();
}

const std::vector<ISharedMenuItem>& MenuItem::items() const
{
    return _impl->items();
}

IEvent<ISharedMenuItem>& MenuItem::ChangedEvent()
{
    return _impl->ChangedEvent();
}

IEvent<ISharedMenuItem>& MenuItem::ClickEvent()
{
    return _impl->ClickEvent();
}
