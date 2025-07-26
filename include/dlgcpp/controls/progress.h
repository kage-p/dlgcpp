#pragma once

#include "dlgcpp/controls/control.h"

namespace dlgcpp
{
    namespace controls
    {
        class ProgressImpl;

        class Progress : public Control
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

            IEvent<ISharedControl>& ChangedEvent();

        private:
            Progress(std::shared_ptr<ProgressImpl> impl);

            std::shared_ptr<ProgressImpl> _impl;
        };
    }
}
