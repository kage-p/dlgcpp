#include "custom_p.h"

using namespace dlgcpp::controls;

CustomImpl::CustomImpl(
    const std::string& className,
    const Position& p) :
    ControlImpl(std::string(), p)
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
