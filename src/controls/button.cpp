#include "button.h"

using namespace dlgcpp::controls;

Button::Button(std::shared_ptr<IDialog> parent, const std::string& text, const Position& p)
    : Control(parent)
{
    this->text(text);
    this->p(p);
}

std::string Button::className() const
{
    return "BUTTON";
}
