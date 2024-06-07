#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class Custom : public dlgcpp::Control
        {
        public:
            explicit Custom(const std::string& className, const Position& p = Position());
            ~Custom() override;

        private:
            struct cus_props* _props;

            std::string className() const override;
        };
    }
}
