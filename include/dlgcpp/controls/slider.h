#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class SliderImpl;

        class Slider : public Control
        {
        public:
            explicit Slider(const std::string& text = std::string(), const Position& p = Position());
            ~Slider() override;

            bool vertical() const;
            void vertical(bool value);
            int value() const;
            void value(int value);
            std::pair<int, int> range() const;
            void range(int from, int to);
            Color barColor() const;
            void barColor(Color value);

            IEvent<ISharedControl>& ChangedEvent();

        private:
            Slider(std::shared_ptr<SliderImpl> impl);

            std::shared_ptr<SliderImpl> _impl;
        };
    }
}
