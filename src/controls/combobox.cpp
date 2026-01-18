#include "controls/combobox_impl.h"
#include "dlgcpp/controls/combobox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ComboBox::ComboBox(const Position& p)
    : ComboBox(std::make_shared<ComboBoxImpl>(), p)
{
    border() = BorderStyle::Sunken;

    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _editable.reset(false, nullptr, ownerFn, "editable");
    _dropDown.reset(true, nullptr, ownerFn, "dropDown");
    _sorted.reset(true, nullptr, ownerFn, "sorted");

    _items.reset(
        std::vector<std::string>(),
        [&]
        (const std::vector<std::string>& items)
        {
            // reset index if exceeds item count
            if (_selectedIndex >= items.size())
                _selectedIndex = -1;

            return true;
        },
        ownerFn, "items");

    _text.reset(
        std::string(),
        [&]
        (const std::string& value)
        {
            // assignment of string when editable only
            if (!value.empty())
                return _editable.value();
            else
                return true;
        }, ownerFn, "text");

    _selectedIndex.reset(
        -1,
        [&](int index)
        {
            if (index == -1)
                return true;

            return (index > -1 && index < _items.value().size());
        },
        ownerFn, "selectedIndex");

    // events
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");
    _selChangedEvent.reset(ownerFn, "SelChangedEvent");
    _selCancelEvent.reset(ownerFn, "SelCancelEvent");
    _listOpenEvent.reset(ownerFn, "ListOpenEvent");
    _listCloseEvent.reset(ownerFn, "ListCloseEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

ComboBox::ComboBox(
    std::shared_ptr<ComboBoxImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

ComboBox::~ComboBox()
{
    _impl.reset();
}

IProperty<int, ISharedControl>& ComboBox::selectedIndex()
{
    return _selectedIndex;
}

void ComboBox::selectedIndex(int value)
{
    _selectedIndex = value;
}

IProperty<bool, ISharedControl>& ComboBox::dropDown()
{
    return _dropDown;
}

void ComboBox::dropDown(bool value)
{
    _dropDown = value;
}

IProperty<bool, ISharedControl>& ComboBox::editable()
{
    return _editable;
}

void ComboBox::editable(bool value)
{
    _editable = value;
}

IProperty<bool, ISharedControl>& ComboBox::sorted()
{
    return _sorted;
}

void ComboBox::sorted(bool value)
{
    _sorted = value;
}

IProperty<std::vector<std::string>, ISharedControl>& ComboBox::items()
{
    return _items;
}

void ComboBox::items(const std::vector<std::string>& items)
{
    _items = items;
}

IProperty<std::string, ISharedControl>& ComboBox::text()
{
    return _text;
}

void ComboBox::text(const std::string& value)
{
    _text = value;
}

IEvent<ISharedControl>& ComboBox::DoubleClickEvent()
{
    return _dblClickEvent;
}

IEvent<ISharedControl>& ComboBox::SelChangedEvent()
{
    return _selChangedEvent;
}

IEvent<ISharedControl>& ComboBox::SelCancelEvent()
{
    return _selCancelEvent;
}

IEvent<ISharedControl>& ComboBox::ListCloseEvent()
{
    return _listCloseEvent;
}

IEvent<ISharedControl>& ComboBox::ListOpenEvent()
{
    return _listOpenEvent;
}
