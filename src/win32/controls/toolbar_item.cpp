#include "dlgcpp/controls/toolbar_item.h"
#include "../event_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

struct dlgcpp::controls::toolbar_item_priv
{
    bool enabled = true;
    bool checked = false;
    bool separator = false;
    ImageSource image;
    std::string text;
    std::string toolTipText;
    Event<ISharedToolBarItem> clickEvent;
};

ToolBarItem::ToolBarItem(const ImageSource& image, const std::string& toolTipText) :
    _priv(new toolbar_item_priv())
{
    _priv->image = image;
    _priv->toolTipText = toolTipText;
    _priv->separator = image.id.empty();
}

ToolBarItem::~ToolBarItem()
{
    delete _priv;
}

bool ToolBarItem::enabled() const
{
    return _priv->enabled;
}

void ToolBarItem::enabled(bool value)
{
    _priv->enabled = value;
}

bool ToolBarItem::checked() const
{
    return _priv->checked;
}

void ToolBarItem::checked(bool value)
{
    _priv->checked = value;
}

bool ToolBarItem::separator() const
{
    return _priv->separator;
}

void ToolBarItem::separator(bool value)
{
    _priv->separator = value;
}

const std::string& ToolBarItem::text() const
{
    return _priv->text;
}

void ToolBarItem::text(const std::string& value)
{
    _priv->text = value;
}

const std::string& ToolBarItem::toolTipText() const
{
    return _priv->toolTipText;
}

void ToolBarItem::toolTipText(const std::string& value)
{
    _priv->toolTipText = value;
}

const ImageSource& ToolBarItem::image() const
{
    return _priv->image;
}

void ToolBarItem::image(ImageSource& value)
{
    _priv->image = value;
}

IEvent<ISharedToolBarItem>& ToolBarItem::ClickEvent()
{
    return _priv->clickEvent;
}
