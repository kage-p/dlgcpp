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
    dump();
    delete _props;
    delete _state;
}

std::shared_ptr<IDialog> Menu::parent() const
{
    return _props->parent;
}

void Menu::parent(std::shared_ptr<IDialog> parent)
{
    if (_props->parent == parent)
        return;
    _props->parent = parent;
    rebuild();
}

int Menu::id() const
{
    return _props->startId;
}

void Menu::id(int value)
{
    _props->startId = value;
    rebuild();
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
            item->Clicked().invoke();
    }
}

void Menu::add(std::shared_ptr<IMenuItem> item)
{
    auto it = std::find(_props->items.begin(),
                        _props->items.end(),
                        item);
    if (it != _props->items.end())
        return;
    _props->items.push_back(item);
}

void Menu::remove(std::shared_ptr<IMenuItem> item)
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

const std::vector<std::shared_ptr<IMenuItem>>& Menu::items() const
{
    return _props->items;
}

void createMenuItems(HMENU hMenu,
                     const std::vector<std::shared_ptr<IMenuItem>>& items,
                     int& nextItemId,
                     std::map<int, std::shared_ptr<IMenuItem>>& idMap)
{
    int index = 0;

    for (auto& item : items)
    {
        std::wstring text;

        if (!item->separator())
            text = toWide(item->text());

        auto mi = MENUITEMINFOW();
        mi.cbSize = sizeof(MENUITEMINFOW);
        mi.fType = !item->separator() ? MFT_STRING : MFT_SEPARATOR;
        mi.fMask = MIIM_ID | MIIM_FTYPE | MIIM_STRING;
        mi.wID = ++nextItemId;
        mi.dwTypeData = (LPWSTR)&text[0];
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
    dump();

    int id = _props->startId;
    if (id < 1)
        return;

    bool isPopUp = (_props->parent == nullptr);
    if (isPopUp)
        _state->hMenu = CreatePopupMenu();
    else
        _state->hMenu = CreateMenu();

    createMenuItems(_state->hMenu, _props->items, id, _state->idMap);

    if (!isPopUp)
    {
        // self-assign to parent
        HWND hwndParent = reinterpret_cast<HWND>(_props->parent->handle());
        SetMenu(hwndParent, _state->hMenu);
    }

}

void Menu::dump()
{
    if (_state->hMenu != NULL)
    {
        DestroyMenu(_state->hMenu);
        _state->hMenu = NULL;
    }
    _state->idMap.clear();
}

void Menu::popup(int x, int y)
{
    if (_props->parent != nullptr)
        // cannot use if has parent
        return;

    if (_state->hMenu == NULL)
    {
        rebuild();
        if (_state->hMenu == NULL)
            return;
    }

    auto id = (int)TrackPopupMenu(_state->hMenu, TPM_LEFTBUTTON | TPM_RETURNCMD, x, y, 0, HWND_DESKTOP, NULL);
    auto item = _state->idMap[id];
    if (item != nullptr)
        item->Clicked().invoke();
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

void MenuItem::text(const std::string& text)
{
    _props->text = text;
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
}

void MenuItem::add(std::shared_ptr<IMenuItem> item)
{
    auto it = std::find(_props->items.begin(),
                        _props->items.end(),
                        item);
    if (it != _props->items.end())
        return;
    _props->items.push_back(item);
}

void MenuItem::remove(std::shared_ptr<IMenuItem> item)
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

const std::vector<std::shared_ptr<IMenuItem>>& MenuItem::items() const
{
    return _props->items;
}

IEvent<>& MenuItem::Clicked()
{
    return _props->clickedEvent;
}
