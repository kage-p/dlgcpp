#include "tabs_p.h"
#include "utility/convert.h"
#include "utility/image_reader.h"
#include "utility/string_encoder.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TabsImpl::TabsImpl(
    const Position& p) :
    ControlImpl(std::string(), p)
{
}

TabsImpl::~TabsImpl()
{
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

    ControlImpl::notify(msg);
}

void TabsImpl::rebuild()
{
    ControlImpl::rebuild();
    updateItems();
    updateSelection();
}

Position TabsImpl::area() const
{
    if (handle() == nullptr)
        return Position();
    auto hwnd = reinterpret_cast<HWND>(handle());
    auto hwndParent = GetParent(hwnd);

    // adjust the rectangle to exclude the tab area
    auto rc = RECT();
    GetClientRect(hwnd, &rc);
    SendMessage(hwnd, TCM_ADJUSTRECT, FALSE, (LPARAM)&rc);

    // map to parent dialog
    MapWindowPoints(hwnd, hwndParent, (LPPOINT)&rc, 2);

    auto pos = Position(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    return Convert(hwndParent).toUnits(pos);
}

const std::vector<ISharedTabItem>& TabsImpl::items() const
{
    return _items;
}

void TabsImpl::items(const std::vector<ISharedTabItem>& items)
{
    _items = items;
    updateItems();
    updateSelection();
}

void TabsImpl::updateItems()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, TCM_DELETEALLITEMS, 0, 0);

    // TODO: imageSize customisation
    HIMAGELIST hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, _items.size(), 0);

    int index = 0;
    for (const auto& item : _items)
    {
        auto tci = TCITEMW();

        auto text = StringEncoder::toWide(item->text());
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
            HANDLE hImage = ImageReader::load(item->image(), imgSize);

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

int TabsImpl::currentIndex() const
{
    return _currentIndex;
}

void TabsImpl::currentIndex(int value)
{
    if (_currentIndex == value)
        return;

    _currentIndex = value;

    updateSelection();
}

void TabsImpl::readSelection()
{
    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    auto index = (int)SendMessage(hwnd, TCM_GETCURSEL, 0, 0);

    if (index != _currentIndex)
    {
        currentIndex(index);
        SelChangedEvent().invoke(control());
    }
}

void TabsImpl::updateSelection()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());

    SendMessage(hwnd, TCM_SETCURSEL, (WPARAM)_currentIndex, 0);
}

IEvent<ISharedControl>& TabsImpl::SelChangedEvent()
{
    return _selChangedEvent;
}
