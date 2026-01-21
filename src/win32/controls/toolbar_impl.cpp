#include "toolbar_impl.h"
#include "utility/convert.h"
#include "utility/image_reader.h"
#include "utility/string_encoder.h"
#include <strsafe.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;

ToolBarImpl::ToolBarImpl(const Position& p) :
    ControlImpl()
{
}

ToolBarImpl::~ToolBarImpl()
{
}

void ToolBarImpl::owner(IToolBar* toolBar)
{
    ControlImpl::owner(toolBar);

    _toolBar = toolBar;
    _toolBar->buttonSize().event() += [&](auto) { updateItems(); };
    _toolBar->imageSize().event() += [&](auto) { updateItems(); };
    _toolBar->items().event() += [&](auto) { updateItems(); };
}

std::string ToolBarImpl::className() const
{
    return TOOLBARCLASSNAMEA;
}

int ToolBarImpl::idRange() const
{
    return ToolbarIdRange;
}

void ToolBarImpl::notify(DialogMessage& msg)
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

            item->ClickEvent().invoke();
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
            _tooltipBuf = StringEncoder::toWide(item->toolTipText());
            ttdi->lpszText = _tooltipBuf.data();
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
            _toolBar->ClickEvent().invoke();
            break;

        case NM_DBLCLK:
            _toolBar->DoubleClickEvent().invoke();
            break;

        case NM_RCLICK:
            _toolBar->RightClickEvent().invoke();
            break;

        case NM_RDBLCLK:
            _toolBar->DoubleRightClickEvent().invoke();
            break;

        case NM_SETFOCUS:
            _toolBar->focus() = true;
            break;

        case NM_KILLFOCUS:
            _toolBar->focus() = false;
            break;
        }
    }
    }
}

void ToolBarImpl::build()
{
    ControlImpl::build();

    if (hwnd() == 0)
        return;

    if (!_toolBar->buttonSize().value().empty())
    {
        auto hwndParent = GetParent(hwnd());
        Size size = Convert(hwndParent).toPixels(_toolBar->buttonSize().value());
        SendMessage(hwnd(), TB_SETBITMAPSIZE, 0, MAKELPARAM(size.width(), size.height()));
    }

    updateItems();
}

unsigned int ToolBarImpl::styles() const
{
    auto styles = ControlImpl::styles();

    styles |= CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE;

    return styles;
}

bool ToolBarImpl::isHandleEqual(void* otherHandle) const
{
    if (hwnd() == 0)
        return false;

    if (ControlImpl::isHandleEqual(otherHandle))
        return true;

    auto hwndOther = reinterpret_cast<HWND>(otherHandle);
    if (hwndOther = (HWND)SendMessage(hwnd(), TB_GETTOOLTIPS, 0, 0))
        return true;

    return false;
}

ISharedToolBarItem ToolBarImpl::findItemById(int ctlId) const
{
    // must be within id reserved range
    int firstId = id() + 1;
    if (ctlId < firstId || ctlId > firstId + idRange())
        return nullptr;

    int index = ctlId - firstId;
    if (index < 0 || index >= (int)_toolBar->items().value().size())
        return nullptr;

    return _toolBar->items().value().at(index);
}

void ToolBarImpl::updateItems()
{
    if (hwnd() == 0)
        return;
    auto hwnd = this->hwnd();

    // delete previous buttons
    int buttonCount = (int)SendMessage(hwnd, TB_BUTTONCOUNT, 0, 0);
    for (int i = buttonCount - 1; i >= 0; i--)
    {
        SendMessage(hwnd, TB_DELETEBUTTON, (WPARAM)i, 0);
    }

    SendMessage(hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

    const auto& items = _toolBar->items().value();
    auto hDlg = GetParent(hwnd);
    Size imageSize = Convert(hDlg).toPixels(_toolBar->imageSize().value());
    Size buttonSize = Convert(hDlg).toPixels(_toolBar->buttonSize().value());

    auto hImageList = ImageList_Create(
        imageSize.width(),
        imageSize.height(),
        ILC_COLOR32 | ILC_MASK,
        items.size(),
        0);

    auto clrPair = _toolBar->colors().value();

    int index = 0;
    int imageListId = 0;
    for (const auto& item : items)
    {
        // use reserved range for button id (id()+1 to reserved)
        const int btnId = id() + (index + 1);
        int imageId = 0;
        auto text = StringEncoder::toWide(item->text());

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

        if (!item->image()->id().empty())
        {
            // add the image to the image list
            HANDLE hImage = ImageReader::load(
                item->image(),
                imageSize,
                clrPair.second);

            if (item->image()->isIcon())
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

    if (!buttonSize.empty())
    {
        SendMessage(hwnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(buttonSize.width(), buttonSize.height()));
    }

    SendMessage(hwnd, TB_AUTOSIZE, 0, 0);
}
