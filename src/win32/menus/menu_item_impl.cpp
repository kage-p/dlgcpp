#include "menu_item_impl.h"
#include "utility/message.h"
#include "utility/string_encoder.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using namespace dlgcpp;
using namespace dlgcpp::menus;

MenuItemImpl::MenuItemImpl()
{
}

void MenuItemImpl::owner(IMenuItem* owner)
{
    _menuItem = owner;

    auto changeHandlerFn =
        [this](auto)
        {
            update();
        };

    // track all changes to item
    owner->text().event() += changeHandlerFn;
    owner->checked().event() += changeHandlerFn;
    owner->enabled().event() += changeHandlerFn;
    owner->defaultItem().event() += changeHandlerFn;
    owner->separator().event() += changeHandlerFn;
    owner->items().event() += changeHandlerFn;
}

MENUITEMINFOW MenuItemImpl::menuItemToStruct(
    int menuItemId,
    std::wstring& buffer)
{
    if (!_menuItem->separator())
        buffer = StringEncoder::toWide(_menuItem->text());

    UINT stateFlags = 0;
    if (_menuItem->checked())
        stateFlags |= MFS_CHECKED;

    if (_menuItem->defaultItem())
        stateFlags |= MFS_DEFAULT;

    if (!_menuItem->enabled())
        stateFlags |= MFS_GRAYED;
    else
        stateFlags |= MFS_ENABLED;

    auto mi = MENUITEMINFOW();
    mi.cbSize = sizeof(MENUITEMINFOW);
    mi.fType = !_menuItem->separator() ? MFT_STRING : MFT_SEPARATOR;
    mi.fMask = MIIM_ID | MIIM_FTYPE | MIIM_STATE | MIIM_STRING;
    mi.wID = menuItemId;
    mi.fState = stateFlags;
    mi.dwTypeData = buffer.data();
    mi.cch = (UINT)buffer.size();

    return mi;
}

bool MenuItemImpl::insert(
    HMENU hMenu,
    int& nextItemId,
    std::map<int, ISharedMenuItem>& menuItemIdMap)
{
    std::wstring buffer;
    auto mi = menuItemToStruct(nextItemId, buffer);
    nextItemId += 1;

    auto& items = _menuItem->items().value();
    if (!items.empty())
    {
        // note: docs state popup submenus will be freed by parent menu.
        mi.fMask |= MIIM_SUBMENU;
        mi.hSubMenu = CreatePopupMenu();
        for (auto& item : items)
        {
            menuItemIdMap[nextItemId] = item;

            // delegate to menu item impl.
            auto menuItemImpl = item->impl();
            if (!menuItemImpl->insert(
                mi.hSubMenu,
                nextItemId,
                menuItemIdMap))
            {
                break;
            }
        }
    }

    UINT index = GetMenuItemCount(hMenu);

    if (InsertMenuItemW(
        hMenu,
        index,
        TRUE,
        &mi) == FALSE)
    {
        // failed
        DLGCPP_CERR("Failed to insert menu with itemId " << mi.wID);
        return false;
    }

    // save menu and id
    _hMenu = hMenu;
    _menuItemId = mi.wID;
    return true;
}

void MenuItemImpl::update()
{
    if (_hMenu == NULL || _menuItemId == 0)
    {
        // no action; item not yet inserted
        return;
    }

    std::wstring buffer;
    auto mi = menuItemToStruct(
        _menuItemId,
        buffer);

    if (SetMenuItemInfoW(
        _hMenu,
        _menuItemId,
        FALSE,
        &mi) == FALSE)
    {
        DLGCPP_CERR("Failed to update menu itemId " << _menuItemId);
    }
}

void MenuItemImpl::destruct()
{
    // nothing to destroy; so clear references
    _hMenu = NULL;
    _menuItemId = 0;

    for (auto& item : _menuItem->items().value())
    {
        item->impl()->destruct();
    }
}
