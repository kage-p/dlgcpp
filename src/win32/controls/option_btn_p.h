#pragma once

#include "control_p.h"
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
            explicit OptionButtonImpl(
                OptionButton& optionBtn,
                const std::string& text = std::string(),
                const Position& p = Position());

            ~OptionButtonImpl() override;

            bool checked() const;
            void checked(bool value);
            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);
            VerticalAlign verticalAlignment() const;
            void verticalAlignment(VerticalAlign value);

        private:
            OptionButton& _optionBtn;
            bool _checked = false;
            HorizontalAlign _horzAlign = HorizontalAlign::Left;
            VerticalAlign _vertAlign = VerticalAlign::Center;

            void rebuild() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;
        };
    }
}
