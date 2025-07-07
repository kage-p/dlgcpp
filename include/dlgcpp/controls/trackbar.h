#pragma once
#include "../control.h"

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

        class TrackBar : public dlgcpp::Control
        {
        public:
            explicit TrackBar(const Position& p = Position());
            ~TrackBar() override;

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
            void colors(Color fgColor, Color bgColor) override;

            IEvent<ISharedControl>& ChangedEvent();

        private:
            struct trackbar_props* _props;
            void rebuild() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
        };
    }
}
