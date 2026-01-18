#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/controls/toolbar_item.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class IToolBar : public virtual IControl
        {
        public:
            // properties
            virtual IProperty<Size, ISharedControl>& buttonSize() = 0;
            virtual IProperty<Size, ISharedControl>& imageSize() = 0;
            virtual IProperty<std::vector<ISharedToolBarItem>, ISharedControl>& items() = 0;

            // events
            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
            virtual IEvent<ISharedControl>& RightClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleRightClickEvent() = 0;
        };

        typedef std::shared_ptr<IToolBar> ISharedToolBar;

        class ToolBarImpl;

        class ToolBar :
            public Control,
            public IToolBar
        {
        public:
            explicit ToolBar(const Position& p = Position());
            ~ToolBar() override;

            // IToolBar impl.
            IProperty<Size, ISharedControl>& buttonSize() override;
            IProperty<Size, ISharedControl>& imageSize() override;
            IProperty<std::vector<ISharedToolBarItem>, ISharedControl>& items() override;
            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;
            IEvent<ISharedControl>& RightClickEvent() override;
            IEvent<ISharedControl>& DoubleRightClickEvent() override;

            // compatibility setters
            void buttonSize(const Size& value);
            void imageSize(const Size& value);
            void items(const std::vector<ISharedToolBarItem>& items);

        private:
            ToolBar(std::shared_ptr<ToolBarImpl> impl, const Position& p);

            std::shared_ptr<ToolBarImpl> _impl;

            Property<Size, ISharedControl> _buttonSize;
            Property<Size, ISharedControl> _imageSize;
            Property<std::vector<ISharedToolBarItem>, ISharedControl> _items;
            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
            Event<ISharedControl> _rightClickEvent;
            Event<ISharedControl> _dblRightClickEvent;
        };
    }
}
