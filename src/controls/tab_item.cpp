#include "dlgcpp/controls/tab_item.h"
#include "utility/event.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

class dlgcpp::controls::TabItemImpl
{
public:
    bool _highlight = false;
    ImageSource _image;
    std::string _text;
    std::string _toolTipText;
    Event<ISharedTabItem> _clickEvent;
};

TabItem::TabItem(
    const std::string& text,
    const ImageSource& image,
    const std::string& toolTipText) :
    _impl(std::make_shared<TabItemImpl>())
{
    _impl->_image = image;
    _impl->_text = text;
    _impl->_toolTipText = toolTipText;
}

bool TabItem::highlight() const
{
    return _impl->_highlight;
}

void TabItem::highlight(bool value)
{
    _impl->_highlight = value;
}

const std::string& TabItem::text() const
{
    return _impl->_text;
}

void TabItem::text(const std::string& value)
{
    _impl->_text = value;
}

const std::string& TabItem::toolTipText() const
{
    return _impl->_toolTipText;
}

void TabItem::toolTipText(const std::string& value)
{
    _impl->_toolTipText = value;
}

const ImageSource& TabItem::image() const
{
    return _impl->_image;
}

void TabItem::image(ImageSource& value)
{
    _impl->_image = value;
}

IEvent<ISharedTabItem>& TabItem::ClickEvent()
{
    return _impl->_clickEvent;
}
