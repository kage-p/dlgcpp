#pragma once
#include "../control.h"
#include "tab_item.h"
#include <vector>

namespace dlgcpp
{
    namespace controls
    {
        class Tabs : public dlgcpp::Control
        {
        public:
            explicit Tabs(const Position& p = Position());
            ~Tabs() override;

            Position area() const;
            int currentIndex() const;
            void currentIndex(int value);
            const std::vector<ISharedTabItem>& items() const;
            void items(const std::vector<ISharedTabItem>& items);
            IEvent<ISharedControl>& SelChangedEvent();

        private:
            struct tabs_props* _props;

            std::string className() const override;
            unsigned int styles() const override;
            void notify(dlg_message&) override;
            void rebuild() override;

            void readSelection();
            void updateSelection();
            void updateItems();
        };
    }
}
