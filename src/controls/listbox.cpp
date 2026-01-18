#include "controls/listbox_impl.h"
#include "dlgcpp/controls/listbox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListBox::ListBox(const Position& p)
    : ListBox(std::make_shared<ListBoxImpl>(), p)
{
    border() = BorderStyle::Sunken;

    auto ownerFn = [this]() -> ISharedControl { return std::static_pointer_cast<dlgcpp::controls::IControl>(shared_from_this()); };

    _highlight.reset(true, nullptr, ownerFn, "highlight");
    _multiselect.reset(false, nullptr, ownerFn, "multiselect");
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


    _selectedIndex.reset(
        -1,
        [&](int index)
        {
            if (index == -1)
                return true;

            if (_multiselect)
                return false;

            return (index > -1 && index < _items.value().size());
        },
        ownerFn, "selectedIndex");

    _selectedIndexes.reset(
        std::vector<int>(),
        [&](const std::vector<int>& indexes)
        {
            if (indexes.empty())
                return true;

            if (!_multiselect)
                return false;

            std::vector<int> cleanIndexes;
            for (int index : indexes)
            {
                // disallow invalid
                if (index < 0 ||
                    index >= (int)_items.value().size())
                {
                    return false;
                }

                // disallow duplicates
                if (std::find(cleanIndexes.begin(),
                    cleanIndexes.end(),
                    index) != cleanIndexes.end())
                {
                    return false;
                }

                cleanIndexes.push_back(index);
            }

            return true;
        },
        ownerFn, "selectedIndexes");

    // events
    _dblClickEvent.reset(ownerFn, "DoubleClickEvent");
    _selChangedEvent.reset(ownerFn, "SelChangedEvent");
    _selCancelEvent.reset(ownerFn, "SelCancelEvent");

    // pass a reference to the implementation class
    _impl->owner(this);
}

ListBox::ListBox(
    std::shared_ptr<ListBoxImpl> impl,
    const Position& p)
    : Control(impl, p), _impl(std::move(impl))
{
}

ListBox::~ListBox()
{
    _impl.reset();
}

IProperty<int, ISharedControl>& ListBox::selectedIndex()
{
    return _selectedIndex;
}

void ListBox::selectedIndex(int value)
{
    _selectedIndex = value;
}

IProperty<std::vector<int>, ISharedControl>& ListBox::selectedIndexes()
{
    return _selectedIndexes;
}

void ListBox::selectedIndexes(const std::vector<int>& indexes)
{
    _selectedIndexes = indexes;
}

IProperty<bool, ISharedControl>& ListBox::highlight()
{
    return _highlight;
}

void ListBox::highlight(bool value)
{
    _highlight = value;
}

IProperty<bool, ISharedControl>& ListBox::multiselect()
{
    return _multiselect;
}

void ListBox::multiselect(bool value)
{
    _multiselect = value;
}

IProperty<bool, ISharedControl>& ListBox::sorted()
{
    return _sorted;
}

void ListBox::sorted(bool value)
{
    _sorted = value;
}

IProperty<std::vector<std::string>, ISharedControl>& ListBox::items()
{
    return _items;
}

void ListBox::items(const std::vector<std::string>& items)
{
    // reset index if exceeds item count
    if (selectedIndex() > -1 &&
        selectedIndex() >= items.size())
        selectedIndex(-1);

    // reset indexes if any exceed item count
    std::vector<int> indexes = _selectedIndexes.value();
    if (!indexes.empty())
    {
        int itemCount = (int)items.size();
        indexes.erase(
            std::remove_if(indexes.begin(), indexes.end(),
                [itemCount](int value) {
                    return value >= itemCount;
                }),
            indexes.end());
        _selectedIndexes = indexes;
    }

    _items = items;
}

IEvent<ISharedControl>& ListBox::DoubleClickEvent()
{
    return _dblClickEvent;
}

IEvent<ISharedControl>& ListBox::SelChangedEvent()
{
    return _selChangedEvent;
}

IEvent<ISharedControl>& ListBox::SelCancelEvent()
{
    return _selCancelEvent;
}
