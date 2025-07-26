#include "dlgcpp/controls/toolbar_item.h"
#include "utility/event.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

class dlgcpp::controls::ToolBarItemImpl
{
public:
    bool _enabled = true;
    bool _checked = false;
    bool _separator = false;
    ImageSource _image;
    std::string _text;
    std::string _toolTipText;
    Event<ISharedToolBarItem> _clickEvent;
};

ToolBarItem::ToolBarItem(
    const ImageSource& image,
    const std::string& text,
    const std::string& toolTipText) :
    _impl(std::make_shared<ToolBarItemImpl>())
{
    _impl->_image = image;
    _impl->_text = text;
    _impl->_toolTipText = toolTipText;
    _impl->_separator = image.id.empty();
}

bool ToolBarItem::enabled() const
{
    return _impl->_enabled;
}

void ToolBarItem::enabled(bool value)
{
    _impl->_enabled = value;
}

bool ToolBarItem::checked() const
{
    return _impl->_checked;
}

void ToolBarItem::checked(bool value)
{
    _impl->_checked = value;
}

bool ToolBarItem::separator() const
{
    return _impl->_separator;
}

void ToolBarItem::separator(bool value)
{
    _impl->_separator = value;
}

const std::string& ToolBarItem::text() const
{
    return _impl->_text;
}

void ToolBarItem::text(const std::string& value)
{
    _impl->_text = value;
}

const std::string& ToolBarItem::toolTipText() const
{
    return _impl->_toolTipText;
}

void ToolBarItem::toolTipText(const std::string& value)
{
    _impl->_toolTipText = value;
}

const ImageSource& ToolBarItem::image() const
{
    return _impl->_image;
}

void ToolBarItem::image(ImageSource& value)
{
    _impl->_image = value;
}

IEvent<ISharedToolBarItem>& ToolBarItem::ClickEvent()
{
    return _impl->_clickEvent;
}
