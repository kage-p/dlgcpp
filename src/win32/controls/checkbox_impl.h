#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/checkbox.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class CheckBoxImpl : public ControlImpl
        {
        public:
            explicit CheckBoxImpl();
            ~CheckBoxImpl() override;
            void owner(ICheckBox* checkBox);

        private:
            ICheckBox* _checkBox = nullptr;

            void build() override;
            std::string className() const;
            unsigned int styles() const;
            void notify(DialogMessage&);

            void onCheckedChanged();
        };
    }
}
