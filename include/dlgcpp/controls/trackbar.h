#pragma once

#include "dlgcpp/controls/control.h"
#include <ostream>

namespace dlgcpp
{
    namespace controls
    {
        enum class TrackBarTickMark
        {
            None = 0,
            Above,
            Below,
            Both
        };

        inline std::ostream& operator<<(std::ostream& os, TrackBarTickMark e)
        {
            switch (e) {
            case TrackBarTickMark::None:   return os << "None";
            case TrackBarTickMark::Above:  return os << "Above";
            case TrackBarTickMark::Below: return os << "Below";
            case TrackBarTickMark::Both:  return os << "Both";
            }
            return os << static_cast<int>(e); // fallback
        }

        class ITrackBar : public virtual IControl
        {
        public:
            // properties
            virtual IProperty<bool, ISharedControl>& vertical() = 0;
            virtual IProperty<int, ISharedControl>& value() = 0;
            virtual IProperty<std::pair<int, int>, ISharedControl>& range() = 0;
            virtual IProperty<int, ISharedControl>& pageSize() = 0;
            virtual IProperty<TrackBarTickMark, ISharedControl>& tickMarks() = 0;
            virtual IProperty<Color, ISharedControl>& barColor() = 0;
            virtual IProperty<Color, ISharedControl>& thumbColor() = 0;

            // events
            virtual IEvent<ISharedControl>& ClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleClickEvent() = 0;
            virtual IEvent<ISharedControl>& RightClickEvent() = 0;
            virtual IEvent<ISharedControl>& DoubleRightClickEvent() = 0;
        };

        typedef std::shared_ptr<ITrackBar> ISharedTrackBar;

        class TrackBarImpl;

        class TrackBar :
            public Control,
            public ITrackBar
        {
        public:
            explicit TrackBar(const Position& p = Position());
            ~TrackBar() override;

            // ITrackBar impl.
            IProperty<bool, ISharedControl>& vertical() override;
            IProperty<int, ISharedControl>& value() override;
            IProperty<std::pair<int, int>, ISharedControl>& range() override;
            IProperty<int, ISharedControl>& pageSize() override;
            IProperty<TrackBarTickMark, ISharedControl>& tickMarks() override;
            IProperty<Color, ISharedControl>& barColor() override;
            IProperty<Color, ISharedControl>& thumbColor() override;
            IEvent<ISharedControl>& ClickEvent() override;
            IEvent<ISharedControl>& DoubleClickEvent() override;
            IEvent<ISharedControl>& RightClickEvent() override;
            IEvent<ISharedControl>& DoubleRightClickEvent() override;

            // compatibility setters
            void vertical(bool value);
            void value(int value);
            void range(int from, int to);
            void pageSize(int value);
            void tickMarks(TrackBarTickMark value);
            void barColor(Color value);
            void thumbColor(Color value);

        private:
            TrackBar(std::shared_ptr<TrackBarImpl> impl, const Position& p);

            std::shared_ptr<TrackBarImpl> _impl;

            Property<bool, ISharedControl> _vertical;
            Property<int, ISharedControl> _value;
            Property<std::pair<int, int>, ISharedControl> _range;
            Property<int, ISharedControl> _pageSize;
            Property<TrackBarTickMark, ISharedControl> _tickMarks;
            Property<Color, ISharedControl> _barColor;
            Property<Color, ISharedControl> _thumbColor;
            Event<ISharedControl> _clickEvent;
            Event<ISharedControl> _dblClickEvent;
            Event<ISharedControl> _rightClickEvent;
            Event<ISharedControl> _dblRightClickEvent;
        };
    }
}
