#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/custom.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class CustomImpl : public ControlImpl
        {
        public:
            explicit CustomImpl();
            ~CustomImpl() override;
            void owner(ICustom* custom);

        private:
            ICustom* _custom = nullptr;

            std::string className() const override;
        };
    }
}
