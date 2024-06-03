#pragma once
#include "../control.h"

#include "p_slider.h"

namespace dlgcpp
{
    namespace controls
    {
        class Slider : public dlgcpp::Control
        {
        public:
            explicit Slider(std::shared_ptr<IDialog> parent, const std::string& text = std::string(), const Position& p = Position());
        private:
            std::string className() const override;
            unsigned int exStyles() const override;
        };
    }
}
