#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/option_btn.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class OptionButtonImpl : public ControlImpl
        {
        public:
            explicit OptionButtonImpl();
            ~OptionButtonImpl() override;
            void owner(IOptionButton* option);

        private:
            IOptionButton* _option = nullptr;

            void build() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void onCheckedChanged();
        };
    }
}
