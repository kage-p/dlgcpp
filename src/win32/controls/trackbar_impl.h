#pragma once

#include "control_impl.h"
#include "dlgcpp/controls/trackbar.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commctrl.h>

namespace dlgcpp
{
    namespace controls
    {
        class TrackBarImpl : public ControlImpl
        {
        public:
            explicit TrackBarImpl();
            ~TrackBarImpl() override;
            void owner(ITrackBar* trackBar);

        private:
            ITrackBar* _trackBar = nullptr;

            void build() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void onPageSizeChanged();
            void onRangeChanged();
            void onValueChanged();
            void onVerticalChanged();
        };
    }
}
