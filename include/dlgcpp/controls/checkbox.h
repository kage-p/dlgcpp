#pragma once
#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class CheckBoxImpl;

        class CheckBox : public Control
        {
        public:
            explicit CheckBox(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~CheckBox() override;

            bool checked() const;
            void checked(bool value);
            bool autoCheck() const;
            void autoCheck(bool value);
            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);
            VerticalAlign verticalAlignment() const;
            void verticalAlignment(VerticalAlign value);

        private:
            CheckBox(std::shared_ptr<CheckBoxImpl> impl);

            std::shared_ptr<CheckBoxImpl> _impl;
        };
    }
}
