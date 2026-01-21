#pragma once
#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class IButton : public virtual IControl
        {
        public:
            // properties
            virtual IProperty<HorizontalAlign, ISharedControl>& horizontalAlignment() = 0;
            virtual IProperty<VerticalAlign, ISharedControl>& verticalAlignment() = 0;
            virtual IProperty<std::string, ISharedControl>& text() = 0;

            // events
            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
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
            IProperty<HorizontalAlign, ISharedControl>& horizontalAlignment() override;
            IProperty<VerticalAlign, ISharedControl>& verticalAlignment() override;
            IProperty<std::string, ISharedControl>& text() override;
            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;

            // compatibility setters
            void horizontalAlignment(HorizontalAlign value);
            void verticalAlignment(VerticalAlign value);
            void text(const std::string& value);

        private:
            Button(std::shared_ptr<ButtonImpl> impl, const Position& p);

            std::shared_ptr<ButtonImpl> _impl;

            Property<HorizontalAlign, ISharedControl> _horizontalAlignment;
            Property<VerticalAlign, ISharedControl> _verticalAlignment;
            Property<std::string, ISharedControl> _text;
            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
        };
    }
}
