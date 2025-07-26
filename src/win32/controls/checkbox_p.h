#pragma once

#include "control_p.h"
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
            explicit CheckBoxImpl(
                CheckBox& checkBox,
                const std::string& text = std::string(),
                const Position& p = Position());

            ~CheckBoxImpl() override;

            bool checked() const;
            void checked(bool value);
            bool autoCheck() const;
            void autoCheck(bool value);
            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);
            VerticalAlign verticalAlignment() const;
            void verticalAlignment(VerticalAlign value);

        private:
            CheckBox& _checkBox;
            bool _checked = false;
            bool _autoCheck = true;
            HorizontalAlign _horzAlign = HorizontalAlign::Left;
            VerticalAlign _vertAlign = VerticalAlign::Center;

            void rebuild();
            std::string className() const;
            unsigned int styles() const;
            void notify(DialogMessage&);
        };
    }
}
