#include "toolbar_p.h"
#include "utility/image.h"
#include "utility/message.h"
#include "utility/string.h"
#include "utility/units.h"
#include <strsafe.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;

ToolBar::ToolBar(const Position& p) :
    Control(std::string(), p),
    _props(new toolbar_props())
{
    this->border(BorderStyle::None);
}

ToolBar::~ToolBar()
{
    delete _props;
}

std::string ToolBar::className() const
{
    return TOOLBARCLASSNAMEA;
}

int ToolBar::idRange() const
{
    return ToolbarIdRange;
}

void ToolBar::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == BN_CLICKED)
        {
            auto ctlId = (int)LOWORD(msg.wParam);

            // find toolbar item
            auto item = findItemById(ctlId);
            if (item == nullptr)
                return;

            item->ClickEvent().invoke(item);
        }
    }
    else if (msg.wMsg == WM_NOTIFY)
    {
        auto ttdi = (NMTTDISPINFO*)msg.lParam;
        if (ttdi->hdr.code == TTN_NEEDTEXT)
        {
            auto ctlId = ttdi->hdr.idFrom;

            // find toolbar item
            auto item = findItemById(ctlId);
            if (item == nullptr)
                return;

            // use own buffer
            _props->tooltipBuf = toWide(item->toolTipText());
            ttdi->lpszText = _props->tooltipBuf.data();
        }
    }

    Control::notify(msg);
}

void ToolBar::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    if (!_props->buttonSize.empty())
    {
        Size size = _props->buttonSize;
        toPixels(GetParent(hwnd), size);
        SendMessage(hwnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(size.width(), size.height()));
    }

    updateItems();
}

unsigned int ToolBar::styles() const
{
    auto styles = Control::styles();

    styles |= CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE;

    return styles;
}

bool ToolBar::isHandleEqual(void* otherHandle) const
{
    if (handle() == nullptr)
        return false;

    if (Control::isHandleEqual(otherHandle))
        return true;

    auto hwnd = reinterpret_cast<HWND>(handle());
    auto hwndOther = reinterpret_cast<HWND>(otherHandle);
    if (hwndOther = (HWND)SendMessage(hwnd, TB_GETTOOLTIPS, 0, 0))
        return true;

    return false;
}

const Size& ToolBar::buttonSize() const
{
    return _props->buttonSize;
}

void ToolBar::buttonSize(const Size& value)
{
    _props->buttonSize = value;
    updateItems();
}

const Size& ToolBar::imageSize() const
{
    return _props->imageSize;
}

void ToolBar::imageSize(const Size& value)
{
    _props->imageSize = value;
    updateItems();
}

const std::vector<ISharedToolBarItem>& ToolBar::items() const
{
    return _props->items;
}

void ToolBar::items(const std::vector<ISharedToolBarItem>& items)
{
    // TODO: LIMIT TO toolbarIdRange
    _props->items = items;

    updateItems();
}

ISharedToolBarItem ToolBar::findItemById(int ctlId) const
{
    // must be within id reserved range
    int firstId = id() + 1;
    if (ctlId < firstId || ctlId > firstId + idRange())
        return nullptr;

    int index = ctlId - firstId;
    if (index < 0 || index >= (int)_props->items.size())
        return nullptr;

    return _props->items.at(index);
}


void ToolBar::updateItems()
{
    if (handle() == nullptr)
        return;

    auto hwnd = reinterpret_cast<HWND>(handle());
    auto hDlg = GetParent(hwnd);

    // delete previous buttons
    int buttonCount = (int)SendMessage(hwnd, TB_BUTTONCOUNT, 0, 0);
    for (int i = buttonCount - 1; i >= 0; i--)
        SendMessage(hwnd, TB_DELETEBUTTON, (WPARAM)i, 0);

    SendMessage(hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

    Size imageSize = _props->imageSize;
    toPixels(hDlg, imageSize);

    auto hImageList = ImageList_Create(
        imageSize.width(),
        imageSize.height(),
        ILC_COLOR32 | ILC_MASK,
        _props->items.size(),
        0);

    auto clrPair = colors();

    int index = 0;
    int imageListId = 0;
    for (const auto& item : _props->items)
    {
        // use reserved range for button id (id()+1 to reserved)
        const int btnId = id() + (index + 1);
        int imageId = 0;
        auto text = toWide(item->text());

        BYTE style = 0;
        if (item->separator())
            style |= BTNS_SEP;
        else
            style |= BTNS_BUTTON;

        BYTE state = 0;
        if (item->enabled())
            state |= TBSTATE_ENABLED;
        if (item->checked())
            state |= TBSTATE_CHECKED;

        INT_PTR btnText = 0;
        if (!text.empty())
            btnText = (INT_PTR)&text[0];

        if (!item->image().id.empty())
        {
            // add the image to the image list
            HANDLE hImage = loadImage(
                item->image(),
                imageSize,
                clrPair.second);

            if (item->image().isIcon)
            {
                imageId = ImageList_AddIcon(hImageList, (HICON)hImage);
                DestroyIcon((HICON)hImage);
            }
            else
            {
                imageId = ImageList_AddMasked(hImageList, (HBITMAP)hImage, (COLORREF)clrPair.second);
                DeleteObject(hImage);
            }
        }

        TBBUTTON tbi = { MAKELONG(imageId, imageListId), btnId, state, style, {0}, 0, btnText };

        SendMessageW(hwnd, TB_INSERTBUTTONW, index, (LPARAM)&tbi);
        index++;
    }

    hImageList = (HIMAGELIST)SendMessage(hwnd, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
    if (hImageList != NULL)
        ImageList_Destroy(hImageList);

    if (!_props->buttonSize.empty())
    {
        Size size = _props->buttonSize;
        toPixels(hDlg, size);
        SendMessage(hwnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(size.width(), size.height()));
    }

    SendMessage(hwnd, TB_AUTOSIZE, 0, 0);
}
