#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class Slider : public dlgcpp::Control
        {
        public:
            explicit Slider(std::shared_ptr<IDialog> parent, const std::string& text = std::string(), const Position& p = Position());
            ~Slider() override;

            bool vertical() const;
            void vertical(bool value);
            int value() const;
            void value(int value);
            std::pair<int, int> range() const;
            void range(int from, int to);
            std::pair<Color, Color> colors() const override;
            void colors(Color fgColor, Color bgColor) override;
            Color barColor() const;
            void barColor(Color value);

            IEvent<>& ChangedEvent();

        private:
            struct slider_props* _props;
            void rebuild() override;
            std::string className() const override;
            void notify(struct dlg_message&) override;
        };
    }
}
