#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ISlider : public virtual IControl
        {
        public:
            virtual bool vertical() const = 0;
            virtual void vertical(bool value) = 0;
            virtual int value() const = 0;
            virtual void value(int value) = 0;
            virtual std::pair<int, int> range() const = 0;
            virtual void range(int from, int to) = 0;
            virtual Color barColor() const = 0;
            virtual void barColor(Color value) = 0;

            virtual IEvent<ISharedControl>& ChangedEvent() = 0;
        };

        typedef std::shared_ptr<ISlider> ISharedSlider;

        class SliderImpl;

        class Slider :
            public Control,
            public ISlider
        {
        public:
            explicit Slider(const std::string& text = std::string(), const Position& p = Position());
            ~Slider() override;

            // ISlider impl.
            bool vertical() const override;
            void vertical(bool value) override;
            int value() const override;
            void value(int value) override;
            std::pair<int, int> range() const override;
            void range(int from, int to) override;
            Color barColor() const override;
            void barColor(Color value) override;

            IEvent<ISharedControl>& ChangedEvent();

        private:
            Slider(std::shared_ptr<SliderImpl> impl);

            std::shared_ptr<SliderImpl> _impl;
        };
    }
}
