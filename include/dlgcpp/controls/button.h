#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class Button : public dlgcpp::Control
        {
        public:
            explicit Button(const std::string& text = std::string(), const Position& p = Position());
            ~Button() override;

            HorizontalAlign horizontalAlignment() const;
            void horizontalAlignment(HorizontalAlign value);
            VerticalAlign verticalAlignment() const;
            void verticalAlignment(VerticalAlign value);

        private:
            struct button_props* _props;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
        };
    }
}
