#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ILabel : public virtual IControl
        {
        public:
            // properties
            virtual IProperty<bool, ISharedControl>& autoSize() = 0;
            virtual IProperty<bool, ISharedControl>& clickable() = 0;
            virtual IProperty<bool, ISharedControl>& elipsis() = 0;
            virtual IProperty<HorizontalAlign, ISharedControl>& horizontalAlignment() = 0;
            virtual IProperty<VerticalAlign, ISharedControl>& verticalAlignment() = 0;
            virtual IProperty<std::string, ISharedControl>& text() = 0;

            // events
            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
        };

        typedef std::shared_ptr<ILabel> ISharedLabel;

        class LabelImpl;

        class Label :
            public Control,
            public ILabel
        {
        public:
            explicit Label(
                const std::string& text = std::string(),
                const Position& p = Position());
            ~Label() override;

            // ILabel impl.
            IProperty<bool, ISharedControl>& autoSize() override;
            IProperty<bool, ISharedControl>& clickable() override;
            IProperty<bool, ISharedControl>& elipsis() override;
            IProperty<HorizontalAlign, ISharedControl>& horizontalAlignment() override;
            IProperty<VerticalAlign, ISharedControl>& verticalAlignment() override;
            IProperty<std::string, ISharedControl>& text() override;
            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;

            // compatibility setters
            void autoSize(bool value);
            void clickable(bool value);
            void elipsis(bool value);
            void horizontalAlignment(HorizontalAlign value);
            void verticalAlignment(VerticalAlign value);
            void text(const std::string& value);

        private:
            Label(std::shared_ptr<LabelImpl> impl, const Position& p);

            std::shared_ptr<LabelImpl> _impl;

            Property<bool, ISharedControl> _autoSize;
            Property<bool, ISharedControl> _clickable;
            Property<bool, ISharedControl> _elipsis;
            Property<HorizontalAlign, ISharedControl> _horizontalAlignment;
            Property<VerticalAlign, ISharedControl> _verticalAlignment;
            Property<std::string, ISharedControl> _text;
            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
        };
    }
}
