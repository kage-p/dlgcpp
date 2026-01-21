#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class IOptionButton : public virtual IControl
        {
        public:
            // properties
            virtual IProperty<bool, ISharedControl>& checked() = 0;
            virtual IProperty<bool, ISharedControl>& autoCheck() = 0;
            virtual IProperty<HorizontalAlign, ISharedControl>& horizontalAlignment() = 0;
            virtual IProperty<VerticalAlign, ISharedControl>& verticalAlignment() = 0;
            virtual IProperty<std::string, ISharedControl>& text() = 0;

            // events
            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
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
            IProperty<bool, ISharedControl>& checked() override;
            IProperty<bool, ISharedControl>& autoCheck() override;
            IProperty<HorizontalAlign, ISharedControl>& horizontalAlignment() override;
            IProperty<VerticalAlign, ISharedControl>& verticalAlignment() override;
            IProperty<std::string, ISharedControl>& text() override;
            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;

            // compatibility setters
            void checked(bool value);
            void autoCheck(bool value);
            void horizontalAlignment(HorizontalAlign value);
            void verticalAlignment(VerticalAlign value);
            void text(const std::string& value);

        private:
            OptionButton(std::shared_ptr<OptionButtonImpl> impl, const Position& p);

            std::shared_ptr<OptionButtonImpl> _impl;

            Property<bool, ISharedControl> _checked;
            Property<bool, ISharedControl> _autoCheck;
            Property<HorizontalAlign, ISharedControl> _horizontalAlignment;
            Property<VerticalAlign, ISharedControl> _verticalAlignment;
            Property<std::string, ISharedControl> _text;
            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
        };
    }
}
