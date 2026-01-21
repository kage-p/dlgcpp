#include "tabs_impl.h"
#include "utility/convert.h"
#include "utility/image_reader.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TabsImpl::TabsImpl() :
    ControlImpl()
{
}

TabsImpl::~TabsImpl()
{
}

void TabsImpl::owner(ITabs* tabs)
{
    ControlImpl::owner(tabs);

    _tabs = tabs;
    _tabs->items().event() += [&](auto) { onItemsChanged(); };
    _tabs->selectedIndex().event() += [&](auto) { updateSelection(); };
    _tabs->p().event() += [&](auto) { updateClientArea(); };
}

std::string TabsImpl::className() const
{
    return WC_TABCONTROLA;
}

unsigned int TabsImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |= TCS_FOCUSONBUTTONDOWN;

    return styles;
}

void TabsImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_NOTIFY)
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);
        if (nmhdr.code == TCN_SELCHANGE)
        {
            readSelection();
        }
    }

    // default notify handler
    switch (msg.wMsg)
    {
    case WM_NOTIFY:
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);
        switch (nmhdr.code)
        {
        case NM_CLICK:
            _tabs->ClickEvent().invoke();
            break;

        case NM_DBLCLK:
            _tabs->DoubleClickEvent().invoke();
            break;

        case NM_RCLICK:
            _tabs->RightClickEvent().invoke();
            break;

        case NM_RDBLCLK:
            _tabs->DoubleRightClickEvent().invoke();
            break;

        case NM_SETFOCUS:
            _tabs->focus() = true;
            break;

        case NM_KILLFOCUS:
            _tabs->focus() = false;
            break;
        }
    }
    }
}

void TabsImpl::build()
{
    ControlImpl::build();

    updateItems();
    updateSelection();
}

void TabsImpl::onItemsChanged()
{
    updateItems();
    updateSelection();
}

void TabsImpl::updateItems()
{
    if (hwnd() == 0)
        return;

    auto hwnd = this->hwnd();
    SendMessage(hwnd, TCM_DELETEALLITEMS, 0, 0);

    // TODO: imageSize customisation
    const auto& items = _tabs->items().value();
    HIMAGELIST hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, items.size(), 0);

    int index = 0;
    for (const auto& item : items)
    {
        auto tci = TCITEMW();

        auto text = StringEncoder::toWide(item->text());
        if (!text.empty())
        {
            tci.mask = TCIF_TEXT;
            tci.pszText = (LPWSTR)text.c_str();
        }

        if (!item->image()->id().empty())
        {
            tci.mask |= TCIF_IMAGE;
            // add the image to the image list
            Size imgSize; // TODO: imageSize customisation
            HANDLE hImage = ImageReader::load(item->image(), imgSize);

            if (item->image()->isIcon())
            {
                tci.iImage = ImageList_AddIcon(hImageList, (HICON)hImage);
                DestroyIcon((HICON)hImage);
            }
            else
            {
                tci.iImage = ImageList_AddMasked(hImageList, (HBITMAP)hImage, 0xFF00FF);
                DeleteObject(hImage);
            }
        }
        if (item->highlight())
        {
            tci.mask |= TCIF_STATE;
            tci.dwState |= TCIS_HIGHLIGHTED;
        }

        if ((int)SendMessageW(hwnd, TCM_INSERTITEMW, (WPARAM)index, (LPARAM)(const TC_ITEM*)(&tci)) < 0)
            break;
        index++;
    }

    hImageList = (HIMAGELIST)SendMessage(hwnd, TCM_SETIMAGELIST, 0, (LPARAM)hImageList);
    if (hImageList != NULL)
        ImageList_Destroy(hImageList);

    // always update area after tabs have been added
    updateClientArea();
}


void TabsImpl::updateClientArea()
{
    if (hwnd() == 0)
    {
        _tabs->area() = Position();
        return;
    }

    auto hwnd = this->hwnd();

    // adjust the rectangle to exclude the tab area
    auto rc = RECT();
    GetClientRect(hwnd, &rc);
    SendMessage(hwnd, TCM_ADJUSTRECT, FALSE, (LPARAM)&rc);

    // map to parent dialog
    auto hwndParent = GetParent(hwnd);
    MapWindowPoints(hwnd, hwndParent, (LPPOINT)&rc, 2);

    auto pos = Position(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    _tabs->area() = Convert(hwndParent).toUnits(pos);
}

void TabsImpl::readSelection()
{
    if (hwnd() == 0)
        return;

    auto index = (int)SendMessage(hwnd(), TCM_GETCURSEL, 0, 0);

    if (index != _tabs->selectedIndex().value())
    {
        _tabs->selectedIndex() = index;
    }
}

void TabsImpl::updateSelection()
{
    if (hwnd() == 0)
        return;

    SendMessage(hwnd(), TCM_SETCURSEL, (WPARAM)_tabs->selectedIndex().value(), 0);
}
