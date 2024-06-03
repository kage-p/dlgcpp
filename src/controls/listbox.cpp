#include "listbox.h"

using namespace dlgcpp::controls;

ListBox::ListBox(std::shared_ptr<IDialog> parent, const Position& p)
    : Control(parent)
{
    this->p(p);
}

std::string ListBox::className() const
{
    return "LISTBOX";
}

unsigned int ListBox::exStyles() const
{
    return Control::exStyles() | WS_EX_CLIENTEDGE;
}
