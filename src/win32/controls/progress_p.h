#pragma once

#include "control_p.h"
#include "dlgcpp/controls/progress.h"
#include "utility/event.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commctrl.h>

namespace dlgcpp
{
    namespace controls
    {
        class ProgressImpl : public ControlImpl
        {
        public:
            explicit ProgressImpl(
                const Position& p = Position());

            ~ProgressImpl() override;

            bool vertical() const;
            void vertical(bool value);
            int value() const;
            void value(int value);
            std::pair<int, int> range() const;
            void range(int from, int to);
            void colors(Color fgColor, Color bgColor) override;

            IEvent<ISharedControl>& ChangedEvent();

        private:
            bool _vertical = false;
            int _value = 0;
            std::pair<int, int> _range = { 0,100 };
            Event<ISharedControl> _changedEvent;

            void rebuild() override;
            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;

            void updateDisplayStyles();
        };
    }
}
