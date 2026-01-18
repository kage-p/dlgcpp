#include "dlgcpp/controls/tab_item.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

TabItem::TabItem(
    const std::string& text,
    const ImageSource& image,
    const std::string& toolTipText)
{
    auto ownerFn = [this]() -> ISharedTabItem
        {
            return std::static_pointer_cast<dlgcpp::controls::ITabItem>(shared_from_this());
        };

    // properties
    _highlight.reset(false, nullptr, ownerFn, "highlight");
    _image.reset(image, nullptr, ownerFn, "image");
    _text.reset(text, nullptr, ownerFn, "text");
    _toolTipText.reset(toolTipText, nullptr, ownerFn, "toolTipText");

    // events
    _clickEvent.reset(ownerFn, "ClickEvent");
}

TabItem& TabItem::operator=(const TabItem& other)
{
    if (this != &other)
    {
        _image = other._image;
        _text = other._text;
        _toolTipText = other._toolTipText;
    }
    return *this;
}

bool TabItem::operator==(const TabItem& other) const
{
    return (
        _image == other._image &&
        _text == other._text &&
        _toolTipText == other._toolTipText);
}

bool TabItem::operator!=(const TabItem& other) const
{
    return !(*this == other);
}

IProperty<bool, ISharedTabItem>& TabItem::highlight()
{
    return _highlight;
}

IProperty<std::string, ISharedTabItem>& TabItem::text()
{
    return _text;
}

IProperty<std::string, ISharedTabItem>& TabItem::toolTipText()
{
    return _toolTipText;
}

IProperty<ImageSource, ISharedTabItem>& TabItem::image()
{
    return _image;
}

IEvent<ISharedTabItem>& TabItem::ClickEvent()
{
    return _clickEvent;
}
