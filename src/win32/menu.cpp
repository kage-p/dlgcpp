#include "dlgcpp/control.h"
#include "dlgcpp/dialog.h"
#include "dlgmsg.h"
#include "menu_p.h"
#include "utility.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

using namespace dlgcpp;

Menu::Menu() :
    _props(new menu_props()),
    _state(new menu_state())
{
}

Menu::~Menu()
{
    destruct();
    delete _props;
    delete _state;
}

ISharedDialog Menu::parent() const
{
    return _props->parent;
}

void Menu::parent(ISharedDialog parent)
{
    if (_props->parent == parent)
        return;
    _props->parent = parent;
}

int Menu::id() const
{
    return _props->startId;
}

void Menu::id(int value)
{
    _props->startId = value;
}

std::shared_ptr<IMenu> Menu::menu()
{
    return shared_from_this();
}

void Menu::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) != 0)
            return;

        // search the menu tree for the id
        auto id = (int)LOWORD(msg.wParam);
        auto item = _state->idMap[id];
        if (item != nullptr)
            item->ClickEvent().invoke(item);
    }
}

void Menu::add(ISharedMenuItem item)
{
    auto it = std::find(_props->items.begin(),
        _props->items.end(),
        item);
    if (it != _props->items.end())
        return;
    _props->items.push_back(item);

    // track changes for updates
    auto bindingRef = item->ChangedEvent() += [this](ISharedMenuItem item)
        {
            updateItem(item);
        };
    // TODO: needs to remove by bindingRef
}

void Menu::remove(ISharedMenuItem item)
{
    auto it = std::find(_props->items.begin(),
        _props->items.end(),
        item);
    if (it == _props->items.end())
        return;
    _props->items.erase(it);
}

void Menu::clear()
{
    _props->items.clear();
}

const std::vector<ISharedMenuItem>& Menu::items() const
{
    return _props->items;
}

MENUITEMINFOW menuItemToStruct(
    ISharedMenuItem item,
    int menuItemId,
    std::wstring& buffer)
{
    if (!item->separator())
        buffer = toWide(item->text());

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

void createMenuItems(
    HMENU hMenu,
    const std::vector<ISharedMenuItem>& items,
    int& nextItemId,
    menu_state& state);

bool insertMenuItem(
    ISharedMenuItem item,
    HMENU hMenu,
    int& nextItemId,
    menu_state& state)
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
            nextItemId,
            state);
    }

    UINT index = GetMenuItemCount(hMenu);

    return (InsertMenuItemW(
        hMenu,
        index,
        TRUE,
        &mi) != FALSE);
}

void createMenuItems(
    HMENU hMenu,
    const std::vector<ISharedMenuItem>& items,
    int& nextItemId,
    menu_state& state)
{
    int index = 0;

    for (auto& item : items)
    {
        state.idMap[nextItemId] = item;

        if (!insertMenuItem(
            item,
            hMenu,
            nextItemId,
            state))
        {
            DLGCPP_CERR("Failed to insert menu itemId " << itemId);
            break;
        }
    }
}

void Menu::rebuild()
{
    destruct();

    bool isPopUp = (_props->parent == nullptr);
    if (isPopUp)
        _state->hMenu = CreatePopupMenu();
    else
        _state->hMenu = CreateMenu();

    int id = _props->startId;
    createMenuItems(
        _state->hMenu,
        _props->items,
        id,
        *_state);

    if (!isPopUp)
    {
        // self-assign to parent
        HWND hwndParent = reinterpret_cast<HWND>(_props->parent->handle());
        if (hwndParent != NULL)
            SetMenu(hwndParent, _state->hMenu);
    }
}

void Menu::updateItem(ISharedMenuItem item)
{
    // reverse-lookup. find the menu item and related Id
    auto it = std::find_if(_state->idMap.begin(), _state->idMap.end(),
        [&item](const auto& pair) { return pair.second == item; });

    if (it == _state->idMap.end())
        return;

    // found menu item; update it immediately
    int itemId = it->first;

    std::wstring buffer;
    auto mi = menuItemToStruct(
        item,
        itemId,
        buffer);

    if (SetMenuItemInfoW(
        _state->hMenu,
        itemId,
        FALSE,
        &mi) == FALSE)
    {
        DLGCPP_CERR("Failed to update menu itemId " << itemId);
    }
}

void Menu::destruct()
{
    if (_state->hMenu != NULL)
    {
        DestroyMenu(_state->hMenu);
        _state->hMenu = NULL;
    }
    _state->idMap.clear();
}

