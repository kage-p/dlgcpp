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

        class ITrackBar
        {
        public:
            // properties
            virtual bool vertical() const = 0;
            virtual void vertical(bool value) = 0;
            virtual int value() const = 0;
            virtual void value(int value) = 0;
            virtual std::pair<int, int> range() const = 0;
            virtual void range(int from, int to) = 0;
            virtual int pageSize() const = 0;
            virtual void pageSize(int value) = 0;
            virtual TrackBarTickMark tickMarks() const = 0;
            virtual void tickMarks(TrackBarTickMark value) = 0;
            virtual std::pair<Color, Color> barColors() const = 0;
            virtual void barColors(Color thumbColor, Color barColor) = 0;

            // events
            virtual IEvent<ISharedControl>& ChangedEvent() = 0;
        };

        class TrackBar :
            public dlgcpp::Control,
            public ITrackBar
        {
        public:
            explicit TrackBar(const Position& p = Position());
            ~TrackBar() override;

            // ITrackBar impl.
            bool vertical() const override;
            void vertical(bool value) override;
            int value() const override;
            void value(int value) override;
            std::pair<int, int> range() const override;
            void range(int from, int to) override;
            int pageSize() const override;
            void pageSize(int value) override;
            TrackBarTickMark tickMarks() const override;
            void tickMarks(TrackBarTickMark value) override;
            std::pair<Color, Color> barColors() const override;
            void barColors(Color thumbColor, Color barColor) override;

            IEvent<ISharedControl>& ChangedEvent() override;

        private:
            struct trackbar_props* _props;
            void rebuild() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
        };
    }
}
