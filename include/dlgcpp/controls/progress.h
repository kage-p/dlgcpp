#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class IProgress : public virtual IControl
        {
        public:
            // properties
            virtual IProperty<bool, ISharedControl>& vertical() = 0;
            virtual IProperty<int, ISharedControl>& value() = 0;
            virtual IProperty<std::pair<int, int>, ISharedControl>& range() = 0;

            // events
            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
            virtual IEvent<ISharedControl>& RightClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleRightClickEvent() = 0;
        };

        typedef std::shared_ptr<IProgress> ISharedProgress;

        class ProgressImpl;

        class Progress :
            public Control,
            public IProgress
        {
        public:
            explicit Progress(const Position& p = Position());
            ~Progress() override;

            // IProgress impl.
            IProperty<bool, ISharedControl>& vertical() override;
            IProperty<int, ISharedControl>& value() override;
            IProperty<std::pair<int, int>, ISharedControl>& range() override;

            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;
            IEvent<ISharedControl>& RightClickEvent() override;
            IEvent<ISharedControl>& DoubleRightClickEvent() override;

            // compatibility setters
            void vertical(bool value);
            void value(int value);
            void range(int from, int to);

        private:
            Progress(std::shared_ptr<ProgressImpl> impl, const Position& p);

            std::shared_ptr<ProgressImpl> _impl;

            Property<bool, ISharedControl> _vertical;
            Property<int, ISharedControl> _value;
            Property<std::pair<int, int>, ISharedControl> _range;
            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
            Event<ISharedControl> _rightClickEvent;
            Event<ISharedControl> _dblRightClickEvent;
        };
    }
}
