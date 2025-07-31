#pragma once

#include "control_p.h"
#include "dlgcpp/controls/tabs.h"
#include "utility/event.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

namespace dlgcpp
{
    namespace controls
    {
        class TabsImpl : public ControlImpl
        {
        public:
            explicit TabsImpl(
                const Position& p = Position());

            ~TabsImpl() override;

            Position area() const;
            int currentIndex() const;
            void currentIndex(int value);
            const std::vector<ISharedTabItem>& items() const;
            void items(const std::vector<ISharedTabItem>& items);
            IEvent<ISharedControl>& SelChangedEvent();

        private:
            std::vector<ISharedTabItem> _items;
            int _currentIndex = -1;
            Event<ISharedControl> _selChangedEvent;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(DialogMessage&) override;
            void rebuild() override;

            void readSelection();
            void updateSelection();
            void updateItems();
        };
    }
}
