#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ListBox : public dlgcpp::Control
        {
        public:
            explicit ListBox(std::shared_ptr<IDialog> parent, const Position& p = Position());
        private:
            std::string className() const override;
            unsigned int exStyles() const override;
        };
    }
}
