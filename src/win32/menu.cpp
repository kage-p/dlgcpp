#include "menu_p.h"
#include "dlgcpp/dialog.h"
#include "dlgmsg.h"
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

void createMenuItems(HMENU hMenu,
                     const std::vector<ISharedMenuItem>& items,
                     int& nextItemId,
                     std::map<int, ISharedMenuItem>& idMap)
{
    int index = 0;

    for (auto& item : items)
    {
        std::wstring text;

        if (!item->separator())
            text = toWide(item->text());

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
        mi.wID = ++nextItemId;
        mi.dwTypeData = (LPWSTR)&text[0];
        mi.fState = stateFlags;
        mi.cch = text.size();

        idMap[nextItemId] = item;

        if (!item->items().empty())
        {
            // note: docs state popup submenus will be freed by parent menu.
            mi.fMask |= MIIM_SUBMENU;
            mi.hSubMenu = CreatePopupMenu();
            createMenuItems(mi.hSubMenu, item->items(), nextItemId, idMap);
        }

        InsertMenuItemW(hMenu,
                        index,
                        FALSE,
                        &mi);

        index++;
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
    createMenuItems(_state->hMenu, _props->items, id, _state->idMap);

    if (!isPopUp)
    {
        // self-assign to parent
        HWND hwndParent = reinterpret_cast<HWND>(_props->parent->handle());
        if (hwndParent != NULL)
            SetMenu(hwndParent, _state->hMenu);
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
        // cannot use if has parent/owner window
        return;

    if (_state->hMenu == NULL)
    {
        rebuild();
        if (_state->hMenu == NULL)
            return;
    }

    auto hwndParent = (HWND)parent->handle();

    // coordinates must be in pixels
    Point pxCoords(coords);
    toPixels(hwndParent, pxCoords);

    auto pt = POINT{pxCoords.x(), pxCoords.y()};
    MapWindowPoints(hwndParent, HWND_DESKTOP, &pt, 1);

    auto id = (int)TrackPopupMenu(_state->hMenu,
                                   TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
                                   pt.x,
                                   pt.y,
                                   0,
                                   hwndParent,
                                   NULL);
    if (id == 0)
        return;

    auto item = _state->idMap[id];
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
    _props->dirty = true;
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
    _props->dirty = true;
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
    _props->dirty = true;
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
    _props->dirty = true;
}

void MenuItem::add(ISharedMenuItem item)
{
    auto it = std::find(_props->items.begin(),
                        _props->items.end(),
                        item);
    if (it != _props->items.end())
        return;
    _props->items.push_back(item);
}

void MenuItem::remove(ISharedMenuItem item)
{
    auto it = std::find(_props->items.begin(),
                        _props->items.end(),
                        item);
    if (it == _props->items.end())
        return;
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

IEvent<ISharedMenuItem>& MenuItem::ClickEvent()
{
    return _props->clickEvent;
}
