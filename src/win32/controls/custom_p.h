#pragma once

#include "control_p.h"
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
            explicit CustomImpl(
                Custom& custom,
                const std::string& className,
                const Position& p = Position());

            ~CustomImpl() override;

        private:
            Custom& _custom;
            std::string _className;

            std::string className() const override;
        };
    }
}
