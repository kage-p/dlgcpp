#include "listbox_p.h"

using namespace dlgcpp::controls;

ListBox::ListBox(std::shared_ptr<IDialog> parent, const Position& p)
    : Control(parent), _props(new listbox_props())
{
    this->p(p);
}

ListBox::~ListBox()
{
    delete _props;
}

std::string ListBox::className() const
{
    return "LISTBOX";
}

unsigned int ListBox::exStyles() const
{
    return Control::exStyles() | WS_EX_CLIENTEDGE;
}
