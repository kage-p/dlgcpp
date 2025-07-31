#include "dlgcpp/controls/control.h"
#include "dlgcpp/dialogs/dialog.h"
#include "menu_p.h"
#include "utility/convert.h"
#include "utility/message.h"
#include "utility/string_encoder.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using namespace dlgcpp;
using namespace dlgcpp::menus;

MenuImpl::MenuImpl()
{
}

MenuImpl::~MenuImpl()
{
    destruct();
}

ISharedDialog MenuImpl::parent() const
{
    return _parent;
}

void MenuImpl::parent(ISharedDialog parent)
{
    if (_parent == parent)
        return;
    _parent = parent;
}

int MenuImpl::id() const
{
    return _startId;
}

void MenuImpl::id(int value)
{
    _startId = value;
}

void MenuImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) != 0)
            return;

        // search the menu tree for the id
        auto id = (int)LOWORD(msg.wParam);
        auto& item = _idMap[id];
        if (item != nullptr)
            item->ClickEvent().invoke(item);
    }
}

void MenuImpl::add(ISharedMenuItem item)
{
    auto it = std::find(_items.begin(),
        _items.end(),
        item);
    if (it != _items.end())
        return;
    _items.push_back(item);

    // track changes for updates
    auto bindingRef = item->ChangedEvent() += [this](ISharedMenuItem item)
        {
            updateItem(item);
        };
    // TODO: needs to remove by bindingRef
}

void MenuImpl::remove(ISharedMenuItem item)
{
    auto it = std::find(_items.begin(),
        _items.end(),
        item);
    if (it == _items.end())
        return;
    _items.erase(it);
}

void MenuImpl::clear()
{
    _items.clear();
}

const std::vector<ISharedMenuItem>& MenuImpl::items() const
{
    return _items;
}

MENUITEMINFOW MenuImpl::menuItemToStruct(
    ISharedMenuItem item,
    int menuItemId,
    std::wstring& buffer)
{
    if (!item->separator())
        buffer = StringEncoder::toWide(item->text());

    UINT stateFlags = 0;
    if (item->checked())
        stateFlags |= MFS_CHECKED;

    if (item->defaultItem())
        stateFlags |= MFS_DEFAULT;

    if (!item->enabled())
        stateFlags |= MFS_GRAYED;
    else
        stateFlags |= MFS_ENABLED;

    auto mi = MENUITEMINFOW();
    mi.cbSize = sizeof(MENUITEMINFOW);
    mi.fType = !item->separator() ? MFT_STRING : MFT_SEPARATOR;
    mi.fMask = MIIM_ID | MIIM_FTYPE | MIIM_STATE | MIIM_STRING;
    mi.wID = menuItemId;
    mi.fState = stateFlags;
    mi.dwTypeData = buffer.data();
    mi.cch = (UINT)buffer.size();

    return mi;
}

bool MenuImpl::insertMenuItem(
    ISharedMenuItem item,
    HMENU hMenu,
    int& nextItemId)
{
    std::wstring buffer;
    auto mi = menuItemToStruct(item, nextItemId, buffer);
    nextItemId += 1;

    if (!item->items().empty())
    {
        // note: docs state popup submenus will be freed by parent menu.
        mi.fMask |= MIIM_SUBMENU;
        mi.hSubMenu = CreatePopupMenu();
        createMenuItems(
            mi.hSubMenu,
            item->items(),
            nextItemId);
    }

    UINT index = GetMenuItemCount(hMenu);

    return (InsertMenuItemW(
        hMenu,
        index,
        TRUE,
        &mi) != FALSE);
}

void MenuImpl::createMenuItems(
    HMENU hMenu,
    const std::vector<ISharedMenuItem>& items,
    int& nextItemId)
{
    int index = 0;

    for (auto& item : items)
    {
        _idMap[nextItemId] = item;

        if (!insertMenuItem(
            item,
            hMenu,
            nextItemId))
        {
            DLGCPP_CERR("Failed to insert menu with itemId " << nextItemId);
            break;
        }
    }
}

