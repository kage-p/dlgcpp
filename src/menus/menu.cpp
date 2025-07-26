#include "menus/menu_p.h"

using namespace dlgcpp;
using namespace dlgcpp::menus;

Menu::Menu()
{
    _impl = std::make_shared<MenuImpl>();
}

Menu::~Menu()
{
    _impl.reset();
}

std::shared_ptr<MenuImpl> Menu::impl()
{
    return _impl;
}

void Menu::add(ISharedMenuItem item)
{
    _impl->add(item);
}

void Menu::remove(ISharedMenuItem item)
{
    _impl->remove(item);
}

void Menu::clear()
{
    _impl->clear();
}

const std::vector<ISharedMenuItem>& Menu::items() const
{
    return _impl->items();
}

void Menu::popup(ISharedDialog parent, const Point& coords)
{
    _impl->popup(parent, coords);
}

void Menu::popup(ISharedControl parent, const Point& coords)
{
    _impl->popup(parent, coords);
}
