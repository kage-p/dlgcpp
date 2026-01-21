#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class CustomImpl;

        class ICustom : public virtual IControl
        {
        public:
            virtual IProperty<std::string, ISharedControl>& className() = 0;
        };

        class Custom :
            public Control,
            public ICustom
        {
        public:
            explicit Custom(
                const std::string& className = std::string(),
                const Position& p = Position());
            ~Custom() override;

            // ICustom impl.
            IProperty<std::string, ISharedControl>& className() override;

            // compatibility setters
            void className(const std::string& value);

        private:
            Custom(std::shared_ptr<CustomImpl> impl, const Position& p);

            std::shared_ptr<CustomImpl> _impl;

            Property<std::string, ISharedControl> _className;
        };
    }
}
