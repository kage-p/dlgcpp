#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ISlider : public virtual IControl
        {
        public:
            virtual IProperty<bool, ISharedControl>& vertical() = 0;
            virtual IProperty<int, ISharedControl>& value() = 0;
            virtual IProperty<std::pair<int, int>, ISharedControl>& range() = 0;
            virtual IProperty<Color, ISharedControl>& barColor() = 0;
            virtual IProperty<std::string, ISharedControl>& text() = 0;

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
            virtual IProperty<bool, ISharedControl>& vertical() override;
            virtual IProperty<int, ISharedControl>& value() override;
            virtual IProperty<std::pair<int, int>, ISharedControl>& range() override;
            virtual IProperty<Color, ISharedControl>& barColor() override;
            virtual IProperty<std::string, ISharedControl>& text() override;

            IEvent<ISharedControl>& ChangedEvent();

            // compatibility setters
            void vertical(bool value);
            void value(int value);
            void range(int from, int to);
            void barColor(Color value);
            void text(const std::string& value);

        private:
            Slider(std::shared_ptr<SliderImpl> impl, const Position& p);

            std::shared_ptr<SliderImpl> _impl;

            Property<bool, ISharedControl> _vertical;
            Property<int, ISharedControl> _value;
            Property<std::pair<int, int>, ISharedControl> _range;
            Property<Color, ISharedControl> _barColor;
            Property<std::string, ISharedControl> _text;
            Event<ISharedControl> _changedEvent;
        };
    }
}
