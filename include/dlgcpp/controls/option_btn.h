#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class OptionButton : public dlgcpp::Control
        {
        public:
            explicit OptionButton(const std::string& text = std::string(), const Position& p = Position());
            ~OptionButton() override;

            bool checked() const;
            void checked(bool value);
            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);
            VerticalAlign verticalAlignment() const;
            void verticalAlignment(VerticalAlign value);

        private:
            struct optionbtn_props* _props;

            void rebuild() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
        };
    }
}
