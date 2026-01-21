#include "dlgcpp/controls/control.h"
#include "dlgcpp/dialogs/dialog.h"
#include "menu_impl.h"
#include "menu_item_impl.h"
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

void MenuImpl::owner(IMenu* menu)
{
    _menu = menu;
}

void MenuImpl::parent(std::weak_ptr<dialogs::IDialog> dialog)
{
    _parent = dialog;
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
        // note: we could do this via the item hierarchy but this is faster.
        auto id = (int)LOWORD(msg.wParam);
        auto& item = _menuItemIdMap[id];
        if (item != nullptr)
            item->ClickEvent().invoke();
    }
}

void MenuImpl::rebuild()
{
    destruct();

    bool isPopUp = _parent.expired();

    if (isPopUp)
        _hMenu = CreatePopupMenu();
    else
        _hMenu = CreateMenu();

    int nextItemId = _startId;

    auto& items = _menu->items().value();
    for (auto& item : items)
    {
        _menuItemIdMap[nextItemId] = item;

        // delegate to menu item impl.
        auto menuItemImpl = item->impl();
        if (!menuItemImpl->insert(
            _hMenu,
            nextItemId,
            _menuItemIdMap))
        {
            break;
        }
    }
}

void MenuImpl::destruct()
{
    _menuItemIdMap.clear();

    for (auto& menuItem : _menu->items().value())
    {
        menuItem->impl()->destruct();
    }

    if (_hMenu != NULL)
    {
        DestroyMenu(_hMenu);
        _hMenu = NULL;
    }
}

void MenuImpl::popup(ISharedDialog parent, const Point& coords)
{
    if (!_parent.expired())
    {
        // cannot popup the menu if it already has a parent
        DLGCPP_CERR("Menu has a parent owner, popup will not function.");
        return;
    }

    if (parent == nullptr)
    {
        // cannot popup the menu if the parent dialog is null
        DLGCPP_CERR("Parent dialog required by popup function is null.");
        return;
    }

    auto hwndDialog = (HWND)parent->handle().value();
    if (hwndDialog == NULL)
    {
        // cannot popup the menu if the parent dialog is not created
        DLGCPP_CERR("Parent dialog required by popup function is not valid.");
        return;
    }

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

    auto& item = _menuItemIdMap[id];
    if (item != nullptr)
        item->ClickEvent().invoke();
}

void MenuImpl::popup(ISharedControl parent, const Point& coords)
{
    if (!_parent.expired())
    {
        // cannot popup the menu if it already has a parent
        DLGCPP_CERR("Menu already has a parent owner, popup will not function.");
        return;
    }

    if (parent == nullptr)
    {
        // cannot popup menu without a parent
        DLGCPP_CERR("Parent control not specified for menu popup.");
        return;
    }

    auto hwndControl = (HWND)parent->handle().value();
    if (hwndControl == NULL)
    {
        // cannot popup the menu if the parent control is not created
        DLGCPP_CERR("Parent control required by popup function is null.");
        return;
    }

    auto hwndDialog = GetParent(hwndControl);
    if (hwndDialog == NULL)
    {
        // cannot popup the menu if the control's parent dialog is not created
        DLGCPP_CERR("Parent dialog required by popup function is not valid.");
        return;
    }

    if (_hMenu == NULL)
    {
        rebuild();
        if (_hMenu == NULL)
        {
            DLGCPP_CERR("Failed to build menu for popup function.");
            return;
        }
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
    {
        // menu dismissed without user selecting an item
        // the call may also have failed but we can't tell if this is the case.
        return;
    }

    auto& item = _menuItemIdMap[id];
    if (item != nullptr)
        item->ClickEvent().invoke();
}
