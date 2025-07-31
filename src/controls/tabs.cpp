#include "controls/tabs_p.h"
#include "dlgcpp/controls/tabs.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

Tabs::Tabs(const Position& p)
    : Tabs(std::make_shared<TabsImpl>(p))
{
}

Tabs::Tabs(std::shared_ptr<TabsImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

Tabs::~Tabs()
{
}

Position Tabs::area() const
{
    return _impl->area();
}

const std::vector<ISharedTabItem>& Tabs::items() const
{
    return _impl->items();
}

void Tabs::items(const std::vector<ISharedTabItem>& items)
{
    _impl->items(items);
}

int Tabs::currentIndex() const
{
    return _impl->currentIndex();
}

void Tabs::currentIndex(int value)
{
    if (_impl->currentIndex() == value)
        return;

    _impl->currentIndex(value);
}

IEvent<ISharedControl>& Tabs::SelChangedEvent()
{
    return _impl->SelChangedEvent();
}
