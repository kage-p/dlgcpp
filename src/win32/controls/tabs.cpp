#include "../dlgmsg.h"
#include "../utility.h"
#include "tabs_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Tabs::Tabs(const Position& p) :
    Control(std::string(), p),
    _props(new tabs_props())
{
}

Tabs::~Tabs()
{
    delete _props;
}

IEvent<ISharedControl>& Tabs::SelChangedEvent()
{
    return _props->selChangedEvent;
}

std::string Tabs::className() const
{
    return WC_TABCONTROLA;
}

unsigned int Tabs::styles() const
{
    auto styles = Control::styles();

    styles |= TCS_FOCUSONBUTTONDOWN;

    return styles;
}

void Tabs::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_NOTIFY)
    {
        auto& nmhdr = *((NMHDR*)msg.lParam);
        if (nmhdr.code == TCN_SELCHANGE)
        {
            readSelection();
        }
    }

    Control::notify(msg);
}

void Tabs::rebuild()
{
    Control::rebuild();
    updateItems();
    updateSelection();
}

Position Tabs::area() const
{
    if (handle() == nullptr)
        return Position();
    auto hwnd = reinterpret_cast<HWND>(handle());

    // adjust the rectangle to exclude the tab area
    auto rc = RECT();
    GetClientRect(hwnd, &rc);
    SendMessage(hwnd, TCM_ADJUSTRECT, FALSE, (LPARAM)&rc);

    // map to parent dialog
    MapWindowPoints(hwnd, GetParent(hwnd), (LPPOINT)&rc, 2);

    auto pos = Position(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    return toUnits(GetParent(hwnd), pos);
}

const std::vector<ISharedTabItem>& Tabs::items() const
{
    return _props->items;
}

void Tabs::items(const std::vector<ISharedTabItem>& items)
{
    _props->items = items;
    updateItems();
    updateSelection();
}

void Tabs::updateItems()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, TCM_DELETEALLITEMS, 0, 0);

    // TODO: imageSize customisation
    HIMAGELIST hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, _props->items.size(), 0);

    int index = 0;
    for (const auto& item : _props->items)
    {
        auto tci = TCITEMW();

        auto text = toWide(item->text());
        if (!text.empty())
        {
            tci.mask = TCIF_TEXT;
            tci.pszText = (LPWSTR)text.c_str();
        }

        if (!item->image().id.empty())
        {
            tci.mask |= TCIF_IMAGE;
            // add the image to the image list
            Size imgSize; // TODO: imageSize customisation
            HANDLE hImage = loadImage(item->image(), imgSize);

            if (item->image().isIcon)
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
}

int Tabs::currentIndex() const
{
    return _props->currentIndex;
}

void Tabs::currentIndex(int value)
{
    if (_props->currentIndex == value)
        return;

    _props->currentIndex = value;

    updateSelection();
}

void Tabs::readSelection()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto index = (int)SendMessage(hwnd, TCM_GETCURSEL, 0, 0);

    if (index != _props->currentIndex)
    {
        currentIndex(index);
        SelChangedEvent().invoke(shared_from_this());
    }
}

void Tabs::updateSelection()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, TCM_SETCURSEL, (WPARAM)_props->currentIndex, 0);
}
