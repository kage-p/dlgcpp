
#include "menu_item_p.h"
#include "utility/message.h"
#include "utility/string.h"
#include "utility/units.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using namespace dlgcpp;
using namespace dlgcpp::menus;

MenuItemImpl::MenuItemImpl(
    MenuItem& menuItem,
    const std::string& text) :
    _menuItem(menuItem)
{
    _text = text;
    _separator = text.empty();
}

const std::string& MenuItemImpl::text() const
{
    return _text;
}

void MenuItemImpl::text(const std::string& value)
{
    if (_text == value)
        return;
    _text = value;

    _changedEvent.invoke(_menuItem.shared_from_this());
}

bool MenuItemImpl::enabled() const
{
    return _enabled;
}

void MenuItemImpl::enabled(bool value)
{
    if (_enabled == value)
        return;
    _enabled = value;

    //_dirty = true;
    _changedEvent.invoke(_menuItem.shared_from_this());
}

bool MenuItemImpl::checked() const
{
    return _checked;
}

void MenuItemImpl::checked(bool value)
{
    if (_checked == value)
        return;
    _checked = value;

    _changedEvent.invoke(_menuItem.shared_from_this());
}

bool MenuItemImpl::defaultItem() const
{
    return _defaultItem;
}

void MenuItemImpl::defaultItem(bool value)
{
    if (_defaultItem == value)
        return;
    _defaultItem = value;

    _changedEvent.invoke(_menuItem.shared_from_this());
}

bool MenuItemImpl::separator() const
{
    return _separator;
}

void MenuItemImpl::separator(bool value)
{
    if (_separator == value)
        return;
    _separator = value;

    _changedEvent.invoke(_menuItem.shared_from_this());
}

void MenuItemImpl::add(ISharedMenuItem item)
{
    auto it = std::find(_items.begin(),
        _items.end(),
        item);
    if (it != _items.end())
        return;

    // TODO: dynamic add/remove subitem not yet supported
    _items.push_back(item);

    // chain event to parent
    auto bindingRef = item->ChangedEvent() += [this](ISharedMenuItem item)
        {
            ChangedEvent().invoke(_menuItem.shared_from_this());
        };
    // TODO: needs to remove by bindingRef
}

void MenuItemImpl::remove(ISharedMenuItem item)
{
    auto it = std::find(_items.begin(),
        _items.end(),
        item);
    if (it == _items.end())
        return;

    // TODO: dynamic add/remove subitem not yet supported
    _items.erase(it);
}

void MenuItemImpl::clear()
{
    _items.clear();
}

const std::vector<ISharedMenuItem>& MenuItemImpl::items() const
{
    return _items;
}

IEvent<ISharedMenuItem>& MenuItemImpl::ChangedEvent()
{
    return _changedEvent;
}

IEvent<ISharedMenuItem>& MenuItemImpl::ClickEvent()
{
    return _clickEvent;
}
