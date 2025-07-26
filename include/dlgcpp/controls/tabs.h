#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/controls/tab_item.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class TabsImpl;

        class Tabs : public Control
        {
        public:
            explicit Tabs(
                const Position& p = Position());

            ~Tabs() override;

            Position area() const;
            int currentIndex() const;
            void currentIndex(int value);
            const std::vector<ISharedTabItem>& items() const;
            void items(const std::vector<ISharedTabItem>& items);
            IEvent<ISharedControl>& SelChangedEvent();

        private:
            Tabs(std::shared_ptr<TabsImpl> impl);

            std::shared_ptr<TabsImpl> _impl;
        };
    }
}
