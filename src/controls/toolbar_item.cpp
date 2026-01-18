#include "dlgcpp/controls/toolbar_item.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;


ToolBarItem::ToolBarItem(
    const ImageSource& image,
    const std::string& text,
    const std::string& toolTipText)
{
    auto ownerFn = [this]() -> ISharedToolBarItem
        {
            return std::static_pointer_cast<dlgcpp::controls::IToolBarItem>(shared_from_this());
        };

    // properties
    _enabled.reset(true, nullptr, ownerFn, "enabled");
    _checked.reset(false, nullptr, ownerFn, "checked");
    _separator.reset(image.id().empty(), nullptr, ownerFn, "separator");
    _image.reset(image, nullptr, ownerFn, "image");
    _text.reset(text, nullptr, ownerFn, "text");
    _toolTipText.reset(toolTipText, nullptr, ownerFn, "toolTipText");

    // events
    _clickEvent.reset(ownerFn, "ClickEvent");
}

ToolBarItem& ToolBarItem::operator=(const ToolBarItem& other)
{
    if (this != &other)
    {
        _image = other._image;
        _text = other._text;
        _toolTipText = other._toolTipText;
        _separator = other._separator;
    }
    return *this;
}

bool ToolBarItem::operator==(const ToolBarItem& other) const
{
    return (
        _image == other._image &&
        _text == other._text &&
        _toolTipText == other._toolTipText &&
        _separator == other._separator);
}

bool ToolBarItem::operator!=(const ToolBarItem& other) const
{
    return !(*this == other);
}

IProperty<bool, ISharedToolBarItem>& ToolBarItem::enabled()
{
    return _enabled;
}

IProperty<bool, ISharedToolBarItem>& ToolBarItem::checked()
{
    return _checked;
}

IProperty<bool, ISharedToolBarItem>& ToolBarItem::separator()
{
    return _separator;
}

IProperty<std::string, ISharedToolBarItem>& ToolBarItem::text()
{
    return _text;
}

IProperty<std::string, ISharedToolBarItem>& ToolBarItem::toolTipText()
{
    return _toolTipText;
}

IProperty<ImageSource, ISharedToolBarItem>& ToolBarItem::image()
{
    return _image;
}

IEvent<ISharedToolBarItem>& ToolBarItem::ClickEvent()
{
    return _clickEvent;
}
