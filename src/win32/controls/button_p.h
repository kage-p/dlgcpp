#pragma once

#include "control_p.h"
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
            explicit ButtonImpl(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~ButtonImpl() override;

            // IButton impl.
            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);
            VerticalAlign verticalAlignment() const;
            void verticalAlignment(VerticalAlign value);

        private:
            HorizontalAlign _horzAlign = HorizontalAlign::Center;
            VerticalAlign _vertAlign = VerticalAlign::Center;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;
        };
    }
}
