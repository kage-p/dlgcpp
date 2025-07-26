#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class OptionButtonImpl;

        class OptionButton : public Control
        {
        public:
            explicit OptionButton(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~OptionButton() override;

            bool checked() const;
            void checked(bool value);
            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);
            VerticalAlign verticalAlignment() const;
            void verticalAlignment(VerticalAlign value);

        private:
            OptionButton(std::shared_ptr<OptionButtonImpl> impl);

            std::shared_ptr<OptionButtonImpl> _impl;
        };
    }
}
