#include "image_impl.h"
#include "utility/convert.h"
#include "utility/image_reader.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ImageImpl::ImageImpl()
    : ControlImpl()
{
}

ImageImpl::~ImageImpl()
{
    if (_hImage != NULL)
    {
        DeleteObject(_hImage);
        _hImage = NULL;
    }
}

void ImageImpl::owner(IImage* image)
{
    _image = image;
    ControlImpl::owner(image);

    // all these trigger autosize logic
    _image->autoSize().event() += [&](auto) { updateImage(); };
    _image->source().event() += [&](auto) { updateImage(); };

    _image->centered().event() += [&](auto) { rebuild(); };
}

void ImageImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == STN_CLICKED)
        {
            _image->ClickEvent().invoke();
        }
        else if (HIWORD(msg.wParam) == STN_DBLCLK)
        {
            _image->DoubleClickEvent().invoke();
        }
    }
}

void ImageImpl::build()
{
    ControlImpl::build();

    if (hwnd() == 0)
        return;

    if (_hImage != NULL)
    {
        auto imageType = (_image->source()->isIcon() ? IMAGE_ICON : IMAGE_BITMAP);
        SendMessage(hwnd(), STM_SETIMAGE, (WPARAM)imageType, (LPARAM)_hImage);
    }
}

unsigned int ImageImpl::styles() const
{
    auto styles = ControlImpl::styles();
    styles = styles & ~WS_TABSTOP;
    styles |= SS_NOTIFY;

    if (_image->source()->isIcon())
        styles |= SS_ICON;
    else
        styles |= SS_BITMAP;

    if (_image->autoSize())
    {
        // use actual image size
        styles |= SS_REALSIZEIMAGE;
    }
    else
    {
        if (_image->centered())
            // center image in control; image may be clipped to fit
            styles |= SS_CENTERIMAGE;
        else
            // use control size; stretch/reduce image to fit
            styles |= SS_REALSIZECONTROL;
    }

    return styles;
}

void ImageImpl::updateImage()
{
    if (_hImage != NULL)
    {
        DeleteObject(_hImage);
        _hImage = NULL;
    }

    Size imgSizePx;
    if (!_image->autoSize())
    {
        // image is same size as control
        imgSizePx = Convert().toPixels(_image->p()->size());
    }

    _hImage = ImageReader::load(_image->source(), imgSizePx);
    if (_image->autoSize())
    {
        // resize the control to fit the image
        Size imgSizeDu = Convert().toUnits(imgSizePx);
        _image->resize(imgSizeDu);
    }

    rebuild();
}
