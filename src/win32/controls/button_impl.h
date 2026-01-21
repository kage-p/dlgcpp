#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/button.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class ButtonImpl : public ControlImpl
        {
        public:
            explicit ButtonImpl();
            ~ButtonImpl() override;
            void owner(IButton* button);

        private:
            IButton* _button = nullptr;

            void build() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;
        };
    }
}
