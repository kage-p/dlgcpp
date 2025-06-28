#pragma once
#include "../control.h"

namespace dlgcpp
{
    namespace controls
    {
        class Progress : public dlgcpp::Control
        {
        public:
            explicit Progress(const Position& p = Position());
            ~Progress() override;

            bool vertical() const;
            void vertical(bool value);
            int value() const;
            void value(int value);
            std::pair<int, int> range() const;
            void range(int from, int to);
            std::pair<Color, Color> colors() const override;
            void colors(Color fgColor, Color bgColor) override;

            IEvent<ISharedControl>& ChangedEvent();

        private:
            struct progress_props* _props;
            void rebuild() override;            
            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
        };
    }
}
