#include "controls/toolbar_p.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;

ToolBar::ToolBar(const Position& p)
    : ToolBar(std::make_shared<ToolBarImpl>(*this, p))
{
}

ToolBar::ToolBar(std::shared_ptr<ToolBarImpl> impl)
    : Control(impl), _impl(std::move(impl))
{
}

ToolBar::~ToolBar()
{
}

const Size& ToolBar::buttonSize() const
{
    return _impl->buttonSize();
}

void ToolBar::buttonSize(const Size& value)
{
    if (_impl->buttonSize() == value)
        return;
    _impl->buttonSize(value);
}

const Size& ToolBar::imageSize() const
{
    return _impl->imageSize();
}

void ToolBar::imageSize(const Size& value)
{
    if (_impl->imageSize() == value)
        return;
    _impl->imageSize(value);
}

const std::vector<ISharedToolBarItem>& ToolBar::items() const
{
    return _impl->items();
}

void ToolBar::items(const std::vector<ISharedToolBarItem>& items)
{
    _impl->items(items);
}
