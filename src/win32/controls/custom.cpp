#include "custom_p.h"

using namespace dlgcpp::controls;

Custom::Custom(const std::string& className,
               const Position& p) :
    Control(std::string(), p),
    _props(new cus_props())
{
    _props->className = className;
}

Custom::~Custom()
{
    delete _props;
}

std::string Custom::className() const
{
    return _props->className;
}
