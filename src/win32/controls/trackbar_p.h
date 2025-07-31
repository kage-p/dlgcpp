#pragma once

#include "control_p.h"
#include "dlgcpp/controls/trackbar.h"
#include "utility/event.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commctrl.h>

namespace dlgcpp
{
    namespace controls
    {
        class TrackBarImpl : public ControlImpl
        {
        public:
            explicit TrackBarImpl(
                const Position& p = Position());

            ~TrackBarImpl() override;

            bool vertical() const;
            void vertical(bool value);
            int value() const;
            void value(int value);
            std::pair<int, int> range() const;
            void range(int from, int to);
            int pageSize() const;
            void pageSize(int value);
            TrackBarTickMark tickMarks() const;
            void tickMarks(TrackBarTickMark value);
            std::pair<Color, Color> barColors() const;
            void barColors(Color thumbColor, Color barColor);

            IEvent<ISharedControl>& ChangedEvent();

        private:
            bool _vertical = false;
            int _value = 0;
            std::pair<int, int> _range = { 0,100 };
            int _pageSize = 1;
            TrackBarTickMark _tickMarks = TrackBarTickMark::Above;
            Color _thumbColor = Color::Default;
            Color _barColor = Color::Default;
            Event<ISharedControl> _changedEvent;

            void rebuild() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;
        };
    }
}
