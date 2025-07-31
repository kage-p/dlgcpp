#pragma once
#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class IButton : public virtual IControl
        {
        public:
            virtual HorizontalAlign horizontalAlignment() const = 0;
            virtual void horizontalAlignment(HorizontalAlign value) = 0;
            virtual VerticalAlign verticalAlignment() const = 0;
            virtual void verticalAlignment(VerticalAlign value) = 0;
        };

        typedef std::shared_ptr<IButton> ISharedButton;

        class ButtonImpl;

        class Button :
            public Control,
            public IButton
        {
        public:
            explicit Button(
                const std::string& text = std::string(),
                const Position& p = Position());
            ~Button() override;

            // IButton impl.
            HorizontalAlign horizontalAlignment() const override;
            void horizontalAlignment(HorizontalAlign value) override;
            VerticalAlign verticalAlignment() const override;
            void verticalAlignment(VerticalAlign value) override;

        private:
            Button(std::shared_ptr<ButtonImpl> impl);

            std::shared_ptr<ButtonImpl> _impl;
        };
    }
}
