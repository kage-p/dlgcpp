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
            ~ListBox() override;

        private:
            struct listbox_props* _props;
            std::string className() const override;
            unsigned int exStyles() const override;
        };
    }
}
