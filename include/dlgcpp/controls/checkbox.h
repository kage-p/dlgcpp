#pragma once
#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ICheckBox : public virtual IControl
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

        typedef std::shared_ptr<ICheckBox> ISharedCheckBox;

        class CheckBoxImpl;

        class CheckBox :
            public Control,
            public ICheckBox
        {
        public:
            explicit CheckBox(
                const std::string& text = std::string(),
                const Position& p = Position());

            ~CheckBox() override;

            // ICheckBox impl.
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
            CheckBox(std::shared_ptr<CheckBoxImpl> impl, const Position& p);

            std::shared_ptr<CheckBoxImpl> _impl;

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
