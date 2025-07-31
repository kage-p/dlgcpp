#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class IOptionButton : public virtual IControl
        {
        public:
            virtual bool checked() const = 0;
            virtual void checked(bool value) = 0;
            virtual HorizontalAlign horizontalAlignment() const = 0;
            virtual void horizontalAlignment(HorizontalAlign value) = 0;
            virtual VerticalAlign verticalAlignment() const = 0;
            virtual void verticalAlignment(VerticalAlign value) = 0;
        };

        typedef std::shared_ptr<IOptionButton> ISharedOptionButton;

        class OptionButtonImpl;

        class OptionButton :
            public Control,
            public IOptionButton
        {
        public:
            explicit OptionButton(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~OptionButton() override;

            // IOptionButton impl.
            bool checked() const override;
            void checked(bool value) override;
            HorizontalAlign horizontalAlignment() const override;
            void horizontalAlignment(HorizontalAlign value) override;
            VerticalAlign verticalAlignment() const override;
            void verticalAlignment(VerticalAlign value) override;

        private:
            OptionButton(std::shared_ptr<OptionButtonImpl> impl);

            std::shared_ptr<OptionButtonImpl> _impl;
        };
    }
}
