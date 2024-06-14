#include "image_p.h"
#include "../dlgmsg.h"
#include "../utility.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Image::Image(const Position& p) :
    Control(std::string(), p),
    _props(new img_props()),
    _state(new img_state())
{
}

Image::~Image()
{
    if (_state->hImage != NULL)
    {
        DeleteObject(_state->hImage);
        _state->hImage = NULL;
    }

    delete _props;
    delete _state;
}

void Image::notify(dlg_message& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == STN_CLICKED)
        {
            ClickEvent().invoke(shared_from_this());
        }
        else if (HIWORD(msg.wParam) == STN_DBLCLK)
        {
            DoubleClickEvent().invoke(shared_from_this());
        }
    }
}

void Image::rebuild()
{
    Control::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_state->hImage != NULL)
    {
        auto imageType = (_props->image.isIcon ? IMAGE_ICON : IMAGE_BITMAP);
        SendMessage(hwnd, STM_SETIMAGE, (WPARAM)imageType, (LPARAM)_state->hImage);
    }
}

unsigned int Image::styles() const
{
    auto styles = Control::styles();
    styles = styles & ~WS_TABSTOP;
    styles |= SS_NOTIFY;

    if (_props->image.isIcon)
        styles |= SS_ICON;
    else
        styles |= SS_BITMAP;

    if (_props->autoSize)
    {
        // use actual image size
        styles |= SS_REALSIZEIMAGE;
    }
    else
    {
        if (_props->centered)
            // center image in control; image may be clipped to fit
            styles |= SS_CENTERIMAGE;
        else
            // use control size; stretch/reduce image to fit
            styles |= SS_REALSIZECONTROL;
    }

    return styles;
}

void Image::updateImage()
{
    if (_state->hImage != NULL)
    {
        DeleteObject(_state->hImage);
        _state->hImage = NULL;
    }

    Size imgSizePx;
    if (!_props->autoSize)
    {
        // image is same size as control
        imgSizePx = p().size();
        toPixels(HWND_DESKTOP, imgSizePx);
    }

    _state->hImage = loadImage(_props->image, imgSizePx);
    if (_props->autoSize)
    {
        // resize the control to fit the image
        Size imgSizeDu(imgSizePx);
        toUnits(HWND_DESKTOP, imgSizeDu);
        Control::resize(imgSizeDu);
    }

    rebuild();
}

bool Image::autoSize() const
{
    return _props->autoSize;
}

void Image::autoSize(bool value)
{
    if (_props->autoSize == value)
        return;
    _props->autoSize = value;
    updateImage();
}

bool Image::centered() const
{
    return _props->centered;
}

void Image::centered(bool value)
{
    if (_props->centered == value)
        return;
    _props->centered = value;
    rebuild();
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
