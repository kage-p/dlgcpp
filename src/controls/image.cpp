#include "controls/image_impl.h"
#include "dlgcpp/controls/image.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Image::Image(const Position& p)
    : Image(std::make_shared<ImageImpl>(), p)
{
    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _autoSize.reset(false, nullptr, ownerFn, "autoSize");
    _centered.reset(false, nullptr, ownerFn, "centered");
    _source.reset(ImageSource(), nullptr, ownerFn, "source");
    _clickEvent.reset(ownerFn, "ClickEvent");
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

Image::Image(
    std::shared_ptr<ImageImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

Image::~Image()
{
    _impl.reset();
}

IProperty<bool, ISharedControl>& Image::autoSize()
{
    return _autoSize;
}

void Image::autoSize(bool value)
{
    _autoSize = value;
}

IProperty<bool, ISharedControl>& Image::centered()
{
    return _centered;
}

void Image::centered(bool value)
{
    _centered = value;
}

IProperty<ImageSource, ISharedControl>& Image::source()
{
    return _source;
}

void Image::source(const ImageSource& source)
{
    _source = source;
}

void Image::image(const ImageSource& source)
{
    _source = source;
}

IEvent<ISharedControl>& Image::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& Image::DoubleClickEvent()
{
    return _dblClickEvent;
}