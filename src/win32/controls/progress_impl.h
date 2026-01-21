#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/progress.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commctrl.h>

namespace dlgcpp
{
    namespace controls
    {
        class ProgressImpl : public ControlImpl
        {
        public:
            explicit ProgressImpl();
            ~ProgressImpl() override;
            void owner(IProgress* progress);

        private:
            IProgress* _progress = nullptr;

            void build() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void updateDisplayStyles();

            void onRangeChanged();
            void onValueChanged();
            void onVerticalChanged();
        };
    }
}
