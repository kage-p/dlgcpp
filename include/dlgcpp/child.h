#pragma once

#include "defs.h"
#include <memory>

namespace dlgcpp
{
    // private interface for child access
    // this is needed to only provide access to these members from the parent.
    class IChild
    {
    public:
        virtual int id() const = 0;
        virtual void id(int value) = 0;
        virtual std::shared_ptr<IControl> control() = 0;
        virtual struct ctl_state state() = 0;
    };
}
