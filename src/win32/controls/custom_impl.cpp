#include "custom_impl.h"

using namespace dlgcpp::controls;

CustomImpl::CustomImpl() :
    ControlImpl()
{
}

CustomImpl::~CustomImpl()
{
}

void CustomImpl::owner(ICustom* custom)
{
    ControlImpl::owner(custom);

    _custom = custom;

    _custom->className().event() +=
        [&](auto)
        {
            // force destruct/build in case class name is invalid
            destruct();
            build();
        };
}

std::string CustomImpl::className() const
{
    return _custom->className().value();
}
