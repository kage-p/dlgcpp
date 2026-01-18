#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/label.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace dlgcpp
{
    namespace controls
    {
        class LabelImpl : public ControlImpl
        {
        public:
            explicit LabelImpl();
            ~LabelImpl() override;
            void owner(ILabel* label);

        private:
            ILabel* _label = nullptr;

            void build() override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void updateAutoSize();
            void onClickableChanged();
        };
    }
}
