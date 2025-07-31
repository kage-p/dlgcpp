#pragma once

#include "dlgcpp/controls/control.h"
#include "dlgcpp/controls/tab_item.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class ITabs : public virtual IControl
        {
        public:
            virtual Position area() const = 0;
            virtual int currentIndex() const = 0;
            virtual void currentIndex(int value) = 0;
            virtual const std::vector<ISharedTabItem>& items() const = 0;
            virtual void items(const std::vector<ISharedTabItem>& items) = 0;

            virtual IEvent<ISharedControl>& SelChangedEvent() = 0;
        };

        typedef std::shared_ptr<ITabs> ISharedTabs;

        class TabsImpl;

        class Tabs :
            public Control,
            public ITabs
        {
        public:
            explicit Tabs(
                const Position& p = Position());

            ~Tabs() override;

            // ITabs impl.
            Position area() const override;
            int currentIndex() const override;
            void currentIndex(int value) override;
            const std::vector<ISharedTabItem>& items() const override;
            void items(const std::vector<ISharedTabItem>& items) override;
            IEvent<ISharedControl>& SelChangedEvent() override;

        private:
            Tabs(std::shared_ptr<TabsImpl> impl);

            std::shared_ptr<TabsImpl> _impl;
        };
    }
}