void Menu::popup(ISharedDialog parent, const Point& coords)
{
    if (_props->parent != nullptr)
        // cannot popup the menu if it has a parent
        return;

    auto hwndDialog = (HWND)parent->handle();
    if (hwndDialog == NULL)
        // cannot popup the menu if the parent dialog is not created
        return;

    if (_state->hMenu == NULL)
    {
        rebuild();
        if (_state->hMenu == NULL)
            return;
    }

    // coordinates must be in pixels
    Point pxCoords(coords);
    toPixels(hwndDialog, pxCoords);

    // map from dialog -> desktop
    auto pt = POINT{ pxCoords.x(), pxCoords.y() };
    MapWindowPoints(hwndDialog, HWND_DESKTOP, &pt, 1);

    auto id = (int)TrackPopupMenu(_state->hMenu,
        TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
        pt.x,
        pt.y,
        0,
        hwndDialog,
        NULL);
    if (id == 0)
        return;

    auto& item = _state->idMap[id];
    if (item != nullptr)
        item->ClickEvent().invoke(item);
}

void Menu::popup(ISharedControl parent, const Point& coords)
{
    if (_props->parent != nullptr)
        // cannot popup the menu if it has a parent
        return;

    auto hwndControl = (HWND)parent->handle();
    if (hwndControl == NULL)
        // cannot popup the menu if the parent control is not created
        return;
    auto hwndDialog = GetParent(hwndControl);
    if (hwndDialog == NULL)
        // cannot popup the menu if the control's parent dialog is not created
        return;

    if (_state->hMenu == NULL)
    {
        rebuild();
        if (_state->hMenu == NULL)
            return;
    }

    // coordinates must be in pixels
    Point pxCoords(coords);
    toPixels(hwndDialog, pxCoords);

    // map from control -> dialog -> desktop
    auto pt = POINT{ pxCoords.x(), pxCoords.y() };
    MapWindowPoints(hwndControl, hwndDialog, &pt, 1);
    MapWindowPoints(hwndDialog, HWND_DESKTOP, &pt, 1);

    auto id = (int)TrackPopupMenu(_state->hMenu,
        TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
        pt.x,
        pt.y,
        0,
        hwndDialog,
        NULL);
    if (id == 0)
        return;

    auto& item = _state->idMap[id];
    if (item != nullptr)
        item->ClickEvent().invoke(item);
}

MenuItem::MenuItem(const std::string& text) :
    _props(new menui_props())
{
    _props->text = text;
    _props->separator = text.empty();
}

MenuItem::~MenuItem()
{
}

const std::string& MenuItem::text() const
{
    return _props->text;
}

void MenuItem::text(const std::string& value)
{
    if (_props->text == value)
        return;
    _props->text = value;

    _props->changedEvent.invoke(shared_from_this());
}

bool MenuItem::enabled() const
{
    return _props->enabled;
}

void MenuItem::enabled(bool value)
{
    if (_props->enabled == value)
        return;
    _props->enabled = value;

    //_props->dirty = true;
    _props->changedEvent.invoke(shared_from_this());
}

bool MenuItem::checked() const
{
    return _props->checked;
}

void MenuItem::checked(bool value)
{
    if (_props->checked == value)
        return;
    _props->checked = value;

    _props->changedEvent.invoke(shared_from_this());
}

bool MenuItem::defaultItem() const
{
    return _props->defaultItem;
}

void MenuItem::defaultItem(bool value)
{
    if (_props->defaultItem == value)
        return;
    _props->defaultItem = value;

    _props->changedEvent.invoke(shared_from_this());
}

bool MenuItem::separator() const
{
    return _props->separator;
}

void MenuItem::seperator(bool value)
{
    if (_props->separator == value)
        return;
    _props->separator = value;

    _props->changedEvent.invoke(shared_from_this());
}

void MenuItem::add(ISharedMenuItem item)
{
    auto it = std::find(_props->items.begin(),
        _props->items.end(),
        item);
    if (it != _props->items.end())
        return;

    // TODO: dynamic add/remove subitem not yet supported
    _props->items.push_back(item);

    // chain event to parent
    auto bindingRef = item->ChangedEvent() += [this](ISharedMenuItem item)
        {
            ChangedEvent().invoke(shared_from_this());
        };
    // TODO: needs to remove by bindingRef
}

void MenuItem::remove(ISharedMenuItem item)
{
    auto it = std::find(_props->items.begin(),
        _props->items.end(),
        item);
    if (it == _props->items.end())
        return;

    // TODO: dynamic add/remove subitem not yet supported
    _props->items.erase(it);
}

void MenuItem::clear()
{
    _props->items.clear();
}

const std::vector<ISharedMenuItem>& MenuItem::items() const
{
    return _props->items;
}

IEvent<ISharedMenuItem>& MenuItem::ChangedEvent()
{
    return _props->changedEvent;
}

IEvent<ISharedMenuItem>& MenuItem::ClickEvent()
{
    return _props->clickEvent;
}
