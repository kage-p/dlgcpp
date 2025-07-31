#include "controls/combobox_p.h"
#include "dlgcpp/controls/combobox.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ComboBox::ComboBox(const Position& p)
    : ComboBox(std::make_shared<ComboBoxImpl>(p))
{
}

ComboBox::ComboBox(std::shared_ptr<ComboBoxImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

ComboBox::~ComboBox()
{
}

int ComboBox::currentIndex() const
{
    return _impl->currentIndex();
}

void ComboBox::currentIndex(int value)
{
    if (_impl->currentIndex() == value)
        return;

    _impl->currentIndex(value);
}

bool ComboBox::dropDown() const
{
    return _impl->dropDown();
}

void ComboBox::dropDown(bool value)
{
    if (_impl->dropDown() == value)
        return;

    _impl->dropDown(value);
}

bool ComboBox::editable() const
{
    return _impl->editable();
}

void ComboBox::editable(bool value)
{
    if (_impl->editable() == value)
        return;

    _impl->editable(value);
}

bool ComboBox::sorted() const
{
    return _impl->sorted();
}

void ComboBox::sorted(bool value)
{
    if (_impl->sorted() == value)
        return;

    _impl->sorted(value);
}

const std::vector<std::string>& ComboBox::items() const
{
    return _impl->items();
}

void ComboBox::items(const std::vector<std::string>& items)
{
    _impl->items(items);
}

IEvent<ISharedControl>& ComboBox::SelChangedEvent()
{
    return _impl->SelChangedEvent();
}

IEvent<ISharedControl>& ComboBox::SelCancelEvent()
{
    return _impl->SelCancelEvent();
}

IEvent<ISharedControl>& ComboBox::ListCloseEvent()
{
    return _impl->ListCloseEvent();
}

IEvent<ISharedControl>& ComboBox::ListOpenEvent()
{
    return _impl->ListOpenEvent();
}

IEvent<ISharedControl>& ComboBox::TextChangedEvent()
{
    return _impl->TextChangedEvent();
}
