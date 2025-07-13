#include "dlgcpp/controls/tab_item.h"
#include "../event_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

struct dlgcpp::controls::tab_item_priv
{
    bool highlight = true;
    ImageSource image;
    std::string text;
    std::string toolTipText;
    Event<ISharedTabItem> clickEvent;
};

TabItem::TabItem(const std::string& text, const ImageSource& image, const std::string& toolTipText) :
    _priv(new tab_item_priv())
{
    _priv->highlight = false;
    _priv->image = image;
    _priv->text = text;
    _priv->toolTipText = toolTipText;
}

TabItem::~TabItem()
{
    delete _priv;
}

bool TabItem::highlight() const
{
    return _priv->highlight;
}

void TabItem::highlight(bool value)
{
    _priv->highlight = value;
}

const std::string& TabItem::text() const
{
    return _priv->text;
}

void TabItem::text(const std::string& value)
{
    _priv->text = value;
}

const std::string& TabItem::toolTipText() const
{
    return _priv->toolTipText;
}

void TabItem::toolTipText(const std::string& value)
{
    _priv->toolTipText = value;
}

const ImageSource& TabItem::image() const
{
    return _priv->image;
}

void TabItem::image(ImageSource& value)
{
    _priv->image = value;
}

IEvent<ISharedTabItem>& TabItem::ClickEvent()
{
    return _priv->clickEvent;
}
