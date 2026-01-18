#include "controls/label_impl.h"
#include "dlgcpp/controls/label.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Label::Label(const std::string& text, const Position& p)
    : Label(std::make_shared<LabelImpl>(), p)
{
    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _autoSize.reset(false, nullptr, ownerFn, "autoSize");
    _clickable.reset(true, nullptr, ownerFn, "clickable");
    _elipsis.reset(false, nullptr, ownerFn, "elipsis");
    _horizontalAlignment.reset(HorizontalAlign::Left, nullptr, ownerFn, "horizontalAlignment");
    _verticalAlignment.reset(VerticalAlign::Top, nullptr, ownerFn, "verticalAlignment");
    _text.reset(text, nullptr, ownerFn, "text");
    _clickEvent.reset(ownerFn, "ClickEvent");
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

Label::Label(
    std::shared_ptr<LabelImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

Label::~Label()
{
    _impl.reset();
}

IProperty<bool, ISharedControl>& Label::autoSize()
{
    return _autoSize;
}

void Label::autoSize(bool value)
{
    _autoSize = value;
}

IProperty<bool, ISharedControl>& Label::clickable()
{
    return _clickable;
}

void Label::clickable(bool value)
{
    _clickable = value;
}

IProperty<bool, ISharedControl>& Label::elipsis()
{
    return _elipsis;
}

void Label::elipsis(bool value)
{
    _elipsis = value;
}

IProperty<HorizontalAlign, ISharedControl>& Label::horizontalAlignment()
{
    return _horizontalAlignment;
}

void Label::horizontalAlignment(HorizontalAlign value)
{
    _horizontalAlignment = value;
}

IProperty<VerticalAlign, ISharedControl>& Label::verticalAlignment()
{
    return _verticalAlignment;
}

void Label::verticalAlignment(VerticalAlign value)
{
    _verticalAlignment = value;
}

IProperty<std::string, ISharedControl>& Label::text()
{
    return _text;
}

void Label::text(const std::string& value)
{
    _text = value;
}

IEvent<ISharedControl>& Label::ClickEvent()
{
    return _clickEvent;
}

IEvent<ISharedControl>& Label::DoubleClickEvent()
{
    return _dblClickEvent;
}