void MenuImpl::rebuild()
{
    destruct();

    bool isPopUp = (_parent == nullptr);
    if (isPopUp)
        _hMenu = CreatePopupMenu();
    else
        _hMenu = CreateMenu();

    int id = _startId;

    createMenuItems(
        _hMenu,
        _items,
        id);

    if (!isPopUp)
    {
        // self-assign to parent
        HWND hwndParent = reinterpret_cast<HWND>(_parent->handle());
        if (hwndParent != NULL)
            SetMenu(hwndParent, _hMenu);
    }
}

void MenuImpl::updateItem(ISharedMenuItem item)
{
    // reverse-lookup. find the menu item and related Id
    auto it = std::find_if(_idMap.begin(), _idMap.end(),
        [&item](const auto& pair) { return pair.second == item; });

    if (it == _idMap.end())
        return;

    // found menu item; update it immediately
    int itemId = it->first;

    std::wstring buffer;
    auto mi = menuItemToStruct(
        item,
        itemId,
        buffer);

    if (SetMenuItemInfoW(
        _hMenu,
        itemId,
        FALSE,
        &mi) == FALSE)
    {
        DLGCPP_CERR("Failed to update menu itemId " << itemId);
    }
}

void MenuImpl::destruct()
{
    if (_hMenu != NULL)
    {
        DestroyMenu(_hMenu);
        _hMenu = NULL;
    }
    _idMap.clear();
}

void MenuImpl::popup(ISharedDialog parent, const Point& coords)
{
    if (_parent != nullptr)
        // cannot popup the menu if it has a parent
        return;

    auto hwndDialog = (HWND)parent->handle();
    if (hwndDialog == NULL)
        // cannot popup the menu if the parent dialog is not created
        return;

    if (_hMenu == NULL)
    {
        rebuild();
        if (_hMenu == NULL)
            return;
    }

    // coordinates must be in pixels
    Point pxCoords = Convert(hwndDialog).toPixels(coords);

    // map from dialog -> desktop
    auto pt = POINT{ pxCoords.x(), pxCoords.y() };
    MapWindowPoints(hwndDialog, HWND_DESKTOP, &pt, 1);

    auto id = (int)TrackPopupMenu(_hMenu,
        TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
        pt.x,
        pt.y,
        0,
        hwndDialog,
        NULL);
    if (id == 0)
        return;

    auto& item = _idMap[id];
    if (item != nullptr)
        item->ClickEvent().invoke(item);
}

void MenuImpl::popup(ISharedControl parent, const Point& coords)
{
    if (parent == nullptr)
        // cannot popup menu without a parent
        return;

    if (_parent != nullptr)
        // cannot popup the menu if it already has a parent
        return;

    auto hwndControl = (HWND)parent->handle();
    if (hwndControl == NULL)
        // cannot popup the menu if the parent control is not created
        return;
    auto hwndDialog = GetParent(hwndControl);
    if (hwndDialog == NULL)
        // cannot popup the menu if the control's parent dialog is not created
        return;

    if (_hMenu == NULL)
    {
        rebuild();
        if (_hMenu == NULL)
            return;
    }

    // coordinates must be in pixels
    Point pxCoords = Convert(hwndDialog).toPixels(coords);

    // map from control -> dialog -> desktop
    auto pt = POINT{ pxCoords.x(), pxCoords.y() };
    MapWindowPoints(hwndControl, hwndDialog, &pt, 1);
    MapWindowPoints(hwndDialog, HWND_DESKTOP, &pt, 1);

    auto id = (int)TrackPopupMenu(_hMenu,
        TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
        pt.x,
        pt.y,
        0,
        hwndDialog,
        NULL);
    if (id == 0)
        return;

    auto& item = _idMap[id];
    if (item != nullptr)
        item->ClickEvent().invoke(item);
}
