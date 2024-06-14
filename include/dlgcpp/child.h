#pragma once

#include "defs.h"
#include <memory>

namespace dlgcpp
{
    // private interfaces to support access to these members only from the parent.

    class IChild
    {
    public:
        virtual ISharedDialog parent() const = 0;
        virtual void parent(ISharedDialog) = 0;

        virtual int id() const = 0;
        virtual void id(int value) = 0;

        virtual void notify(dlg_message&) = 0;
        virtual void rebuild() = 0;
    };

    class IChildControl : public IChild
    {
    public:
        virtual std::shared_ptr<IControl> control() = 0;

        // TODO: remove control state access and use notify only.
        // it is possible to use a custom (reserved) message to communicate
        virtual struct ctl_state state() = 0;
    };

    class IChildDialog : public IChild
    {
    public:
        virtual ISharedDialog dialog() = 0;
    };

    class IChildMenu : public IChild
    {
    public:
        virtual std::shared_ptr<IMenu> menu() = 0;
    };

}
