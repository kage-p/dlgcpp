#include "controls/image_p.h"
#include "dlgcpp/controls/image.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Image::Image(const Position& p)
    : Image(std::make_shared<ImageImpl>(p))
{
}

Image::Image(std::shared_ptr<ImageImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

Image::~Image()
{
}

bool Image::autoSize() const
{
    return _impl->autoSize();
}

void Image::autoSize(bool value)
{
    if (_impl->autoSize() == value)
        return;
    _impl->autoSize(value);
}

bool Image::centered() const
{
    return _impl->centered();
}

void Image::centered(bool value)
{
    if (_impl->centered() == value)
        return;
    _impl->centered(value);
}

const ImageSource& Image::image() const
{
    return _impl->image();
}

void Image::image(const ImageSource& image)
{
    _impl->image(image);
}
