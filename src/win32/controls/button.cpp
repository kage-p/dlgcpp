#include "button_p.h"

using namespace dlgcpp::controls;

Button::Button(std::shared_ptr<IDialog> parent, const std::string& text, const Position& p)
    : Control(parent), _props(new button_props())
{
    this->text(text);
    this->p(p);
}

Button::~Button()
{
    delete _props;
}

std::string Button::className() const
{
    return "BUTTON";
}
