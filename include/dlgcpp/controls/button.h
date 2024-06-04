#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class Button : public dlgcpp::Control
        {
        public:
            explicit Button(std::shared_ptr<IDialog> parent, const std::string& text = std::string(), const Position& p = Position());
            ~Button() override;

        private:
            struct button_props* _props;

            std::string className() const override;
        };
    }
}
