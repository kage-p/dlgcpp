#include "custom_p.h"

using namespace dlgcpp::controls;

CustomImpl::CustomImpl(
    Custom& custom,
    const std::string& className,
    const Position& p) :
    ControlImpl(custom, std::string(), p),
    _custom(custom)
{
    _className = className;
}

CustomImpl::~CustomImpl()
{
}

std::string CustomImpl::className() const
{
    return _className;
}
