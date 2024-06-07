#include "custom_p.h"

using namespace dlgcpp::controls;

Custom::Custom(const std::string& className,
               const Position& p) :
    Control(),
    _props(new cus_props())
{
    _props->className = className;
    this->p(p);
}

Custom::~Custom()
{
    delete _props;
}

std::string Custom::className() const
{
    return _props->className;
}
