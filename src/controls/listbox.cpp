#include "controls/listbox_p.h"
#include "dlgcpp/controls/listbox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ListBox::ListBox(const Position& p)
    : ListBox(std::make_shared<ListBoxImpl>(p))
{
}

ListBox::ListBox(std::shared_ptr<ListBoxImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

ListBox::~ListBox()
{
}

int ListBox::currentIndex() const
{
    if (_impl->multiselect())
        return -1;
    return _impl->currentIndex();
}

void ListBox::currentIndex(int value)
{
    if (_impl->multiselect())
        return;
    if (_impl->currentIndex() == value)
        return;

    _impl->currentIndex(value);
}

const std::vector<int>& ListBox::currentIndexes() const
{
    static const std::vector<int> empty;
    if (!_impl->multiselect())
        return empty;
    return _impl->currentIndexes();
}

void ListBox::currentIndexes(const std::vector<int>& indexes)
{
    if (!_impl->multiselect())
        return;
    if (_impl->currentIndexes() == indexes)
        return;

    _impl->currentIndexes(indexes);
}

bool ListBox::highlight() const
{
    return _impl->highlight();
}

void ListBox::highlight(bool value)
{
    if (_impl->highlight() == value)
        return;

    _impl->highlight(value);
}

bool ListBox::multiselect() const
{
    return _impl->multiselect();
}

void ListBox::multiselect(bool value)
{
    if (_impl->multiselect() == value)
        return;

    _impl->multiselect(value);
}

bool ListBox::sorted() const
{
    return _impl->sorted();
}

void ListBox::sorted(bool value)
{
    if (_impl->sorted() == value)
        return;

    _impl->sorted(value);
}

const std::vector<std::string>& ListBox::items() const
{
    return _impl->items();
}

void ListBox::items(const std::vector<std::string>& items)
{
    _impl->items(items);
}

IEvent<ISharedControl>& ListBox::SelChangedEvent()
{
    return _impl->SelChangedEvent();
}

IEvent<ISharedControl>& ListBox::SelCancelEvent()
{
    return _impl->SelCancelEvent();
}
