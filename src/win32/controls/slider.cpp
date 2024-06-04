#include "dlgcpp/controls/slider.h"
#include "dlgcpp/dialog.h"
#include "p_slider.h"

using namespace dlgcpp::controls;

Slider::Slider(std::shared_ptr<IDialog> parent, const std::string& text, const Position& p)
    : Control(parent)
{
    sldRegister();
    this->text(text);
    this->p(p);
}

std::string Slider::className() const
{
    return SLDR_CLASS;
}

unsigned int Slider::exStyles() const
{
    return Control::exStyles() | WS_EX_STATICEDGE;
}
