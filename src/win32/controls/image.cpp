#include "image_p.h"
#include "../dlgmsg.h"
#include "../utility.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Image::Image(const Position& p) :
    Control(),
    _props(new img_props()),
    _state(new img_state())
{
    this->p(p);
    this->border(BorderStyle::Thin);
}

Image::~Image()
{
    delete _props;
    delete _state;
}

void Image::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == STN_CLICKED)
        {
            ClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == STN_DBLCLK)
        {
            DoubleClickEvent().invoke();
        }
    }
}

void Image::rebuild()
{
    Control::rebuild();
    updateImage();
}

unsigned int Image::styles() const
{
    auto styles = Control::styles();
    styles = styles & ~WS_TABSTOP;
    styles |= SS_BITMAP | SS_NOTIFY;

    if (_props->autoSize)
        styles |= SS_REALSIZEIMAGE;

    return styles;
}

void Image::updateImage()
{
    if (_state->hImage != NULL)
    {
        DeleteObject(_state->hImage);
        _state->hImage = NULL;
    }

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_props->image.id.empty())
    {
        SendMessage(hwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
        return;
    }

    auto rc = RECT();
    if (!_props->autoSize)
    {
        SetRect(&rc, 0, 0, p()._cx, p()._cy);
        MapDialogRect(GetParent(hwnd), &rc);
    }

    auto hInstRes = GetModuleHandle(NULL);
    auto imageType = (_props->image.isIcon ? IMAGE_ICON : IMAGE_BITMAP);
    _state->hImage = LoadImage(hInstRes,
                               _props->image.id.c_str(),
                               imageType,
                               rc.right,
                               rc.bottom,
                               (_props->image.isFile ? LR_LOADFROMFILE : 0));
    if (_state->hImage == NULL)
        return;

    SendMessage(hwnd, STM_SETIMAGE, (WPARAM)imageType, (LPARAM)_state->hImage);

    if (_props->autoSize)
    {
        // size around image
        auto rc = RECT();
        GetClientRect(hwnd, &rc);
        auto pos = toUnits(GetParent(hwnd), Position{0,0,rc.right,rc.bottom});
        Control::p( Position{p()._x, p()._y, pos._cx, pos._cy} );
    }
}

bool Image::autoSize() const
{
    return _props->autoSize;
}

void Image::autoSize(bool value)
{
    _props->autoSize = value;
    updateImage();
}

const ImageSource& Image::image() const
{
    return _props->image;
}

void Image::image(const ImageSource& image)
{
    _props->image = image;
    updateImage();
}
