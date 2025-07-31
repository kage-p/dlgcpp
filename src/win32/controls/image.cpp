#include "image_p.h"
#include "utility/convert.h"
#include "utility/image_reader.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ImageImpl::ImageImpl(
    const Position& p)
    : ControlImpl(std::string(), p)
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

void ImageImpl::notify(DialogMessage& msg)
{
    if (msg.wMsg == WM_COMMAND)
    {
        if (HIWORD(msg.wParam) == STN_CLICKED)
        {
            ClickEvent().invoke(control());
        }
        else if (HIWORD(msg.wParam) == STN_DBLCLK)
        {
            DoubleClickEvent().invoke(control());
        }
    }
    ControlImpl::notify(msg);
}

void ImageImpl::rebuild()
{
    ControlImpl::rebuild();

    if (handle() == nullptr)
        return;
    auto hwnd = reinterpret_cast<HWND>(handle());

    if (_hImage != NULL)
    {
        auto imageType = (_imageSource.isIcon ? IMAGE_ICON : IMAGE_BITMAP);
        SendMessage(hwnd, STM_SETIMAGE, (WPARAM)imageType, (LPARAM)_hImage);
    }
}

unsigned int ImageImpl::styles() const
{
    auto styles = ControlImpl::styles();
    styles = styles & ~WS_TABSTOP;
    styles |= SS_NOTIFY;

    if (_imageSource.isIcon)
        styles |= SS_ICON;
    else
        styles |= SS_BITMAP;

    if (_autoSize)
    {
        // use actual image size
        styles |= SS_REALSIZEIMAGE;
    }
    else
    {
        if (_centered)
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
    if (!_autoSize)
    {
        // image is same size as control
        imgSizePx = Convert().toPixels(p().size());
    }

    _hImage = ImageReader::load(_imageSource, imgSizePx);
    if (_autoSize)
    {
        // resize the control to fit the image
        Size imgSizeDu = Convert().toUnits(imgSizePx);
        ControlImpl::resize(imgSizeDu);
    }

    if (handle() == nullptr)
        return;
    rebuild();
}

bool ImageImpl::autoSize() const
{
    return _autoSize;
}

void ImageImpl::autoSize(bool value)
{
    if (_autoSize == value)
        return;
    _autoSize = value;
    updateImage();
}

bool ImageImpl::centered() const
{
    return _centered;
}

void ImageImpl::centered(bool value)
{
    if (_centered == value)
        return;
    _centered = value;

    if (handle() == nullptr)
        return;
    rebuild();
}

const ImageSource& ImageImpl::image() const
{
    return _imageSource;
}

void ImageImpl::image(const ImageSource& image)
{
    _imageSource = image;
    updateImage();
}
