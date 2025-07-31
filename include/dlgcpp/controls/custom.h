#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class CustomImpl;

        class Custom :
            public Control
        {
        public:
            explicit Custom(const std::string& className, const Position& p = Position());
            ~Custom() override;

        private:
            Custom(std::shared_ptr<CustomImpl> impl);

            std::shared_ptr<CustomImpl> _impl;
        };
    }
}
